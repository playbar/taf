#include "jce2cpp.h"
#include "util/tc_md5.h"
#include "util/tc_file.h"
#include "util/tc_common.h"
#include <string>

#define TAB g_parse->getTab()
#define INC_TAB g_parse->incTab()
#define DEL_TAB g_parse->delTab()

//////////////////////////////////////////////////////////////////////////////////
//
Jce2Cpp::Jce2Cpp():m_bPDU(false), m_bCheckDefault(false)
{

}

string Jce2Cpp::writeTo(const TypeIdPtr &pPtr) const
{
    ostringstream s;
    if(EnumPtr::dynamicCast(pPtr->getTypePtr()))
    {
        s << TAB << "_os.write((taf::Int32)" << pPtr->getId() << ", " << pPtr->getTag() << ");" << endl;
    }
    else if(pPtr->getTypePtr()->isArray())
    {
        s << TAB << "_os.write((const " << tostr(pPtr->getTypePtr()) << " *)"<< pPtr->getId() <<", "<<pPtr->getId() << "Len"<< ", " << pPtr->getTag() << ");" << endl;
    }
    else if(pPtr->getTypePtr()->isPointer())
    {
        s << TAB << "_os.write((const " << tostr(pPtr->getTypePtr()) << ")"<< pPtr->getId() <<", "<<pPtr->getId() << "Len"<< ", " << pPtr->getTag() << ");" << endl;
    }
    else
    {
        MapPtr mPtr = MapPtr::dynamicCast(pPtr->getTypePtr());
        VectorPtr vPtr = VectorPtr::dynamicCast(pPtr->getTypePtr());

        if (!m_bCheckDefault || pPtr->isRequire() || (!pPtr->hasDefault() && !mPtr && !vPtr))
        {
            s << TAB << "_os.write("<< pPtr->getId() << ", " << pPtr->getTag() << ");" << endl;    
        }
        else
        {
            string sDefault = pPtr->def();

            BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr->getTypePtr());
            if (bPtr && bPtr->kind() == Builtin::KindString)
            {
                sDefault = "\"" + taf::TC_Common::replace(pPtr->def(), "\"", "\\\"") + "\"";
            }

            if (mPtr || vPtr)
            {
                s << TAB << "if (" << pPtr->getId() << ".size() > 0)" << endl;
            }
            else
            {
                s << TAB << "if (" << pPtr->getId() << " != " << sDefault << ")" << endl;
            }
            
            s << TAB << "{" << endl;
            INC_TAB;
            s << TAB << "_os.write("<< pPtr->getId() << ", " << pPtr->getTag() << ");" << endl;
            DEL_TAB;
            s << TAB << "}" << endl;
        }
    }

    return s.str();
}

string Jce2Cpp::readFrom(const TypeIdPtr &pPtr, bool bIsRequire) const
{
    ostringstream s;
    if(EnumPtr::dynamicCast(pPtr->getTypePtr()))
    {
        s << TAB << "_is.read((taf::Int32&)" << pPtr->getId();
    }
    else if(pPtr->getTypePtr()->isArray())
    {
        s << TAB << "_is.read("<< pPtr->getId()<<", "<<getSuffix(pPtr)<<", "<< pPtr->getId() << "Len";
    }
    else if(pPtr->getTypePtr()->isPointer())
    {
        s << TAB << pPtr->getId() <<" = ("<<tostr(pPtr->getTypePtr())<<")_is.cur();"<<endl;
        s << TAB << "_is.read("<< pPtr->getId()<<", _is.left(), "<< pPtr->getId() << "Len";
    }
    else
    {
        s << TAB << "_is.read("<< pPtr->getId();
    }

    s << ", " << pPtr->getTag() << ", " << ((pPtr->isRequire() && bIsRequire)?"true":"false") << ");" << endl;

    if(pPtr->getTypePtr()->isPointer())
    s << TAB <<"_is.mapBufferSkip("<<pPtr->getId() << "Len);"<<endl;

    return s.str();
}

string Jce2Cpp::display(const TypeIdPtr &pPtr) const
{
    ostringstream s;
    if(EnumPtr::dynamicCast(pPtr->getTypePtr()))
    {
        s << TAB << "_ds.display((taf::Int32)" << pPtr->getId() << ",\"" << pPtr->getId() << "\");" << endl;;
    }
    else if(pPtr->getTypePtr()->isArray()||pPtr->getTypePtr()->isPointer())
    {
        s << TAB << "_ds.display(" << pPtr->getId() <<", "<< pPtr->getId() << "Len"<< ",\"" << pPtr->getId() << "\");" << endl;
    }
    else
    {
        s << TAB << "_ds.display(" << pPtr->getId() << ",\"" << pPtr->getId() << "\");" << endl;;
    }

    return s.str();
}

string Jce2Cpp::displaySimple(const TypeIdPtr &pPtr, bool bSep) const
{
    ostringstream s;
    if(EnumPtr::dynamicCast(pPtr->getTypePtr()))
    {
        s << TAB << "_ds.displaySimple((taf::Int32)" << pPtr->getId() << ", "
            <<(bSep ? "true" : "false") << ");" << endl;
    }
    else if(pPtr->getTypePtr()->isArray())
    {
        s << TAB << "_ds.displaySimple("<<pPtr->getId() <<", "<< pPtr->getId() << "Len"<< ","
            <<(bSep ? "true" : "false") << ");" << endl;
    }
    else if(pPtr->getTypePtr()->isPointer())
    {
        s << TAB << "_ds.displaySimple(";
        s <<(bSep ?"":("(const "+tostr(pPtr->getTypePtr())+")"));
        s << pPtr->getId() <<", "<< pPtr->getId() << "Len"<< "," << (bSep ? "true" : "false") << ");" << endl;
    }
    else
    {
        s << TAB << "_ds.displaySimple(" << pPtr->getId() << ", "
            <<(bSep ? "true" : "false") << ");" << endl;
    }

    return s.str();
}


/*******************************获取定长数组坐标********************************/
int Jce2Cpp::getSuffix(const TypeIdPtr &pPtr) const
{
    BuiltinPtr bPtr = BuiltinPtr::dynamicCast(pPtr->getTypePtr());
    if(bPtr && bPtr->kind() == Builtin::KindString && bPtr->isArray())
    {
        return bPtr->getSize();  
    }

    VectorPtr vPtr = VectorPtr::dynamicCast(pPtr->getTypePtr());
    if(vPtr && vPtr->isArray())
    {
        return vPtr->getSize();
    }

    return -1;
}

/*******************************定长数组坐标********************************/

string Jce2Cpp::toStrSuffix(const TypeIdPtr &pPtr) const
{
    ostringstream s;

    int i = getSuffix(pPtr);

    if(i >= 0)
    {
         s << "[" << i << "]";
    }
    return s.str();     
}
/*******************************BuiltinPtr********************************/

string Jce2Cpp::tostr(const TypePtr &pPtr) const
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

string Jce2Cpp::tostrBuiltin(const BuiltinPtr &pPtr) const
{
    string s;

    switch(pPtr->kind())
    {
    case Builtin::KindBool:
        s = "taf::Bool";
        break;
    case Builtin::KindByte:
        s ="taf::Char";
        break;
    case Builtin::KindShort: 
    //为了兼容java无unsigned, 编结码时把jce问件中 unsigned char 对应到short
    //c++中需要还原回来
        s = (pPtr->isUnsigned()?"taf::UInt8":"taf::Short");
        break;
    case Builtin::KindInt:
        s = (pPtr->isUnsigned()?"taf::UInt16":"taf::Int32");
        break;
    case Builtin::KindLong:
        s =  (pPtr->isUnsigned()?"taf::UInt32":"taf::Int64");
        break;
    case Builtin::KindFloat:
        s = "taf::Float";
        break;
    case Builtin::KindDouble:
        s = "taf::Double";
        break;
    case Builtin::KindString:
        if(pPtr->isArray())
            s = "taf::Char"; //char a [8];
        else
            s = "std::string";//string a;
        break;
    case Builtin::KindVector:
        s = "std::vector";
        break;
    case Builtin::KindMap:
        s = "std::map";
        break;
    default:
        assert(false);
        break;
    }

    return s;
}
/*******************************VectorPtr********************************/
string Jce2Cpp::tostrVector(const VectorPtr &pPtr) const
{
    //数组类型
    if(pPtr->isArray())
    {
        return tostr(pPtr->getTypePtr());
    }

    //指针类型
    if(pPtr->isPointer())
    {
        return tostr(pPtr->getTypePtr())+" *";
    }

    string s = Builtin::builtinTable[Builtin::KindVector] + string("<") + tostr(pPtr->getTypePtr());

    if(MapPtr::dynamicCast(pPtr->getTypePtr()) || VectorPtr::dynamicCast(pPtr->getTypePtr()))
    {
        s += " >";
    }
    else
    {
        s += ">";
    }
    return s;
}
/*******************************MapPtr********************************/
string Jce2Cpp::tostrMap(const MapPtr &pPtr) const
{
    string s = Builtin::builtinTable[Builtin::KindMap] + string("<") + tostr(pPtr->getLeftTypePtr()) + ", " + tostr(pPtr->getRightTypePtr());
    if(MapPtr::dynamicCast(pPtr->getRightTypePtr()) || VectorPtr::dynamicCast(pPtr->getRightTypePtr()))
    {
        s += " >";
    }
    else
    {
        s += ">";
    }
    return s;
}

