#ifndef	__TC_MEM_CHUNK_H__
#define __TC_MEM_CHUNK_H__

#include <string>
#include <vector>

using namespace std;

namespace taf
{
/////////////////////////////////////////////////
// 说明: 内存分配器
// Author : j@syswin.com              
// Author : .com              
/////////////////////////////////////////////////
/**
* 原始内存块, 由TC_MemChunkAllocator来负责分配和维护
*
* 将连续的内存分成大小相同的块, 形成链表
* 并能够分配和释放这些大小相同的快
*/
class TC_MemChunk
{
public:

    /**
    * 构造函数
    */
    TC_MemChunk();

    /**
     * 计算Chunk需要的内存块大小
     * @param iBlockSize
     * @param iBlockCount
     *
     * @return size_t
     */
    static size_t calcMemSize(size_t iBlockSize, size_t iBlockCount);

    /**
     * 计算block个数
     * @param iMemSize
     * @param iBlockSize
     *
     * @return size_t
     */
    static size_t calcBlockCount(size_t iMemSize, size_t iBlockSize);

     /**
     * tagChunkHead的大小
     *
     * @return size_t
     */
    static size_t getHeadSize() { return sizeof(tagChunkHead); }

    /**
     * 初始化, 要保证p指向的内存指针=getMemSize大小
     * @param pAddr, 地址, 换到应用程序的绝对地址
     * @param iBlockSize, block大小
     * @param iBlockCount, block个数
     */
    void create(void *pAddr, size_t iBlockSize, size_t iBlockCount);

    /**
     * 连接上
     * @param pAddr, 地址, 换到应用程序的绝对地址
     */
    void connect(void *pAddr);

    /**
     * block的大小
     *
     * @return size_t
     */
    size_t getBlockSize() const { return _pHead->_iBlockSize; }

    /**
     * 所有的内存大小
     *
     * @return size_t
     */
    size_t getMemSize() const { return _pHead->_iBlockSize * _pHead->_iBlockCount + sizeof(tagChunkHead); }

    /**
     * 可以存放数据的总容量
     *
     * @return size_t
     */
    size_t getCapacity() const { return _pHead->_iBlockSize * _pHead->_iBlockCount; }

    /**
     * block的个数
     *
     * @return unsigned char
     */
    size_t getBlockCount() const { return _pHead->_iBlockCount; }

    /**
     * 是否还有可用block
     *
     * @return bool
     */
    bool isBlockAvailable() const { return _pHead->_blockAvailable > 0; }

    /**
     * 可以利用的block的个数
     *
     * @return unsigned char
     */
    size_t getBlockAvailableCount() const { return _pHead->_blockAvailable; }

    /**
     * 分配一个区块
     *
     * @return void*
     */
    void* allocate();

	/**
    * 分配一个区块，返回以1为基数的区块索引，没有可分配空间时返回0
    * 通查索引都是比较小(即使在64位操作系统上), 4个字节以内
    * 便于节省内存
	*/
	void* allocate2(size_t &iIndex);

    /**
     * 释放区块
     * @param p
     */
    void deallocate(void *pAddr);

    /**
     * 根据索引释放区块
     * @param iIndex
     */
	void deallocate2(size_t iIndex);

    /**
     * 重建
     */
    void rebuild();

    /**
     * chunk头部
     */
    struct tagChunkHead
    {
        size_t  _iBlockSize;            /**区块大小*/
        size_t  _iBlockCount;           /**block个数*/
        size_t  _firstAvailableBlock;   /**第一个可用的block索引*/
        size_t  _blockAvailable;        /**可用block个数*/
    }__attribute__((packed));

    /**
     * 获取头部信息
     *
     * @return tagChunkHead
     */
    tagChunkHead getChunkHead() const;

	/**
	* 根据索引获取绝对地址
	*/
	void* getAbsolute(size_t iIndex);

    /**
     * 绝对地址换成索引
     * 
     * @param pAddr 
     * 
     * @return size_t 
     */
	size_t getRelative(void *pAddr);

protected:
    /**
     * 初始化
     */
    void init(void *pAddr);

private:

    /**
     * 区块头指针
     */
    tagChunkHead    *_pHead;

    /**
     * 数据区指针
     */
    unsigned char   *_pData;
};

/**
* 内存块分配器
* 最下层的原始内存块分配
* 内存结构:
* 内存块长度, 4个字节
* Block大小, 4个字节
* Chunk个数, 4个字节
* TC_MemChunk
* 暂时只支持同一个Block大小的MemChunk
*/
class TC_MemChunkAllocator
{
public:

