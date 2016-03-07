// **********************************************************************
// WUP version 1.0.5 by WSRD Tencent.
// **********************************************************************

#ifndef __WUP_SYMBIAN_H__
#define __WUP_SYMBIAN_H__

#include <stdlib.h>
#include "Jce_sym.h"
#include "RequestF_sym.h"

using namespace taf;

namespace wup
{

//存放taf返回值的key
#define STATUS_RESULT_CODE (Char*)"STATUS_RESULT_CODE"
#define STATUS_RESULT_DESC (Char*)"STATUS_RESULT_DESC"

////////////////////////////////////////////////////////////////////////////////////////////////
//返回值定义
// extern const taf::Int32 WUP_SUCCESS        ;      //成功
// extern const taf::Int32 WUP_ATTR_NOT_FOUND ;      //属性不存在
// extern const taf::Int32 WUP_ENCODE_ERROR   ;      //编码错误
// extern const taf::Int32 WUP_DECODE_ERROR   ;      //解码错误
// extern const taf::Int32 WUP_RUNTIME_ERROR  ;      //其他运行时错误

#define  WUP_SUCCESS          0      //成功
#define  WUP_ATTR_NOT_FOUND  -1      //属性不存在
#define  WUP_ENCODE_ERROR    -2      //编码错误
#define  WUP_DECODE_ERROR    -3      //解码错误
#define  WUP_RUNTIME_ERROR   -4      //其他运行时错误


/////////////////////////////////////////////////////////////////////////////////
// 属性封装类

class UniAttribute
{
public:
    IMPORT_C UniAttribute();
    IMPORT_C virtual ~UniAttribute();

public:
    /**
     * 添加属性值
     * 
     * @param T:   属性类型
     * @param name:属性名称
     * @param t:   属性值
     * @return：    处理结果，见返回值定义
     */

    IMPORT_C Int32 putAttr(taf::JceOutputStream &os, const String &key);
    
#define DO_PUT  {_os->reset(); _os->write(t, 0); return putAttr(*_os, name);  }    
    
    IMPORT_C Int32 put (const String& name, const Bool &t);
    IMPORT_C Int32 put (const String& name, const Char &t);
    IMPORT_C Int32 put (const String& name, const Short &t);
    IMPORT_C Int32 put (const String& name, const Int32 &t);
    IMPORT_C Int32 put (const String& name, const Int64 &t);
    IMPORT_C Int32 put (const String& name, const Float &t);
    IMPORT_C Int32 put (const String& name, const Double &t);
    IMPORT_C Int32 put (const String& name, const String &t);
    IMPORT_C Int32 put (const String& name, const JceStructBase &t);

    //add by edwardsu
    IMPORT_C Int32 put (const String& name, const UInt8 &t);
    IMPORT_C Int32 put (const String& name, const UInt16 &t);
    IMPORT_C Int32 put (const String& name, const UInt32 &t);
    //end add
 
    template < typename T >
    Int32 put (const String& name, const JArray<T> &t)
        { DO_PUT }

    template <typename K, typename V >
    Int32 put (const String& name, const JMapWrapper<K, V> &t)
        { DO_PUT }
    /**
     * 获取属性值
     * 
     * @param T:   属性类型
     * @param name:属性名称
     * @param t:   属性值输出参数
     * @return：    处理结果，见返回值定义
     */

    IMPORT_C Int32 getAttr(taf::JceInputStream &is, const String &key);
 
#define DO_GET \
    int ret = getAttr(*_is, name); \
    if(ret == WUP_SUCCESS) \
    { \
        TRAPD(error, _is->read(t, 0, true);); \
        if(error) \
        { \
            _sLastError = _is->getLastError(); \
            return WUP_DECODE_ERROR; \
        } \
        return WUP_SUCCESS; \
    } \
    else \
        return ret; \
    
    IMPORT_C Int32 get (const String& name,  Bool &t);
    IMPORT_C Int32 get (const String& name,  Char &t);
    IMPORT_C Int32 get (const String& name,  Short &t);
    IMPORT_C Int32 get (const String& name,  Int32 &t);
    IMPORT_C Int32 get (const String& name,  Int64 &t);
    IMPORT_C Int32 get (const String& name,  Float &t);
    IMPORT_C Int32 get (const String& name,  Double &t);
    IMPORT_C Int32 get (const String& name,  String &t);
    IMPORT_C Int32 get (const String& name,  JceStructBase &t);

