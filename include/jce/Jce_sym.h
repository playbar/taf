#ifndef _JCE_SYMBIAN_H_
#define _JCE_SYMBIAN_H_

#include <e32base.h>
#include <e32std.h>
#include <string.h>
#include <limits.h>

namespace taf
{

typedef unsigned char   UInt8;
typedef unsigned int    UInt16;
typedef unsigned short  UInt32;
typedef unsigned char   uint8_t;
/**
*wup 的数据类型定义
*/
typedef bool        Bool;
typedef signed char Char;
typedef short       Short;
typedef float       Float;
typedef double      Double;
typedef int         Int32;

#ifndef EKA2
    typedef unsigned long       Int64;
#else
    #ifndef __GNUC__
        typedef long long       Int64;
    #endif
    //typedef TInt64       Int64;
#endif

//#ifdef EKA2
//typedef  unsigned int size_t;
//#endif

#ifndef JCE_MAX_STRING_LENGTH
#define JCE_MAX_STRING_LENGTH   (100 * 1024 * 1024)
#endif

#define jce_htonf(x)        jce_ntohf(x)
#define jce_htond(x)        jce_ntohd(x)
#define jce_htonll(x)       jce_ntohll(x)

#define jce_ntohs(x)        jce__bswap_constant_16(x)
#define jce_htons(x)        jce__bswap_constant_16(x)

#define jce_ntohl(x)        jce__bswap_constant_32(x)
#define jce_htonl(x)        jce__bswap_constant_32(x)

#define jce__bswap_constant_16(x) \
    ((((x) & 0xff00) >> 8) | (((x) & 0x00ff) << 8))

#define jce__bswap_constant_32(x) \
    ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) |           \
    (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))

//#ifdef __SERIES60_3X__ || __S60_50__
#define jce__bswap_constant_64(x) \
    ( (((x) & 0xff00000000000000ull) >> 56)                     \
    | (((x) & 0x00ff000000000000ull) >> 40)                     \
    | (((x) & 0x0000ff0000000000ull) >> 24)                     \
    | (((x) & 0x000000ff00000000ull) >> 8)                      \
    | (((x) & 0x00000000ff000000ull) << 8)                      \
    | (((x) & 0x0000000000ff0000ull) << 24)                     \
    | (((x) & 0x000000000000ff00ull) << 40)                     \
        | (((x) & 0x00000000000000ffull) << 56))
/*
#define jce__bswap_constant_64(x) \
    ( (((x) & 0xff00000000000000) >> 56)                   \
    | (((x) & 0x00ff000000000000) >> 40)                     \
    | (((x) & 0x0000ff0000000000) >> 24)                     \
    | (((x) & 0x000000ff00000000) >> 8)                      \
    | (((x) & 0x00000000ff000000) << 8)                      \
    | (((x) & 0x0000000000ff0000) << 24)                     \
    | (((x) & 0x000000000000ff00) << 40)                     \
    | (((x) & 0x00000000000000ff) << 56))
#endif  
*/

inline Int64 jce_ntohll(Int64 x)
{
    return jce__bswap_constant_32(x);
    //return jce__bswap_constant_64(x);
}

inline Float jce_ntohf(Float x)
{
    Int32 __t__ = jce__bswap_constant_32((*((Int32 *)&x)));
    return *((Float *) &__t__);
}

inline Double jce_ntohd(Double x)
{
    Int64 __t__ = jce__bswap_constant_32((*((Int64 *)&x)));
    //Int64 __t__ = jce__bswap_constant_64((*((Int64 *)&x)));
    return *((Double *) &__t__);

}

class JceInputStream;
class JceOutputStream;
class JString;

struct JceStructBase
{
protected:
    JceStructBase(){}
    virtual ~JceStructBase(){}
public:
    virtual void writeTo(taf::JceOutputStream& _os) const = 0;
    virtual void readFrom(taf::JceInputStream& _is) = 0;
    virtual JString  structName() const = 0;
};