    /**
    * 构造函数
    */
    TC_MemChunkAllocator();

    /**
     * 初始化
     * @param pAddr, 地址, 换到应用程序的绝对地址
     * @param iSize, 内存大小
     * @param iBlockSize, block的大小
     */
    void create(void *pAddr, size_t iSize, size_t iBlockSize);

    /**
     * 连接上
     * @param pAddr, 地址, 换到应用程序的绝对地址
     */
    void connect(void *pAddr);

	/**
	 * 头地址指针
	 */
	void *getHead()	const       { return _pHead; }

    /**
     * 每个block的大小
     *
     * @return size_t
     */
    size_t getBlockSize()  const { return _pHead->_iBlockSize; }

    /**
     * 总计内存大小
     *
     * @return size_t
     */
    size_t getMemSize()  const { return _pHead->_iSize; }

    /**
     * 可以存放数据的总容量
     *
     * @return size_t
     */
    size_t getCapacity() const { return _chunk.getCapacity(); }

    /**
     * 分配一个区块,绝对地址
     *
     * @return void*
     */
    void* allocate();

	/**
	* 分配一个区块，返回以1为基数的区块索引，没有可分配空间时返回0
	*/
	void* allocate2(size_t &iIndex);

    /**
     * 释放区块, 绝对地址
     * @param p
     */
    void deallocate(void *pAddr);

    /**
     * 释放区块, 区块索引
     * @param p
     */
    void deallocate2(size_t iIndex);

    /**
     * 所有chunk的区块合计的block的个数
     *
     * @return size_t
     */
    size_t blockCount() const           { return _chunk.getBlockCount(); }

	/**
	* 根据索引获取绝对地址
	*/
	void* getAbsolute(size_t iIndex)    { return _chunk.getAbsolute(iIndex); };

    /**
     * 绝对地址换成索引
     * @param pAddr 
     * 
     * @return size_t 
     */
	size_t getRelative(void *pAddr)     { return _chunk.getRelative(pAddr); };

    /**
     * 获取头部信息
     *
     * @return TC_MemChunk::tagChunkHead
     */
    TC_MemChunk::tagChunkHead getBlockDetail() const;

    /**
     * 重建
     */
    void rebuild();

    /**
     * 头部内存块
     */
    struct tagChunkAllocatorHead
    {
        size_t  _iSize;
        size_t  _iBlockSize;
    }__attribute__((packed));

    /**
     * 头部大小
     *
     * @return size_t
     */
    static size_t getHeadSize() { return sizeof(tagChunkAllocatorHead); }

protected:

    /**
     * 初始化
     */
    void init(void *pAddr);

    /**
     * 初始化
     */
    void initChunk();

    /**
     * 连接
     */
    void connectChunk();

    //不允许copy构造
    TC_MemChunkAllocator(const TC_MemChunkAllocator &);
    //不允许赋值
    TC_MemChunkAllocator& operator=(const TC_MemChunkAllocator &);
    bool operator==(const TC_MemChunkAllocator &mca) const;
    bool operator!=(const TC_MemChunkAllocator &mca) const;

private:

    /**
     * 头指针
     */
    tagChunkAllocatorHead   *_pHead;

    /**
     *  chunk开始的指针
     */
    void                    *_pChunk;

    /**
     * chunk链表
     */
    TC_MemChunk             _chunk;
};

/**
 * 多块分配器,可以分配多个不同大小的块
 *
 * 初始化时指定:最小块大小, 最大块大小, 块间大小比值
 * 自动计算出块的个数(每种大小块的个数相同)
 *
 */
class TC_MemMultiChunkAllocator
{
public:

    /**
    * 构造函数
    */
    TC_MemMultiChunkAllocator();

    /**
     * 析够函数
     */
    ~TC_MemMultiChunkAllocator();


    /**
     * 初始化
     * @param pAddr, 地址, 换到应用程序的绝对地址
     * @param iSize, 内存大小
     * @param iMinBlockSize, block的大小
     * @param iMaxBlockSize,
     * @param fFactor, 因子
     */
    void create(void *pAddr, size_t iSize, size_t iMinBlockSize, size_t iMaxBlockSize, float fFactor = 1.1);

    /**
     * 连接上
     * @param pAddr, 地址, 换到应用程序的绝对地址
     */
    void connect(void *pAddr);