/*******************************StructPtr********************************/
string Jce2Cpp::tostrStruct(const StructPtr &pPtr) const
{
    return pPtr->getSid();
}

string Jce2Cpp::MD5(const StructPtr &pPtr) const
{
    string s;
    vector<TypeIdPtr>& member = pPtr->getAllMemberPtr();
    for(size_t j = 0; j < member.size(); j++)
    {
        s += "_" + tostr(member[j]->getTypePtr());
    }

    return "\"" + taf::TC_MD5::md5str(s) + "\"";
}

/////////////////////////////////////////////////////////////////////
string Jce2Cpp::tostrEnum(const EnumPtr &pPtr) const
{
    return pPtr->getSid();
}
///////////////////////////////////////////////////////////////////////
string Jce2Cpp::generateH(const StructPtr &pPtr, const string& namespaceId) const
{
    ostringstream s;

    s << TAB << "struct " << pPtr->getId() << " : public taf::JceStructBase" << endl;
    s << TAB << "{" << endl;
    s << TAB << "public:" << endl;

    INC_TAB;

    s << TAB << "static string className()" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "return " << "\"" << namespaceId << "." << pPtr->getId() << "\"" << ";" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    s << TAB << "static string MD5()" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "return " << MD5(pPtr) << ";" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    ////////////////////////////////////////////////////////////
    //定义缺省构造函数
    s << TAB << pPtr->getId() << "()" << endl;

    vector<TypeIdPtr>& member = pPtr->getAllMemberPtr();
    bool b = false;
    for(size_t j = 0; j < member.size(); j++)
    {

        if(member[j]->getTypePtr()->isArray())
        {
            if(!b) s << TAB << ":"; else s << ",";
            s << member[j]->getId() << "Len(0)";
            b = true;
            continue;
        }

        if( member[j]->getTypePtr()->isPointer())
        {
            if(!b) s << TAB << ":"; else s << ",";
            s << member[j]->getId() << "Len(0),"<<member[j]->getId()<<"(NULL)";
            b = true;
            continue;
        }

        if(member[j]->hasDefault())
        {
            if(!b) s << TAB << ":"; else s << ",";

            BuiltinPtr bPtr  = BuiltinPtr::dynamicCast(member[j]->getTypePtr());
            //string值要转义
            if(bPtr && bPtr->kind() == Builtin::KindString)
            {
                string tmp = taf::TC_Common::replace(member[j]->def(), "\"", "\\\"");
                s << member[j]->getId() << "(\"" << tmp << "\")";
            }
            else
            {
                s << member[j]->getId() << "(" << member[j]->def() << ")";
            }
            b = true;
        }
    }
    if(b)
    {
        s << endl;
    }

    s << TAB << "{" << endl;
    INC_TAB;
    for(size_t j = 0; j < member.size(); j++)
    {
        VectorPtr vPtr   = VectorPtr::dynamicCast(member[j]->getTypePtr());
        if(vPtr)
        {
            BuiltinPtr bPtr  = BuiltinPtr::dynamicCast(vPtr->getTypePtr());
            if(!bPtr ||  (bPtr && bPtr->kind() == Builtin::KindString) ) //非内建类型或者string 类型不能memset
            {
                continue;
            }
        }
        if( !member[j]->getTypePtr()->isArray() )
        {
            continue;
        }
        s << TAB << "memset(" << member[j]->getId() <<", 0, " << "sizeof(" << member[j]->getId() << "));" << endl;
    }
    DEL_TAB;
    s << TAB << "}" << endl;

    ////////////////////////////////////////////////////////////
    s << TAB << "template<typename WriterT>" << endl;
    s << TAB << "void writeTo(taf::JceOutputStream<WriterT>& _os) const" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    for(size_t j = 0; j < member.size(); j++)
    {
        s << writeTo(member[j]);
    }
    DEL_TAB;
    s << TAB << "}" << endl;

    ///////////////////////////////////////////////////////////
    s << TAB << "template<typename ReaderT>" << endl;
    s << TAB << "void readFrom(taf::JceInputStream<ReaderT>& _is)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    for(size_t j = 0; j < member.size(); j++)
    {
        s << readFrom(member[j]);
    }

    DEL_TAB;
    s << TAB << "}" << endl;

    s << TAB << "ostream& display(ostream& _os, int _level=0) const" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "taf::JceDisplayer _ds(_os, _level);" << endl;

    for(size_t j = 0; j < member.size(); j++)
    {
        s << display(member[j]);
    }
    s << TAB << "return _os;" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;


    s << TAB << "ostream& displaySimple(ostream& _os, int _level=0) const" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "taf::JceDisplayer _ds(_os, _level);" << endl;

    for(size_t j = 0; j < member.size(); j++)
    {
        s << displaySimple(member[j], (j!=member.size()-1 ? true : false));
    }
    s << TAB << "return _os;" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;


    DEL_TAB;
    s << TAB << "public:" << endl;
    INC_TAB;

    //定义成员变量
    for(size_t j = 0; j < member.size(); j++)
    {
        if(member[j]->getTypePtr()->isArray() || member[j]->getTypePtr()->isPointer()) //数组类型、指针类型需要定义长度
        {
            s << TAB << "taf::UInt32 " << member[j]->getId() << "Len" << ";" << endl;
        }
        s << TAB << tostr(member[j]->getTypePtr()) << " " << member[j]->getId() << toStrSuffix(member[j])<< ";" << endl;
    }

    DEL_TAB;
    s << TAB << "};" << endl;

    //定义==操作
    s << TAB << "inline bool operator==(const " << pPtr->getId() << "&l, const " << pPtr->getId() << "&r)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "return ";
    for(size_t j = 0; j < member.size(); j++)
    {
        if(member[j]->getTypePtr()->isArray() || member[j]->getTypePtr()->isPointer()) //数组类型、指针类型
        {
            s << "!memcmp(l." << member[j]->getId() << ",r." << member[j]->getId()<<",l."<<member[j]->getId() << "Len)";
        }
        else 
        {
            s << "l." << member[j]->getId() << " == r." << member[j]->getId();
        }
        if(j != member.size() - 1)
        {
            s << " && ";
        }
    }
    s << ";" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    //定义!=
    s << TAB << "inline bool operator!=(const " << pPtr->getId() << "&l, const " << pPtr->getId() << "&r)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "return !(l == r);" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    vector<string> key = pPtr->getKey();
    //定义<
    if(key.size() > 0)
    {
        s << TAB << "inline bool operator<(const " << pPtr->getId() << "&l, const " << pPtr->getId() << "&r)" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        for(size_t i=0; i < key.size(); i++)
        {
            s << TAB << "if(l." << key[i] << " != r." << key[i] << ") ";
            for(size_t z =0; z < member.size(); z++)
            {
                if( key[i] == member[z]->getId() && (member[z]->getTypePtr()->isArray() || member[z]->getTypePtr()->isPointer())) //数组类型、指针类型
                {
                    s << "memcmp(l." << key[i] << ",r." <<key[i] <<",l."<<key[i] << "Len)< 0";
                }
            }
            s << " return (l." << key[i] << " < r." << key[i] << ");" << endl;

        }

        s << TAB << "return false;" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;

        //定义<=
        s << TAB << "inline bool operator<=(const " << pPtr->getId() << "&l, const " << pPtr->getId() << "&r)" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "return !(r < l);" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;

        //定义>
        s << TAB << "inline bool operator>(const " << pPtr->getId() << "&l, const " << pPtr->getId() << "&r)" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "return r < l;" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;

        //定义>=
        s << TAB << "inline bool operator>=(const " << pPtr->getId() << "&l, const " << pPtr->getId() << "&r)" << endl;
        s << TAB << "{" << endl;
        INC_TAB;
        s << TAB << "return !(l < r);" << endl;
        DEL_TAB;
        s << TAB << "}" << endl;
    }

    if (m_bPDU)
    {
        s << TAB << "int struct_" << pPtr->getId() << "_pack(" << pPtr->getId() << " * pst_struct, uint8_t * pui_buff, int32_t * pi_buff_len, pdu_protocol_header * pdu_header);" << endl;
        s << TAB << "int struct_" << pPtr->getId() << "_unpack(uint8_t * pui_buff, int32_t * pi_buff_len, " << pPtr->getId() << " * pst_struct, pdu_protocol_header * pdu_header);" << endl;
        s << TAB << "int struct_" << pPtr->getId() << "_unpack(uint8_t * pui_buff, int32_t * pi_buff_len, " << pPtr->getId() << " * pst_struct, pdu_protocol_header * pdu_header, uint8_t * pui_mapbuff, int32_t * pi_mapbuff_len);" << endl;
    }

    return s.str();
}

