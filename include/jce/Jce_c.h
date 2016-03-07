#ifndef __JCE_C_H__
#define __JCE_C_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/////////////////////////////////////////////////////////////////////////////////////
typedef int     Bool;
typedef char    Char;
typedef short   Short;
typedef float   Float;
typedef double  Double;
typedef int     Int32;

typedef unsigned char   uint8_t;
typedef unsigned int    uint32_t;

typedef unsigned char	UInt8;
typedef unsigned short	UInt16;
typedef unsigned int	UInt32;

#ifndef __WORDSIZE
#define __WORDSIZE 32
#endif

typedef struct
{
    Int32 high;
    Int32 low;
}MTKInt64;

#ifdef __MTK_64
typedef MTKInt64  Int64;
#else
#if __WORDSIZE == 64
typedef long    Int64;
#else
typedef long long   Int64;
#endif
#endif

#ifndef true
#define true  1
#endif
#ifndef false
#define false 0
#endif

#ifndef JceMalloc
#define JceMalloc(nsize)       malloc(nsize);
#endif
#ifndef JceFree
#define JceFree(p)             free(p);
#endif

//返回值定义
extern const Int32 JCE_SUCCESS        ;      //成功
extern const Int32 JCE_ATTR_NOT_FOUND ;      //查找不到相关属性
extern const Int32 JCE_ENCODE_ERROR   ;      //编码错误
extern const Int32 JCE_DECODE_ERROR   ;      //解码错误
extern const Int32 JCE_RUNTIME_ERROR  ;      //其他运行时错误
extern const Int32 JCE_MALLOC_ERROR   ;      //内存申请失败错误

#ifndef JCE_MAX_STRING_LENGTH
#define JCE_MAX_STRING_LENGTH   (100 * 1024 * 1024)
#endif

#define jce__bswap_constant_16(x) \
        ((((x) & 0xff00) >> 8) | (((x) & 0x00ff) << 8));

#define jce__bswap_constant_32(x) \
        ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) |           \
            (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24));
            
/*
#define jce__bswap_constant_64(x) \
        ((((x) & 0xff00000000000000ull) >> 56)                   \
            | (((x) & 0x00ff000000000000ull) >> 40)                     \
            | (((x) & 0x0000ff0000000000ull) >> 24)                     \
            | (((x) & 0x000000ff00000000ull) >> 8)                      \
            | (((x) & 0x00000000ff000000ull) << 8)                      \
            | (((x) & 0x0000000000ff0000ull) << 24)                     \
            | (((x) & 0x000000000000ff00ull) << 40)                     \
            | (((x) & 0x00000000000000ffull) << 56));
 */
          
Int64 jce__bswap_constant_64(Int64 x);
           
Int64 jce_ntohll(Int64 x);
Float jce_ntohf(Float x);
Double jce_ntohd(Double x);

#if 0// __BYTE_ORDER == __BIG_ENDIAN
#   define jce_ntohll(x)    (x);
#   define jce_htonll(x)    (x);
#   define jce_ntohf(x)     (x);
#   define jce_htonf(x)     (x);
#   define jce_ntohd(x)     (x);
#   define jce_htond(x)     (x);
#   define jce_ntohs(x)     (x);
#   define jce_htons(x)     (x);
#   define jce_ntohl(x)     (x);
#   define jce_htonl(x)     (x);
#else
//# if __BYTE_ORDER == __LITTLE_ENDIAN

#       define jce_htonll(x)    jce_ntohll(x);
#       define jce_htonf(x)     jce_ntohf(x);
#       define jce_htond(x)     jce_ntohd(x);
#       define jce_ntohs(x)     jce__bswap_constant_16(x);
#       define jce_htons(x)     jce__bswap_constant_16(x);
#       define jce_ntohl(x)     jce__bswap_constant_32(x);
#       define jce_htonl(x)     jce__bswap_constant_32(x);

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct JStructBase JStructBase; 
typedef struct JString JString;
typedef struct JArray JArray;
typedef struct JMapWrapper JMapWrapper;

