#include "jce2cs.h"
#include "util/tc_md5.h"
#include "util/tc_file.h"
#include "util/tc_common.h"
#include <string>

#define JCE_PACKAGE     "Com.QQ.Taf.Jce"
#define PROXY_PACKAGE   "Com.QQ.Taf.Proxy"
#define HOLDER_PACKAGE  "Com.QQ.Taf.Holder"
#define SERVER_PACKAGE  "Com.QQ.Taf.Server"
#define WUP_PACKAGE     "Com.QQ.Jce.Wup"

#define TAB g_parse->getTab()
#define INC_TAB g_parse->incTab()
#define DEL_TAB g_parse->delTab()

//////////////////////////////////////////////////////////////////////////////////
//
string Jce2Cs::writeTo(const TypeIdPtr &pPtr) const
{
    ostringstream s;
    if(EnumPtr::dynamicCast(pPtr->getTypePtr()))
    {
        s << TAB << "_os.write((taf::Int32)" << pPtr->getId() << ", " << pPtr->getTag() << ");" << endl;
    }
    else
    {
        s << TAB << "_os.write(" << pPtr->getId() << ", " << pPtr->getTag() << ");" << endl;
    }

    return s.str();
}

string Jce2Cs::readFrom(const TypeIdPtr &pPtr) const
{
    ostringstream s;
    if(EnumPtr::dynamicCast(pPtr->getTypePtr()))
    {
        s << TAB << "_is.read((taf::Int32&)" << pPtr->getId();
    }
    else
    {
        s << TAB << "_is.read(" << pPtr->getId();
    }

    s << ", " << pPtr->getTag() << ", " << (pPtr->isRequire()?"true":"false") << ");" << endl;

    return s.str();
}

string Jce2Cs::display(const TypeIdPtr &pPtr) const
{
    ostringstream s;
    if(EnumPtr::dynamicCast(pPtr->getTypePtr()))
    {
        s << TAB << "_ds.display((taf::Int32)" << pPtr->getId() << ",\"" << pPtr->getId() << "\");" << endl;;
    }
    else
    {
        s << TAB << "_ds.display(" << pPtr->getId() << ",\"" << pPtr->getId() << "\");" << endl;;
    }

    return s.str();
}

string Jce2Cs::toTypeInit(const TypePtr &pPtr) const
{
    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
    if(bPtr)
    {
        switch(bPtr->kind())
        {
            case Builtin::KindBool:
                return "false;";
            case Builtin::KindByte:
                return "0;";
            case Builtin::KindShort:
                return "0;";
            case Builtin::KindInt:
                return "0;";
            case Builtin::KindLong:
                return "0L;";
            case Builtin::KindFloat:
                return "0.0f;";
            case Builtin::KindDouble:
                return "0.0;";
            case Builtin::KindString:
                return "\"\";";
            default:
                return "";
        }
    }

    VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
    //if(vPtr) return "new " + tostrVector(vPtr) + "();";
    //if(vPtr) return "(" + toObjStr(vPtr->getTypePtr()) + "[]) new Object[1];";
    if(vPtr)
    {
        //数组特殊处理
        string sType;
        size_t iPosBegin, iPosEnd;
        sType = tostr(vPtr->getTypePtr());
        //map<>的数组去掉 '<...>'
        if((iPosBegin = sType.find("<")) != string::npos && (iPosEnd = sType.rfind(">")) != string::npos)
        {
            sType = sType.substr(0, iPosBegin) +  sType.substr(iPosEnd+1);
        }
        //[] (数组)的数组变为[1]
        sType = taf::TC_Common::replace(sType, "[]" , "[1]");
        return "(" + tostr(vPtr->getTypePtr()) + "[]) new " + sType + "[1];";;
    }

    MapPtr mPtr = MapPtr::dynamicCast(pPtr);
    if(mPtr) return "new " + tostrMap(mPtr, true) + "();";

    StructPtr sPtr = StructPtr::dynamicCast(pPtr);
    if(sPtr) return "new " + tostrStruct(sPtr) + "();";

    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
    if(ePtr) return "0;";

    return "";
}


string Jce2Cs::toObjStr(const TypePtr &pPtr) const
{
    string sType = tostr(pPtr);

    if(sType == "boolean") return "boolean";
    if(sType == "byte")    return "byte";
    if(sType == "short" )  return "short";
    if(sType == "int" )    return "int";
    if(sType == "long" )   return "long";
    if(sType == "float" )  return "float";
    if(sType == "double" ) return "double";

    return sType;
}

bool Jce2Cs::isObjType(const TypePtr &pPtr) const
{
    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
    if(ePtr)
    {
        return false;
    }

    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
    if(bPtr)
    {
        switch(bPtr->kind())
        {
            case Builtin::KindBool:
                return false;
            case Builtin::KindByte:
                return false;
            case Builtin::KindShort:
                return false;
            case Builtin::KindInt:
                return false;
            case Builtin::KindLong:
                return false;
            case Builtin::KindFloat:
                return false;
            case Builtin::KindDouble:
                return false;
            default:
                return true;
        }
    }

    return true;
}

string Jce2Cs::tostr(const TypePtr &pPtr) const
{
    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
    if(bPtr) return tostrBuiltin(bPtr);

    VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
    if(vPtr) return tostrVector(vPtr);

    MapPtr mPtr = MapPtr::dynamicCast(pPtr);
    if(mPtr) return tostrMap(mPtr);

    StructPtr sPtr = StructPtr::dynamicCast(pPtr);
    if(sPtr) return tostrStruct(sPtr);

    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
    if(ePtr) return tostrEnum(ePtr);

    if(!pPtr) return "void";

    assert(false);
    return "";
}

/*******************************BuiltinPtr********************************/
string Jce2Cs::tostrBuiltin(const BuiltinPtr &pPtr) const
{
    string s;

    switch(pPtr->kind())
    {
        case Builtin::KindBool:
            s = "boolean";
            break;
        case Builtin::KindByte:
            s = "byte";
            break;
        case Builtin::KindShort:
            s = "short";
            break;
        case Builtin::KindInt:
            s = "int";
            break;
        case Builtin::KindLong:
            s = "long";
            break;
        case Builtin::KindFloat:
            s = "float";
            break;
        case Builtin::KindDouble:
            s = "double";
            break;
        case Builtin::KindString:
            s = "string";
            break;
        case Builtin::KindVector:
            s = "System.Collections.Generic.List";
            break;
        case Builtin::KindMap:
            s = "System.Collections.Generic.Dictionary";
            break;
        default:
            assert(false);
            break;
    }

    return s;
}
/*******************************VectorPtr********************************/
string Jce2Cs::tostrVector(const VectorPtr &pPtr) const
{
    string s = "System.Collections.Generic.List" + string("<") + toObjStr(pPtr->getTypePtr()) + ">";
    //string s = tostr(pPtr->getTypePtr()) + "[]";
    return s;
}

/*******************************MapPtr********************************/
string Jce2Cs::tostrMap(const MapPtr &pPtr, bool bNew) const
{
    string s;
    if(!bNew)
    {
        s = "System.Collections.Generic.Dictionary";
    }
    else
    {
        s = "System.Collections.Generic.Dictionary";
    }
    s += string("<") + toObjStr(pPtr->getLeftTypePtr()) + ", " + toObjStr(pPtr->getRightTypePtr()) + ">";

    return s;
}

string Jce2Cs::tostrStruct(const StructPtr &pPtr) const
{
    //return _packagePrefix + taf::TC_Common::replace(pPtr->getSid(), "::", ".");
    //return taf::TC_Common::replace(pPtr->getSid(), "::", "");
    return taf::TC_Common::replace(pPtr->getSid(), "::", ".").substr(taf::TC_Common::replace(pPtr->getSid(), "::", ".").find(".")+1);
}