string Jce2Cpp::generateCpp(const StructPtr &pPtr, const string& namespaceId) const
{
    ostringstream s;

    //特殊用途：定义互联网的编解码函数
    if (!m_bPDU) return s.str();

    //生成编码函数
    s << TAB << "int struct_" << pPtr->getId() << "_pack(" << pPtr->getId() << " * pst_struct, uint8_t * pui_buff, int32_t * pi_buff_len, pdu_protocol_header * pdu_header)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "try" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "int i_len = pdu_header?*pi_buff_len:0;" << endl;
    s << TAB << "int i_ret = 0;" << endl;
    s << TAB << "if (pdu_header && (i_ret = ::pdu_header_pack1(pdu_header, pui_buff, (uint32_t *)&i_len)) != 0) { return i_ret; }" << endl << endl;

    s << TAB << "taf::JceOutputStream<taf::BufferWriterBuff> os;" << endl;
    s << TAB << "os.setBuffer(reinterpret_cast<char *>(pui_buff + i_len), *pi_buff_len - i_len);" << endl;
    s << TAB << "pst_struct->writeTo(os);" << endl;
    s << TAB << "i_len = i_len + static_cast<int>(os.getLength());" << endl;

    s << TAB << "if (pdu_header && (i_ret = pdu_header_pack2(pui_buff, (uint32_t *)pi_buff_len, (uint32_t *)&i_len)) != 0) { return i_ret; }" << endl;
    s << TAB << "*pi_buff_len = i_len;" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    s << TAB << "catch (taf::JceNotEnoughBuff & e)" << endl;
    s << TAB << "{" << endl;
    s << TAB << "    return -100;" << endl;
    s << TAB << "}" << endl;
    s << TAB << "catch (taf::JceDecodeInvalidValue & e)" << endl;
    s << TAB << "{" << endl;
    s << TAB << "    return -104;" << endl;
    s << TAB << "}" << endl;
    s << TAB << "catch (...)" << endl;
    s << TAB << "{" << endl;
    s << TAB << "    return -1;" << endl;
    s << TAB << "}" << endl;
    s << TAB << "return 0;" << endl;
    DEL_TAB;
    s << TAB << "}" << endl << endl;

    //生成解码函数
    s << TAB << "static int struct_" << pPtr->getId() << "_unpack__(uint8_t * pui_buff, int32_t * pi_buff_len, " << pPtr->getId() << " * pst_struct, pdu_protocol_header * pdu_header, uint8_t * pui_mapbuff, int32_t * pi_mapbuff_len)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "try" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "int i_ret = 0;" << endl;
    s << TAB << "int i_wup_len = pdu_header?*pi_buff_len:0;" << endl;
    s << TAB << "int i_end_len = 0;" << endl;
    s << TAB << "if (pdu_header && (i_ret = pdu_header_unpack(pui_buff, (uint32_t *)&i_wup_len, pdu_header, (uint32_t *)&i_end_len)) != 0) return i_ret;" << endl;
    s << endl;

    s << TAB << "taf::JceInputStream<taf::MapBufferReader> is;" << endl;
    s << TAB << "is.setBuffer(reinterpret_cast<const char*>(pui_buff + i_wup_len), static_cast<size_t>(*pi_buff_len - i_wup_len - i_end_len));" << endl;
    s << TAB << "if (pui_mapbuff && pi_mapbuff_len) is.setMapBuffer((char *)pui_mapbuff, *pi_mapbuff_len);" << endl;
    s << endl;
    s << TAB << "pst_struct->readFrom(is);" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    s << TAB << "catch (taf::JceDecodeMismatch & e)" << endl;
    s << TAB << "{" << endl;
    s << TAB << "    return -101;" << endl;
    s << TAB << "}" << endl;
    s << TAB << "catch (taf::JceDecodeInvalidValue & e)" << endl;
    s << TAB << "{" << endl;
    s << TAB << "    return -104;" << endl;
    s << TAB << "}" << endl;
    s << TAB << "catch (taf::JceDecodeRequireNotExist & e)" << endl;
    s << TAB << "{" << endl;
    s << TAB << "    return -103;" << endl;
    s << TAB << "}" << endl;
    s << TAB << "catch (...)" << endl;
    s << TAB << "{" << endl;
    s << TAB << "    return -1;" << endl;
    s << TAB << "}" << endl;
      
    s << TAB << "return 0;" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    s << TAB << "int struct_" << pPtr->getId() << "_unpack(uint8_t * pui_buff, int32_t * pi_buff_len, " << pPtr->getId() << " * pst_struct, pdu_protocol_header * pdu_header)" << endl;
    s << TAB << "{" << endl;
    s << TAB << "    return struct_" << pPtr->getId() << "_unpack__(pui_buff, pi_buff_len, pst_struct, pdu_header, NULL, NULL);" << endl;
    s << TAB << "}" << endl;
    s << endl;
                        
    s << TAB << "int struct_" << pPtr->getId() << "_unpack(uint8_t * pui_buff, int32_t * pi_buff_len, " << pPtr->getId() << " * pst_struct, pdu_protocol_header * pdu_header, uint8_t * pui_mapbuff, int32_t * pi_mapbuff_len)" << endl;
    s << TAB << "{" << endl;
    s << TAB << "    return struct_" << pPtr->getId() << "_unpack__(pui_buff, pi_buff_len, pst_struct, pdu_header, pui_mapbuff, pi_mapbuff_len);" << endl;
    s << TAB << "}" << endl;

    return s.str();
}


/*******************************ContainerPtr********************************/
string Jce2Cpp::generateH(const ContainerPtr &pPtr) const
{
    ostringstream s;
    for(size_t i = 0; i < pPtr->getAllNamespacePtr().size(); i++)
    {
        s << generateH(pPtr->getAllNamespacePtr()[i]) << endl;
        s << endl;
    }
    return s.str();
}

string Jce2Cpp::generateCpp(const ContainerPtr &pPtr) const
{
    ostringstream s;
    for(size_t i = 0; i < pPtr->getAllNamespacePtr().size(); i++)
    {
        s << generateCpp(pPtr->getAllNamespacePtr()[i]) << endl;
        s << endl;
    }
    return s.str();
}
/******************************ParamDeclPtr***************************************/
string Jce2Cpp::generateH(const ParamDeclPtr &pPtr) const
{
    ostringstream s;

    //输出参数, 或简单类型
    if(pPtr->isOut() || pPtr->getTypeIdPtr()->getTypePtr()->isSimple())
    {
        s << tostr(pPtr->getTypeIdPtr()->getTypePtr());
    }
    else
    {
        //结构, map, vector, string
        s << "const " << tostr(pPtr->getTypeIdPtr()->getTypePtr()) << " &";
    }

    if(pPtr->isOut())
    {
        s << " &";
    }
    else
    {
        s << " ";
    }
    s << pPtr->getTypeIdPtr()->getId();

    return s.str();
}

string Jce2Cpp::generateOutH(const ParamDeclPtr &pPtr) const
{
    if(!pPtr->isOut()) return "";

    ostringstream s;

    //输出参数, 或简单类型
    if(pPtr->getTypeIdPtr()->getTypePtr()->isSimple())
    {
        s << tostr(pPtr->getTypeIdPtr()->getTypePtr())<<" ";
    }
    else
    {
        //结构, map, vector, string
        s << "const " << tostr(pPtr->getTypeIdPtr()->getTypePtr()) << " &";
    }
    s << pPtr->getTypeIdPtr()->getId();

    return s.str();
}

string Jce2Cpp::generateCpp(const ParamDeclPtr &pPtr) const
{
    ostringstream s;

    if(pPtr->isOut() || pPtr->getTypeIdPtr()->getTypePtr()->isSimple())
    {
        s << tostr(pPtr->getTypeIdPtr()->getTypePtr());

        if(pPtr->isOut()) s << " &";
        else s << " ";
    }
    else
    {
        //输入参数
        s << "const " << tostr(pPtr->getTypeIdPtr()->getTypePtr()) << " &";
    }

    s << pPtr->getTypeIdPtr()->getId();

    return s.str();
}

/******************************OperationPtr***************************************/
string Jce2Cpp::generateCpp(const OperationPtr &pPtr, const string &cn) const
{
    ostringstream s;

    //生成函数声明
    s << TAB << tostr(pPtr->getReturnPtr()->getTypePtr()) << " " << cn << "Proxy::" << pPtr->getId() << "(" ;

    vector<ParamDeclPtr>& vParamDecl = pPtr->getAllParamDeclPtr();

    string routekey = "";

    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        s << generateCpp(vParamDecl[i]);
        if(i != vParamDecl.size() - 1)
        {
            s << ", ";
        }
        else
        {
            s << ", const map<string, string> &context)";
        }
        if (routekey.empty() && vParamDecl[i]->isRouteKey()) 
        {
            routekey = vParamDecl[i]->getTypeIdPtr()->getId();
        }
    }
    if(vParamDecl.size() == 0)
    {
        s << "const map<string, string> &context)";
    }

    s << endl;

    s << TAB << "{" << endl;

    INC_TAB;

    s << TAB << "taf::JceOutputStream<taf::BufferWriter> _os;" << endl;

    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        //if(vParamDecl[i]->isOut()) continue;
        s << writeTo(vParamDecl[i]->getTypeIdPtr());
    }

    s << TAB << "taf::ResponsePacket rep;" << endl;

    s << TAB << "std::map<string, string> status;" << endl;

    if (!routekey.empty())
    {
        ostringstream os; 
        
        os << routekey;

        s << TAB << "status.insert(std::make_pair(ServantProxy::STATUS_GRID_KEY, " << os.str() << "));" << endl;
    }

    s << TAB << "taf_invoke(taf::JCENORMAL,\"" << pPtr->getId() << "\", _os.getByteBuffer(), context, status, rep);" << endl;

    if(vParamDecl.size() > 0 || pPtr->getReturnPtr()->getTypePtr())
    {
        s << TAB << "taf::JceInputStream<taf::BufferReader> _is;" << endl;
        s << TAB << "_is.setBuffer(rep.sBuffer);" << endl;
        if(pPtr->getReturnPtr()->getTypePtr())
        {
            s << TAB << tostr(pPtr->getReturnPtr()->getTypePtr()) << " " << pPtr->getReturnPtr()->getId() << ";"  << endl;
            s << readFrom(pPtr->getReturnPtr());
        }
        for(size_t i = 0; i < vParamDecl.size(); i++)
        {
            if(vParamDecl[i]->isOut())
            {
                s << readFrom(vParamDecl[i]->getTypeIdPtr());
            }
        }
        if(pPtr->getReturnPtr()->getTypePtr())
        {
            s << TAB << "return " << pPtr->getReturnPtr()->getId() << ";" << endl;
        }
    }
    DEL_TAB;
    s << TAB << "}" << endl;

    return s.str();
}