struct DataHead;
struct JceStream;

typedef struct DataHead    DataHead;
typedef struct JceStream   JceStream;
typedef struct JceStream   JceInputStream;
typedef struct JceStream   JceOutputStream;


struct JStructBase
{
    char * className;
    Int32 (*writeTo)(const void*, JceOutputStream *);
    Int32 (*readFrom)(void*, JceInputStream *);
};

//string封装
struct JString
{
    char *        _data;
    unsigned int  _len;
    unsigned int  _buf_len;
};

void JString_del(JString **st);


void JString_copy(char * dest, const char * src, uint32_t len);

Int32 JString_copyChar(JString * s, char * data, uint32_t len);

void JString_clear(JString *s);

Int32 JString_reserve(JString *s, uint32_t n);

Int32 JString_assign(JString *s, const char * data, uint32_t len);

Int32 JString_append(JString * s, const char * data, uint32_t len);

Int32 JString_insert(JString *s, uint32_t pos, char v);

uint32_t JString_size(JString *s);
int JString_empty(JString *s);
uint32_t JString_capacity(JString *s);
char * JString_data(JString *s);
Int32 JString_resize(JString *s, uint32_t n);


Int32 JString_init(JString *s);

JString * JString_new(void);


//数组封装
struct JArray
{
    char *   elem_type_name;//类型名称
    unsigned elem_num;
    unsigned list_len;
    int *    list;
    unsigned buff_used;
    unsigned buff_len;
    char *   buff;          //元素的jce编码结果
};

void JArray_del(JArray ** arr);

Int32 JArray_reserveList(JArray *arr, uint32_t num);

Int32 JArray_reserveBuff(JArray *arr, uint32_t len);

Int32 JArray_pushBack(JArray *arr, const char * data, uint32_t len);

Int32 JArray_pushBackString(JArray *arr, const char * data);

int JArray_get(JArray *arr, unsigned index, char *data, uint32_t * len);

char *JArray_getPtr(JArray *arr, unsigned index);

int JArray_getLength(JArray *arr, unsigned index);

int JArray_size(JArray *arr);

int JArray_empty(JArray *arr);

void JArray_clear(JArray *arr);

void JArray_init(JArray *arr);


JArray * JArray_new(const char * type);



//map封装
struct JMapWrapper
{
    JArray * first;
    JArray * second;
};

void JMapWrapper_del(JMapWrapper ** m);

int JMapWrapper_put(JMapWrapper *m, const char * first_data, unsigned first_len, const char *value_data, unsigned value_len);

int JMapWrapper_find(JMapWrapper *m, const char * first_data, unsigned first_len, char **value_data, unsigned * value_len);

int JMapWrapper_size(JMapWrapper * m);

int JMapWrapper_getByIndex(JMapWrapper *m, unsigned index, char *first, uint32_t * first_len, char* second, uint32_t * second_len);

int JMapWrapper_empty(JMapWrapper *m);

void JMapWrapper_clear(JMapWrapper *m);

void JMapWrapper_init(JMapWrapper *m);


JMapWrapper * JMapWrapper_new(const char * first_type, const char * second_type);


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
    eSimpleList = 13
};

////////////////////////////////////////////////////////////////////////////////////////////////////


//缓冲区读写器封装
struct JceStream
{
    JString     *    _buf;       //< 缓冲区
    uint32_t         _cur;       //< 当前位置
    char             _err[32];   //< 错误描述
    DataHead    *    _h;         //< 辅助协议头解析
};

/*
/// 缓冲区读取器封装
struct JceInputStream
{
    JString     *    _buf;       //< 缓冲区
    uint32_t         _cur;       //< 当前位置
    char             _err[32];  //< 错误描述
    DataHead    *    _h;         //  辅助协议头解析
};

////////////////////////////////////////////////////////////////////////////
/// 缓冲区写入器封装
struct  JceOutputStream
{
    JString    * _buf;      //< 缓冲区
    char       _err[32];   //< 错误描述
    DataHead   * _h;
};
*/

