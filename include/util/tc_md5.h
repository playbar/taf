#ifndef __TC_MD5_H
#define __TC_MD5_H

#include <string>
#include "util/tc_common.h"

using namespace std;

namespace taf
{
/////////////////////////////////////////////////
// 说明: md5类(修改并完善至md5的c版本)
// Author : j@syswin.com              
// Modify: j@syswin.com              
/////////////////////////////////////////////////
/**
* 该类提供MD5的算法
* 提供两种输出方式:字符串(32个字符)或二进制(16个字节)
*/
class TC_MD5
{
    typedef unsigned char *POINTER;
    typedef unsigned short int UINT2;
    //typedef unsigned long int UINT4;
    typedef uint32_t UINT4;

    typedef struct 
    {
        /**
        * state (ABCD)
        */
        //unsigned long state[4];        
        UINT4 state[4];        

        /**
        * number of bits, modulo 2^64 (lsb first)
        */
        //unsigned long count[2];        
        UINT4 count[2];        

        /**
        * input buffer
        */
        unsigned char buffer[64];      
    }MD5_CTX;

public:

    /**
    * 对字符串进行md5处理,返回16字节二进制数据
    * @param sString : 输入字符串
    * @return string ,输出,16个字节的二进制数据
    */
    static string md5bin(const string &sString);

    /**
    * 对字符串进行md5处理,将md5的二进制数据转换成HEX的32个字节的字符串
    * @param sString : 输入字符串
    * @return string ,输出,32个字符
    */
    static string md5str (const string &sString);

    /**
     * 
     * @param fileName
     * 
     * @return string
     */
    static string md5file(const string& fileName);

protected:

	static string bin2str(const void *buf, size_t len, const string &sSep);

    /**
    * MD5 init
    * @param context : 上下文信息
    * @return void
    */ 
    static void md5init(MD5_CTX *context);
    
    /**
    * MD5 block update operation. Continues an MD5 message-digest
    * operation, processing another message block, and updating the
    * context.
    * @param context : 上下文信息
    * @param input : 输入
    * @param inputLen : 输入长度
    * @return void
    */    
    static void md5update (MD5_CTX *context, unsigned char *input,unsigned int inputLen);
    
    /**
    * MD5 finalization. Ends an MD5 message-digest operation,
    * writing the message digest and zeroizing the context.
    * @param digest : 摘要
    * @param context : 上下文信息
    */
    static void md5final (unsigned char digest[16], MD5_CTX *context);
    
    /**
    * MD5 basic transformation. Transforms state based on block.
    * @param state : 状态
    * @param block : ...
    */
    static void md5transform (UINT4 state[4],unsigned char block[64]);
    
    /**
    * Encodes input (UINT4) into output (unsigned char). Assumes len is a multiple of 4.
    * @param output : 输出
    * @param input : 输入
    * @param len : 输入长度
    */    
    static void encode (unsigned char *output,UINT4 *input,unsigned int len);
    
    /**
    * Decodes input (unsigned char) into output (UINT4). Assumes len is a multiple of 4.
    * @param output : 输出
    * @param input : 输入
    * @param len : 输入长度
    */    
    static void decode (UINT4 *output,unsigned char *input,unsigned int len);
    
    /**
    * replace "for loop" with standard memcpy if possible.
    * @param output : 输出
    * @param input : 输入
    * @param len : 输入长度
    */    
    static void md5_memcpy (POINTER output,POINTER input,unsigned int len);
    
    /**
    * replace "for loop" with standard memset if possible.
    * @param output : 输出
    * @param value : 值
    * @param len : 输入长度
    */ 
    static void md5_memset (POINTER output,int value,unsigned int len);
    
    /**
    * 填充值
    */
    static unsigned char PADDING[64];
};

}
#endif