string Jce2Cs::tostrEnum(const EnumPtr &pPtr) const
{
    return "int";
}

/////////////////////////////////////////////////////////////////////

string Jce2Cs::MD5(const StructPtr &pPtr) const
{
    string s;
    vector<TypeIdPtr>& member = pPtr->getAllMemberPtr();
    for(size_t j = 0; j < member.size(); j++)
    {
        s += "_" + tostr(member[j]->getTypePtr());
    }

    return "\"" + taf::TC_MD5::md5str(s) + "\"";
}

string Jce2Cs::generateCs(const ParamDeclPtr &pPtr) const
{
    ostringstream s;

    TypePtr typePtr = pPtr->getTypeIdPtr()->getTypePtr();
    if(!pPtr->isOut())//不带out
    {
        s << tostr(typePtr);
        s << " " << pPtr->getTypeIdPtr()->getId();
    }
    else//带out参数
    {
        s << "out "<< tostr(typePtr);
        s << " "<<pPtr->getTypeIdPtr()->getId();
    }

    return s.str();
}

string Jce2Cs::generateHolder(const ParamDeclPtr &pPtr) const
{
    ostringstream s;

    TypePtr typePtr = pPtr->getTypeIdPtr()->getTypePtr();

    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(typePtr);
    if(bPtr) s << HOLDER_PACKAGE".Jce" + taf::TC_Common::upper(tostrBuiltin(bPtr).substr(0,1))
        + tostrBuiltin(bPtr).substr(1) + "Holder";

    VectorPtr vPtr = VectorPtr::dynamicCast(typePtr);
    if(vPtr) s << HOLDER_PACKAGE".JceArrayHolder";

    MapPtr mPtr = MapPtr::dynamicCast(typePtr);
    if(mPtr) s << HOLDER_PACKAGE".JceHashMapHolder";

    StructPtr sPtr = StructPtr::dynamicCast(typePtr);
    if(sPtr) s << _packagePrefix + taf::TC_Common::replace(sPtr->getSid(), "::", ".") + "Holder";

    EnumPtr ePtr = EnumPtr::dynamicCast(typePtr);
    if(ePtr) s << HOLDER_PACKAGE".JceIntHolder";

    return s.str();
}

struct SortOperation
{
    bool operator()(const OperationPtr &o1, const OperationPtr &o2)
    {
        return o1->getId() < o2->getId();
    }
};

string Jce2Cs::generateCs(const InterfacePtr &pPtr, const NamespacePtr &nPtr) const
{
    ostringstream s;

    //生成代理接口
    generatePrx(pPtr, nPtr);

    //生成代理帮助类
    generatePrxHelper(pPtr, nPtr);

    //生成代理回调类
    generatePrxCallback(pPtr, nPtr);

    //生成服务端框架
    if(_bWithServant)
    {
        //generateServant(pPtr, nPtr);
    }

    return s.str();
}

string Jce2Cs::generatePrx(const InterfacePtr &pPtr, const NamespacePtr &nPtr) const
{
    ostringstream s;
    s << g_parse->printHeaderRemark();

    //s << TAB << "namespace " << _packagePrefix << nPtr->getId() << endl;
	s << TAB << "namespace " << nPtr->getId() << endl;
    s << TAB << "{" <<endl;
    INC_TAB;
    s << endl;

    s << TAB << "public interface " << pPtr->getId() << "Prx" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    string sParams;
    vector<OperationPtr>& vOperation = pPtr->getAllOperationPtr();
    std::sort(vOperation.begin(), vOperation.end(), SortOperation());

    //生成客户端接口的声明
    for(size_t i = 0; i < vOperation.size(); i++)
    {
        vector<ParamDeclPtr>& vParamDecl = vOperation[i]->getAllParamDeclPtr();//取到参数

        //1.无context同步调用
        s << TAB << tostr(vOperation[i]->getReturnPtr()->getTypePtr()) << " "
            << vOperation[i]->getId() << "(" ;
        sParams = "";
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            sParams += generateCs(vParamDecl[j]) + ", ";
        }
        s << taf::TC_Common::trimright(sParams, ", ", false) << ");" << endl;
        s << endl;

        //2.带context同步调用
        s << TAB << tostr(vOperation[i]->getReturnPtr()->getTypePtr()) << " "
            << vOperation[i]->getId() << "(" ;
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            s << generateCs(vParamDecl[j]) << ", ";
        }
        s << " System.Collections.Generic.Dictionary<string,string> __ctx);" << endl;
        s << endl;

        //3.无context异步调用
        s << TAB << "void async_" << vOperation[i]->getId() << "(" ;
        s << _packagePrefix << nPtr->getId() << "." << pPtr->getId() << "Prx callback";
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(vParamDecl[j]->isOut()) continue;
            s << ", " << generateCs(vParamDecl[j]);
        }
        s << ");" << endl;
        s << endl;

        //4.带context异步调用
        s << TAB << "void async_" << vOperation[i]->getId() << "(" ;
        s << _packagePrefix << nPtr->getId() << "." << pPtr->getId() << "Prx callback";
        s << ", ";
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(vParamDecl[j]->isOut()) continue;
            s << generateCs(vParamDecl[j]) << ", ";
        }
        s << " System.Collections.Generic.Dictionary<string,string> __ctx);" << endl;
        s << endl << endl;
    }

    DEL_TAB;
    s << TAB << "}" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    string fileJava  = getFilePath(nPtr->getId()) + pPtr->getId() + "Prx.cs";
    taf::TC_File::makeDirRecursive(getFilePath(nPtr->getId()), 0755);
    taf::TC_File::save2file(fileJava, s.str());

    return s.str();
}

