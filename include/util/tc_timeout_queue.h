#ifndef _TAF_TIMEOUT_QUEUE_H_
#define _TAF_TIMEOUT_QUEUE_H_

#include <deque>
//#include <map>
#include <ext/hash_map>
#include <iostream>
#include <list>
#include <cassert>
#include "util/tc_autoptr.h"
#include "util/tc_monitor.h"
#include "util/tc_functor.h"
#include "util/tc_timeprovider.h"

using namespace std;
using namespace __gnu_cxx;
 
namespace taf
{
/////////////////////////////////////////////////
// 说明: 超时队列(模板元素只能是智能指针)
// Author : j@syswin.com              
/////////////////////////////////////////////////

template<class T>
class TC_TimeoutQueue: public TC_ThreadMutex, public TC_HandleBase
{
public:

    struct PtrInfo;

    struct NodeInfo;

   // typedef map<uint32_t, PtrInfo> data_type;
    typedef hash_map<uint32_t, PtrInfo> data_type;

    typedef list<NodeInfo>         time_type;

    typedef TC_Functor<void, TL::TYPELIST_1(T&)> data_functor;

    struct PtrInfo
    {
        T ptr;

        typename time_type::iterator timeIter;
    };

    struct NodeInfo
    {
        bool hasPoped;

        time_t createTime;

        typename data_type::iterator dataIter;
    };

    /**
     * 超时队列，缺省5s超时
     * @param timeout
     * @param flow
     */
    TC_TimeoutQueue(int timeout = 5*1000,size_t size = 100 ) : _uniqId(0), _timeout(timeout) {_data.resize(size);}

    /**
     * 产生该队列的下一个ID
     */
    uint32_t generateId();

    /**
     * 获取指定id的数据
     * @param id
     * @return T
     */
    T get(uint32_t uniqId, bool bErase = true);

    /**
     * 删除
     * @param uniqId
     * 
     * @return T
     */
    T erase(uint32_t uniqId);

    /**
     * 设置消息到队列尾端
     * @param ptr
     * @return uint32_t
     */
    bool push(T& ptr, uint32_t uniqId);

    /**
     * 超时删除数据
     */
    void timeout();

    /**
     * 删除超时的数据
     * 并用df对数据做处理
     */
    void timeout(data_functor &df);

    /**
     * 取出队列头部的消息
     * @return T
     */
    T pop();

    /**
     * 交换数据 
     * @param q
     * @return bool
     */
    bool swap(deque<T> &q);

    /**
     * 设置超时时间(毫秒)
     * @param timeout
     */
    void setTimeout(int timeout) { _timeout = timeout; }

    /**
     * 队列中的数据
     * 
     * @return size_t
     */
    size_t size() const { TC_LockT<TC_ThreadMutex> lock(*this); return _data.size(); }

protected:
    uint32_t                        _uniqId;
    time_t                          _timeout;
    data_type                       _data;
    time_type                       _time;
    typename time_type::iterator    _firstNoPopIter;
};

template<typename T> T TC_TimeoutQueue<T>::get(uint32_t uniqId, bool bErase)
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    typename data_type::iterator it = _data.find(uniqId);

    if(it == _data.end())
    {
        return NULL;
    }

    T ptr = it->second.ptr;

    if(bErase)
    {
        _time.erase(it->second.timeIter);
        _data.erase(it);
    }

    return ptr;    
}

template<typename T> uint32_t TC_TimeoutQueue<T>::generateId()
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    while (++_uniqId == 0);

    return _uniqId;
}

template<typename T> bool TC_TimeoutQueue<T>::push(T& ptr, uint32_t uniqId)
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    time_t now = TC_TimeProvider::getInstance()->getNow();

    PtrInfo pi;

    pi.ptr = ptr;

    pair<typename data_type::iterator, bool> result;
   
    result = _data.insert(make_pair(uniqId, pi));

    if (result.second == false) return false;

    typename data_type::iterator it = result.first;

    NodeInfo ni;

    ni.createTime = now;

    ni.dataIter = it;

    ni.hasPoped = false;

    _time.push_back(ni);

    typename time_type::iterator tmp = _time.end();

    --tmp;

    it->second.timeIter = tmp;

    if (_firstNoPopIter == _time.end())
    {
        _firstNoPopIter = tmp;
    }

    return true;
}

template<typename T> void TC_TimeoutQueue<T>::timeout()
{
    time_t now = TC_TimeProvider::getInstance()->getNow();

    while(true)
    {
        TC_LockT<TC_ThreadMutex> lock(*this);

        typename time_type::iterator it = _time.begin();

        if(it != _time.end() && now - it->createTime > _timeout/1000)
        {
            _data.erase(it->dataIter);

            _time.erase(it);
        }
        else
        {
            break;
        }
    }
}

template<typename T> void TC_TimeoutQueue<T>::timeout(data_functor &df)
{
    time_t now = TC_TimeProvider::getInstance()->getNow();

    while(true)
    {
        T ptr;

        {
            TC_LockT<TC_ThreadMutex> lock(*this);

            typename time_type::iterator it = _time.begin();

            if(it != _time.end() && now - it->createTime > _timeout/1000)
            {
                ptr = (*it->dataIter).second.ptr;

                _data.erase(it->dataIter);

                _time.erase(it);
            }
            else
            {
                break;
            }
        }

        try { df(ptr); } catch(...) { }
    }
}

template<typename T> T TC_TimeoutQueue<T>::erase(uint32_t uniqId)
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    typename data_type::iterator it = _data.find(uniqId);

    if(it == _data.end())
    {
        return NULL;
    }

    T ptr = it->second.ptr;

    _time.erase(it->second.timeIter);

    _data.erase(it);

    return ptr;    
}

template<typename T> T TC_TimeoutQueue<T>::pop()
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    if(_time.empty())
    {
        return NULL;
    }

    typename time_type::iterator it = _time.begin();

    if (it->hasPoped == true)
    {
        it = _firstNoPopIter;
    }

    if (it == _time.end())
    {
        return NULL;
    }

    assert(it->hasPoped == false);

    T ptr = it->dataIter->second.ptr;

    it->hasPoped = true;

    _firstNoPopIter = it;

    ++_firstNoPopIter;

    return ptr;
}

template<typename T> bool TC_TimeoutQueue<T>::swap(deque<T> &q)
{
    TC_LockT<TC_ThreadMutex> lock(*this);

    if(_time.empty())
    {
        return false;
    }

    typename time_type::iterator it = _time.begin();

    while(it != _time.end())
    {
    
        if (it->hasPoped == true)
        {
            it = _firstNoPopIter;
        }

        if (it == _time.end())
        {
            break;
        }
    
        assert(it->hasPoped == false);
    
        T ptr = it->dataIter->second.ptr;
    
        it->hasPoped = true;
    
        _firstNoPopIter = it;
    
        ++_firstNoPopIter;

        q.push_back(ptr);

        ++it;
    }

    if(q.empty())
    {
        return false;
    }

    return true;
}
/////////////////////////////////////////////////////////////////
}
#endif
