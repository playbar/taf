#ifndef _JMEM_QUEUE_H
#define _JMEM_QUEUE_H

#include "util/tc_mem_queue.h"
#include "jmem/jmem_policy.h"
#include "jce/Jce.h"

namespace taf
{

/**
 * 基于Jce协议的内存循环队列
 * 编解码出错则抛出JceDecodeException和JceEncodeException
 * 可以对锁策略和存储策略进行组合, 例如:
 * 基于信号量锁, 文件存储的队列:
 * JceQueue<Test::QueueElement, SemLockPolicy, FileStorePolicy>
 * 基于信号量锁, 共享内存存储的队列
 * JceQueue<Test::QueueElement, SemLockPolicy, ShmStorePolicy>
 * 基于线程锁, 内存存储的队列
 * JceQueue<Test::QueueElement, ThreadLockPolicy, MemStorePolicy>
 * 
 * 使用上, 不同的组合, 初始化函数不完全一样
 * 初始化函数有:
 * SemLockPolicy::initLock(key_t)
 * ShmStorePolicy::initStore(key_t, size_t)
 * FileStorePolicy::initStore(const char *file, size_t)
 * 等, 具体参见jmem_policy.h
 */

template<typename T,
         typename LockPolicy,
         template<class,class> class StorePolicy>
class JceQueue : public StorePolicy<TC_MemQueue, LockPolicy>
{
public:
    /**
     * 弹出一个元素
     * @param t
     *
     * @return bool,true:成功, false:无元素
     */
    bool pop_front(T &t)
    {
        string s;

        {
            TC_LockT<typename LockPolicy::Mutex> lock(LockPolicy::mutex());
            if(!this->_t.pop_front(s))
            {
                return false;
            }
        }
        taf::JceInputStream<BufferReader> is;
        is.setBuffer(s.c_str(), s.length());
        t.readFrom(is);

        return true;
    }

    /**
     * 插入到队列
     * @param t
     *
     * @return bool, ture:成功, false:队列满
     */
    bool push_back(const T &t)
    {
        taf::JceOutputStream<BufferWriter> os;
        t.writeTo(os);

        TC_LockT<typename LockPolicy::Mutex> lock(LockPolicy::mutex());
        return this->_t.push_back(os.getBuffer(), os.getLength());
    }

    /**
     * 是否满了
     * @param t
     * 
     * @return bool
     */
    bool isFull(const T &t) 
    { 
        taf::JceOutputStream<BufferWriter> os;
        t.writeTo(os);

        TC_LockT<typename LockPolicy::Mutex> lock(LockPolicy::mutex());
        return this->_t.isFull(os.getLength()); 
    }

    /**
    * 队列是否满
    * @param : iSize, 输入数据块长度
    * @return bool , true:满, false: 非满
    */
    bool isFull(size_t iSize) 
    { 
        TC_LockT<typename LockPolicy::Mutex> lock(LockPolicy::mutex());
        return this->_t.isFull(iSize); 
    }

    /**
    * 队列是否空
    * @return bool , true: 满, false: 非满
    */
    bool isEmpty()
    {
        TC_LockT<typename LockPolicy::Mutex> lock(LockPolicy::mutex());
        return this->_t.isEmpty();
    }

    /**
    * 队列中元素个数, 不加锁的情况下不保证一定正确
    * @return size_t, 元素个数
    */
    size_t elementCount()
    {
        TC_LockT<typename LockPolicy::Mutex> lock(LockPolicy::mutex());
        return this->_t.elementCount();
    }


    /**
    * 队列长度(字节), 小于总存储区长度(总存储区长度包含了控制快)
    * @return size_t : 队列长度
    */
    size_t queueSize()
    {
        TC_LockT<typename LockPolicy::Mutex> lock(LockPolicy::mutex());
        return this->_t.queueSize();
    }

    /**
    * 共享内存长度
    * @return size_t : 共享内存长度
    */
    size_t memSize() const 
    {
        TC_LockT<typename LockPolicy::Mutex> lock(LockPolicy::mutex());
        return this->_t.memSize();
    };
};

}

#endif