    //add by edwardsu
    IMPORT_C Int32 get (const String& name,  UInt8 &t);
    IMPORT_C Int32 get (const String& name,  UInt16 &t);
    IMPORT_C Int32 get (const String& name,  UInt32 &t);
    //end add
    
    template < typename T >
    Int32 get (const String& name,  JArray<T> &t) 
        {DO_GET}

    template <typename K, typename V >
    Int32 get (const String& name,  JMapWrapper<K, V> &t)
        {DO_GET}


    
    /**
     * 获取属性值，不存在的属性返回缺省值
     * 
     * @param T:   属性类型
     * @param name:属性名称
     * @param t:   属性值输出参数
     * @param def: 默认值
     * @return：    处理结果，见返回值定义
     */
    template<typename T> Int32 getByDefault(const String& name, T& t, const T& def)
    {
        Int32 iRet = get(name, t);
        if(iRet == WUP_ATTR_NOT_FOUND)
        {
            t = def;
            return WUP_SUCCESS;
        }
        else
        {
            return iRet;
        }
    }
    
    
    /**
     *清除全部属性值
     */
    IMPORT_C void clear() ;
    
    /** 编码
     * 
     * @param buff：编码结果输出参数
     * @return：    处理结果，见返回值定义
     */
    IMPORT_C Int32 encode(String& buff);

    /** 编码
     * 
     * @param buff：编码结果输出参数
     * @return：    处理结果，见返回值定义
     */
    //howlpan IMPORT_C Int32 encode(JArray<char>& buff);
    IMPORT_C Int32 encode(JArray<Char>& buff);

    /** 编码
     * 
     * @param buff：输出存放编码结果的buffer指针
     * @param len： 输入buff长度，输出编码结果长度
     * @return：    处理结果，见返回值定义
     */
    IMPORT_C Int32 encode(char* buff, unsigned int & len);

    /** 解码
     * 
     * @param buff：待解码字节流的buffer指针
     * @param len： 待解码字节流的长度
     * @return：    处理结果，见返回值定义
     */
    //howlpan IMPORT_C Int32 decode(const char* buff, unsigned int len);
    IMPORT_C Int32 decode(const Char* buff, unsigned int len);

    /**
     * 解码
     * 
     * @param buff： 待解码的字节流
     * @return：    处理结果，见返回值定义
     */
    //howlpan IMPORT_C Int32 decode(const JArray<char>& buff);
    IMPORT_C Int32 decode(const JArray<Char>& buff);

    /**
     * 获取已有的属性
     * @return const JMapWrapper<String,JMapWrapper<String,JArray<char>>>& ：属性map
     */
    IMPORT_C const JMapWrapper<String, JArray<Char> >& getData() const;

    /**
     * 判断属性集合是否为空
     * 
     * @return bool
     */
    IMPORT_C bool isEmpty();

    /**
     * 获取属性集合大小
     * 
     * @return size_t：集合大小
     */
    IMPORT_C unsigned int size();
    /**
     * 获取最后一个错误的描述信息
     * 
     * @return string：错误的描述信息
     */
    IMPORT_C String getLastError();

protected:
    taf::JceOutputStream* _os;
    taf::JceInputStream* _is;

    JMapWrapper<String, JArray<signed char> > _data;
    
    String _sLastError; 
};

/////////////////////////////////////////////////////////////////////////////////
// 请求、回应包封装类

struct UniPacket : protected RequestPacket, public UniAttribute
{
    /**
     * 构造函数
     */
    IMPORT_C UniPacket() ;
    
    /**
     * 由请求包生成回应包基本结构，回填关键的请求信息
     * 
     * @return UniPacket： 回应包
     */
    //UniPacket createResponse();
    
    IMPORT_C UniPacket* createResponseLC();


    IMPORT_C UniPacket* createResponseL();
    
    void ConstructL()
    {

    }
    /**
     * 编码，结果的包头4个字节为整个包的长度，网络字节序
     * 
     * @param buff： 编码结果输出参数
     * @return：    处理结果，见返回值定义
     */
    IMPORT_C Int32 encode(String& buff);

