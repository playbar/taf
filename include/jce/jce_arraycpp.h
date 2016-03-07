#ifndef __JCE_ARRAYCPP_H__
#define __JCE_ARRAYCPP_H__

/*
    支持定长数组、不抛异常版本
*/

#ifndef WIN32
#include <netinet/in.h>
#else
#include <winsock.h>
#endif

#include <stdio.h>
#include <string.h>
#include <limits.h>


#ifdef WIN32
extern void ShowDebugMsg(LPCTSTR lpszFormat, ...);
#define PRINTF_INFO ShowDebugMsg 
#else
#define PRINTF_INFO printf 
#endif

namespace jceCode
{

    typedef bool    Bool;
    typedef char    Char;
    typedef short   Short;
    typedef float   Float;
    typedef double  Double;
    typedef int     Int32;

#ifdef WIN32
    typedef __int64     Int64;
#else 
#if __WORDSIZE == 64
    typedef long    Int64;
#else
    typedef long long   Int64;
#endif
#endif
    

#ifndef WIN32
    typedef uint8_t     UInt8;
    typedef uint32_t    UInt32;
#else
    typedef unsigned __int8 UInt8;
    typedef unsigned __int32 UInt32;
#endif


#ifndef JCE_MAX_STRING_LENGTH
//#define JCE_MAX_STRING_LENGTH (100 * 1024 * 1024)
#define JCE_MAX_STRING_LENGTH   (20  * 1024)
#endif

#if __BYTE_ORDER == __BIG_ENDIAN
#   define jce_ntohll(x)    (x)
#   define jce_htonll(x)    (x)
#   define jce_ntohf(x)     (x)
#   define jce_htonf(x)     (x)
#   define jce_ntohd(x)     (x)
#   define jce_htond(x)     (x)
#else
#   if __BYTE_ORDER == __LITTLE_ENDIAN
#       define jce_ntohll(x)    jce_htonll(x)
namespace jce
{
        union bswap_helper
        {
                Int64   i64;
                Int32   i32[2];
        };
}
inline Int64 jce_htonll(Int64 x)
{
        jce::bswap_helper h;
        h.i64 = x;
        Int32 tmp = htonl(h.i32[1]);
        h.i32[1] = htonl(h.i32[0]);
        h.i32[0] = tmp;
        return h.i64;
}
inline Float jce_ntohf(Float x)
{
    Int32 __t__ = htonl((*((Int32 *)&x)));
    return *((Float *) &__t__);
}
#       define jce_htonf(x)     jce_ntohf(x)
inline Double jce_ntohd(Double x)
{
    Int64 __t__ = jce_htonll((*((Int64 *)&x)));
    return *((Double *) &__t__);
}
#       define jce_htond(x)     jce_ntohd(x)
#   endif
#endif


//返回值定义
const Int32 JCE_SUCCESS         =  0;      //成功
const Int32 JCE_CODE_ERROR      = -1;      //编码错误
const Int32 JCE_BUFFER_ERROR    = -2;      //buffer错误
   
//////////////////////////////////////////////////////////////////
class BufferWriter;
class BufferReader;

class JceClassBase
{
public:
    virtual void dumpInfo() const = 0;
    virtual Int32 writeTo(BufferWriter& bw) const = 0;
    virtual Int32 readFrom(BufferReader& br) = 0;
};

//////////////////////////////////////////////////////////////////
/// 数据头信息的封装，包括类型和tag
class DataHead
{
    UInt8 _type;
    UInt8 _tag;
public:
    enum
    {
        eChar = 0,
        eShort = 1,
        eInt32 = 2,
        eInt64 = 3,
        eFloat = 4,
        eDouble = 5,
        eString1 = 6,
        eString4 = 7,
        eMap = 8,
        eList = 9,
        eStructBegin = 10,
        eStructEnd = 11,
        eZeroTag = 12,
        eSimpleList = 13,
    };

#ifdef WIN32
    #pragma pack(push,1)

    struct helper
    {
        unsigned char    type : 4;
        unsigned char    tag : 4;
    };
    #pragma pack(pop)

#else

    struct helper
    {
        unsigned int    type : 4;
        unsigned int    tag : 4;
    }__attribute__((packed));

#endif

public:
    DataHead() : _type(0), _tag(0) {}
    DataHead(UInt8 type, UInt8 tag) : _type(type), _tag(tag) {}