template < typename T >
class JArray
{
public:
    typedef unsigned int    size_type;

private:
    /**
     * 数组元素的指针，数组是利用链表的形式来存储数据的。
     */
    T *         _data;
    
    /**
     * 当前数组中元素的长度。
     */
    size_type   _len;
    
    /**
     * 数组最大可以存放的元素长度。
     * note：_buf_len值必须大于_len。
     */
    size_type   _buf_len;

public:
    JArray()
        : _data(NULL)
        , _len(0)
        , _buf_len(0)
    {}
    
    JArray(const JArray& r)
        : _data(NULL)
        , _len(0)
        , _buf_len(0)
    {
        assign(r._data, r._len);
    }
    
    JArray(const T * data, size_type len)
    {
        _data = new T[len];
        _len = _buf_len = len;
        copy(_data, data, len);
    }
    
    ~JArray()
    {
        delete[] _data;
    }
    
    JArray& operator=(const JArray& r)
    {
        assign(r._data, r._len);
        return *this;
    }
    
    /**
     * 初始化数组。
     * @param data：用于初始化数组的新数组。
     * @param len：新数组的元素个数。
     */
    void assign(const T * data, size_type len)
    {
        clear();
        reserve(len);
        copy(_data, data, len);
        _len = len;
    }
    
    /**
     * 添加新元素到数组尾部
     * @param v：新元素
     * 
     * note：当数组空间为0时，重新分配数组内存空间。
     */
    void append(const T& v)
    {
        if(_len >= _buf_len)
            reserve((_len + 1) * 2);
        _data[_len++] = v;
    }
    
    /**
     * 添加新数组到原数组尾部。
     * @param data：新数组元素指针
     * @param len：新数组元素个数。
     * 
     * note：当data的数组个数超过原数组所能存储的空间时，重新分配数组内存空间。
     */
    void append(const T * data, size_type len)
    {
        if(_len + len > _buf_len)
            reserve(2*(_len + len));
        copy(_data + _len, data, len);
        _len += len;
    }
    
    /**
     * 插入新元素到数组中。
     * @param pos：新元素所插入的位置
     * @param v：新元素数据。
     */
    int insert(size_type pos, const T& v)
    {
        //越界
        if (pos >= _buf_len)
            return -1;
        
        if(_len >= _buf_len)
            reserve(_len * 2);
        
        copyreverse(_data + pos + 1, _data + pos, _len - pos);
        _data[pos] = v;
        ++_len;
        
        return 0;
    }

    int remove(size_type pos)
    {
        if(pos >= _len)
            return -1;
        copy(_data + pos, _data + pos + 1, _len - pos - 1);
        --_len;
        return 0;
    }

    T& operator[](size_type n)  { return _data[n]; }
    const T& operator[](size_type n) const  { return _data[n]; }
    bool operator==(const JArray<T>& r) const
    {
        if(_len != r._len)
            return false;
        for(unsigned int i = 0; i < _len; ++i)
            if(!(_data[i] == r._data[i]))
                return false;
        return true;
    }
    bool operator<(const JArray<T>& r) const
    {
        const T * f1 = _data;
        const T * l1 = _data + _len;
        const T * f2 = r._data;
        const T * l2 = r._data + r._len;
        for (; f1 != l1 && f2 != l2; ++f1, ++f2)
        {
            if (*f1 < *f2)
                return true;
            if (*f2 < *f1)
                return false;
        }
        return f1 == l1 && f2 != l2;
    }   
    void reserve(size_type n)
    {
        if(_buf_len < n){
            T * p = new T[n];
            copy(p, _data, _len);
            delete[] _data;
            _data = p;
            _buf_len = n;
        }
    }
    