string Jce2Cs::generatePrxHelper(const InterfacePtr &pPtr, const NamespacePtr &nPtr) const
{
    ostringstream s;
    s << g_parse->printHeaderRemark();

    //s << TAB << "namespace " << _packagePrefix << nPtr->getId()  << endl;
	s << TAB << "namespace " << nPtr->getId()  << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << endl;

    s << TAB << "public sealed class " << pPtr->getId() << "PrxHelper "
         << " :  "
        <<pPtr->getId() << "Prx"<< endl;
    s << TAB << "{" << endl;
    INC_TAB;

    s << endl;

    //hash
    s << TAB << "public " << pPtr->getId() << "PrxHelper taf_hash(int hashCode)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    //s << TAB << "super.taf_hash(hashCode);" << endl;
    s << TAB << "return this;" << endl;
    DEL_TAB;
    s << TAB << "}"<< endl;
    s << endl;

    //__defaultContext
    s << TAB << "public System.Collections.Generic.Dictionary<string,string> __defaultContext()" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "System.Collections.Generic.Dictionary<string,string> _ctx = new System.Collections.Generic.Dictionary<string, string>();" << endl;
    s << TAB << "return _ctx;" << endl;
    DEL_TAB;
    s << TAB << "}" <<endl;
    s << endl;

    //setServerEncoding
    s << TAB << "string sServerEncoding = \"GBK\";" << endl;
    s << TAB << "public int setServerEncoding(string se){" << endl;
    INC_TAB;
    s << TAB << "sServerEncoding = se;" << endl;
    s << TAB << "return 0;" << endl;
    DEL_TAB;
    s << TAB << "}" <<endl;
    s << endl;

    string sParams;
    vector<OperationPtr>& vOperation = pPtr->getAllOperationPtr();
    std::sort(vOperation.begin(), vOperation.end(), SortOperation());

    //生成客户端接口的实现
    for(size_t i = 0; i < vOperation.size(); i++)
    {
        vector<ParamDeclPtr>& vParamDecl = vOperation[i]->getAllParamDeclPtr();
        TypePtr retTypePtr = vOperation[i]->getReturnPtr()->getTypePtr();

        //1.无context同步调用
        s << TAB << "public " << tostr(retTypePtr) << " "
            << vOperation[i]->getId() << "(" ;
        sParams = "";
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            sParams += generateCs(vParamDecl[j]) + ", ";
        }
        s << taf::TC_Common::trimright(sParams, ", ", false) << ")" << endl;

        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << ((tostr(retTypePtr) == "void") ? "" : "return ")
            << vOperation[i]->getId() << "(";
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(!vParamDecl[j]->isOut())//不带out
            {
                s << vParamDecl[j]->getTypeIdPtr()->getId() << ", ";
            }
            else//带out参数
            {
                s <<"out "<< vParamDecl[j]->getTypeIdPtr()->getId() << ", ";
            }
        }
        s << "__defaultContext());" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;
        s << endl;


        //2.带context同步调用
        string routekey = "";
        s << TAB << "public " << tostr(retTypePtr) << " " << vOperation[i]->getId() << "(" ;
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            s << generateCs(vParamDecl[j]) << ", ";

            if (routekey.empty() && vParamDecl[j]->isRouteKey())
            {
                routekey = vParamDecl[j]->getTypeIdPtr()->getId();
            }
        }
        s << "System.Collections.Generic.Dictionary<string,string> __ctx)" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        //--------------------------add by edwardsu------------------------
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
             if((vParamDecl[j]->isOut()))
             {
                 //如果是out的话就设置为null
                 s<<TAB<< vParamDecl[j]->getTypeIdPtr()->getId() <<"= null;"<<endl; 
             }
        }
        s<<TAB << "return 0;"<<endl;
        //--------------------------end add--------------------------------

        /*s << TAB << JCE_PACKAGE".JceOutputStream _os = new "JCE_PACKAGE".JceOutputStream(0);" << endl;
        s << TAB << "_os.setServerEncoding(sServerEncoding);" << endl;
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(!(vParamDecl[j]->isOut()))
            {
                s << TAB << "_os.write(" << vParamDecl[j]->getTypeIdPtr()->getId() << ", "
                    << taf::TC_Common::tostr(j+1) << ");" << endl;
            }
            else
            {
                if(isObjType(vParamDecl[j]->getTypeIdPtr()->getTypePtr()))
                {
                    s << TAB << "if(" << vParamDecl[j]->getTypeIdPtr()->getId() << ".value != null) ";
                }
                s << TAB << "_os.write(" << vParamDecl[j]->getTypeIdPtr()->getId() << ".value, "
                    << taf::TC_Common::tostr(j+1) << ");" << endl;
            }
        }
        s << TAB << "byte[] _sBuffer = "JCE_PACKAGE".JceUtil.getJceBufArray(_os.getMemoryStream());" << endl;
        s << endl;

        s << TAB << "System.Collections.Generic.Dictionary<string, string> status =  new System.Collections.Generic.Dictionary<string, string>();" << endl;

        if (!routekey.empty())
        {
            s << TAB << "status.put(com.qq.taf.cnst.Const.STATUS_GRID_KEY, " << routekey << ");" << endl;
        }

        s << endl;
        //s << TAB << "byte[] _returnSBuffer = taf_invoke(\"" <<vOperation[i]->getId()
            //<< "\", _sBuffer, __ctx, status);" << endl;
        s << endl;

        s << TAB <<  JCE_PACKAGE".JceInputStream _is = new "JCE_PACKAGE".JceInputStream(_returnSBuffer);" << endl;
        s << TAB << "_is.setServerEncoding(sServerEncoding);" << endl;

        if(tostr(retTypePtr) != "void")
        {
            s << TAB << tostr(retTypePtr) << " _ret = " << toTypeInit(retTypePtr) << endl;
            s << generateDefautElem(retTypePtr, "_ret");
            s << TAB << "_ret = ("<< tostr(retTypePtr)
                <<") _is.read(_ret, 0, true);" << endl;
        }
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(!vParamDecl[j]->isOut()) continue;

            TypePtr outTypePtr = vParamDecl[j]->getTypeIdPtr()->getTypePtr();
            string sOutValue = vParamDecl[j]->getTypeIdPtr()->getId() + ".value";
            s << TAB << sOutValue << " = " << toTypeInit(outTypePtr) << endl;
            s << generateDefautElem(outTypePtr, sOutValue);

            s << TAB << vParamDecl[j]->getTypeIdPtr()->getId() << ".value = ("
                << tostr(vParamDecl[j]->getTypeIdPtr()->getTypePtr()) + ")" << " _is.read("
                << vParamDecl[j]->getTypeIdPtr()->getId() << ".value, "
                << taf::TC_Common::tostr(j+1) << ", true);" << endl;
        }

        s << TAB <<((tostr(retTypePtr) != "void") ? "return _ret;" : "")<< endl;*/

        DEL_TAB;
        s << TAB << "}" << endl << endl;

        //3.无context异步调用
        s << TAB << "public void async_" << vOperation[i]->getId() << "(" ;
        s << _packagePrefix << nPtr->getId() << "." << pPtr->getId() << "Prx callback";
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(vParamDecl[j]->isOut()) continue;
            s << ", " << generateCs(vParamDecl[j]);
        }
        s << ")" << endl;

        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "async_" << vOperation[i]->getId() << "(";
        s << "callback";
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(vParamDecl[j]->isOut()) continue;
            s << ", " << vParamDecl[j]->getTypeIdPtr()->getId();
        }
        s << ", __defaultContext());" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;
        s << endl;

        //4.带context异步调用
        s << TAB << "public void async_" << vOperation[i]->getId() << "(" ;
        s << _packagePrefix << nPtr->getId() << "." << pPtr->getId() << "Prx callback";
        s << ", ";
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(vParamDecl[j]->isOut()) continue;

            if (routekey.empty() && vParamDecl[j]->isRouteKey())
            {
                routekey = vParamDecl[j]->getTypeIdPtr()->getId();
            }

            s << generateCs(vParamDecl[j]) << ", ";
        }
        s << "System.Collections.Generic.Dictionary<string,string> __ctx)" << endl;

        s << TAB << "{" << endl;
        INC_TAB;
        //--------------------------add by edwardsu------------------------
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
             if((vParamDecl[j]->isOut()))
             {
                 //如果是out的话就设置为null
                 s<<TAB<< vParamDecl[j]->getTypeIdPtr()->getId() <<"= null;"<<endl; 
             }
        }
        //--------------------------end add--------------------------------
        /*s << TAB << JCE_PACKAGE".JceOutputStream _os = new "JCE_PACKAGE".JceOutputStream(0);" << endl;
        s << TAB << "_os.setServerEncoding(sServerEncoding);" << endl;
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(vParamDecl[j]->isOut()) continue;

            s << TAB << "_os.write(" << vParamDecl[j]->getTypeIdPtr()->getId() << ", "
                << taf::TC_Common::tostr(j+1) << ");" << endl;
        }
        s << TAB << "byte[] _sBuffer = "JCE_PACKAGE".JceUtil.getJceBufArray(_os.getMemoryStream());" << endl;
        s << endl;

        s << TAB << "System.Collections.Generic.Dictionary<string, string> status = new System.Collections.Generic.Dictionary<string, string>();" << endl;

        if (!routekey.empty())
        {
            s << TAB << "status.put(com.qq.taf.cnst.Const.STATUS_GRID_KEY, " << routekey << ");" << endl;
        }
        s << endl;
        //s << TAB << "taf_invokeAsync(callback, \"" <<vOperation[i]->getId()
            //<< "\", _sBuffer, __ctx, status);" << endl;
        s << endl;*/
        DEL_TAB;
        s << TAB << "}" << endl << endl;
    }

    DEL_TAB;
    s << TAB << "}" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    string fileJava  = getFilePath(nPtr->getId()) + pPtr->getId() + "PrxHelper.cs";
    taf::TC_File::makeDirRecursive(getFilePath(nPtr->getId()), 0755);
    taf::TC_File::save2file(fileJava, s.str());

    return s.str();
}