string Jce2Cpp::generateCppAsync(const OperationPtr &pPtr, const string &cn) const
{
    ostringstream s;
    //生成函数声明
    s << TAB << "void "<< cn << "Proxy::async_" << pPtr->getId() << "(" ;
    s << cn << "PrxCallbackPtr callback,";

    string routekey = "";

    vector<ParamDeclPtr>& vParamDecl = pPtr->getAllParamDeclPtr();

    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        if (!vParamDecl[i]->isOut())
        {
            s << generateCpp(vParamDecl[i]) << ",";
        }
        if (routekey.empty() && vParamDecl[i]->isRouteKey()) 
        {
            routekey = vParamDecl[i]->getTypeIdPtr()->getId();
        }
    }
    s << "const map<string, string>& context)";
    s << endl;

    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "taf::JceOutputStream<taf::BufferWriter> _os;" << endl;

    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        if(vParamDecl[i]->isOut())
        {
            continue;
        }
        s << writeTo(vParamDecl[i]->getTypeIdPtr());
    }

    s << TAB << "std::map<string, string> status;" << endl;

    if (!routekey.empty())
    {
        ostringstream os; 
        
        os << routekey;

        s << TAB << "status.insert(std::make_pair(ServantProxy::STATUS_GRID_KEY, " << os.str() << "));" << endl;
    }

    s << TAB << "taf_invoke_async(taf::JCENORMAL,\"" << pPtr->getId() << "\", _os.getByteBuffer(), context, status, callback);" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    return s.str();
}
///////////////////////////////////////////////////////////////////////////////////////////////
string Jce2Cpp::generateDispatchAsyncCpp(const OperationPtr &pPtr, const string &cn) const
{
    ostringstream s;
    s << TAB << "if (msg->response.iRet != taf::JCESERVERSUCCESS)" << endl
      << TAB << "{" << endl;

    INC_TAB;
    s << TAB << "callback_" << pPtr->getId() << "_exception(msg->response.iRet);" << endl;
    s << endl;

    s << TAB << "return msg->response.iRet;" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    s << TAB << "taf::JceInputStream<taf::BufferReader> _is;" << endl;
    s << endl;
    vector<ParamDeclPtr>& vParamDecl = pPtr->getAllParamDeclPtr();

    s << TAB << "_is.setBuffer(msg->response.sBuffer);" << endl;

    //对输出参数编码
    if(pPtr->getReturnPtr()->getTypePtr())
    {
        s << TAB << tostr(pPtr->getReturnPtr()->getTypePtr()) << " " << pPtr->getReturnPtr()->getId() << ";" << endl;
        s << readFrom(pPtr->getReturnPtr()) << endl;
    }

    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        if(vParamDecl[i]->isOut())
        {
            s << TAB << tostr(vParamDecl[i]->getTypeIdPtr()->getTypePtr()) << " "
              << vParamDecl[i]->getTypeIdPtr()->getId() << ";" << endl;
            s << readFrom(vParamDecl[i]->getTypeIdPtr());
        }
    }

    //异步回调都无返回值
    s << TAB << "callback_" << pPtr->getId() << "(";
    string sParams;
    if(pPtr->getReturnPtr()->getTypePtr())
    {
        sParams = pPtr->getReturnPtr()->getId() + ", ";
    }
    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        if (vParamDecl[i]->isOut())
        {
            sParams += vParamDecl[i]->getTypeIdPtr()->getId() + ", ";
        }
    }
    s << taf::TC_Common::trimright(sParams, ", ", false) <<  ");" << endl;
    s << TAB << "return taf::JCESERVERSUCCESS;" << endl;

    return s.str();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
string Jce2Cpp::generateHAsync(const OperationPtr &pPtr) const
{
    ostringstream s;
    vector<ParamDeclPtr>& vParamDecl = pPtr->getAllParamDeclPtr();
    s << TAB << "virtual void " << "callback_" << pPtr->getId() << "(";

    string sParams;
    if (pPtr->getReturnPtr()->getTypePtr())
    {
        if(pPtr->getReturnPtr()->getTypePtr()->isSimple())
        {
            sParams = tostr(pPtr->getReturnPtr()->getTypePtr()) + " ret, ";
        }
        else
        {
            //结构, map, vector, string
            sParams =  "const " + tostr(pPtr->getReturnPtr()->getTypePtr()) + "& ret, ";
        }
    }
    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        ParamDeclPtr& pPtr = vParamDecl[i];
        if (pPtr->isOut())
        {
            //输出参数, 或简单类型
            if(pPtr->getTypeIdPtr()->getTypePtr()->isSimple())
            {
                sParams += tostr(pPtr->getTypeIdPtr()->getTypePtr());
            }
            else
            {
                //结构, map, vector, string
                sParams += " const " + tostr(pPtr->getTypeIdPtr()->getTypePtr()) + "&";
            }
            sParams += " " + pPtr->getTypeIdPtr()->getId() + ", ";
        }
    }
    s << taf::TC_Common::trimright(sParams, ", ", false) << ")" << endl;

    s << TAB << "{ throw std::runtime_error(\"callback_" << pPtr->getId() << "() overloading incorrect.\"); }" << endl;
    s << TAB << "virtual void " << "callback_" << pPtr->getId() << "_exception(taf::Int32 ret)" << endl;
    s << TAB << "{ throw std::runtime_error(\"callback_" << pPtr->getId() << "_exception() overloading incorrect.\"); }";
    s << endl;

    return s.str();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