    UInt8 getTag() const      { return _tag; }
    void setTag(UInt8 t)      { _tag = t; }
    UInt8 getType() const     { return _type; }
    void setType(UInt8 t)     { _type = t; }

    /// 读取数据头信息
    template<typename BufferReaderT>
    Int32 readFrom(BufferReaderT& br)
    {
        Int32 ret;
        size_t n;
        ret = peekFrom(br,n);
        if(jceCode::JCE_SUCCESS != ret) return ret;
        ret = br.skip(n);
        if(jceCode::JCE_SUCCESS != ret) return ret;

        return jceCode::JCE_SUCCESS;
    }

    /// 读取头信息，但不前移流的偏移量
    template<typename BufferReaderT>
    Int32 peekFrom(BufferReaderT& br, size_t& n)
    {
        Int32 ret;
        helper h;
        n = sizeof(h);
        ret = br.peekBuf(&h, sizeof(h));
        if(jceCode::JCE_SUCCESS != ret) return ret;
        _type = h.type;
        if(h.tag == 15){
            br.peekBuf(&_tag, sizeof(_tag), sizeof(h));
            n += sizeof(_tag);
        }else{
            _tag = h.tag;
        }
        return jceCode::JCE_SUCCESS;
    }

    /// 写入数据头信息
    template<typename BufferWriterT>
    Int32 writeTo(BufferWriterT& bw)
    {
        return writeTo(bw, _type, _tag);
    }

    /// 写入数据头信息
    template<typename BufferWriterT>
    static Int32 writeTo(BufferWriterT& bw, UInt8 type, UInt8 tag)
    {
        Int32 ret;
        helper h;
        h.type = type;
        if(tag < 15){
            h.tag = tag;
            ret = bw.writeBuf(&h, sizeof(h));
            if(jceCode::JCE_SUCCESS != ret) return ret;
        }else{
            h.tag = 15;
            ret = bw.writeBuf(&h, sizeof(h));
            if(jceCode::JCE_SUCCESS != ret) return ret;
            ret = bw.writeBuf(&tag, sizeof(tag));
            if(jceCode::JCE_SUCCESS != ret) return ret;
        }

        return jceCode::JCE_SUCCESS;
    }

};

//////////////////////////////////////////////////////////////////
/// 缓冲区读取器封装
class BufferReader
{
    const char *        _buf;       ///< 缓冲区
    size_t              _buf_len;   ///< 缓冲区长度
    size_t              _cur;       ///< 当前位置
    char                _err[128];  ///< 错误信息缓冲区
public:
    BufferReader() : _buf(NULL),_cur(0),_buf_len(0) {}

    void reset() { _cur = 0; }

    /// 读取缓存
    Int32 readBuf(void * buf, size_t len)
    {
        Int32 ret;
        ret = peekBuf(buf, len);
        if(JCE_SUCCESS != ret) return ret;
        _cur += len;
        return JCE_SUCCESS;
    }

    /// 读取缓存，但不改变偏移量
    Int32 peekBuf(void * buf, size_t len, size_t offset = 0)
    {
        if(_cur + offset + len > _buf_len)
        {
            ////snprintf(_err, sizeof(_err), "buffer overflow when peekBuf, over %u.", (UInt32)_buf_len);
            return JCE_BUFFER_ERROR;
        }
        ::memcpy(buf, _buf + _cur + offset, len);
        return JCE_SUCCESS;
    }

    /// 跳过len个字节
    Int32 skip(size_t len)
    {
        if(_cur + len > _buf_len)
        {
            ////snprintf(_err, sizeof(_err), "buffer overflow when skip, over %u.", (UInt32)_buf_len);
            return JCE_BUFFER_ERROR;
        }
        _cur += len;
        return JCE_SUCCESS;
    }

    char * getErrBuffer() const                             { return (char*)_err; }//{ return &_buf[0]; }
    size_t getErrLength() const                             { return sizeof(_err); }//{ return _buf.size(); }

    /// 设置缓存
    void setBuffer(const char * buf, size_t len)
    {
        _buf = buf;
        _buf_len = len;
        _cur = 0;
    }

};

//////////////////////////////////////////////////////////////////
/// 缓冲区写入器封装
class BufferWriter
{
    char *    _buf;
    size_t    _cur;
    size_t    _buf_len;
    char      _err[128];  ///< 错误信息缓冲区

    BufferWriter(const BufferWriter&);
    BufferWriter& operator=(const BufferWriter&);
public:
    BufferWriter() : _buf(NULL) , _cur(0) , _buf_len(0) {} 