struct DataHead
{
    uint8_t _type;
    uint8_t _tag;
};

typedef struct helper
{
    //unsigned int    type : 4;
    //unsigned int    tag : 4;
    unsigned char type_tag; 
} helper;
//}__attribute__((packed))  helper;

uint8_t helper_getTag(helper *h);

void helper_setTag(helper *h, unsigned int t);

uint8_t helper_getType(helper *h);

void helper_setType(helper *h, unsigned int t);

void DataHead_del(DataHead ** head);

uint8_t DataHead_getTag(DataHead * head);
//void DataHead_setTag(DataHead * head, uint8_t t);
uint8_t DataHead_getType(DataHead * head);
//void DataHead_setType(DataHead * head, uint8_t t);

/// 读取头信息，但不前移流的偏移量
Int32 DataHead_peekFrom(DataHead * head, JceInputStream* is, uint32_t *n);

/// 读取数据头信息
Int32 DataHead_readFrom(DataHead * head, JceInputStream* is);

/// 写入数据头信息
Int32 DataHead_writeTo(DataHead * head, JceOutputStream* os);

//设置type tag 并写到os
Int32 DataHead_setAndWriteTo(DataHead * head, unsigned int type, unsigned int tag,JceOutputStream* os);
 
 
void DataHead_init(DataHead * head);

DataHead * DataHead_new(void);
////////////////////////////////////////////////////////////////////////////

void JceInputStream_del(JceInputStream ** is);

void JceInputStream_reset(JceInputStream * is);

/// 读取缓存
Int32 JceInputStream_readBuf(JceInputStream * is, void * buf, uint32_t len);

/// 读取缓存，但不改变偏移量
Int32 JceInputStream_peekBuf(JceInputStream * is, void * buf, uint32_t len, uint32_t offset);

/// 跳过len个字节
Int32 JceInputStream_skip(JceInputStream * is, uint32_t len);

/// 设置缓存
Int32 JceInputStream_setBuffer(JceInputStream * is, const char * buf, uint32_t len);


/// 跳到指定标签的元素前
Int32 JceInputStream_skipToTag(JceInputStream * is, uint8_t tag);

/// 跳到当前结构的结束
Int32 JceInputStream_skipToStructEnd(JceInputStream * is);

/// 跳过一个字段
Int32 JceInputStream_skipField(JceInputStream * is);

/// 跳过一个字段，不包含头信息
Int32 JceInputStream_skipFieldByType(JceInputStream * is, uint8_t type);

Int32 JceInputStream_checkValid(JceInputStream * is, uint8_t tag, Bool isRequire);

Int32 JceInputStream_readByChar(JceInputStream * is, Char * n);

Int32 JceInputStream_readByShort(JceInputStream * is, Short * n);

Int32 JceInputStream_readByInt32(JceInputStream * is, Int32 * n);

Int32 JceInputStream_readBool	(JceInputStream * is, Bool* b, uint8_t tag, Bool isRequire);

Int32 JceInputStream_readChar	(JceInputStream * is, Char* c, uint8_t tag, Bool isRequire);

Int32 JceInputStream_readUInt8	(JceInputStream * is, UInt8  * n, uint8_t tag, Bool isRequire);

Int32 JceInputStream_readShort	(JceInputStream * is, Short  * n, uint8_t tag, Bool isRequire);

Int32 JceInputStream_readUInt16	(JceInputStream * is, UInt16 * n, uint8_t tag, Bool isRequire);

Int32 JceInputStream_readInt32	(JceInputStream * is, Int32 * n, uint8_t tag, Bool isRequire);

Int32 JceInputStream_readUInt32	(JceInputStream * is, UInt32 * n, uint8_t tag, Bool isRequire);