string Jce2Cpp::generateDispatchCpp(const OperationPtr &pPtr, const string &cn) const
{
    ostringstream s;
    s << TAB << "taf::JceInputStream<taf::BufferReader> _is;" << endl;
    s << TAB << "_is.setBuffer(_current->getRequestBuffer());" << endl;

    vector<ParamDeclPtr>& vParamDecl = pPtr->getAllParamDeclPtr();

    string routekey;
    //对输入输出参数编码
    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        s << TAB << tostr(vParamDecl[i]->getTypeIdPtr()->getTypePtr()) << " "
            << vParamDecl[i]->getTypeIdPtr()->getId() << ";" << endl;

        if (routekey.empty() && vParamDecl[i]->isRouteKey()) 
        {
            routekey = vParamDecl[i]->getTypeIdPtr()->getId();
        }
    }


    //WUP的支持
    s << TAB << "if (_current->getRequestVersion() == WUPVERSION || _current->getRequestVersion() == WUPVERSION2)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "UniAttribute<taf::BufferWriter, taf::BufferReader>  tafAttr;" << endl;
	s << TAB << "tafAttr.setVersion(_current->getRequestVersion());" << endl;
    s << TAB << "tafAttr.decode(_current->getRequestBuffer());" << endl;
    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        string sParamName =  vParamDecl[i]->getTypeIdPtr()->getId();
        string sEnum2Int = (EnumPtr::dynamicCast(vParamDecl[i]->getTypeIdPtr()->getTypePtr())) ? "(taf::Int32)" : "";
        if(!vParamDecl[i]->isOut())
        {
            //枚举类型转成int
            if(EnumPtr::dynamicCast(vParamDecl[i]->getTypeIdPtr()->getTypePtr()))
            {
                s << TAB << sParamName << " = (" << tostr(vParamDecl[i]->getTypeIdPtr()->getTypePtr()) 
                    << ") tafAttr.get<taf::Int32>(\"" << sParamName << "\");" << endl;
            }
            else
            {
                s << TAB << "tafAttr.get(\"" << sParamName << "\", " << sParamName << ");" << endl;
            }
        }
        else
        {
            //枚举类型转成int
            if(EnumPtr::dynamicCast(vParamDecl[i]->getTypeIdPtr()->getTypePtr()))
            {
                s << TAB << sParamName << " = (" << tostr(vParamDecl[i]->getTypeIdPtr()->getTypePtr()) 
                    << ") tafAttr.getByDefault<taf::Int32>(\"" << sParamName << "\", " << sEnum2Int << sParamName << ");" << endl;
            }
            else
            {
                s << TAB << "tafAttr.getByDefault(\"" << sParamName << "\", " << sEnum2Int << sParamName << ", " 
                    << sEnum2Int << sParamName << ");" << endl;
            }
        }
    }
    DEL_TAB;
    s << TAB << "}" << endl;
    s << TAB << "else" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    //普通taf请求
    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        s << readFrom(vParamDecl[i]->getTypeIdPtr(), (!vParamDecl[i]->isOut()) );
    }

    DEL_TAB;
    s << TAB << "}" << endl;

    if(!routekey.empty())
    {
        ostringstream os; 
        
        //os << routekey;

        //s << TAB << "status.insert(std::make_pair(ServantProxy::STATUS_GRID_KEY, " << os.str() << "));" << endl;

        //s << TAB <<"SET_MSG_TYPE(req->request.iMessageType, taf::JCEMESSAGETYPEGRID);"<<endl;

        s << TAB << "if( doGridRouter(_current,"<<routekey<<") == 0 ) return taf::JCESERVERSUCCESS;"<<endl;
    }

    if(pPtr->getReturnPtr()->getTypePtr())
    {
        //有返回值的函数调用
        s << TAB << tostr(pPtr->getReturnPtr()->getTypePtr()) << " " << pPtr->getReturnPtr()->getId() << " = " << pPtr->getId() << "(";
    }
    else
    {
        //无返回值的函数调用
        s << TAB << pPtr->getId() << "(";
    }

    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        s << vParamDecl[i]->getTypeIdPtr()->getId();
        if(i != vParamDecl.size() - 1)
            s << ",";
        else
            s << ", _current);" << endl;
    }
    if(vParamDecl.size() == 0)
    {
        s << "_current);";
    }
    s << TAB << "if(_current->isResponse())" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    //WUP输出参数 
    s << TAB << "if (_current->getRequestVersion() == WUPVERSION || _current->getRequestVersion() == WUPVERSION2)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    s << TAB << "UniAttribute<taf::BufferWriter, taf::BufferReader>  tafAttr;" << endl;
	s << TAB << "tafAttr.setVersion(_current->getRequestVersion());" << endl;
    if(pPtr->getReturnPtr()->getTypePtr())
    {
        string sEnum2Int = (EnumPtr::dynamicCast(pPtr->getReturnPtr()->getTypePtr())) ? "(taf::Int32)" : "";
        s << TAB << "tafAttr.put(\"\", " << sEnum2Int << "_ret);" << endl;
    }
    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        string sParamName = vParamDecl[i]->getTypeIdPtr()->getId();
        string sEnum2Int = (EnumPtr::dynamicCast(vParamDecl[i]->getTypeIdPtr()->getTypePtr())) ? "(taf::Int32)" : "";
        if(vParamDecl[i]->isOut())
        {
            s << TAB << "tafAttr.put(\"" << sParamName << "\", " << sEnum2Int << sParamName << ");" << endl;
        }
    }
    s << TAB << "tafAttr.encode(_sResponseBuffer);"<< endl;
 
    DEL_TAB;
    s << TAB << "}" << endl;
    s << TAB << "else" << endl;

    //普通taf调用输出参数
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "taf::JceOutputStream<taf::BufferWriter> _os;" << endl;

    if(pPtr->getReturnPtr()->getTypePtr())
    {
        s << writeTo(pPtr->getReturnPtr());
    }
    //解码输出参数
    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        if(vParamDecl[i]->isOut())
        {
             s << writeTo(vParamDecl[i]->getTypeIdPtr());
        }
    }
    s << TAB << "_os.swap(_sResponseBuffer);" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    s << TAB << "return taf::JCESERVERSUCCESS;" << endl;

    return s.str();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
string Jce2Cpp::generateHAsync(const OperationPtr &pPtr, const string& interfaceId) const
{
    ostringstream s;
    vector<ParamDeclPtr>& vParamDecl = pPtr->getAllParamDeclPtr();

    s << TAB << "void async_" << pPtr->getId() << "(" << interfaceId << "PrxCallbackPtr callback,";

    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        if (!vParamDecl[i]->isOut())
        {
            s << generateH(vParamDecl[i]) << ",";
        }
    }
    s << "const map<string, string> &context = TAF_CONTEXT());";
    s << endl;
    return s.str();
}
/////////////////////////////////////////////////////////////////////////////////////////////////
string Jce2Cpp::generateH(const OperationPtr &pPtr, bool bVirtual, const string& interfaceId) const
{
    ostringstream s;
    vector<ParamDeclPtr>& vParamDecl = pPtr->getAllParamDeclPtr();

    s << TAB;

    if(bVirtual) s << "virtual ";

    s << tostr(pPtr->getReturnPtr()->getTypePtr()) << " " << pPtr->getId() << "(";

    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        s << generateH(vParamDecl[i]) << ",";
    }

    if(bVirtual)
        s << "taf::JceCurrentPtr current) = 0;";
    else
        s << "const map<string, string> &context = TAF_CONTEXT());";

    s << endl;

    if(bVirtual)
    {
        //异步回调
        s << TAB << "static void async_response_" << pPtr->getId() << "(taf::JceCurrentPtr current";
        if(pPtr->getReturnPtr()->getTypePtr())
        {
            s << ", ";
            if(pPtr->getReturnPtr()->getTypePtr()->isSimple())
            {
                s << tostr(pPtr->getReturnPtr()->getTypePtr()) << " ";
            }
            else
            {
                //结构, map, vector, string
                s << "const " << tostr(pPtr->getReturnPtr()->getTypePtr()) << " &";
            }
            s << pPtr->getReturnPtr()->getId();
        }
        for(size_t i = 0; i < vParamDecl.size(); i++)
        {
            if(!vParamDecl[i]->isOut())
                continue;

            s << ", ";
            s << generateOutH(vParamDecl[i]);
        }
        s << ");" << endl;
    }
    return s.str();
}
/******************************InterfacePtr***************************************/
string Jce2Cpp::generateH(const InterfacePtr &pPtr) const
{
    ostringstream s;
    vector<OperationPtr>& vOperation = pPtr->getAllOperationPtr();

    //生成异步回调Proxy
    s << TAB << "/* callback of async proxy for client */" << endl;
    s << TAB << "class " << pPtr->getId() << "PrxCallback: public taf::ServantProxyCallback" << endl;
    s << TAB << "{" << endl;
    s << TAB << "public:" << endl;
    INC_TAB;
    s << TAB << "virtual ~" << pPtr->getId() << "PrxCallback(){}" << endl;

    for(size_t i = 0; i < vOperation.size(); i++)
    {
        s << generateHAsync(vOperation[i]) << endl;
    }

    DEL_TAB;
    s << TAB << "public:" << endl;
    INC_TAB;

    s << TAB << "int onDispatch(taf::ReqMessagePtr msg);" << endl;
    DEL_TAB;
    s << TAB << "};" << endl;

    s << TAB << "typedef taf::TC_AutoPtr<" << pPtr->getId() << "PrxCallback> " << pPtr->getId() << "PrxCallbackPtr;" << endl;
    s << endl;

    //生成客户端代理
    s << TAB << "/* proxy for client */" << endl;
    s << TAB << "class " << pPtr->getId() << "Proxy : public taf::ServantProxy" << endl;
    s << TAB << "{" << endl;
    s << TAB << "public:" << endl;
    INC_TAB;
    s << TAB << "typedef map<string, string> TAF_CONTEXT;" << endl;

    for(size_t i = 0; i < vOperation.size(); i++)
    {
        s << generateH(vOperation[i], false, pPtr->getId());// << endl;
        s << generateHAsync(vOperation[i], pPtr->getId()) << endl;
    }
    s << TAB << pPtr->getId() << "Proxy* taf_hash(int64_t key);" << endl;

    DEL_TAB;
    s << TAB << "};" << endl;

    s << TAB << "typedef taf::TC_AutoPtr<" << pPtr->getId() << "Proxy> " << pPtr->getId() << "Prx;" << endl;
    s << endl;

    //生成服务端Servant
    s << TAB <<  "/* servant for server */" << endl;
    s << TAB << "class " << pPtr->getId() << " : public taf::Servant" << endl;
    s << TAB << "{" << endl;
    s << TAB << "public:" << endl;
    INC_TAB;
    s << TAB << "virtual ~" << pPtr->getId() << "(){}" << endl;

    for(size_t i = 0; i < vOperation.size(); i++)
    {
        s << generateH(vOperation[i], true, pPtr->getId()) << endl;
    }

    DEL_TAB;
    s << TAB << "public:" << endl;
    INC_TAB;

    s << TAB << "int onDispatch(taf::JceCurrentPtr _current, vector<char> &_sResponseBuffer);" << endl;
    DEL_TAB;
    s << TAB << "};" << endl;

    return s.str();
}