    void reset() { _cur = 0; }

    Int32 writeBuf(const void * buf, size_t len)
    {
        if(_cur + len > _buf_len)
        {
            //snprintf(_err, sizeof(_err), "buffer overflow when writeBuf, over %u.", (UInt32)_buf_len);
            return JCE_BUFFER_ERROR;
        }
        ::memcpy(_buf + _cur, buf, len);
        _cur += len;

        return JCE_SUCCESS;
    }

    const char * getBuffer() const                      { return _buf; }//{ return &_buf[0]; }
    size_t getLength() const                            { return _cur; }//{ return _buf.size(); }
    char * getErrBuffer() const                         { return (char*)_err; }//{ return &_buf[0]; }
    size_t getErrLength() const                            { return sizeof(_err); }//{ return _buf.size(); }

    /// 设置缓存
    void setBuffer(char * buf, size_t len)
    {
        _buf = buf;
        _buf_len = len;
        _cur = 0;
    }
};

//////////////////////////////////////////////////////////////////
class BuiltinType
{
public:
    /// 跳到指定标签的元素前
    static bool skipToTag(BufferReader& br, UInt8 tag)
    {
        Int32 ret;
        DataHead h;
        while(true){
            size_t len;
            ret = h.peekFrom(br,len);
            if(JCE_SUCCESS != ret) return false;
            if(tag <= h.getTag() || h.getType() == DataHead::eStructEnd)
                return tag == h.getTag();
            ret = br.skip(len);
            if(JCE_SUCCESS != ret) return false;
            ret = skipField(br,h.getType());
            if(JCE_SUCCESS != ret) return false;
        }
        return false;
    }

    /// 跳到当前结构的结束
    static Int32 skipToStructEnd(BufferReader& br)
    {
        Int32 ret;
        DataHead h;
        do{
            ret = h.readFrom(br);
            if(JCE_SUCCESS != ret) return ret;
            ret = skipField(br,h.getType());
            if(JCE_SUCCESS != ret) return ret;
        }while(h.getType() != DataHead::eStructEnd);

        return JCE_SUCCESS;
    }

    /// 跳过一个字段
    static Int32 skipField(BufferReader& br)
    {
        Int32 ret;
        DataHead h;
        ret = h.readFrom(br);
        if(JCE_SUCCESS != ret) return ret;
        ret = skipField(br,h.getType());
        if(JCE_SUCCESS != ret) return ret;

        return JCE_SUCCESS;
    }

    /// 跳过一个字段，不包含头信息
    static Int32 skipField(BufferReader& br, UInt8 type)
    {
        Int32 ret;
        switch(type){
        case DataHead::eChar:
            ret = br.skip(sizeof(Char));
            if(JCE_SUCCESS != ret) return ret;
            break;
        case DataHead::eShort:
            ret = br.skip(sizeof(Short));
            if(JCE_SUCCESS != ret) return ret;
            break;
        case DataHead::eInt32:
            ret = br.skip(sizeof(Int32));
            if(JCE_SUCCESS != ret) return ret;
            break;
        case DataHead::eInt64:
            ret = br.skip(sizeof(Int64));
            if(JCE_SUCCESS != ret) return ret;
            break;
        case DataHead::eFloat:
            ret = br.skip(sizeof(Float));
            if(JCE_SUCCESS != ret) return ret;
            break;
        case DataHead::eDouble:
            ret = br.skip(sizeof(Double));
            if(JCE_SUCCESS != ret) return ret;
            break;
        case DataHead::eString1:
            {
                UInt8 len;
                ret = br.readBuf(&len, sizeof(len));
                if(JCE_SUCCESS != ret) return ret;
                ret = br.skip(len);
                if(JCE_SUCCESS != ret) return ret;
            }
            break;
        case DataHead::eString4:
            {
                UInt32 len;
                ret = br.readBuf(&len, sizeof(len));
                if(JCE_SUCCESS != ret) return ret;
                ret = br.skip(ntohl(len));
                if(JCE_SUCCESS != ret) return ret;
            }
            break;
        case DataHead::eMap:
            {
                Int32 size;
                ret = read(br,size, 0);
                if(JCE_SUCCESS != ret) return ret;
                for(Int32 i = 0; i < size * 2; ++i){
                    ret = skipField(br);
                    if(JCE_SUCCESS != ret) return ret;
                }
            }
            break;
        case DataHead::eList:
            {
                Int32 size;
                ret = read(br,size, 0);
                if(JCE_SUCCESS != ret) return ret;
                for(Int32 i = 0; i < size; ++i){
                    ret = skipField(br);
                    if(JCE_SUCCESS != ret) return ret;
                }
            }
            break;
        case DataHead::eSimpleList:
            {
                DataHead h;
                ret = h.readFrom(br);
                if(JCE_SUCCESS != ret) return ret;
                if(h.getType() != DataHead::eChar){
                    //snprintf(br.getErrBuffer(), br.getErrLength(), "skipField with invalid type, type value: %d, %d.", type, h.getType());
                    return JCE_CODE_ERROR;
                }
                Int32 size;
                ret = read(br,size, 0);
                if(JCE_SUCCESS != ret) return ret;
                ret = br.skip(size);
                if(JCE_SUCCESS != ret) return ret;
            }
            break;
        case DataHead::eStructBegin:
            ret = skipToStructEnd(br);
            if(JCE_SUCCESS != ret) return ret;
            break;
        case DataHead::eStructEnd:
        case DataHead::eZeroTag:
            break;
        default:
            {
                //snprintf(br.getErrBuffer(), br.getErrLength(), "skipField with invalid type, type value:%d.", type);
                return JCE_CODE_ERROR;
            }
        }

        return JCE_SUCCESS;
    }

