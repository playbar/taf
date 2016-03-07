#ifndef	__TC_BIT_MAP_H__
#define __TC_BIT_MAP_H__

#include <iostream>
#include <string>
#include <vector>
#include "util/tc_ex.h"

using namespace std;

namespace taf
{
/////////////////////////////////////////////////
// 说明: 多位bitmap类
// Author : j@syswin.com              
/////////////////////////////////////////////////
/**
 * 异常
 */
struct TC_BitMap_Exception : public TC_Exception
{
	TC_BitMap_Exception(const string &buffer) : TC_Exception(buffer){};
	~TC_BitMap_Exception() throw(){};
};


/**
 * 支持多bit的bitmap
 * 一个整形数, 支持多个Bit
 * 
 */
class TC_BitMap
{
public:
    /**
     * 内存的bitmap
     * 每个整数保持1位
     *
     */
    class BitMap
    {
    public:
    	
        static const int _magic_bits[8];

        #define _set_bit(n,m)   (n|_magic_bits[m])
        #define _clear_bit(n,m) (n&(~_magic_bits[m]))
        #define _get_bit(n,m)   (n&_magic_bits[m])

        //共享内存版本
        #define BM_VERSION      1

        /**
         * 根据元素个数计算需要内存的大小
         * @param iElementCount, 需要保存的元素个数(元素从0开始记)
         * 
         * @return size_t
         */
        static size_t calcMemSize(size_t iElementCount);

        /**
         * 初始化
         * @param pAddr 绝对地址
         * @param iSize 大小, 采用(calcMemSize)计算出来
         * @return 0: 成功, -1:内存不够
         */
        void create(void *pAddr, size_t iSize);

        /**
         * 链接到内存块
         * @param pAddr  ,地址, 采用(calcMemSize)计算出来
         * @return 0, 成功, -1,版本不对, -2:大小不对
         */
        int connect(void *pAddr, size_t iSize);

        /**
         * 是否有标识
         * @param i
         * 
         * @return int, >0:有标识, =0:无标识, <0:超过范围
         */
        int get(size_t i);

        /**
         * 设置表示
         * @param i
         * 
         * @return int, >0:有标识, =0:无标识, <0:超过范围
         */
        int set(size_t i);

        /**
         * 清除标识
         * @param i
         * 
         * @return int, >0:有标识, =0:无标识, <0:超过范围
         */
        int clear(size_t i);

		/**
		 * 清除所有的数据
		 * 
		 * @author kevintian (2010-9-29)
		 * 
		 * @return int 
		 */
		int clear4all();

        /**
         * dump到文件
         * @param sFile
         * 
         * @return int
         */
        int dump2file(const string &sFile);

        /**
         * 从文件load
         * @param sFile
         * 
         * @return int
         */
        int load5file(const string &sFile);

        struct tagBitMapHead
        {
           char     _cVersion;          //版本, 当前版本为1
           size_t   _iMemSize;          //共享内存大小
        }__attribute__((packed));

        /**
         * 获取头部地址
         * 
         * @return tagBitMapHead*
         */
        BitMap::tagBitMapHead *getAddr() const   { return _pHead; }

        /**
         * 内存大小
         * 
         * @return size_t
         */
        size_t getMemSize() const                   { return _pHead->_iMemSize; }

    protected:

        /**
         * 共享内存头部
         */
        tagBitMapHead               *_pHead;

        /**
         * 数据块指针
         */
        unsigned char *             _pData;
    };

    /**
     * 根据元素个数计算需要内存的大小
     * @param iElementCount, 需要保存的元素个数(元素从0开始记)
     * @param iBitCount, 每个元素支持几位(默认1位) (位数>=1)
     * 
     * @return size_t
     */
    static size_t calcMemSize(size_t iElementCount, unsigned iBitCount = 1);

    /**
     * 初始化
     * @param pAddr 绝对地址
     * @param iSize 大小, 采用(calcMemSize)计算出来
     * @return 0: 成功, -1:内存不够
     */
    void create(void *pAddr, size_t iSize, unsigned iBitCount = 1);

    /**
     * 链接到内存块
     * @param pAddr  ,地址, 采用(calcMemSize)计算出来
     * @return 0, 成功, -1,版本不对, -2:大小不对
     */
    int connect(void *pAddr, size_t iSize, unsigned iBitCount = 1);

    /**
     * 是否有标识
     * @param i, 元素值
     * @param iBit, 第几位
     * 
     * @return int, >0:有标识, =0:无标识, <0:超过范围
     */
    int get(size_t i, unsigned iBit = 1);

    /**
     * 设置表示
     * @param i, 元素值
     * @param iBit, 第几位
     * 
     * @return int, >0:有标识, =0:无标识, <0:超过范围
     */
    int set(size_t i, unsigned iBit = 1);

    /**
     * 清除标识
     * @param i, 元素值
     * @param iBit, 第几位
     * 
     * @return int, >0:有标识, =0:无标识, <0:超过范围
     */
    int clear(size_t i, unsigned iBit = 1);

	/**
	 * 清除所有的标识
	 * 
	 * @author kevintian (2010-9-29)
	 * 
	 * @param iBit 
	 * 
	 * @return int 
	 */
	int clear4all(unsigned iBit = (unsigned)(-1));

    /**
     * dump到文件
     * @param sFile
     * 
     * @return int
     */
    int dump2file(const string &sFile);

    /**
     * 从文件load
     * @param sFile
     * 
     * @return int
     */
    int load5file(const string &sFile);

protected:
    vector<BitMap>   _bitmaps;
};

}

#endif