string Jce2Cs::generatePrxCallback(const InterfacePtr &pPtr, const NamespacePtr &nPtr) const
{
    ostringstream s;
    s << g_parse->printHeaderRemark();

    //s << TAB << "namespace " << _packagePrefix << nPtr->getId()<< endl;
	s << TAB << "namespace " << nPtr->getId()<< endl;
    s << "{" << endl;
    INC_TAB;
    s << endl;

    s << TAB << "public abstract class " << pPtr->getId()<<endl;
        //<< "PrxCallback : "PROXY_PACKAGE".ServantProxyCallback" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    string sParams;
    vector<OperationPtr>& vOperation = pPtr->getAllOperationPtr();
    std::sort(vOperation.begin(), vOperation.end(), SortOperation());

    //function string
    string sFuncArray =  "__" + nPtr->getId() + "__" + pPtr->getId() + "_all";
    s << TAB << "protected string[] " << sFuncArray << " = new string[]" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    for(size_t i = 0; i < vOperation.size(); i++)
    {
        s << TAB << "\"" << vOperation[i]->getId() << "\""
            <<((i < vOperation.size()-1) ? "," : "" )<< endl;
    }
    DEL_TAB;
    s << TAB << "};" << endl;
    s << endl;

    //setServerEncoding
    s << TAB << "string sServerEncoding = \"GBK\";" << endl;
    s << TAB << "public int setServerEncoding(string se){" << endl;
    INC_TAB;
    s << TAB << "sServerEncoding = se;" << endl;
    s << TAB << "return 0;" << endl;
    DEL_TAB;
    s << TAB << "}" <<endl;
    s << endl;


    //生成客户端接口的声明
    for(size_t i = 0; i < vOperation.size(); i++)
    {
        vector<ParamDeclPtr>& vParamDecl = vOperation[i]->getAllParamDeclPtr();
        TypePtr retTypePtr = vOperation[i]->getReturnPtr()->getTypePtr();

        //callback_FUNCTION
        s << TAB << "public abstract void callback_" << vOperation[i]->getId() << "(";

        sParams = ((tostr(retTypePtr) == "void") ? "" : (tostr(retTypePtr) + " _ret, "));
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(!vParamDecl[j]->isOut()) continue;
            sParams += tostr(vParamDecl[j]->getTypeIdPtr()->getTypePtr()) + " "
                + vParamDecl[j]->getTypeIdPtr()->getId() + ", ";
        }
        s << taf::TC_Common::trimright(sParams, ", ", false) << ");" << endl;
        s << endl;

        //callback_FUNCTION_exception
        s << TAB << "public abstract void callback_" << vOperation[i]->getId()
            << "_exception(int _iRet);" << endl;

        s << endl;

    }

    DEL_TAB;
    s << TAB << "}" << endl;

    DEL_TAB;
    s << TAB << "}" << endl;

    string fileJava  = getFilePath(nPtr->getId()) + pPtr->getId() + "PrxCallback.cs";
    taf::TC_File::makeDirRecursive(getFilePath(nPtr->getId()), 0755);
    taf::TC_File::save2file(fileJava, s.str());

    return s.str();
}