    static Int32 read(BufferReader& br, Bool& b, UInt8 tag, bool isRequire = true)
    {
        Int32 ret;
        Char c;
        ret = read(br,c, tag, isRequire);
        if(JCE_SUCCESS != ret) return ret;
        b = c ? true : false;

        return JCE_SUCCESS;
    }

    static Int32 read(BufferReader& br, Char& c, UInt8 tag, bool isRequire = true)
    {
        Int32 ret;
        if(skipToTag(br,tag)){
            DataHead h;
            ret = h.readFrom(br);
            if(JCE_SUCCESS != ret) return ret;
            switch(h.getType()){
            case DataHead::eZeroTag:
                c = 0;
                break;
            case DataHead::eChar:
                ret = br.readBuf(&c, sizeof(c));
                if(JCE_SUCCESS != ret) return ret;
                break;
            default:
                {
                    //snprintf(br.getErrBuffer(), br.getErrLength(), "read 'Char' type mismatch, tag: %d, get type: %d.", tag, h.getType());
                    return JCE_CODE_ERROR;
                }

            }
        }else if(isRequire){
            //snprintf(br.getErrBuffer(), br.getErrLength(), "require field not exist, tag: %d.", tag);
            return JCE_CODE_ERROR;
        }

        return JCE_SUCCESS;
    }

    static Int32 read(BufferReader& br, Short& n, UInt8 tag, bool isRequire = true)
    {
        Int32 ret;
        if(skipToTag(br,tag)){
            DataHead h;
            ret = h.readFrom(br);
            if(JCE_SUCCESS != ret) return ret;
            switch(h.getType()){
            case DataHead::eZeroTag:
                n = 0;
                break;
            case DataHead::eChar:
                {
                    Char Charn;
                    ret = br.readBuf(&Charn, sizeof(Charn));
                    if(JCE_SUCCESS != ret) return ret;
                    n = Charn;
                    break;
                }
            case DataHead::eShort:
                ret = br.readBuf(&n, sizeof(n));
                if(JCE_SUCCESS != ret) return ret;
                n = ntohs(n);
                break;
            default:
                {
                    //snprintf(br.getErrBuffer(), br.getErrLength(), "read 'Short' type mismatch, tag: %d, get type: %d.", tag, h.getType());
                    return JCE_CODE_ERROR;
                }
            }
        }else if(isRequire){
            //snprintf(br.getErrBuffer(), br.getErrLength(), "require field not exist, tag: %d", tag);
            return JCE_CODE_ERROR;
        }

        return JCE_SUCCESS;
    }