    void clear()    { _len = 0; }
    size_type size() const  { return _len; }
    bool empty() const      { return _len == 0; }
    size_type capacity() const  { return _buf_len; }
    T * data()      { return _data; }
    const T * data() const  { return _data; }
    void swap(JArray<T>& r)
    {
        swap(_data, r._data);
        swap(_len, r._len);
        swap(_buf_len, r._buf_len);
    }
    void resize(size_type n)
    {
        if(n > size())
            reserve(n);
        _len = n;
    }
private:
    template < typename U >
    static void swap(U& l, U& r)
    {
        U t = l;
        l = r;
        r = t;
    }
    static void copy(T * dest, const T * src, size_type len)
    {
        T * pe = dest + len;
        for(; dest != pe; ++dest, ++src)
            *dest = *src;
    }
    static void copyreverse(T * dest, const T * src, size_type len)
    {
        T * pdt = dest + len ;
        const T * pst = src + len ; 
        for(; dest != pdt; --pdt, --pst)
            *(pdt - 1 ) = *(pst - 1);
    }
};

class JString;
typedef JString String;

class JString
{
private:
    typedef JArray<signed char>::size_type size_type;

public:
    /**
    *   构造函数重载
    */
    IMPORT_C JString();
    IMPORT_C JString(const JString& r);
    IMPORT_C JString(const signed char * s, size_type len);
    IMPORT_C JString(const signed char * s);
    
    /*
    *  重载操作符
    */
    IMPORT_C signed char&           operator[](size_type n);
    IMPORT_C const signed char&     operator[](size_type n) const;
    IMPORT_C JString&               operator=(const JString& r);
    IMPORT_C JString&               operator=(const signed char * r);
    IMPORT_C bool                   operator==(const JString& r) const;
    IMPORT_C bool                   operator<(const JString& r) const;
    
    /*
    *  重载操作符
    */
    IMPORT_C void                   assign(const signed char * r, size_type len);
    IMPORT_C void                   assign(const signed char * f, const signed char * l);
    
    /*
    *  重载操作符
    */
    IMPORT_C void                   append(const signed char * r, size_type len);
    IMPORT_C void                   append(const JString &s);
    
    /*
    *  重载操作符
    */
    IMPORT_C size_type              length() const;
    IMPORT_C size_type              size() const;
    IMPORT_C const signed char*     data() const;
    IMPORT_C const signed char*     c_str() const;
    
    /*
    *  重载操作符
    */
    IMPORT_C void                   AppendData(unsigned char * r, size_type len);
    IMPORT_C void                   AppendData(const JString &s);
    
    /*
    *  重载操作符
    */
    IMPORT_C void                   ToString( int nNum );
    IMPORT_C bool                   IsEmpty();
    IMPORT_C void                   Empty();

private:
    JArray<signed char>             _data;
};

template < typename K, typename V >
struct JMapPair
{
    K   first;
    V   second;
    
    JMapPair(const K& k, const V& v)
        : first(k), second(v)
    {}
    
    JMapPair()
    {}
    
    bool operator<(const JMapPair& r) const
    {
        return first < r.first;
    }
    bool operator==(const JMapPair<K, V> & r) const { return first == r.first && second == r.second;}
};

