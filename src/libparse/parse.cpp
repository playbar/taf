#include "parse/parse.h"
#include "jce.tab.hpp"
#include <errno.h>
#include <fstream>
#include <string.h>
#include "util/tc_common.h"

JceParsePtr g_parse = new JceParse();

void yyerror(char const *msg)
{
    g_parse->error(msg);
}

JceParse::JceParse()
{
    _bWithTaf = false;
    _itab = 0;
    initScanner();
}

void JceParse::setTaf(bool bWithTaf)
{
    _bWithTaf = bWithTaf;
}

void JceParse::setHeader(const string &sHeader)
{
    _sHeader = sHeader;
}

string JceParse::getHeader()
{
    if(_sHeader.empty())
        return _sHeader;
    return _sHeader + "/";
}

void JceParse::clear()
{
    while(!_contexts.empty()) _contexts.pop();
    while(!_contains.empty()) _contains.pop();
    _vcontexts.clear();
    _structs.clear();
    _enums.clear();
    _namespaces.clear();
}

void JceParse::parse(const string& sFileName)
{
    clear();

    _contains.push(new Container(""));
    if(!(yyin = fopen(sFileName.c_str(), "r")))
    {
        error("open file '" + sFileName + "' error :" + string(strerror(errno)));
    }

    pushFile(sFileName);

    yyparse();
}

void JceParse::pushFile(const string &file)
{
    ContextPtr c = new Context(file);
    _contexts.push(c);
    _vcontexts.push_back(c);
}

ContextPtr JceParse::popFile()
{
    ContextPtr c = _contexts.top();
    _contexts.pop();
    return c;
}

bool JceParse::getFilePath(const string &s, string &file)
{
    if(s.length() < 2 || s[0] != '\"' || s[s.length()-1] != '\"')
    {
        error("#include need \"FILENAME\"");
    }

    file = s.substr(1, s.length() - 2);

    for(size_t i = 0; i < _vcontexts.size(); i++)
    {
        if(_vcontexts[i]->getFileName() == file)
        {
            return false;
        }
    }

    return true;
}

string JceParse::getCurrFileName()
{
    return _contexts.top()->getFileName();
}

void JceParse::nextLine()
{
   _contexts.top()->nextLine();
}

ContextPtr JceParse::currentContextPtr()
{
    return _contexts.top();
}

void JceParse::error(const string &msg)
{
    cerr <<  _contexts.top()->getFileName() << ": " << _contexts.top()->getCurrLine() << ": error: " << msg << endl;
    exit(-1);
}

int JceParse::checkKeyword(const string& s)
{
    std::map<std::string, int>::const_iterator it = _keywordMap.find(s);
    if(it != _keywordMap.end())
    {
        return it->second;
    }

    if(!_bWithTaf)
    {
        string sPrefix = "taf";
        //不能以taf开头
        if((s.length() >= sPrefix.length()) && (s.substr(0, sPrefix.length()) == sPrefix))
        {
            error("identifier can't start with 'taf'");
        }
    }

    return JCE_IDENTIFIER;
}

void JceParse::initScanner()
{
    _keywordMap["void"]     = JCE_VOID;
    _keywordMap["struct"]   = JCE_STRUCT;
    _keywordMap["bool"]     = JCE_BOOL;
    _keywordMap["byte"]     = JCE_BYTE;
    _keywordMap["short"]    = JCE_SHORT;
    _keywordMap["int"]      = JCE_INT;
    _keywordMap["double"]   = JCE_DOUBLE;
    _keywordMap["float"]    = JCE_FLOAT;
    _keywordMap["long"]     = JCE_LONG;
    _keywordMap["string"]   = JCE_STRING;
    _keywordMap["vector"]   = JCE_VECTOR;
    _keywordMap["map"]      = JCE_MAP;
    _keywordMap["key"]      = JCE_KEY;
    _keywordMap["routekey"] = JCE_ROUTE_KEY;
    _keywordMap["module"]   = JCE_NAMESPACE;
    _keywordMap["interface"]= JCE_INTERFACE;
    _keywordMap["out"]      = JCE_OUT;
    _keywordMap["require"]  = JCE_REQUIRE;
    _keywordMap["optional"] = JCE_OPTIONAL;
    _keywordMap["false"]    = JCE_FALSE;
    _keywordMap["true"]     = JCE_TRUE;
    _keywordMap["enum"]     = JCE_ENUM;
    _keywordMap["const"]    = JCE_CONST;
    _keywordMap["unsigned"] = JCE_UNSIGNED;
}

string JceParse::getTab()
{
    ostringstream s;
    for(int i = 0; i < _itab; i++)
    {
        s << "    ";
    }

    return s.str();
}

BuiltinPtr JceParse::createBuiltin(Builtin::Kind kind,bool isUnsigned)
{
    return new Builtin(kind,isUnsigned);
}

VectorPtr JceParse::createVector(const TypePtr &ptr)
{
    return new Vector(ptr);
}

MapPtr JceParse::createMap(const TypePtr &pleft, const TypePtr &pright)
{
    return new Map(pleft, pright);
}

void JceParse::addNamespacePtr(const NamespacePtr &nPtr)
{
    _namespaces.push_back(nPtr);
}