    static Int32 read(BufferReader& br, Int32& n, UInt8 tag, bool isRequire = true)
    {
        Int32 ret;
        if(skipToTag(br,tag)){
            DataHead h;
            ret = h.readFrom(br);
            if(JCE_SUCCESS != ret) return ret;
            switch(h.getType()){
            case DataHead::eZeroTag:
                n = 0;
                break;
            case DataHead::eChar:
                {
                    Char Charn;
                    ret = br.readBuf(&Charn, sizeof(Charn));
                    if(JCE_SUCCESS != ret) return ret;
                    n = (Int32)Charn;
                    break;
                }
            case DataHead::eShort:
                {
                    Short Shortn;
                    ret = br.readBuf(&Shortn, sizeof(Shortn));
                    if(JCE_SUCCESS != ret) return ret;
                    n = (Short) ntohs(Shortn);
                    break;
                }
            case DataHead::eInt32:
                ret = br.readBuf(&n, sizeof(n));
                if(JCE_SUCCESS != ret) return ret;
                n = ntohl(n);
                break;
            default:
                {
                    //snprintf(br.getErrBuffer(), br.getErrLength(), "read 'Int32' type mismatch, tag: %d, get type: %d.", tag, h.getType());
                    return JCE_CODE_ERROR;
                }
            }
        }else if(isRequire){
            //snprintf(br.getErrBuffer(), br.getErrLength(), "require field not exist, tag: %d", tag);
            return JCE_CODE_ERROR;
        }

        return JCE_SUCCESS;
    }

    static Int32 read(BufferReader& br, Int64& n, UInt8 tag, bool isRequire = true)
    {
        Int32 ret;
        if(skipToTag(br,tag)){
            DataHead h;
            ret = h.readFrom(br);
            if(JCE_SUCCESS != ret) return ret;
            switch(h.getType()){
            case DataHead::eZeroTag:
                n = 0;
                break;
            case DataHead::eChar:
                {
                    Char Charn;
                    ret = br.readBuf(&Charn, sizeof(Charn));
                    if(JCE_SUCCESS != ret) return ret;
                    n = (Int64)Charn;
                    break;
                }
            case DataHead::eShort:
                {
                    Short Shortn;
                    ret = br.readBuf(&Shortn, sizeof(Shortn));
                    if(JCE_SUCCESS != ret) return ret;
                    n = (Short) ntohs(Shortn);
                    break;
                }
            case DataHead::eInt32:
                {
                    Int32 Int32n;
                    ret = br.readBuf(&Int32n, sizeof(Int32n));
                    if(JCE_SUCCESS != ret) return ret;
                    n = (Int32) ntohl(Int32n);
                    break;
                }
            case DataHead::eInt64:
                ret = br.readBuf(&n, sizeof(n));
                if(JCE_SUCCESS != ret) return ret;
                n = jce_ntohll(n);
                break;
            default:
                {
                    //snprintf(br.getErrBuffer(), br.getErrLength(), "read 'Int64' type mismatch, tag: %d, get type: %d.", tag, h.getType());
                    return JCE_CODE_ERROR;
                }

            }
        }else if(isRequire){
            //snprintf(br.getErrBuffer(), br.getErrLength(), "require field not exist, tag: %d", tag);
            return JCE_CODE_ERROR;
        }

        return JCE_SUCCESS;
    }

    static Int32 read(BufferReader& br, Float& n, UInt8 tag, bool isRequire = true)
    {
        Int32 ret;
        if(skipToTag(br,tag)){
            DataHead h;
            ret = h.readFrom(br);
            if(JCE_SUCCESS != ret) return ret;
            switch(h.getType()){
            case DataHead::eZeroTag:
                n = 0;
                break;
            case DataHead::eFloat:
                ret = br.readBuf(&n, sizeof(n));
                if(JCE_SUCCESS != ret) return ret;
                n = jce_ntohf(n);
                break;
            default:
                {
                    //snprintf(br.getErrBuffer(), br.getErrLength(), "read 'Float' type mismatch, tag: %d, get type: %d.", tag, h.getType());
                    return JCE_CODE_ERROR;
                }
            }
        }else if(isRequire){
            //snprintf(br.getErrBuffer(), br.getErrLength(), "require field not exist, tag: %d", tag);
            return JCE_CODE_ERROR;
        }

        return JCE_SUCCESS;
    }