/*string Jce2Cs::generateServant(const InterfacePtr &pPtr, const NamespacePtr &nPtr) const
{
    ostringstream s;
    s << g_parse->printHeaderRemark();

    s << TAB << "package " << _packagePrefix << nPtr->getId() << ";" << endl;
    s << endl;

    s << TAB << "public abstract class " << pPtr->getId() << " extends " << SERVER_PACKAGE << ".Servant" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    s << TAB << "private  java.util.Map<string, " << SERVER_PACKAGE ".ResponseHandler> responseHandlerMap=new java.util.HashMap<string, "
        << SERVER_PACKAGE << ".ResponseHandler>();" << endl;
    s << endl;

    string sParams;
    vector<OperationPtr>& vOperation = pPtr->getAllOperationPtr();
    std::sort(vOperation.begin(), vOperation.end(), SortOperation());

    //构造函数
    s << TAB << "public " << pPtr->getId() << "()" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    for(size_t i = 0; i < vOperation.size(); i++)
    {
        s << TAB << "responseHandlerMap.put(\"" << vOperation[i]->getId() << "\", new " << SERVER_PACKAGE <<".ResponseHandler()" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "public " << JCE_PACKAGE << ".JceOutputStream handle(" << JCE_PACKAGE << ".JceInputStream _is, " << SERVER_PACKAGE << ".JceCurrent _jc)" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "return response_" << vOperation[i]->getId() << "(_is, _jc);" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;

        s << TAB << "public " << WUP_PACKAGE << ".UniAttribute handle_wup(" << WUP_PACKAGE << ".UniAttribute _unaIn, " << SERVER_PACKAGE << ".JceCurrent _jc)" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "return response_" << vOperation[i]->getId() <<"_wup"<< "(_unaIn, _jc);" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;

        DEL_TAB;
        s << TAB << "});" << endl;

    }

    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;

    //回应接口
    for(size_t i = 0; i < vOperation.size(); i++)
    {
        vector<ParamDeclPtr>& vParamDecl = vOperation[i]->getAllParamDeclPtr();

        //业务处理的抽象接口
        s << TAB << "public abstract " << tostr(vOperation[i]->getReturnPtr()->getTypePtr()) << " "
            << vOperation[i]->getId() << "(" ;
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            s << generateCs(vParamDecl[j]) << ", ";
        }
        s << SERVER_PACKAGE << ".JceCurrent _jc);" << endl;
        s << endl;

        
        //解包、处理的接口
        s << TAB << "private " << JCE_PACKAGE << ".JceOutputStream response_" <<vOperation[i]->getId() 
            << "(" << JCE_PACKAGE << ".JceInputStream _is, " << SERVER_PACKAGE << ".JceCurrent _jc)" << endl;
        s << TAB << "{" << endl;
        INC_TAB;

        s << TAB << JCE_PACKAGE << ".JceOutputStream _os = null;" << endl;
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(!(vParamDecl[j]->isOut()))
            {
              //  s << TAB << "is.read(" << vParamDecl[j]->getTypeIdPtr()->getId() << ", "
               //     << taf::TC_Common::tostr(j+1) << ", true);" << endl;
                s << TAB << generateCs(vParamDecl[j]) << " = " << toTypeInit(vParamDecl[j]->getTypeIdPtr()->getTypePtr()) << endl;
 
                TypePtr outTypePtr = vParamDecl[j]->getTypeIdPtr()->getTypePtr();
                string sOutValue = vParamDecl[j]->getTypeIdPtr()->getId();
                //s << TAB << sOutValue << " = " << toTypeInit(outTypePtr) << endl;
                s << generateDefautElem(outTypePtr, sOutValue);

                s << TAB << vParamDecl[j]->getTypeIdPtr()->getId() << " = ("
                    << tostr(vParamDecl[j]->getTypeIdPtr()->getTypePtr()) + ")" << " _is.read("
                    << vParamDecl[j]->getTypeIdPtr()->getId() << ", "
                    << taf::TC_Common::tostr(j+1) << ", true);" << endl;
            }
            else
            {
            //    s << TAB << "is.read(" << vParamDecl[j]->getTypeIdPtr()->getId() << ".value, "
             //       << taf::TC_Common::tostr(j+1) << ", false);" << endl;

                s << TAB << generateCs(vParamDecl[j]) << " = new " << generateHolder(vParamDecl[j]) << "();" << endl;

                TypePtr outTypePtr = vParamDecl[j]->getTypeIdPtr()->getTypePtr();
                string sOutValue = vParamDecl[j]->getTypeIdPtr()->getId() + ".value";
                s << TAB << sOutValue << " = " << toTypeInit(outTypePtr) << endl;
                s << generateDefautElem(outTypePtr, sOutValue);

                s << TAB << vParamDecl[j]->getTypeIdPtr()->getId() << ".value = ("
                    << tostr(vParamDecl[j]->getTypeIdPtr()->getTypePtr()) + ")" << " _is.read("
                    << vParamDecl[j]->getTypeIdPtr()->getId() << ".value, "
                    << taf::TC_Common::tostr(j+1) << ", false);" << endl;
            }
 
        }
 
        TypePtr retTypePtr = vOperation[i]->getReturnPtr()->getTypePtr();
        s << TAB << ((tostr(retTypePtr) == "void") ? "" : (tostr(retTypePtr) + " _ret = "));
        s << vOperation[i]->getId() << "(";
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            s << vParamDecl[j]->getTypeIdPtr()->getId() << ", ";
        }
        s << "_jc);" << endl;
        s << TAB << "if(_jc.isResponse())" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "_os = new " << JCE_PACKAGE << ".JceOutputStream(0);" << endl;
        s << TAB << ((tostr(retTypePtr) == "void") ? "" : "_os.write(_ret, 0);") << endl;
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(!vParamDecl[j]->isOut()) continue;
            s << TAB << "_os.write(" << vParamDecl[j]->getTypeIdPtr()->getId() << ".value, "
                << taf::TC_Common::tostr(j+1) << ");" << endl;
        }

        DEL_TAB;
        s << TAB << "}" << endl;

        s << TAB << "return _os;" << endl;

        DEL_TAB;
        s << TAB << "}" << endl;

        s << endl;
        //wup解包、处理的接口
        s << TAB << "private " << WUP_PACKAGE << ".UniAttribute response_" <<vOperation[i]->getId()<<"_wup" 
            << "(" << WUP_PACKAGE << ".UniAttribute _unaIn, " << SERVER_PACKAGE << ".JceCurrent _jc)" << endl;
        s << TAB << "{" << endl;
        INC_TAB;

        s << TAB << WUP_PACKAGE << ".UniAttribute _unaOut = null;" << endl;
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(!(vParamDecl[j]->isOut()))
            {
              //  s << TAB << "is.read(" << vParamDecl[j]->getTypeIdPtr()->getId() << ", "
               //     << taf::TC_Common::tostr(j+1) << ", true);" << endl;
                s << TAB << generateCs(vParamDecl[j]) << " = " << toTypeInit(vParamDecl[j]->getTypeIdPtr()->getTypePtr()) << endl;
 
                TypePtr outTypePtr = vParamDecl[j]->getTypeIdPtr()->getTypePtr();
                string sOutValue = vParamDecl[j]->getTypeIdPtr()->getId();
                //s << TAB << sOutValue << " = " << toTypeInit(outTypePtr) << endl;
                s << generateDefautElem(outTypePtr, sOutValue);

                s << TAB << vParamDecl[j]->getTypeIdPtr()->getId() << " = ("
                    << toObjStr(vParamDecl[j]->getTypeIdPtr()->getTypePtr()) + ")" << " _unaIn.get(\""
                    << vParamDecl[j]->getTypeIdPtr()->getId()+"\");" << endl;
            }
            else
            {
            //    s << TAB << "is.read(" << vParamDecl[j]->getTypeIdPtr()->getId() << ".value, "
             //       << taf::TC_Common::tostr(j+1) << ", false);" << endl;

                s << TAB << generateCs(vParamDecl[j]) << " = new " << generateHolder(vParamDecl[j]) << "();" << endl;

                TypePtr outTypePtr = vParamDecl[j]->getTypeIdPtr()->getTypePtr();
                string sOutValue = vParamDecl[j]->getTypeIdPtr()->getId() + ".value";
                s << TAB << sOutValue << " = " << toTypeInit(outTypePtr) << endl;
                s << generateDefautElem(outTypePtr, sOutValue);

                s << TAB << vParamDecl[j]->getTypeIdPtr()->getId() << ".value = ("
                    << toObjStr(vParamDecl[j]->getTypeIdPtr()->getTypePtr()) + ")" << " _unaIn.get(\""
                    << vParamDecl[j]->getTypeIdPtr()->getId() <<"\");" << endl;
            }
 
        }
 
        //TypePtr retTypePtr = vOperation[i]->getReturnPtr()->getTypePtr();
        s << TAB << ((tostr(retTypePtr) == "void") ? "" : (tostr(retTypePtr) + " _ret = "));
        s << vOperation[i]->getId() << "(";
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            s << vParamDecl[j]->getTypeIdPtr()->getId() << ", ";
        }
        s << "_jc);" << endl;
        s << TAB << "if(_jc.isResponse())" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "_unaOut = new " << WUP_PACKAGE << ".UniAttribute();" << endl;
        s << TAB << ((tostr(retTypePtr) == "void") ? "" : "_unaOut.put(\"\",_ret);") << endl;
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(!vParamDecl[j]->isOut()) continue;
            s << TAB << "_unaOut.put(\""+vParamDecl[j]->getTypeIdPtr()->getId()+"\"," << vParamDecl[j]->getTypeIdPtr()->getId() << ".value);" << endl;
        }

        DEL_TAB;
        s << TAB << "}" << endl;

        s << TAB << "return _unaOut;" << endl;

        DEL_TAB;
        s << TAB << "}" << endl;
        s << endl;

        //异步返回的接口
        s << TAB << "protected void async_response_" <<vOperation[i]->getId() << "(" << SERVER_PACKAGE << ".JceCurrent _jc";
        s << ((tostr(retTypePtr) == "void") ? "" : (", " + tostr(retTypePtr) + " _ret"));
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(!vParamDecl[j]->isOut()) continue;
            s << ", " << generateCs(vParamDecl[j]);
        }
        s << ")" << endl;
        s << TAB << "{" << endl;
        INC_TAB;

        s << TAB << "if(isWupRequest(_jc)){" << endl;
        INC_TAB;
        s << TAB << "async_response_"<<vOperation[i]->getId()<<"_wup(_jc";
        s <<", _ret";
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(!vParamDecl[j]->isOut()) continue;
            s << ", " << vParamDecl[j]->getTypeIdPtr()->getId();
        } 
        s<<");"<< endl;
        DEL_TAB;
        s << TAB << "}else{" << endl;
        INC_TAB;
        s << TAB << JCE_PACKAGE << ".JceOutputStream _os = new " << JCE_PACKAGE << ".JceOutputStream(0);" << endl;
        s << TAB << ((tostr(retTypePtr) == "void") ? "" : "_os.write(_ret, 0);") << endl;
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(!vParamDecl[j]->isOut()) continue;
            s << TAB << "_os.write(" << vParamDecl[j]->getTypeIdPtr()->getId() << ".value, "
                << taf::TC_Common::tostr(j+1) << ");" << endl;
        }
        s << TAB << "sendResponseMessage(_jc, _os);" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;

        DEL_TAB;
        s << TAB << "}" << endl;

        s << endl;
    

       //异步返回的接口 wup
        s << TAB << "protected void async_response_" <<vOperation[i]->getId() << "_wup(" << SERVER_PACKAGE << ".JceCurrent _jc";
        s << ((tostr(retTypePtr) == "void") ? "" : (", " + tostr(retTypePtr) + " _ret"));
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(!vParamDecl[j]->isOut()) continue;
            s << ", " << generateCs(vParamDecl[j]);
        }
        s << ")" << endl;
        s << TAB << "{" << endl;
        INC_TAB;

        s << TAB << WUP_PACKAGE << ".UniAttribute _unaOut = new " << WUP_PACKAGE << ".UniAttribute();" << endl;
        s << TAB << ((tostr(retTypePtr) == "void") ? "" : "_unaOut.put(\"\",_ret);") << endl;
        for(size_t j = 0; j < vParamDecl.size(); j++)
        {
            if(!vParamDecl[j]->isOut()) continue;
            s << TAB << "_unaOut.put(\""+vParamDecl[j]->getTypeIdPtr()->getId()+"\"," << vParamDecl[j]->getTypeIdPtr()->getId() << ".value);" << endl;
        }
        s << TAB << "sendResponseMessage_wup(_jc, _unaOut);" << endl;

        DEL_TAB;
        s << TAB << "}" << endl;

        s << endl;
    }

    s << TAB << "@Override" << endl;
    s << TAB << "protected " << JCE_PACKAGE << ".JceOutputStream doResponse(string funcName, "
        << JCE_PACKAGE << ".JceInputStream _is, " << SERVER_PACKAGE << ".JceCurrent _jc)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "return responseHandlerMap.get(funcName).handle(_is, _jc);" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;

    //for wup
    s << TAB << "@Override" << endl;
    s << TAB << "protected " << WUP_PACKAGE << ".UniAttribute doResponse_wup(string funcName, "
        << WUP_PACKAGE << ".UniAttribute _unaIn, " << SERVER_PACKAGE << ".JceCurrent _jc)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "return responseHandlerMap.get(funcName).handle_wup(_unaIn, _jc);" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    DEL_TAB;
    s << TAB << "}" << endl;

    string fileJava  = getFilePath(nPtr->getId()) + pPtr->getId() + ".cs";
    taf::TC_File::makeDirRecursive(getFilePath(nPtr->getId()), 0755);
    taf::TC_File::save2file(fileJava, s.str());

    return s.str();
}*/