Int32 JceInputStream_readInt64(JceInputStream * is, Int64* n, uint8_t tag, Bool isRequire);

Int32 JceInputStream_readFloat(JceInputStream * is, Float* n, uint8_t tag, Bool isRequire);

Int32 JceInputStream_readDouble(JceInputStream * is, Double* n, uint8_t tag, Bool isRequire);

Int32 JceInputStream_readString(JceInputStream * is, JString* s, uint8_t tag, Bool isRequire);

Int32 JceInputStream_readMap(JceInputStream * is, JMapWrapper* m, uint8_t tag, Bool isRequire);

Int32 JceInputStream_readVector(JceInputStream * is, JArray* v, uint8_t tag, Bool isRequire);

Int32 JceInputStream_readVectorChar(JceInputStream * is, JString *v, uint8_t tag, Bool isRequire);

/// 读取结构
Int32 JceInputStream_readStruct(JceInputStream * is, void *st, uint8_t tag, Bool isRequire);

Int32 JceInputStream_readStructString(JceInputStream * is, JString * st, uint8_t tag, Bool isRequire);

Int32 JceInputStream_init(JceInputStream* is);

JceInputStream * JceInputStream_new(void);

////////////////////////////////////////////////
void JceOutputStream_del(JceOutputStream ** os);

void JceOutputStream_reset(JceOutputStream * os);

Int32 JceOutputStream_writeBuf(JceOutputStream * os, const void * buf, uint32_t len);

char * JceOutputStream_getBuffer(JceOutputStream * os);

uint32_t JceOutputStream_getLength(JceOutputStream * os);

Int32 JceOutputStream_writeBool(JceOutputStream * os, Bool b, uint8_t tag);

Int32 JceOutputStream_writeChar(JceOutputStream * os, Char n, uint8_t tag);

Int32 JceOutputStream_writeUInt8(JceOutputStream * os, UInt8 n, uint8_t tag);

Int32 JceOutputStream_writeShort(JceOutputStream * os, Short n, uint8_t tag);

Int32 JceOutputStream_writeUInt16(JceOutputStream * os, UInt16 n, uint8_t tag);

Int32 JceOutputStream_writeInt32(JceOutputStream * os, Int32 n, uint8_t tag);

Int32 JceOutputStream_writeUInt32(JceOutputStream * os, UInt32 n, uint8_t tag);

Int32 JceOutputStream_writeInt64(JceOutputStream * os, Int64 n, uint8_t tag);

Int32 JceOutputStream_writeFloat(JceOutputStream * os, Float n, uint8_t tag);

Int32 JceOutputStream_writeDouble(JceOutputStream * os, Double n, uint8_t tag);

Int32 JceOutputStream_writeString(JceOutputStream * os, JString* s, uint8_t tag);
Int32 JceOutputStream_writeStringBuffer(JceOutputStream * os, const char* buff, uint32_t len, uint8_t tag);

Int32 JceOutputStream_writeMap(JceOutputStream * os, JMapWrapper* m, uint8_t tag);

Int32 JceOutputStream_writeVector(JceOutputStream * os, JArray* v, uint8_t tag);

Int32 JceOutputStream_writeVectorChar(JceOutputStream * os, JString *v, uint8_t tag);
Int32 JceOutputStream_writeVectorCharBuffer(JceOutputStream * os, const char* buff, uint32_t len, uint8_t tag);

Int32 JceOutputStream_writeStruct(JceOutputStream * os,const void * st, uint8_t tag);
Int32 JceOutputStream_writeStructString(JceOutputStream * os, JString * v, uint8_t tag);
Int32 JceOutputStream_writeStructBuffer(JceOutputStream * os, const char* buff, uint32_t len, uint8_t tag);

Int32 JceOutputStream_init(JceOutputStream * os);

JceOutputStream * JceOutputStream_new(void);

#endif