    static Int32 read(BufferReader& br, Double& n, UInt8 tag, bool isRequire = true)
    {
        Int32 ret;
        
        if(skipToTag(br,tag)){
            DataHead h;
            ret = h.readFrom(br);
            if(JCE_SUCCESS != ret) return ret;
            switch(h.getType()){
            case DataHead::eZeroTag:
                n = 0;
                break;
            case DataHead::eFloat:
                {
                    Float Floatn;
                    ret = br.readBuf(&Floatn, sizeof(Floatn));
                    if(JCE_SUCCESS != ret) return ret;
                    n = jce_ntohf(Floatn);
                    break;
                }
            case DataHead::eDouble:
                ret = br.readBuf(&n, sizeof(n));
                if(JCE_SUCCESS != ret) return ret;
                n = jce_ntohd(n);
                break;
            default:
                {
                    //snprintf(br.getErrBuffer(), br.getErrLength(), "read 'Double' type mismatch, tag: %d, get type: %d.", tag, h.getType());
                    return JCE_CODE_ERROR;
                }
            }
        }else if(isRequire){
            //snprintf(br.getErrBuffer(), br.getErrLength(), "require field not exist, tag: %d", tag);
            return JCE_CODE_ERROR;
        }

        return JCE_SUCCESS;
    }

    static Int32 readString(BufferReader& br, Char s[], Int32 buffSize, Int32& readLen, UInt8 tag, bool isRequire = true)
    {
        Int32 ret;
        if(skipToTag(br,tag)){
            DataHead h;
            ret = h.readFrom(br);
            if(JCE_SUCCESS != ret) return ret;
            switch(h.getType()){
            case DataHead::eString1:
                {
                    UInt8 len;
                    ret = br.readBuf(&len, sizeof(len));
                    if(JCE_SUCCESS != ret) return ret;
                    if((Int32)len > (size_t)buffSize){
                        //snprintf(br.getErrBuffer(), br.getErrLength(), "invalid string size, tag: %d, size: %d, buffsize: %d", tag, len, buffSize);
                        printf("invalid string size, tag: %d, size: %d, buffsize: %d", tag, len, buffSize);
                        return JCE_BUFFER_ERROR;
                    }
                    ret = br.readBuf(s, len);
                    if(JCE_SUCCESS != ret) return ret;
                    readLen = len;
                }
                break;
            case DataHead::eString4:
                {
                    Int32 len;
                    ret = br.readBuf(&len, sizeof(len));
                    if(JCE_SUCCESS != ret) return ret;
                    len = (Int32)ntohl(len);
                    if(len < 0 ){
                        //snprintf(br.getErrBuffer(), br.getErrLength(), "invalid string size, tag: %d, size: %d", tag, len);
                        return JCE_CODE_ERROR;
                    }
                    
                    if(len > buffSize){
                        //snprintf(br.getErrBuffer(), br.getErrLength(), "invalid string size, tag: %d, size: %d, buffsize: %d", tag, len,buffSize);
                        return JCE_BUFFER_ERROR;
                    }
                    readLen = len;
                }
                break;
            default:
                {
                    //snprintf(br.getErrBuffer(), br.getErrLength(), "read 'string' type mismatch, tag: %d, get type: %d.", tag, h.getType());
                    return JCE_CODE_ERROR;
                }
            }
        }else if(isRequire){
            //snprintf(br.getErrBuffer(), br.getErrLength(), "require field not exist, tag: %d", tag);
            return JCE_CODE_ERROR;
        }

        return JCE_SUCCESS;
    }

    static Int32 readVectorChar(BufferReader& br, Char v[], Int32 buffSize, Int32& readLen, UInt8 tag, bool isRequire = true)
    {
        Int32 ret;
        if(skipToTag(br,tag)){
            DataHead h;
            ret = h.readFrom(br);
            if(JCE_SUCCESS != ret) return ret;
            switch(h.getType()){
            case DataHead::eSimpleList:
                {
                    DataHead hh;
                    hh.readFrom(br);
                    if(hh.getType() != DataHead::eChar){
                        //snprintf(br.getErrBuffer(), br.getErrLength(), "type mismatch, tag: %d, type: %d, %d", tag, h.getType(), hh.getType());
                        return JCE_CODE_ERROR;
                    }
                    Int32 size;
                    if(JCE_SUCCESS != ret) return ret;
                    ret = read(br,size, 0);
                    if(JCE_SUCCESS != ret) return ret;
                    if(size < 0){
                        //snprintf(br.getErrBuffer(), br.getErrLength(), "invalid size, tag: %d, type: %d, %d, size: %d", tag, h.getType(), hh.getType(), size);
                        return JCE_CODE_ERROR;
                    }
                    if(size > buffSize){
                        //snprintf(br.getErrBuffer(), br.getErrLength(), "invalid size, tag: %d, type: %d, %d, size: %d, buffsize: %d", tag, h.getType(), hh.getType(), size, buffSize);
                        return JCE_BUFFER_ERROR;
                    }
                    ret = br.readBuf(v, size);
                    if(JCE_SUCCESS != ret) return ret;
                    readLen = size;
                }
                break;
                #if 0
            case DataHead::eList:
                {
                    Int32 size;
                    ret = read(size, 0);
                    if(JCE_SUCCESS != ret) return ret;
                    if(size < 0){
                        //snprintf(br.getErrBuffer(), br.getErrLength(), "invalid size, tag: %d, type: %d, size: %d", tag, h.getType(), size);
                        return JCE_CODE_ERROR;
                    }
                    for(Int32 i = 0; i < size; ++i){
                        ret = read(v[i], 0);
                        if(JCE_SUCCESS != ret) return ret;
                    }
                }
                break;
                #endif
            default:
                {
                    //snprintf(br.getErrBuffer(), br.getErrLength(), "type mismatch, tag: %d, type: %d", tag, h.getType());
                    return JCE_CODE_ERROR;
                }
            }
        }else if(isRequire){
            //snprintf(br.getErrBuffer(), br.getErrLength(), "require field not exist, tag: %d", tag);
            return JCE_CODE_ERROR;
        }

        return JCE_SUCCESS;
    }

