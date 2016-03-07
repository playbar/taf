#ifndef __CONSISTENT_HASH__
#define __CONSISTENT_HASH__

#include "util/tc_md5.h"
/////////////////////////////////////////////////
// 说明: 一致性hash算法类
// Author : j@syswin.com              
/////////////////////////////////////////////////
/**
 * 一致性hash算法类
 */

using namespace std;

namespace taf
{

struct node_T
{
    unsigned int iHashCode;  //节点hash值
    unsigned int iIndex;     //节点下标
};

class  TC_ConsistentHash
{
    public:
        TC_ConsistentHash()
        {
        }

        /**
         * 节点比较
         *
         * @param m1
         * @param m2
         *
         * @return less or not
         */
        static bool less_hash(const node_T & m1, const node_T & m2)
        {
            return m1.iHashCode < m2.iHashCode;
        }

        /**
         * 增加节点
         *
         * @param node  节点名称
         * @param index 节点的下标值
         *
         * @return 节点的hash值
         */
        unsigned addNode(const string & node, unsigned int index)
        {
            node_T stItem;
            stItem.iHashCode = hash_md5(TC_MD5::md5bin(node));
            stItem.iIndex = index;
            vHashList.push_back(stItem);

            sort(vHashList.begin(), vHashList.end(), less_hash);

            return stItem.iHashCode;
        }

        /**
         * 删除节点
         *
         * @param node  节点名称
         *
         * @return  0 : 删除成功  -1 : 没有对应节点
         */
        int removeNode(const string & node)
        {
            unsigned iIndex = hash_md5(TC_MD5::md5bin(node));
            vector<node_T>::iterator it;
            for(it=vHashList.begin() ; it!=vHashList.end(); it++)
            {
                if(it->iIndex == iIndex)
                {
                    vHashList.erase(it);
                    return 0;
                }
            }
            return -1;
        }

        /**
         * 获取某key对应到的节点node的下标
         *
         * @param key         key名称
         * @param out  iIndex 对应到的节点下标
         *
         * @return  0:获取成功   -1:没有被添加的节点
         */
        int getIndex(const string & key, unsigned int & iIndex)
        {
            unsigned iCode = hash_md5(TC_MD5::md5bin(key));
            if(vHashList.size() == 0)
            {
                iIndex = 0;
                return -1;
            }

            int low = 0;
            int high = vHashList.size();

            if(iCode <= vHashList[0].iHashCode || iCode > vHashList[high-1].iHashCode)
            {
                iIndex = vHashList[0].iIndex;
                return 0;
            }

            while (low < high - 1)
            {
                int mid = (low + high) / 2;
                if (vHashList[mid].iHashCode > iCode)
                {
                    high = mid;
                }
                else
                {
                    low = mid;
                }
            }
            iIndex = vHashList[low+1].iIndex;
            return 0;
        }

   protected:
        /*
         * 计算md5值的hash，分布范围在 0 -- 2^32-1
         *
         * @param  sMd5 : md5 值
         *
         * @return hash值
         */
        unsigned int hash_md5(const string & sMd5)
        {
            char *p = (char *) sMd5.c_str();
            return (*(int*)(p)) ^ (*(int*)(p+4)) ^ (*(int*)(p+8)) ^ (*(int*)(p+12));
        }

        vector<node_T> vHashList;

};

}
#endif