/*string Jce2Cs::generateHolder(const StructPtr &pPtr, const NamespacePtr &nPtr) const
{
    ostringstream s;
    s << g_parse->printHeaderRemark();

    //s << TAB << "namespace " << _packagePrefix << nPtr->getId()<< endl;
	s << TAB << "namespace " << nPtr->getId()<< endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "public sealed class " << pPtr->getId() << "Holder" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "public " << pPtr->getId() << "Holder()" << endl;
    s << TAB << "{" << endl;
    s << TAB << "}" << endl;
    s << TAB << "public " << pPtr->getId() << "Holder(" << pPtr->getId() << " value)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "this.value = value;" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;
    s << TAB << "public " << pPtr->getId() << " value;" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    string fileCs  = getFilePath(nPtr->getId()) + pPtr->getId() + "Holder.cs";
    taf::TC_File::makeDirRecursive(getFilePath(nPtr->getId()), 0755);
    taf::TC_File::save2file(fileCs, s.str());

    return s.str();
}*/

string Jce2Cs::generateDefautElem(const TypePtr &pPtr, const string & sElemName) const
{
    static int iCount = 0;
    ostringstream s;

    if(sElemName.empty())
    {
        iCount++;
    }

    VectorPtr vPtr = VectorPtr::dynamicCast(pPtr);
    if(vPtr)
    {
        int iId = iCount;
        string sVar = sElemName.empty() ? "__var_" + taf::TC_Common::tostr(iId) : sElemName;
        if(sElemName.empty())
        {
            s << TAB << toObjStr(vPtr) << " " << sVar
                << " = " << toTypeInit(vPtr) << endl;
        }

        s << generateDefautElem(vPtr->getTypePtr(), "");

        //s << TAB << sVar << ".add(__var_" << taf::TC_Common::tostr(iId+1) << ");" << endl;
        s << TAB << "((" << tostr(vPtr->getTypePtr()) << "[])" << sVar
            << ")[0] = __var_" << taf::TC_Common::tostr(iId+1) << ";" << endl;

        return s.str();
    }

    MapPtr mPtr = MapPtr::dynamicCast(pPtr);
    if(mPtr)
    {
        int iId = iCount;
        string sVar = sElemName.empty() ? "__var_" + taf::TC_Common::tostr(iId) : sElemName;

        if(sElemName.empty())
        {
            s << TAB << toObjStr(mPtr) << " " << sVar
                << " = " << toTypeInit(mPtr) << endl;
        }

        s << generateDefautElem(mPtr->getLeftTypePtr(), "");
        int iIdRight = iCount+1;
        s << generateDefautElem(mPtr->getRightTypePtr(), "");

        s << TAB << sVar << ".put(__var_" << taf::TC_Common::tostr(iId+1) << ", __var_"
            << taf::TC_Common::tostr(iIdRight) << ");" << endl;

        return s.str();
    }

    //首个非map、vector调用，不用生成默认元素
    if(!sElemName.empty())
    {
        return s.str();
    }

    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr);
    if(bPtr)
    {
        s << TAB << toObjStr(bPtr) << " __var_" << taf::TC_Common::tostr(iCount)
            << " = " << toTypeInit(bPtr) << endl;
        return s.str();
    }

    StructPtr sPtr = StructPtr::dynamicCast(pPtr);
    if(sPtr)
    {
        s << TAB << toObjStr(sPtr) << " __var_" << taf::TC_Common::tostr(iCount)
            << " = " << toTypeInit(sPtr) << endl;
        return s.str();
    }

    EnumPtr ePtr = EnumPtr::dynamicCast(pPtr);
    if(ePtr)
    {
        s << TAB << toObjStr(ePtr) << " __var_" << taf::TC_Common::tostr(iCount)
            << " = " << toTypeInit(ePtr) << endl;
        return s.str();
    }

    return s.str();
}