string Jce2Cpp::generateAsyncResponseCpp(const OperationPtr &pPtr, const string &cn) const
{
    ostringstream s;
    //异步回调
    s << TAB << "void " << cn << "::async_response_" << pPtr->getId() << "(taf::JceCurrentPtr current" ;

    if(pPtr->getReturnPtr()->getTypePtr())
    {
        s << ", ";
        if(pPtr->getReturnPtr()->getTypePtr()->isSimple())
        {
            s << tostr(pPtr->getReturnPtr()->getTypePtr()) << " ";
        }
        else
        {
            //结构, map, vector, string
            s << "const " << tostr(pPtr->getReturnPtr()->getTypePtr()) << " &";
        }
        s << pPtr->getReturnPtr()->getId();
    }

    vector<ParamDeclPtr>& vParamDecl = pPtr->getAllParamDeclPtr();

    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        if(!vParamDecl[i]->isOut())
            continue;

        s << ", ";
        s << generateOutH(vParamDecl[i]);
    }
    s << ")" << endl;

    s << TAB << "{" << endl;
    INC_TAB;

    //WUP输出参数
    s << TAB << "if (current->getRequestVersion() == WUPVERSION || current->getRequestVersion() == WUPVERSION2)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    s << TAB << "UniAttribute<taf::BufferWriter, taf::BufferReader>  tafAttr;" << endl;
	s << TAB << "tafAttr.setVersion(current->getRequestVersion());" << endl;
    if(pPtr->getReturnPtr()->getTypePtr())
    {
        string sEnum2Int = (EnumPtr::dynamicCast(pPtr->getReturnPtr()->getTypePtr())) ? "(taf::Int32)" : "";
        s << TAB << "tafAttr.put(\"\", " << sEnum2Int << "_ret);" << endl;
    }
    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        string sParamName = vParamDecl[i]->getTypeIdPtr()->getId();
        string sEnum2Int = (EnumPtr::dynamicCast(vParamDecl[i]->getTypeIdPtr()->getTypePtr())) ? "(taf::Int32)" : "";
        if(vParamDecl[i]->isOut())
        {
            s << TAB << "tafAttr.put(\"" << sParamName << "\", " << sEnum2Int << sParamName << ");" << endl;
        }
    }
    s << endl;
    s << TAB << "vector<char> sWupResponseBuffer;" << endl;
    s << TAB << "tafAttr.encode(sWupResponseBuffer);"<< endl;
    s << TAB << "current->sendResponse(taf::JCESERVERSUCCESS, sWupResponseBuffer);" << endl;
 
    DEL_TAB;
    s << TAB << "}" << endl;
    s << TAB << "else" << endl;
    s << TAB << "{" << endl;

    INC_TAB;

    s << TAB << "taf::JceOutputStream<taf::BufferWriter> _os;" << endl;
    if(pPtr->getReturnPtr()->getTypePtr())
    {
        s << writeTo(pPtr->getReturnPtr()) << endl;
    }
    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        if(!vParamDecl[i]->isOut())
            continue;

        s << writeTo(vParamDecl[i]->getTypeIdPtr()) << endl;
    }


//    s << TAB << "current->sendResponse(taf::JCESERVERSUCCESS, string(_os.getBuffer(), _os.getLength()));" << endl;
    s << TAB << "current->sendResponse(taf::JCESERVERSUCCESS, _os.getByteBuffer());" << endl;

    DEL_TAB;
    s << TAB << "}" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    return s.str();
}

struct SortOperation
{
    bool operator()(const OperationPtr &o1, const OperationPtr &o2)
    {
        return o1->getId() < o2->getId();
    }
};

string Jce2Cpp::generateCpp(const InterfacePtr &pPtr, const NamespacePtr &nPtr) const
{
    ostringstream s;
    vector<OperationPtr>& vOperation = pPtr->getAllOperationPtr();

    std::sort(vOperation.begin(), vOperation.end(), SortOperation());

    //生成客户端接口的实现
    for(size_t i = 0; i < vOperation.size(); i++)
    {
        s << generateCpp(vOperation[i], pPtr->getId()) << endl;
        s << generateAsyncResponseCpp(vOperation[i], pPtr->getId()) << endl;
        s << generateCppAsync(vOperation[i], pPtr->getId()) << endl;
    }

    //生成Hash实现
    s << TAB << pPtr->getId() << "Proxy* " << pPtr->getId() << "Proxy::taf_hash(int64_t key)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "return (" << pPtr->getId() + "Proxy*)ServantProxy::taf_hash(key);" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    s << endl;

    string dname    = "__" + nPtr->getId() + "__" + pPtr->getId() + "_all";
    string dispatch =  "static ::std::string " + dname;
    s << TAB << dispatch << "[]=" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    for(size_t i = 0; i < vOperation.size(); i++)
    {
        s << TAB << "\"" << vOperation[i]->getId() << "\"";
        if(i != vOperation.size() - 1)
        {
            s << ",";
        }
        s << endl;
    }

    DEL_TAB;
    s << TAB << "};" << endl;

    s << endl;

    //生成异步回调接口
    s << TAB << "int " << pPtr->getId() << "PrxCallback::onDispatch(taf::ReqMessagePtr msg)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    s << TAB << "pair<string*, string*> r = equal_range(" << dname << ", " << dname << "+" << vOperation.size() << ", msg->request.sFuncName);" << endl;

    s << TAB << "if(r.first == r.second) return taf::JCESERVERNOFUNCERR;" << endl;

    s << TAB << "switch(r.first - " << dname << ")" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    for(size_t i = 0; i < vOperation.size(); i++)
    {
        s << TAB << "case " << i << ":" << endl;
        s << TAB << "{" << endl;
        INC_TAB;

        s << generateDispatchAsyncCpp(vOperation[i], pPtr->getId()) << endl;

        DEL_TAB;
        s << TAB << "}" << endl;
    }

    DEL_TAB;
    s << TAB << "}" << endl;

    s << TAB << "return taf::JCESERVERNOFUNCERR;" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    s << endl;

    //生成服务端接口
    s << TAB << "int " << pPtr->getId() << "::onDispatch(taf::JceCurrentPtr _current, vector<char> &_sResponseBuffer)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    s << TAB << "pair<string*, string*> r = equal_range(" << dname << ", " << dname << "+" << vOperation.size() << ", _current->getFuncName());" << endl;

    s << TAB << "if(r.first == r.second) return taf::JCESERVERNOFUNCERR;" << endl;

    s << TAB << "switch(r.first - " << dname << ")" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    for(size_t i = 0; i < vOperation.size(); i++)
    {
        s << TAB << "case " << i << ":" << endl;
        s << TAB << "{" << endl;
        INC_TAB;

        s << generateDispatchCpp(vOperation[i], pPtr->getId()) << endl;

        DEL_TAB;
        s << TAB << "}" << endl;
    }

    DEL_TAB;
    s << TAB << "}" << endl;

    s << TAB << "return taf::JCESERVERNOFUNCERR;" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    return s.str();
}

/******************************EnumPtr***************************************/

string Jce2Cpp::generateH(const EnumPtr &pPtr) const
{
    ostringstream s;
    s << TAB << "enum " << pPtr->getId() << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    vector<TypeIdPtr>& member = pPtr->getAllMemberPtr();
    for(size_t i = 0; i < member.size(); i++)
    {
        s << TAB << member[i]->getId();
        if(member[i]->hasDefault())
        {
            s << " = " << member[i]->def();
        }
        s << "," << endl;
    }
    DEL_TAB;
    s << TAB << "};" << endl;

    //生成枚举转字符串函数
    s << TAB << "inline string etos" << "(const " <<  pPtr->getId() << " & e)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    s << TAB << "switch(e)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    for(size_t i = 0; i < member.size(); i++)
    {
        s << TAB << "case " <<  member[i]->getId() << ": return "
            << "\"" << member[i]->getId() << "\";" << endl;
    }
    s << TAB << "default: return \"\";" << endl;

    DEL_TAB;
    s << TAB << "}" << endl;
    s << TAB << "return \"\";" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    //生成字符串转枚举函数
    s << TAB << "inline int stoe" << "(const string & s, " <<  pPtr->getId() << " & e)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    for(size_t i = 0; i < member.size(); i++)
    {
        s << TAB << "if(s == \"" << member[i]->getId() << "\")  { e="<< member[i]->getId() << "; return 0;}" << endl;
    }
    s << endl;
    s << TAB << "return -1;" << endl;

    DEL_TAB;
    s << TAB << "}" << endl;


    return s.str();
}

/******************************ConstPtr***************************************/
string Jce2Cpp::generateH(const ConstPtr &pPtr) const
{
    ostringstream s;

    if(pPtr->getConstTokPtr()->t == ConstTok::STRING)
    {
        string tmp = taf::TC_Common::replace(pPtr->getConstTokPtr()->v, "\"", "\\\"");
        s << TAB << "const " << tostr(pPtr->getTypeIdPtr()->getTypePtr()) << " " << pPtr->getTypeIdPtr()->getId() << " = \"" << tmp << "\";"<< endl;
    }
    else
    {
        s << TAB << "const " << tostr(pPtr->getTypeIdPtr()->getTypePtr()) << " " << pPtr->getTypeIdPtr()->getId() << " = " << pPtr->getConstTokPtr()->v 
            << ((tostr(pPtr->getTypeIdPtr()->getTypePtr()) == "taf::Int64") ? "LL;" : ";" )<< endl;
    }

    return s.str();
}
/******************************NamespacePtr***************************************/