    template<typename T>
    static Int32 readVector(BufferReader& br, T v[], Int32 buffSize, Int32& readLen, UInt8 tag, bool isRequire = true)
    {
        Int32 ret;

        if(skipToTag(br,tag)){
            DataHead h;
            ret = h.readFrom(br);
            if(JCE_SUCCESS != ret) return ret;
            switch(h.getType()){
            case DataHead::eList:
                {
                    ret = read(br, readLen, 0);
                    if(JCE_SUCCESS != ret) return ret;
                    if(readLen < 0){
                        //snprintf(br.getErrBuffer(), br.getErrLength(), "invalid size, tag: %d, type: %d, size: %d", tag, h.getType(), size);
                        return JCE_CODE_ERROR;
                    }
                    if(readLen > buffSize){
                        //snprintf(br.getErrBuffer(), br.getErrLength(), "invalid size, tag: %d, type: %d, size: %d,buffsize: %d", tag, h.getType(), size, buffSize);
                        return JCE_CODE_ERROR;
                    }
                    for(Int32 i = 0; i < readLen; ++i){
                        ret = read(br, v[i], 0);
                        if(JCE_SUCCESS != ret) return ret;
                    }
                }
                break;
            default:
                {
                    //snprintf(br.getErrBuffer(), br.getErrLength(), "read 'vector' type mismatch, tag: %d, get type: %d.", tag, h.getType());
                    return JCE_CODE_ERROR;
                }
            }
        }else if(isRequire){
            //snprintf(br.getErrBuffer(), br.getErrLength(), "require field not exist, tag: %d", tag);
            return JCE_CODE_ERROR;
        }

        return JCE_SUCCESS;
    }

    static Int32 write(BufferWriter& bw, Bool b, UInt8 tag)
    {
        return write(bw,(Char) b, tag);
    }

    static Int32 write(BufferWriter& bw, Char n, UInt8 tag)
    {
        Int32 ret;

        if (n == 0) {
            ret = DataHead::writeTo(bw, DataHead::eZeroTag, tag);
            if(JCE_SUCCESS != ret) return ret;
        } else {
            ret = DataHead::writeTo(bw, DataHead::eChar, tag);
            if(JCE_SUCCESS != ret) return ret;
            ret = bw.writeBuf(&n, sizeof(n));
            if(JCE_SUCCESS != ret) return ret;
        }

        return JCE_SUCCESS;
    }

    static Int32 write(BufferWriter& bw, Short n, UInt8 tag)
    {
        Int32 ret;
        if(n >= CHAR_MIN && n <= CHAR_MAX){
            ret = write(bw,(Char) n, tag);
            if(JCE_SUCCESS != ret) return ret;
        }else{
            ret = DataHead::writeTo(bw, DataHead::eShort, tag);
            if(JCE_SUCCESS != ret) return ret;
            n = htons(n);
            ret = bw.writeBuf(&n, sizeof(n));
            if(JCE_SUCCESS != ret) return ret;
        }

        return JCE_SUCCESS;
    }