template < typename K, typename V >
class JMapWrapper
{
public:
    typedef JMapPair<K, V>  pair_type;
    typedef typename JArray<pair_type>::size_type   size_type;
private:
    JArray<pair_type>       _v;
public:
    JArray<pair_type>& getArray()   { return _v; }
    const JArray<pair_type>& getArray() const   { return _v; }
    void clear()    { _v.clear(); }
    size_type size() const  { return _v.size(); }
    bool empty() const  { return _v.empty(); }
    bool operator==(const JMapWrapper<K, V> & r) const
    {
        //return _v == r.getArray();
        return size() == r.size();
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////
//获取类型名称
inline String getStructName(const taf::JceStructBase &t)    {return t.structName();}
inline String getStructName(const taf::Bool & )             {return (signed char*)("bool");}
inline String getStructName(const taf::Char& )              {return (signed char*)"char";}
inline String getStructName(const taf::Short& )             {return (signed char*)"short";}
inline String getStructName(const taf::Float& )             {return (signed char*)"float";}
inline String getStructName(const taf::Double& )            {return (signed char*)"double";}
inline String getStructName(const taf::Int32& )             {return (signed char*)"int32";}
inline String getStructName(const taf::Int64& )             {return (signed char*)"int64";}
inline String getStructName(const String& )                 {return (signed char*)"string";}

//add by edwardsu
inline String getStructName(const taf::UInt8 & )            {return (signed char*)"short";}
inline String getStructName(const taf::UInt16& )            {return (signed char*)"int32";}
inline String getStructName(const taf::UInt32& )            {return (signed char*)"int64";}

//end add
template<typename T>
inline String getStructName(const JArray<T> &)
{
    String n;
    T elem;
    //howlpan n.AppendData("list<");
    n.AppendData((Char*)"list<");
    n.AppendData(getStructName(elem));
    //howlpan n.AppendData(">");
    n.AppendData((Char*)">");
    return n;
}

template<typename T, typename U> 
inline String getStructName(const taf::JMapWrapper<T, U> &) 
{
    String n;
    T elem1;
    U elem2;
    n.AppendData((const signed char *)"map<");
    n.AppendData(getStructName(elem1));
    n.AppendData((const signed char *)",");
    n.AppendData(getStructName(elem2));
    n.AppendData((const signed char *)">");
    return n;
}

////////////////////////////////////////////////////////////////////////////////////////////////

class DataHead : public CBase
{
public:

    IMPORT_C ~DataHead();

    IMPORT_C static DataHead* NewL(uint8_t type=0, uint8_t tag=0);
    
    IMPORT_C static DataHead* NewLC(uint8_t type=0, uint8_t tag=0);
    
private:

    DataHead(uint8_t type=0, uint8_t tag=0);
    
    void ConstructL();

public:
    /**
     * 设置或获取属性值
     */
    IMPORT_C uint8_t     getTag() const;
    IMPORT_C void        setTag(uint8_t t);
    IMPORT_C uint8_t     getType() const;
    IMPORT_C void        setType(uint8_t t);
    
private:
    struct helper
    {
        unsigned char   type : 4;
        unsigned char   tag : 4;
    };
    
public:
    /**
     * 读取数据头信息
     * @param is : 模板类型参数。
     */
    IMPORT_C void readFrom(JceInputStream& is);
    
    /**
     * 读取头信息，但不改变偏移量
     * @param is ： 模板类型参数。
     */
    IMPORT_C unsigned int peekFrom(JceInputStream& is);

    /**
     * 写入数据头信息
     * @param os ： 模板类型参数。
     */
    IMPORT_C void writeTo(JceOutputStream& os);    
    
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
    
private:
    uint8_t _type;
    uint8_t _tag;
};

class BufferReader : public CBase
{
public:

    IMPORT_C BufferReader();
    
    IMPORT_C ~BufferReader();

    IMPORT_C static BufferReader* NewL();

    IMPORT_C static BufferReader* NewLC();

private:
    void ConstructL();

public:
    /**
     * 重置缓冲区当前位置。
     * @param ： NULL。
     * @param ： NULL。
     */
    IMPORT_C void            reset();
    
    /**
     * 读取缓冲区数据
     * @param buf ： 读取出来的数据所要存储内存。
     * @param len ： 所要读取数据的长度。
     */
    IMPORT_C void            readBuf(void * buf, unsigned int len);
    
    /**
     * 读取缓冲数据，但不改变数据的偏移量。
     * @param buf :     读取出来的数据所要存储内存。
     * @param len ：     所要读取的数据的长度。
     * @param offset：   数据开始读取位置的偏移量。
     */
    IMPORT_C void            peekBuf(void * buf, unsigned int len, unsigned int offset = 0);
    
    /**
     * 当前位置跳过指定的长度
     * @param len ： 所要跳过的长度。
     */
    IMPORT_C void            skip(unsigned int len);

    /**
     * 设置缓冲区数据，当数据超过当前缓冲区大小时，自动增长。
     * @param buf ： 所要填充的数据。
     * @param len ： 所填充的数据长度。
     */
    IMPORT_C void            setBuffer(const signed char * buf, unsigned int len);
        
    /**
     * 获取最后一个发生错误的字符串描述。
     * @param ： NULL
     * @return ：返回错误的字符描述。 
     */
    IMPORT_C String          getLastError();

protected:
    String          _sLastError;
    
private:
    /**
     * 缓冲区内存块
     */
    JArray<signed char>    _buf;
    
    /**
     * 缓冲区当前位置。
     */
    unsigned int            _cur;

};

class BufferWriter : public CBase
{
public:

    IMPORT_C BufferWriter();
    
    IMPORT_C ~BufferWriter();

    IMPORT_C static BufferWriter* NewL();

    IMPORT_C static BufferWriter* NewLC();

private:
    void ConstructL();

public:
    /**
     * 重置缓冲区当前位置
     * @param ： NULL
     * @param ： NULL
     */
    IMPORT_C void reset();

    /**
     * 写入指定数据到缓冲区中。
     * @param buf ： 所要写入缓冲区的数据。
     * @param len ： 写入的数据长度。
     * @return :     NULL。
     */
    IMPORT_C void writeBuf(const void * buf, unsigned int len);

    /**
     * 获取当前缓冲区数据
     * @return ：指向缓冲区数据指针。
     */
    //howlpan IMPORT_C const char * getBuffer() const;
    IMPORT_C const signed char * getBuffer() const;
    
    /**
     * 获取当前缓冲去数据大小
     * @return ：缓冲区数据长度
     */
    IMPORT_C unsigned int getLength() const;
    
    /**
     * 获取最后一个错误描述符字符串。
     * @return : 最后一个错误描述符。
     */
    IMPORT_C String getLastError();
    
protected:
    /**
     * 最后一个错误描述符字符串。
     */
    String          _sLastError; 
    
private:
    /**
     * 缓冲区数据内存。
     */
    //howlpan JArray<char>    _buf;       ///< 缓冲区
    JArray<signed char>    _buf;       ///< 缓冲区
};

class JceInputStream : public BufferReader
{
public:
    IMPORT_C ~JceInputStream();
    IMPORT_C JceInputStream();

    IMPORT_C static JceInputStream* NewL();

    IMPORT_C static JceInputStream* NewLC();

private:


    void ConstructL();

protected:

    void error(const String &t1, uint8_t tag, unsigned char type, const String &t2, Int32 i);

    IMPORT_C void errorRequire(uint8_t tag);

public:
    /**
     * 跳到指点的标签元素前
     * @param tag ： 指定跳的元素标签。
     * @return ：    true，成功；false，发生错误或没有找到该标签。
     */
    IMPORT_C bool    skipToTag(uint8_t tag);
    
    /**
     * 跳到当前结构的结束位置
     * @param ：     NULL.
     * @return :    NULL.
     */
    IMPORT_C void    skipToStructEnd();
    
    /**
     * 跳过一个字段.
     * @param :  NULL.
     * @return : NULL.
     */
    IMPORT_C void    skipField();
    
    IMPORT_C void    skipField(uint8_t type);
    
    IMPORT_C Char    readChar();

    IMPORT_C Short   readShort();

    IMPORT_C Int32   readInt32();
    
    IMPORT_C void    read(Bool& b, uint8_t tag, bool isRequire = true);
    
    IMPORT_C void    read(Char& c, uint8_t tag, bool isRequire = true);
    
    IMPORT_C void    read(Short& n, uint8_t tag, bool isRequire = true);
    
    IMPORT_C void    read(Int32& n, uint8_t tag, bool isRequire = true);
    
    IMPORT_C void    read(Int64& n, uint8_t tag, bool isRequire = true);
    
    IMPORT_C void    read(Float& n, uint8_t tag, bool isRequire = true);
    
    IMPORT_C void    read(Double& n, uint8_t tag, bool isRequire = true);
    
    IMPORT_C void    read(String& s, uint8_t tag, bool isRequire = true);

    IMPORT_C void    read(JceStructBase& v, uint8_t tag, bool isRequire = true);

    //add by edwardsu
    
    IMPORT_C void    read(UInt8& c, uint8_t tag, bool isRequire = true);
    
    IMPORT_C void    read(UInt16& n, uint8_t tag, bool isRequire = true);
    
    IMPORT_C void    read(UInt32& n, uint8_t tag, bool isRequire = true);

    //end add

    template <typename K, typename V >
    void read(JMapWrapper<K, V>& m, uint8_t tag, bool isRequire = true)
    {
        if(skipToTag(tag))
        {
            Int32 size;
            if(readMap(tag, size))
            {
                JArray<JMapPair<K, V> >& v = m.getArray();
                v.clear();
                
                JMapPair<K, V> pr;
                for(Int32 i = 0; i < size; ++i)
                {
                    read(pr.first, 0);
                    read(pr.second, 1);
                    v.append(pr);
                }
            }
        }
        else if(isRequire)
        {
            errorRequire(tag);
        }
    }

    template < typename T >
    void read(JArray<T>& v, uint8_t tag, bool isRequire = true)
    {
        if(skipToTag(tag))
        {
            //howlpan if(getStructName(v) == "list<char>")
            if(getStructName(v) == (Char*)"list<char>")
            {
                Int32 size;
                if(readVectorChar(tag, size))
                {
                    v.resize(size);
                    readBuf(&v[0], size);
                }
            }
            else
            {
                Int32 size;
                if(readVector(tag, size))
                {
                    v.resize(size);
                    for(Int32 i = 0; i < size; ++i)
                        read(v[i], 0);
                }
            }
        }
        else if(isRequire)
        {
            errorRequire(tag);
        }
    }

protected:

    IMPORT_C bool readVectorChar(uint8_t tag, Int32 &size);

    IMPORT_C bool readVector(uint8_t tag, Int32 &size);

    bool readMap(uint8_t tag, Int32 &size);
};

class JceOutputStream : public BufferWriter
{
public:
    IMPORT_C ~JceOutputStream();
    EXPORT_C JceOutputStream();
    
    IMPORT_C static JceOutputStream* NewL();

    IMPORT_C static JceOutputStream* NewLC();

private:
    void ConstructL();

public:
    IMPORT_C void        write(Bool b, uint8_t tag);
    IMPORT_C void        write(Char n, uint8_t tag);
    IMPORT_C void        write(Short n, uint8_t tag);
    IMPORT_C void        write(Int32 n, uint8_t tag);
    IMPORT_C void        write(Int64 n, uint8_t tag);
    IMPORT_C void        write(Float n, uint8_t tag);
    IMPORT_C void        write(Double n, uint8_t tag);
    IMPORT_C void        write(const String& s, uint8_t tag);
    IMPORT_C void        write(const JceStructBase& v, uint8_t tag);

    //add by edwardsu
    IMPORT_C void        write(UInt8 n, uint8_t tag);
    IMPORT_C void        write(UInt16 n, uint8_t tag);
    IMPORT_C void        write(UInt32 n, uint8_t tag);
    //end add

    template <typename K, typename V >
    void write(const JMapWrapper<K, V>& m, uint8_t tag)
    {
        writeHead(DataHead::eMap, tag);

        Int32 n = m.size();
        write(n, 0);
        const JArray<JMapPair<K, V> >& v = m.getArray();
        for(size_t i = 0; i < v.size(); ++i)
        {
            write(v[i].first, 0);
            write(v[i].second, 1);
        }
    }

    template < typename T >
    void write(const JArray<T>& v, uint8_t tag)
    {
        //howlpan if(getStructName(v) == "list<char>")
        if(getStructName(v) == (Char*)"list<char>")
        {
            writeHead(DataHead::eSimpleList, tag);
            writeHead(DataHead::eChar, 0);

            Int32 n = v.size();
            write(n, 0);
            writeBuf(&v[0], v.size());
        }
        else
        {
            writeHead(DataHead::eList, tag);
                
            Int32 n = v.size();
            write(n, 0);
            for(unsigned int i = 0; i < v.size(); ++i)
                write(v[i], 0);
        }
    }

public:
    IMPORT_C void writeHead(uint8_t type, uint8_t tag);
};

}
#endif /*_JCE_SYMBIAN_H_*/