string Jce2Cpp::generateH(const NamespacePtr &pPtr) const
{
    ostringstream s;
    vector<InterfacePtr>    &is    = pPtr->getAllInterfacePtr();
    vector<StructPtr>        &ss    = pPtr->getAllStructPtr();
    vector<EnumPtr>            &es    = pPtr->getAllEnumPtr();
    vector<ConstPtr>        &cs    = pPtr->getAllConstPtr();

    s << endl;
    s << TAB << "namespace " << pPtr->getId() << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    for(size_t i = 0; i < cs.size(); i++)
    {
        s << generateH(cs[i]) << endl;
    }

    for(size_t i = 0; i < es.size(); i++)
    {
        s << generateH(es[i]) << endl;
    }

    for(size_t i = 0; i < ss.size(); i++)
    {
        s << generateH(ss[i], pPtr->getId()) << endl;
    }

    s << endl;

    for(size_t i = 0; i < is.size(); i++)
    {
        s << generateH(is[i]) << endl;
        s << endl;
    }

    DEL_TAB;
    s << "}";

    s << endl << endl;

    //定义结构拷贝宏
    for(size_t i = 0; i < ss.size() && !m_bPDU; i++)
    {
        s << "#define "<< pPtr->getId() << "_" << ss[i]->getId() << "_" << "JCE_COPY_STRUCT_HELPER   \\" << endl;
        s << "        ";

        vector<TypeIdPtr>& member = ss[i]->getAllMemberPtr();
        for(size_t j = 0; j < member.size(); j++)
        {   
            s << "jce_copy_struct(a." << member[j]->getId() << ",b." << member[j]->getId() << ");" ;
        }
        s << endl << endl;
    }

    return s.str();
}

string Jce2Cpp::generateCpp(const NamespacePtr &pPtr) const
{
    ostringstream s;
    vector<InterfacePtr>    &is    = pPtr->getAllInterfacePtr();

    s << endl;
    s << TAB << "namespace " << pPtr->getId() << endl;
    s << TAB << "{" << endl;
    s << endl;
    INC_TAB;

    for (size_t i = 0; i < is.size(); i++)
    {
        s << generateCpp(is[i], pPtr) << endl;
        s << endl;
    }

    //如果是互联网特殊应用生成struct的读写.cpp
    vector<StructPtr> & vecStruct = pPtr->getAllStructPtr();
    for (size_t i = 0; i < vecStruct.size() && m_bPDU; i++)
    {
        s << generateCpp(vecStruct[i], pPtr->getId()) << endl;
        s << endl;
    }

    DEL_TAB;
    s << "}";

    return s.str();
}

/******************************Jce2Cpp***************************************/

void Jce2Cpp::generateH(const ContextPtr &pPtr) const
{
    string n        = taf::TC_File::excludeFileExt(taf::TC_File::extractFileName(pPtr->getFileName()));

    string fileH    = m_sBaseDir + "/" + n + ".h";
    string fileCpp  = m_sBaseDir + "/" + n + ".cpp";

    string define   = taf::TC_Common::upper("__" + n + "_h_");

    ostringstream s;

    s << g_parse->printHeaderRemark();

    s << "#ifndef " << define << endl;
    s << "#define " << define << endl;
    s << endl;
    s << "#include <map>" << endl;
    s << "#include <string>" << endl;
    s << "#include <vector>" << endl;
    s << "#include \"jce/Jce.h\"" << endl;

    s << "using namespace std;" << endl;

    vector<string> include = pPtr->getIncludes();
    for(size_t i = 0; i < include.size(); i++)
    {
        s << "#include \"" << g_parse->getHeader() << taf::TC_File::extractFileName(include[i]) << "\"" << endl;
    }

    vector<NamespacePtr> namespaces = pPtr->getNamespaces();

    //名字空间有接口
    for(size_t i = 0; i < namespaces.size(); i++)
    {
        if(namespaces[i]->hasInterface())
        {
            s << "#include \"servant/ServantProxy.h\"" << endl;
            s << "#include \"servant/Servant.h\"" << endl;
            break;
        }
    }

    s << endl;

    if (m_bPDU)
    {
        s << endl;
        s << TAB << "struct pdu_protocol_header;" << endl;
    }

    for(size_t i = 0; i < namespaces.size(); i++)
    {
        s << generateH(namespaces[i]) << endl;
    }

    s << endl;
    s << "#endif" << endl;

    taf::TC_File::makeDirRecursive(m_sBaseDir, 0755);
    taf::TC_File::save2file(fileH, s.str());
}

void Jce2Cpp::generateCpp(const ContextPtr &pPtr) const
{
    string n        = taf::TC_File::excludeFileExt(taf::TC_File::extractFileName(pPtr->getFileName()));
    string fileCpp  = m_sBaseDir + "/" + n + ".cpp";

    string define   = taf::TC_Common::upper("__" + n + "_h_");

    ostringstream s;

    s << g_parse->printHeaderRemark();

    vector<NamespacePtr> namespaces = pPtr->getNamespaces();

    s << "#include \"" << g_parse->getHeader() << n << ".h\"" << endl;
    if (!m_bPDU)
    {
        s << "#include \"jce/wup.h\"" << endl;
        s << "#include \"servant/BaseF.h\"" << endl;
        s << endl;
        s << "using namespace wup;" << endl;
    }
    else
    {
        s << endl;
        s << TAB << "extern int pdu_header_pack1(pdu_protocol_header * pdu_header, uint8_t * pui_buff, uint32_t * pi_buff_len);" << endl;
        s << TAB << "extern int pdu_header_pack2(uint8_t * pui_buff, uint32_t * pi_buff_len, uint32_t * pi_used_len);" << endl;
        s << TAB << "extern int pdu_header_unpack(uint8_t * pui_buff, uint32_t * pi_buff_len, pdu_protocol_header * pdu_header, uint32_t * pi_end_len);" << endl;
    }

    bool bHasInterface = false;
    for(size_t i = 0; i < namespaces.size(); i++)
    {
        s << generateCpp(namespaces[i]) << endl;

        vector<InterfacePtr>    &is    = namespaces[i]->getAllInterfacePtr();
        if(is.size() > 0)
        {
            bHasInterface = true;
        }
    }
    //没有接口的jce不用生成cpp，并且不是互联网特殊应用
    if(!bHasInterface && !m_bPDU)
    {
        return;
    }

    s << endl;

    taf::TC_File::makeDirRecursive(m_sBaseDir, 0755);
    taf::TC_File::save2file(fileCpp, s.str());

}

void Jce2Cpp::createFile(const string &file, const vector<string> &vsCoder)
{
    std::vector<ContextPtr> contexts = g_parse->getContexts();
    for(size_t i = 0; i < contexts.size(); i++)
    {
        if(file == contexts[i]->getFileName())
        {
            if(vsCoder.size() == 0)
            {
                generateH(contexts[i]);
                generateCpp(contexts[i]);
            }
            else
            {
                for(size_t j = 0 ; j < vsCoder.size(); j++)
                {
                    generateCoder(contexts[i], vsCoder[j]);
                }
            }
        }
    }
}

StructPtr Jce2Cpp::findStruct(const ContextPtr &pPtr,const string &id)
{
    string sid = id;

    //在当前namespace中查找
    vector<NamespacePtr> namespaces = pPtr->getNamespaces();
    for(size_t i = 0; i < namespaces.size(); i++)
    {
        NamespacePtr np = namespaces[i];
        vector<StructPtr> structs = np->getAllStructPtr();

        for(size_t i = 0; i < structs.size(); i++)
        {
            if(structs[i]->getSid() == sid)
            {
                return structs[i];
            }
        }
    }

    return NULL;
}


////////////////////////////////
//for coder generating
////////////////////////////////

string Jce2Cpp::generateCoder(const NamespacePtr &pPtr,const string & sInterface) const
{
    ostringstream s;
    vector<InterfacePtr>    &is    = pPtr->getAllInterfacePtr();
    vector<StructPtr>       &ss    = pPtr->getAllStructPtr();
    vector<EnumPtr>         &es    = pPtr->getAllEnumPtr();
    vector<ConstPtr>        &cs    = pPtr->getAllConstPtr();

    s << endl;
    s << TAB << "namespace " << pPtr->getId() << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    for(size_t i = 0; i < cs.size(); i++)
    {
        s << generateH(cs[i]) << endl;
    }

    for(size_t i = 0; i < es.size(); i++)
    {
        s << generateH(es[i]) << endl;
    }

    for(size_t i = 0; i < ss.size(); i++)
    {
        s << generateH(ss[i], pPtr->getId()) << endl;
    }

    s << endl;

    for(size_t i = 0; i < is.size(); i++)
    {
        if(pPtr->getId() + "::" + is[i]->getId() == sInterface)
        {
            s << generateCoder(is[i]) << endl;
            s << endl;
        }
    }

    DEL_TAB;
    s << "}";

    s << endl << endl;

    return s.str();
}