    static Int32 write(BufferWriter& bw, Int32 n, UInt8 tag)
    {
        Int32 ret;
        if(n >= SHRT_MIN && n <= SHRT_MAX){
            ret = write(bw,(Short) n, tag);
            if(JCE_SUCCESS != ret) return ret;
        }else{
            ret = DataHead::writeTo(bw, DataHead::eInt32, tag);
            if(JCE_SUCCESS != ret) return ret;
            n = htonl(n);
            ret = bw.writeBuf(&n, sizeof(n));
            if(JCE_SUCCESS != ret) return ret;
        }
        
        return JCE_SUCCESS;
    }

    static Int32 write(BufferWriter& bw, Int64 n, UInt8 tag)
    {
        Int32 ret;
        if(n >= INT_MIN && n <= INT_MAX){
            ret = write(bw,(Int32) n, tag);
            if(JCE_SUCCESS != ret) return ret;
        }else{
            ret = DataHead::writeTo(bw, DataHead::eInt64, tag);
            if(JCE_SUCCESS != ret) return ret;
            n = jce_htonll(n);
            ret = bw.writeBuf(&n, sizeof(n));
            if(JCE_SUCCESS != ret) return ret;
        }

        return JCE_SUCCESS;
    }

    static Int32 write(BufferWriter& bw, Float n, UInt8 tag)
    {
        Int32 ret;
        ret = DataHead::writeTo(bw, DataHead::eFloat, tag);
        if(JCE_SUCCESS != ret) return ret;
        n = jce_htonf(n);
        ret = bw.writeBuf(&n, sizeof(n));
        if(JCE_SUCCESS != ret) return ret;

        return JCE_SUCCESS;
    }

    static Int32 write(BufferWriter& bw, Double n, UInt8 tag)
    {
        Int32 ret;
        ret = DataHead::writeTo(bw, DataHead::eDouble, tag);
        if(ret != JCE_SUCCESS) return ret;
        n = jce_htond(n);
        ret = bw.writeBuf(&n, sizeof(n));
        if(ret != JCE_SUCCESS) return ret;
        return JCE_SUCCESS;
    }

    static Int32 writeString(BufferWriter& bw, const Char s[], Int32 len, UInt8 tag)
    {
        Int32 ret;
        if(len > 255){
            if(len > JCE_MAX_STRING_LENGTH){
                //snprintf(bw.getErrBuffer(), bw.getErrLength(), "invalid string size, tag: %d, size: %u", tag, (UInt32)len);
                return JCE_CODE_ERROR;
            }
            ret = DataHead::writeTo(bw, DataHead::eString4, tag);
            if(ret != JCE_SUCCESS) return ret;
            UInt32 n = htonl(UInt32(len));
            ret = bw.writeBuf(&n, sizeof(n));
            if(ret != JCE_SUCCESS) return ret;
            ret = bw.writeBuf(s,(size_t)len);
            if(ret != JCE_SUCCESS) return ret;
        }else{
            ret = DataHead::writeTo(bw, DataHead::eString1, tag);
            if(ret != JCE_SUCCESS) return ret;
            UInt8 n = (UInt8)len;
            ret = bw.writeBuf(&n, sizeof(n));
            if(ret != JCE_SUCCESS) return ret;
            ret = bw.writeBuf(s, len);
            if(ret != JCE_SUCCESS) return ret;
        }

        return JCE_SUCCESS;
    }
    
    static Int32 writeVectorChar(BufferWriter& bw, const Char v[], Int32 len, UInt8 tag)
    {
        Int32 ret;
        ret = DataHead::writeTo(bw, DataHead::eSimpleList, tag);
        if(JCE_SUCCESS != ret) return ret;
        ret = DataHead::writeTo(bw, DataHead::eChar, 0);
        if(JCE_SUCCESS != ret) return ret;
        ret = write(bw,len, 0);
        if(JCE_SUCCESS != ret) return ret;
        ret = bw.writeBuf(v, (size_t)len);
        if(JCE_SUCCESS != ret) return ret;

        return JCE_SUCCESS;
    }

    template<typename T>
    static Int32 writeVector(BufferWriter& bw,const T v[], Int32 len, UInt8 tag)
    {
        Int32 ret;
        ret = DataHead::writeTo(bw, DataHead::eList, tag);
        if(JCE_SUCCESS != ret) return ret;
        ret = write(bw,len, 0);
        if(JCE_SUCCESS != ret) return ret;
        for(Int32 i = 0; i < len; ++i){
            ret = write(bw, v[i], 0);
            if(JCE_SUCCESS != ret) return ret;
        }

        return JCE_SUCCESS;
    }
};
}


#endif