/******************************StructPtr***************************************/
string Jce2Cs::generateCs(const StructPtr &pPtr, const NamespacePtr &nPtr) const
{
    //生成Holder类
    //generateHolder(pPtr, nPtr);

    ostringstream s;
    s << g_parse->printHeaderRemark();

    vector<string> key = pPtr->getKey();
    vector<TypeIdPtr>& member = pPtr->getAllMemberPtr();

    s << TAB << "using System;"<<endl;
    //s << TAB << "namespace " << _packagePrefix << nPtr->getId() << endl;
    s << TAB << "namespace "<< nPtr->getId()<< endl;
	s << "{"<<endl;
    INC_TAB;
    s << endl;

    s << TAB << "public sealed class " << pPtr->getId()
        << " : "JCE_PACKAGE".JceStruct"<< endl;
    s << TAB << "{" << endl;
    INC_TAB;

    //生成结构名称
    /*s << TAB << "public string className()" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "return " << "\"" << nPtr->getId() << "." << pPtr->getId() << "\"" << ";" << endl;
    DEL_TAB; 
    s << TAB << "}" << endl;
    s << endl;*/

    //定义成员变量
    for(size_t i = 0; i < member.size(); i++)
    {
        string sDefalut;
        if(member[i]->hasDefault())
        {
            BuiltinPtr bPtr  = BuiltinPtr::dynamicCast(member[i]->getTypePtr());
            //string转义
            if(bPtr && bPtr->kind() == Builtin::KindString)
            {
                sDefalut = taf::TC_Common::replace(member[i]->def(), "\"", "\\\"");
                //sDefalut = " = (" + tostr(member[i]->getTypePtr()) + ")\"" + sDefalut + "\"";
                sDefalut = " = \"" + sDefalut + "\"";
            }
            else if(bPtr && bPtr->kind() == Builtin::KindInt)
            {
                //sDefalut = " = (" + tostr(member[i]->getTypePtr()) + ")" + member[i]->def();
                sDefalut = " = " + member[i]->def();
            }
            else if(bPtr && bPtr->kind() == Builtin::KindByte)
            {
                //sDefalut = " = (" + tostr(member[i]->getTypePtr()) + ")" + member[i]->def();
                sDefalut = " = " + member[i]->def();
            }
            else if(bPtr && bPtr->kind() == Builtin::KindShort)
            {
                //sDefalut = " = (" + tostr(member[i]->getTypePtr()) + ")" + member[i]->def();
                sDefalut = " = " + member[i]->def();
            }
            else if(bPtr && bPtr->kind() == Builtin::KindBool)
            {
                //sDefalut = " = (" + tostr(member[i]->getTypePtr()) + ")" + member[i]->def();
                sDefalut = " = " + member[i]->def();
            }
            else if(bPtr && bPtr->kind() == Builtin::KindLong)
            {
                //sDefalut = " = (" + tostr(member[i]->getTypePtr()) + ")" + member[i]->def();
                sDefalut = " = " + member[i]->def();
            }
            else if(bPtr && bPtr->kind() == Builtin::KindFloat)
            {
                //sDefalut = " = (" + tostr(member[i]->getTypePtr()) + ")" + member[i]->def();
                sDefalut = " = " + member[i]->def();
            }
            else if(bPtr && bPtr->kind() == Builtin::KindDouble)
            {
                //sDefalut = " = (" + tostr(member[i]->getTypePtr()) + ")" + member[i]->def();
                sDefalut = " = " + member[i]->def();
            }
            else if(bPtr && bPtr->kind() == Builtin::KindVector)
            {
                sDefalut = " = new " + tostr(member[i]->getTypePtr()) + "()";
            }
            else if(bPtr && bPtr->kind() == Builtin::KindMap)
            {
                 sDefalut = " = new " + tostr(member[i]->getTypePtr()) + "()";
            }
            else
            {
                //sDefalut = " error ...";
            }
            s << TAB << "public " << tostr(member[i]->getTypePtr()) << " "<< member[i]->getId() << sDefalut << ";" << endl;
        }
        else//没有默认值 
        {
            EnumPtr ePtr = EnumPtr::dynamicCast(member[i]->getTypePtr());
            if(ePtr)
            {
                s << TAB << "public int "<< member[i]->getId() << " = 0;" << endl;
            }
            else
            {
                sDefalut = " = new " + tostr(member[i]->getTypePtr()) + "();";
                s << TAB << "public " << tostr(member[i]->getTypePtr()) << " "<< member[i]->getId() << sDefalut << endl;
            }
        }
        s << endl;
    }

    //成员变量get/set 
    /*for(size_t i = 0; i < member.size(); i++)
    {
        s << TAB << "public " << tostr(member[i]->getTypePtr()) << " get" <<  taf::TC_Common::upper(member[i]->getId().substr(0, 1)) 
            << member[i]->getId().substr(1) << "()" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "return " <<  member[i]->getId() << ";" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;
        s << endl;

        s << TAB << "public void " << " set" <<  taf::TC_Common::upper(member[i]->getId().substr(0, 1)) 
            << member[i]->getId().substr(1) << "(" << tostr(member[i]->getTypePtr()) << " " << member[i]->getId() << ")" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "this." <<  member[i]->getId() << " = " <<  member[i]->getId() << ";" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;
        s << endl;
    }*/

    //(constructor)()
    s << TAB << "public " << pPtr->getId() << "()" << endl;
    s << TAB << "{" << endl;
    s << TAB << "}" << endl;
    s << endl;

    //(constructor)(...)
    s << TAB << "public " << pPtr->getId() << "(";
    for(size_t i = 0; i < member.size(); i++)
    {
        s << tostr(member[i]->getTypePtr()) << " " << member[i]->getId()
            << ( (i < member.size() - 1) ? ", " : "" );
    }
    s << ")" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    for(size_t i = 0; i < member.size(); i++)
    {
        s << TAB << "this." << member[i]->getId() << " = " << member[i]->getId() << ";" << endl;
    }
    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;

	//newInit()
    s << TAB << "public override Com.QQ.Taf.Jce.JceStruct newInit()" <<endl;
    s << TAB << "{" << endl;
    INC_TAB;
	s << TAB << "return new "<< pPtr->getId()<<"();"<< endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;

    //writeTo()
    s << TAB << "public override void writeTo("JCE_PACKAGE".JceOutputStream _os)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    for(size_t i = 0; i < member.size(); i++)
    {
        s << TAB << "_os.write(" << member[i]->getId()
            << ", " << member[i]->getTag() << ");" << endl;
    }
    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;

    //readFrom()
    s << TAB << "public override void readFrom("JCE_PACKAGE".JceInputStream _is)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    for(size_t i = 0; i < member.size(); i++)
    {
        //BuiltinPtr bPtr  = BuiltinPtr::dynamicCast(member[i]->getTypePtr());
        if(strstr(tostr(member[i]->getTypePtr()).c_str(),"System"))
        {
              s << TAB << member[i]->getId() << ".Clear();" <<endl;
              s << TAB << member[i]->getId() << " = (" + tostr(member[i]->getTypePtr()) + ")"
                 << "_is.read("<<member[i]->getId()<<", "<<member[i]->getTag() << ", " 
                 << (member[i]->isRequire() ? "true" : "false") << ");" << endl;
              s << endl;
        }
        else
        {
            s << TAB << member[i]->getId() << " = " << toTypeInit(member[i]->getTypePtr()) << endl;   
            s << TAB << member[i]->getId() << " = (" + tostr(member[i]->getTypePtr()) + ")"
                << " _is.read(" << member[i]->getId()
                << ", " << member[i]->getTag() << ", " << (member[i]->isRequire() ? "true" : "false") << ");" << endl;
            s << endl;
        }
    }

    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;

    //display()
    s << TAB << "public override void display(System.Text.StringBuilder _os, int _level)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << JCE_PACKAGE".JceDisplayer _ds = new "JCE_PACKAGE".JceDisplayer(_os, _level);" << endl;
    for(size_t i = 0; i < member.size(); i++)
    {
        s << TAB << "_ds.display(" << member[i]->getId()
            << ", \"" << member[i]->getId() << "\");" << endl;
    }
    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;

    DEL_TAB;
    s << TAB << "}" << endl;
    DEL_TAB; 
    s << TAB << "}" << endl;
    s << endl;

    string fileCs  = getFilePath(nPtr->getId()) + pPtr->getId() + ".cs";
    taf::TC_File::makeDirRecursive(getFilePath(nPtr->getId()), 0755);
    taf::TC_File::save2file(fileCs, s.str());

    return s.str();
}