    /**
     * 扩展空间
     * 
     * @param pAddr, 已经是空间被扩展之后的地址
     * @param iSize
     */
    void append(void *pAddr, size_t iSize);

    /**
     * 每个block的大小, 包括后续增加的内存块的大小
     *
     * @return vector<size_t>
     */
    vector<size_t> getBlockSize()  const;

    /**
     * 每个block中chunk个数(都是相等的)
     * 
     * @return size_t 
     */
    size_t getBlockCount() const { return _iBlockCount; }

    /**
     * 获取每个块头部信息, 包括后续增加的内存块的大小
     * @param i
     *
     * @return vector<TC_MemChunk::tagChunkHead>
     */
    vector<TC_MemChunk::tagChunkHead> getBlockDetail() const;

    /**
     * 总计内存大小, 包括后续增加的内存块的大小
     *
     * @return size_t
     */
    size_t getMemSize()  const              { return _pHead->_iTotalSize; }

    /**
     * 真正可以放数据的容量, 包括后续增加的内存块的数据容量
     *
     * @return size_t
     */
    size_t getCapacity() const;

    /**
     * 一个chunk的block个数, 包括后续增加的内存块的
     *
     * @return vector<size_t>
     */
    vector<size_t> singleBlockChunkCount() const;

    /**
     * 所有chunk的区块合计的block的个数
     *
     * @return size_t
     */
    size_t allBlockChunkCount() const;

    /**
     * 分配一个区块,绝对地址
     *
     * @param iNeedSize,需要分配的大小
     * @param iAllocSize, 分配的数据块大小
     * @return void*
     */
    void* allocate(size_t iNeedSize, size_t &iAllocSize);

    /**
     * 分配一个区块, 返回区块索引
     * @param iNeedSize,需要分配的大小
     * @param iAllocSize, 分配的数据块大小
     * @param size_t，以1为基数的索引，0表示无效
     * @return void *
     */
    void* allocate2(size_t iNeedSize, size_t &iAllocSize, size_t &iIndex);

    /**
     * 释放区块, 绝对地址
     * @param p
     */
    void deallocate(void *pAddr);

    /**
     * 释放区块, 区块索引
     * @param p
     */
    void deallocate2(size_t iIndex);

    /**
     * 重建
     */
    void rebuild();

    /**
     * 相对索引换算成绝对地址
     * @param iIndex
     *
     * @return void*
     */
    void *getAbsolute(size_t iIndex);

    /**
     * 绝对地址换成索引地址
     *
     * @return size_t
     */
    size_t getRelative(void *pAddr);

    /**
     * 头部内存块
     */
    struct tagChunkAllocatorHead
    {
        size_t  _iSize;             //当前块大小
        size_t  _iTotalSize;        //后续分配块合在一起的大小 
        size_t  _iMinBlockSize;
        size_t  _iMaxBlockSize;
        float   _fFactor;
        size_t  _iNext;             //下一个分配器地址, 如果没有则为0
    }__attribute__((packed));

    /**
     * 头部大小
     *
     * @return size_t
     */
    static size_t getHeadSize() { return sizeof(tagChunkAllocatorHead); }

protected:

    /**
     * 初始化
     */
    void init(void *pAddr);

    /**
     * 计算
     */
	void calc();

    /**
     * 清空
     */
    void clear();

    /**
     * 最后一个分配器
     * 
     * @return TC_MemMultiChunkAllocator*
     */
    TC_MemMultiChunkAllocator *lastAlloc();

    //不允许copy构造
    TC_MemMultiChunkAllocator(const TC_MemMultiChunkAllocator &);
    //不允许赋值
    TC_MemMultiChunkAllocator& operator=(const TC_MemMultiChunkAllocator &);
    bool operator==(const TC_MemMultiChunkAllocator &mca) const;
    bool operator!=(const TC_MemMultiChunkAllocator &mca) const;

private:

    /**
     * 头指针
     */
    tagChunkAllocatorHead   *_pHead;

    /**
     *  chunk开始的指针
     */
    void                    *_pChunk;

    /**
     * 区块大小
     */
    vector<size_t>          _vBlockSize;

    /**
     * 每个chunk中block的个数
     */
    size_t                  _iBlockCount;

    /**
     * chunk链表
     */
    vector<TC_MemChunkAllocator*>       _allocator;

    /**
     * 所有的索引个数
     */
    size_t                              _iAllIndex;

    /**
     * 后续的多块分配器
     */
    TC_MemMultiChunkAllocator           *_nallocator;
};

}

#endif