NamespacePtr JceParse::findNamespace(const string &id)
{
    for(size_t i = 0; i < _namespaces.size(); i++)
    {
        if(_namespaces[i]->getId() == id)
        {
            return _namespaces[i];
        }
    }

    return NULL;
}

NamespacePtr JceParse::currentNamespace()
{
    return _namespaces.back();
}

void JceParse::addStructPtr(const StructPtr &sPtr)
{
    _structs.push_back(sPtr);
}

void JceParse::addEnumPtr(const EnumPtr &ePtr)
{
    _enums.push_back(ePtr);
}

StructPtr JceParse::findStruct(const string &sid)
{
    string ssid = sid;

    //在当前namespace中查找
    NamespacePtr np = currentNamespace();
    if(ssid.find("::") == string::npos)
    {
        ssid = np->getId() + "::" + ssid;
    }

    for(size_t i = 0; i < _structs.size(); i++)
    {
        if(_structs[i]->getSid() == ssid)
        {
            return _structs[i];
        }
    }

    return NULL;
}

EnumPtr JceParse::findEnum(const string &sid)
{
    string ssid = sid;

    //在当前namespace中查找
    NamespacePtr np = currentNamespace();
    if(ssid.find("::") == string::npos)
    {
        ssid = np->getId() + "::" + sid;
    }

    for(size_t i = 0; i < _enums.size(); i++)
    {
        if(_enums[i]->getSid() == ssid)
        {
            return _enums[i];
        }
    }

    return NULL;
}

void JceParse::checkConflict(const string &sid)
{
    //是否和枚举重名
    if(findEnum(sid))
    {
        error("conflicts with enum '" + sid + "'");
    }

    //是否和结构重名
    if(findStruct(sid))
    {
        error("conflicts with struct '" + sid + "'");
    }
}

TypePtr JceParse::findUserType(const string &sid)
{
    StructPtr sPtr = findStruct(sid);
    if(sPtr) return sPtr;

    EnumPtr ePtr = findEnum(sid);
    if(ePtr) return ePtr;

    return NULL;
}

ContainerPtr JceParse::currentContainer()
{
    return _contains.top();
}

void JceParse::pushContainer(const ContainerPtr &c)
{
    _contains.push(c);
    NamespacePtr np = NamespacePtr::dynamicCast(c);
    if(np)
    {
        addNamespacePtr(np);
    }
}

ContainerPtr JceParse::popContainer()
{
    ContainerPtr c = _contains.top();
    _contains.pop();

    return c;
}

void JceParse::checkTag(int i)
{
    if(i >= 256)
    {
        error("struct memeber tag can't beyond 256");
    }
    if(i < 0)
    {
        error("struct memeber tag can't less then 0");
    }
}

void JceParse::checkSize(int i)
{
    if(i >= 1024*1024)
    {
        error("struct memeber size can't beyond 1M");
    }
    if(i < 1)
    {
        error("struct memeber size can't less than 1");
    }
}

void JceParse::checkArrayVaid(TypePtr &tPtr,int size)
{
    checkSize(size);
    //只有string/vector可以为array类型
    //vector不可以嵌套array类型 例如不允许vector<string:8>:2; 

    VectorPtr vPtr = VectorPtr::dynamicCast(tPtr);
    if(vPtr )
    {
        if(vPtr->getTypePtr()->isArray())
        {
            error("fixed array type can not be nested");
        }
        return; 
    }
    BuiltinPtr bPtr  = BuiltinPtr::dynamicCast(tPtr);
    if(bPtr && bPtr->kind()== Builtin::KindString)
    {
       return;
    }
    error("only string or vector can use fix array");
}

void JceParse::checkPointerVaid(TypePtr &tPtr)
{
    //必须为vector<Byte>类型

    VectorPtr vPtr = VectorPtr::dynamicCast(tPtr);
    if(vPtr)
    {
        BuiltinPtr bPtr  = BuiltinPtr::dynamicCast(vPtr->getTypePtr());
        if( bPtr && bPtr->kind()== Builtin::KindByte) 
        return; 
    }  
    error("only 'byte *' can  be pointer type");
}

void JceParse::checkConstValue(TypeIdPtr &tPtr, int c)
{
    //只有内建类型才能有缺省值
    BuiltinPtr bPtr  = BuiltinPtr::dynamicCast(tPtr->getTypePtr());
    if(!bPtr)
    {
        error("only base type can have default value");
    }

    int b = bPtr->kind();

    if(c == ConstTok::VALUE)
    {
        if(b == Builtin::KindBool)
        {
            error("default value of bool can only be true or false");
        }
        if(b == Builtin::KindString)
        {
            error("default value of string can only be \"string\"");
        }
    }
    else if(c == ConstTok::BOOL)
    {
        if(b != Builtin::KindBool)
        {
            error("only bool type can be true or false");
        }
    }
    else if(c == ConstTok::STRING)
    {
        if(b != Builtin::KindString)
        {
            error("only string type can be \"string\"");
        }
    }
}

string JceParse::printHeaderRemark()
{
    ostringstream s;
    s << "// **********************************************************************" << endl;
    s << "// This file was generated by a TAF parser!" << endl;
    s << "// TAF version " << TAF_VERSION << " by WSRD Tencent." << endl;
    s << "// Generated from `" << getCurrFileName() << "'" << endl;
    s << "// **********************************************************************" << endl;
    s << endl;

    return s.str();
}