    /**
     * 编码，结果的包头4个字节为整个包的长度，网络字节序
     * 
     * @param buff： 编码结果输出参数
     * @return：    处理结果，见返回值定义
     */
    IMPORT_C Int32 encode(JArray<char>& buff);

    /**
     * 编码，结果的包头4个字节为整个包的长度，网络字节序
     * @throw runtime_error
     * @param buff：存放编码结果的buffer指针
     * @param len： 输入buff长度，输出编码结果长度
     * @return：    处理结果，见返回值定义
     */
    IMPORT_C Int32 encode(char* buff, unsigned int & len);

    /** 解码
     * 
     * @param buff：待解码字节流的buffer指针
     * @param len： 待解码字节流的长度
     * @return：    处理结果，见返回值定义
     */
    IMPORT_C Int32 decode(const signed char* buff, unsigned int len);

public:
    /**
     * 获取消息ID
     * @return Int32
     */
    IMPORT_C Int32 getRequestId() const;
    /**
     * 设置请求ID
     * @param value
     */
    IMPORT_C void setRequestId(Int32 value) ;
    /**
     * 获取对象名称
     * @return const String&
     */
    IMPORT_C const String& getServantName() const;
    /**
     * 设置对象名称
     * @param value
     */
    IMPORT_C void setServantName(const String& value);
    /**
     * 获取方法名
     * @return const String&
     */
    IMPORT_C const String& getFuncName() const;
    /**
     * 设置方法名
     * @param value
     */
    IMPORT_C void setFuncName(const String& value);

protected:
    /**
     * 内部编码
     */
    void doEncode(taf::JceOutputStream & os);
};

/////////////////////////////////////////////////////////////////////////////////
// 调用TAF的服务时使用的类

struct TafUniPacket : public UniPacket
{

public:
    IMPORT_C TafUniPacket();
    IMPORT_C ~TafUniPacket();

public:
    /**
     * 设置协议版本
     * @param value
     */
    IMPORT_C void setTafVersion(Short value);
    /**
     * 设置调用类型
     * @param value
     */
    IMPORT_C void setTafPacketType(Char value);
    /**
     * 设置消息类型
     * @param value
     */
    IMPORT_C void setTafMessageType(Int32 value);
    /**
     * 设置超时时间
     * @param value
     */
    IMPORT_C void setTafTimeout(Int32 value);
    /**
     * 设置参数编码内容
     * @param value
     */
    IMPORT_C void setTafBuffer(const JArray<Char>& value);
    /**
     * 设置上下文
     * @param value
     */
    IMPORT_C void setTafContext(const JMapWrapper<String, String>& value);
    /**
     * 设置特殊消息的状态值
     * @param value
     */
    IMPORT_C void setTafStatus(const JMapWrapper<String, String>& value);
    /**
     * 获取协议版本
     * @return Short
     */
    IMPORT_C Short getTafVersion() const;
    /**
     * 获取调用类型
     * @return Char
     */
    IMPORT_C Char getTafPacketType() const ;
    /**
     * 获取消息类型
     * @return Int32
     */
    IMPORT_C Int32 getTafMessageType() const ;
    /**
     * 获取超时时间
     * @return Int32
     */
    IMPORT_C Int32 getTafTimeout() const;
    /**
     * 获取参数编码后内容
     * @return const JArray<Char>&
     */
    IMPORT_C const JArray<Char>& getTafBuffer() const;
    /**
     * 获取上下文
     * @return const JMapWrapper<String,String>&
     */
    IMPORT_C const JMapWrapper<String, String>& getTafContext() const;
    /**
     * 获取特殊消息的状态值
     * @return const JMapWrapper<String,String>&
     */
    IMPORT_C const JMapWrapper<String, String>& getTafStatus() const;

    /**
     * 获取调用taf的返回值
     * @retrun taf::Int32
     */
    IMPORT_C taf::Int32 getTafResultCode() const;

    /**
     * 获取调用taf的返回描述
     * @retrun string
     */
    IMPORT_C String getTafResultDesc() const;

};


}
////////////////////////////////////////////////////////////////////////////////////////////////
#endif