string Jce2Cpp::generateCoder(const InterfacePtr &pPtr) const
{
    ostringstream s;

    vector<OperationPtr>& vOperation = pPtr->getAllOperationPtr();

    //生成编解码类
    s << TAB << "// encode and decode for client" << endl;
    s << TAB << "class " << pPtr->getId() << "Coder" << endl;
    s << TAB << "{" << endl;
    s << TAB << "public:" << endl << endl;
    INC_TAB;
    s << TAB << "typedef map<string, string> TAF_CONTEXT;" << endl << endl;

    s << TAB << "enum enumResult" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    s << TAB << "eJceServerSuccess      = 0," << endl;
    s << TAB << "eJcePacketLess         = 1," << endl;
    s << TAB << "eJcePacketErr          = 2," << endl;
    s << TAB << "eJceServerDecodeErr    = -1," << endl;
    s << TAB << "eJceServerEncodeErr    = -2," << endl;
    s << TAB << "eJceServerNoFuncErr    = -3," << endl;
    s << TAB << "eJceServerNoServantErr = -4," << endl;
    s << TAB << "eJceServerQueueTimeout = -6," << endl;
    s << TAB << "eJceAsyncCallTimeout   = -7," << endl;
    s << TAB << "eJceProxyConnectErr    = -8," << endl;
    s << TAB << "eJceServerUnknownErr   = -99," << endl;

    DEL_TAB;
    s << TAB << "};" << endl << endl;

    for(size_t i = 0; i < vOperation.size(); i++)
    {
        s << generateCoder(vOperation[i]) << endl;
    }

    DEL_TAB;
    s << TAB << "protected:" << endl << endl;
    INC_TAB;
    s << TAB << "static taf::Int32 fetchPacket(const string & in, string & out)" << endl;
    s << TAB << "{" << endl;

    INC_TAB;
    s << TAB << "if(in.length() < sizeof(taf::Int32)) return eJcePacketLess;" << endl;

    s << TAB << "taf::Int32 iHeaderLen;" << endl;
    s << TAB << "memcpy(&iHeaderLen, in.c_str(), sizeof(taf::Int32));" << endl;

    s << TAB << "iHeaderLen = ntohl(iHeaderLen);" << endl;
    s << TAB << "if(iHeaderLen < (taf::Int32)sizeof(taf::Int32) || iHeaderLen > 100000000) return eJcePacketErr;" << endl;
    s << TAB << "if((taf::Int32)in.length() < iHeaderLen) return eJcePacketLess;" << endl;

    s << TAB << "out = in.substr(sizeof(taf::Int32), iHeaderLen - sizeof(taf::Int32)); " << endl;
    s << TAB << "return 0;" << endl;

    DEL_TAB;
    s << TAB << "}" << endl;

    s << endl;
    s << TAB << "static string encodeBasePacket(const string & sServantName, const string & sFuncName, const vector<char> & buffer, "
        <<"const map<string, string>& context = TAF_CONTEXT())" <<endl;
    s << TAB << "{" << endl;
    INC_TAB;

    s << TAB << "taf::JceOutputStream<taf::BufferWriter> os;" << endl;
    s << TAB << "os.write(1, 1);" << endl;
    s << TAB << "os.write(0, 2);" << endl;
    s << TAB << "os.write(0, 3);" << endl; 
    s << TAB << "os.write(0, 4);" << endl;
    s << TAB << "os.write(sServantName, 5);" << endl;
    s << TAB << "os.write(sFuncName, 6);" << endl;
    s << TAB << "os.write(buffer, 7);" << endl;
    s << TAB << "os.write(60, 8);" << endl;
    s << TAB << "os.write(context, 9);" << endl;
    s << TAB << "os.write(map<string, string>(), 10);" << endl;

    s << TAB << "taf::Int32 iHeaderLen;" << endl;
    s << TAB << "iHeaderLen = htonl(sizeof(taf::Int32) + os.getLength());" << endl;
    s << TAB << "string s;" << endl;
    s << TAB << "s.append((const char*)&iHeaderLen, sizeof(taf::Int32));" << endl;
    s << TAB << "s.append(os.getBuffer(), os.getLength());" << endl;

    s << TAB << "return s;" << endl;

    DEL_TAB;
    s << TAB << "}" << endl;

    s << endl;
    s << TAB << "static taf::Int32 decodeBasePacket(const string & in, taf::Int32 & iServerRet, vector<char> & buffer)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;

    s << TAB << "taf::JceInputStream<taf::BufferReader> is;" << endl;
    s << TAB << "is.setBuffer(in.c_str(), in.length());" << endl;
    s << TAB << "is.read(iServerRet, 5, true);" << endl;
    s << TAB << "is.read(buffer, 6, true);" << endl;

    s << TAB << "return 0;" << endl;

    DEL_TAB;
    s << TAB << "}" << endl;

    s << endl;

    DEL_TAB;
    s << TAB << "};" << endl;

    return s.str();
}

string Jce2Cpp::generateCoder(const OperationPtr &pPtr) const
{
    ostringstream s;
    vector<ParamDeclPtr>& vParamDecl = pPtr->getAllParamDeclPtr();

    //编码函数
    s << TAB << "//encode & decode function for '" << pPtr->getId() << "()'"<< endl << endl;
    s << TAB << "static string encode_" << pPtr->getId() << "(const string & sServantName, ";

    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        if(!vParamDecl[i]->isOut())
        {
            s << generateH(vParamDecl[i]) << ",";
        }
    }
    s << endl;
    s << TAB << "    const map<string, string>& context = TAF_CONTEXT())" << endl;
    s << TAB << "{" << endl;

    INC_TAB;
    s << TAB << "try" << endl;
    s << TAB << "{" << endl;

    INC_TAB;
    s << TAB << "taf::JceOutputStream<taf::BufferWriter> _os;" << endl;

    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        if(vParamDecl[i]->isOut()) continue;
        s << writeTo(vParamDecl[i]->getTypeIdPtr());
    }

    s << TAB << "return encodeBasePacket(sServantName, \""<< pPtr->getId() << "\", _os.getByteBuffer(), context);" <<endl;

    DEL_TAB;

    s << TAB << "}" << endl;
    s << TAB << "catch (taf::JceException & ex)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "return \"\";" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    s << endl;

    //解码函数

    s << TAB << "static taf::Int32 decode_" << pPtr->getId() << "(const string & in ";

    if(pPtr->getReturnPtr()->getTypePtr())
    {
        s << ", " << tostr(pPtr->getReturnPtr()->getTypePtr()) << " & _ret ";
    }
    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        if(!vParamDecl[i]->isOut())
            continue;

        s << ", " << generateH(vParamDecl[i]);
    }
    s << ")" <<endl;

    s << TAB << "{" << endl;

    INC_TAB;
    s << TAB << "try" << endl;
    s << TAB << "{" << endl;

    INC_TAB;
    s << TAB << "string out;" << endl;
    s << TAB << "taf::Int32 iRet = 0;" << endl;
    s << TAB << "if((iRet = fetchPacket(in, out)) != 0) return iRet;" << endl;

    s << TAB << "taf::JceInputStream<taf::BufferReader> _is;" << endl;
    s << TAB << "taf::Int32 iServerRet=0;" << endl;
    s << TAB << "vector<char> buffer;" << endl;
    s << TAB << "decodeBasePacket(out, iServerRet, buffer);" << endl;
    s << TAB << "if(iServerRet != 0)  return iServerRet;" << endl;

    s << TAB << "_is.setBuffer(buffer);" << endl;

    if(pPtr->getReturnPtr()->getTypePtr())
    {
        s << readFrom(pPtr->getReturnPtr());
    }

    for(size_t i = 0; i < vParamDecl.size(); i++)
    {
        if(vParamDecl[i]->isOut())
        {
            s << readFrom(vParamDecl[i]->getTypeIdPtr());
        }
    }


    s << TAB << "return 0;" << endl;

    DEL_TAB;
    s << TAB << "}" << endl;
    s << TAB << "catch (taf::JceException & ex)" << endl;
    s << TAB << "{" << endl;
    INC_TAB;
    s << TAB << "return eJcePacketErr;" << endl;
    DEL_TAB;
    s << TAB << "}" << endl;

    DEL_TAB;
    s << TAB << "}" << endl;


    s << endl;

    return s.str();
}

void Jce2Cpp::generateCoder(const ContextPtr &pPtr,const string &sInterface) const
{
    cout<< "Interface:" << sInterface <<endl;
    string n        = taf::TC_File::excludeFileExt(taf::TC_File::extractFileName(pPtr->getFileName())) + "Coder";

    string fileH    = m_sBaseDir + "/" + n + ".h";

    string define   = taf::TC_Common::upper("__" + n + "_h_");

    ostringstream s;

    s << g_parse->printHeaderRemark();

    s << "#ifndef " << define << endl;
    s << "#define " << define << endl;
    s << endl;
    s << "#include <map>" << endl;
    s << "#include <string>" << endl;
    s << "#include <vector>" << endl;
    s << "#include \"jce/Jce.h\"" << endl;

    s << "using namespace std;" << endl;

    vector<string> include = pPtr->getIncludes();
    for(size_t i = 0; i < include.size(); i++)
    {
        s << "#include \"" << g_parse->getHeader() 
            << taf::TC_Common::replace(taf::TC_File::extractFileName(include[i]), ".h", "Coder.h") << "\"" << endl;
    }

    vector<NamespacePtr> namespaces = pPtr->getNamespaces();

    s << endl;

    for(size_t i = 0; i < namespaces.size(); i++)
    {
        s << generateCoder(namespaces[i], sInterface) << endl;
    }

    s << endl;
    s << "#endif" << endl;

    taf::TC_File::makeDirRecursive(m_sBaseDir, 0755);
    taf::TC_File::save2file(fileH, s.str());

    return;
}