/******************************EnumPtr***************************************/
/*string Jce2Cs::generateCs(const EnumPtr &pPtr, const NamespacePtr &nPtr) const
{
    ostringstream s;
    s << g_parse->printHeaderRemark();

    s << TAB << "using System;"<<endl;
    s << TAB << "namespace " << _packagePrefix << nPtr->getId() << endl;
    s << "{"<<endl;
    INC_TAB;
    s << endl;

    s << TAB << "public sealed class " << pPtr->getId()
        << " : "JCE_PACKAGE".JceStruct"<< endl;
    s << TAB << "{" << endl;
    INC_TAB;

    //成员变量
    vector<TypeIdPtr>& member = pPtr->getAllMemberPtr();
    s << TAB << "private static " << pPtr->getId()
        <<"[] __values = new " << pPtr->getId()
        << "[" << taf::TC_Common::tostr(member.size()) << "];" << endl;
    s << TAB << "private int __value;" << endl;

    s << TAB << "private string __T = null;" << endl;
    s << endl;

    bool bFlag = false; 
    string sValue;
    for(size_t i = 0; i < member.size(); i++)
    {
        s << TAB << "public const int _" << member[i]->getId()<< " = ";
        if(member[i]->hasDefault())
        {
            bFlag = true;
            sValue = member[i]->def();
            s << sValue;
        }
        else
        {
            if(bFlag == true)
            {    
                assert(i>0);
                string stem = taf::TC_Common::tostr(taf::TC_Common::strto<int>(sValue)+1);
                sValue = stem;
                s << stem;               
            }
            else
            {
                s << taf::TC_Common::tostr(i);
            }
        }
        s <<";"<<endl;
        s << TAB << "public const " << pPtr->getId() << " "
            << member[i]->getId() << " = new " << pPtr->getId() << "(_"
            << member[i]->getId() << ",\""<<member[i]->getId()<<"\");" << endl;
    }
    s << endl;


    //convert(int)
    s << TAB << "public static " << pPtr->getId() << " convert(int val)"<< endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "for(int __i = 0; __i < __values.Length; ++__i)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "if(__values[__i].value() == val)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "return __values[__i];" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    s << TAB << "System.Diagnostics.Debug.Assert(false);" << endl;
    s << TAB << "return null;" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;

    //convert(String)
    s << TAB << "public static " << pPtr->getId() << " convert(string val)"<< endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "for(int __i = 0; __i < __values.Length; ++__i)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "if(__values[__i].toString().Equals(val))" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "return __values[__i];" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    s << TAB << "System.Diagnostics.Debug.Assert(false);" << endl;
    s << TAB << "return null;" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;

    //value()
    s << TAB << "public int value()"<< endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "return __value;" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;

    //toString()
    s << TAB << "public string toString()"<< endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "return __T;" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;

    //(constructor)(int)
    s << TAB << "private " << pPtr->getId() << "(int val, string s)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "__T = s;" << endl;
    s << TAB << "__value = val;" << endl;
    s << TAB << "__values[val] = this;" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;

    DEL_TAB;
    s << TAB << "}" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    string fileJava  = getFilePath(nPtr->getId()) + pPtr->getId() + ".cs";
    taf::TC_File::makeDirRecursive(getFilePath(nPtr->getId()), 0755);
    taf::TC_File::save2file(fileJava, s.str());


    return s.str();
}*/

/******************************ConstPtr***************************************/
void Jce2Cs::generateCs(const vector<EnumPtr> &es,const vector<ConstPtr> &cs,const NamespacePtr &nPtr) const
{
    if(es.size()==0 && cs.size()==0) 
    {
        return;
    }
    ostringstream s;
    s << g_parse->printHeaderRemark();

    s << TAB << "using System;"<<endl;
    //s << TAB << "namespace " << _packagePrefix << nPtr->getId() << endl;
    s << TAB << "namespace " << nPtr->getId()<< endl;
	s << "{"<<endl;
    INC_TAB;
    
    if(cs.size()>0) 
    {
        s << TAB << "class Const " << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        //-----------------const类型开始------------------------------------
        for(size_t i = 0; i < cs.size(); i++)
        {
            if(cs[i]->getConstTokPtr()->t == ConstTok::STRING)
            {
                string tmp = taf::TC_Common::replace(cs[i]->getConstTokPtr()->v, "\"", "\\\"");
                s  << TAB << "public const "<< tostr(cs[i]->getTypeIdPtr()->getTypePtr()) << " "<<cs[i]->getTypeIdPtr()->getId()<< " = "
                    << tmp << "\";"<< endl;
            }
            else if(tostr(cs[i]->getTypeIdPtr()->getTypePtr()) == "long")
            {
                s  << TAB << "public const "<< tostr(cs[i]->getTypeIdPtr()->getTypePtr()) << " "<<cs[i]->getTypeIdPtr()->getId()<< " = "
                    << cs[i]->getConstTokPtr()->v << "L;" << endl;
            }
            else
            {                
                s  << TAB << "public const "<< tostr(cs[i]->getTypeIdPtr()->getTypePtr()) <<" "<<cs[i]->getTypeIdPtr()->getId()<< " = "
                   << cs[i]->getConstTokPtr()->v << ";" << endl;
            }
        }
        DEL_TAB;
        s << TAB << "}" << endl;
    }
    //-----------------const类型结束--------------------------------
    if(es.size()>0) 
    {
        //-----------------枚举类型开始---------------------------------
        for(size_t i = 0; i < es.size(); i++)
        {
            s << TAB << "public enum "<<es[i]->getId()<<endl;
            s<< TAB <<"{"<<endl;
            INC_TAB;
            vector<TypeIdPtr>& member = es[i]->getAllMemberPtr();
            for(size_t i = 0; i < member.size(); i++)
            {
                 s << TAB << member[i]->getId() <<","<<endl;
            }
            DEL_TAB;
            s<< TAB <<"}"<<endl;
        }
    }
    //-----------------枚举类型结束---------------------------------
    DEL_TAB;
    s << TAB << "}" << endl;

    string fileCs  = getFilePath(nPtr->getId()) + nPtr->getId()+"_const.cs";
    taf::TC_File::makeDirRecursive(getFilePath(nPtr->getId()), 0755);
    taf::TC_File::save2file(fileCs, s.str());

    return;
}

/******************************NamespacePtr***************************************/

void Jce2Cs::generateCs(const NamespacePtr &pPtr) const
{
    //vector<InterfacePtr>    &is    = pPtr->getAllInterfacePtr();
    vector<StructPtr>       &ss    = pPtr->getAllStructPtr();
    vector<EnumPtr>         &es    = pPtr->getAllEnumPtr();
    vector<ConstPtr>        &cs    = pPtr->getAllConstPtr();

    //for(size_t i = 0; i < is.size(); i++)
    //{
       // generateCs(is[i], pPtr);
   // }

    for(size_t i = 0; i < ss.size(); i++)
    {
        generateCs(ss[i], pPtr);
    }

    //for(size_t i = 0; i < es.size(); i++)
    //{
        //generateCs(es[i], pPtr);
    //}

    //for(size_t i = 0; i < cs.size(); i++)
    //{
        //generateCs(cs[i], pPtr);//对于一个namespace只产生一个cs文件来存放所有的Const
    //}
    generateCs(es,cs,pPtr);//c#里面的枚举、const都放到一起。


    return;
}

void Jce2Cs::generateCs(const ContextPtr &pPtr) const
{
    ostringstream s;

    vector<NamespacePtr> namespaces = pPtr->getNamespaces();

    for(size_t i = 0; i < namespaces.size(); i++)
    {
        generateCs(namespaces[i]);
    }

}

void Jce2Cs::createFile(const string &file)
{
    std::vector<ContextPtr> contexts = g_parse->getContexts();
    for(size_t i = 0; i < contexts.size(); i++)
    {
        if(file == contexts[i]->getFileName())
        {
            generateCs(contexts[i]);
        }
    }
}

void Jce2Cs::setBaseDir(const string &dir)
{
    _baseDir = dir;
}


void Jce2Cs::setBasePackage(const string &prefix)
{
    _packagePrefix = prefix;
    if(_packagePrefix.length() != 0
            && _packagePrefix.substr(_packagePrefix.length()-1, 1) != ".")
    {
        _packagePrefix += ".";
		//_packagePrefix = "";
    }
}


string Jce2Cs::getFilePath(const string &ns) const
{
    return _baseDir + "/" + taf::TC_Common::replace(_packagePrefix, ".", "/") + "/" + ns + "/";
}



