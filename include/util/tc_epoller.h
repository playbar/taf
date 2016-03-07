#ifndef	__TC_EPOLLER_H_
#define __TC_EPOLLER_H_

#include <sys/epoll.h>
#include <cassert>

namespace taf
{
/////////////////////////////////////////////////
// 说明: epoll操作封装类
// Author : j@syswin.com              
/////////////////////////////////////////////////
// 
/**
 * epoller操作类
 * 已经默认采用了EPOLLET方式做触发
 */
class TC_Epoller
{
public:

	/**
     * 构造函数
     * @param bEt: 默认是ET模式
	 */
	TC_Epoller(bool bEt = true);

	/**
     * 析够函数
	 */
	~TC_Epoller();

	/**
     * 生成epoll句柄
     * @param max_connections : epoll服务需要支持的最大连接数
	 */
	void create(int max_connections);

	/**
     * 添加监听句柄
     * @param fd, 句柄
     * @param data, 辅助的数据, 可以后续在epoll_event中获取到
     * @param event, 需要监听的事件EPOLLIN|EPOLLOUT
     *              
	 */
	void add(int fd, long long data, __uint32_t event);

	/**
     * 修改句柄事件
     * @param fd, 句柄
     * @param data, 辅助的数据, 可以后续在epoll_event中获取到
     * @param event, 需要监听的事件EPOLLIN|EPOLLOUT
	 */
	void mod(int fd, long long data, __uint32_t event);

	/**
     * 删除句柄事件
     * @param fd, 句柄
     * @param data, 辅助的数据, 可以后续在epoll_event中获取到
     * @param event, 需要监听的事件EPOLLIN|EPOLLOUT
	 */
	void del(int fd, long long data, __uint32_t event);

	/**
     * 等待时间
     * @param millsecond, 毫秒
	 *
     * @return int, 返回多少个句柄有事件触发
	 */
	int wait(int millsecond);

	/**
     * 获取被触发的事件
	 *
	 * @return struct epoll_event&
	 */
	struct epoll_event& get(int i) { assert(_pevs != 0); return _pevs[i]; }

protected:

	/**
     * 控制epoll
     * @param fd, 句柄
     * @param data, 辅助的数据, 可以后续在epoll_event中获取到
     * @param event, 需要监听的事件
     * @param op, 操作
	 */
	void ctrl(int fd, long long data, __uint32_t events, int op);

protected:

    /**
     * 	epoll
     */
    int _iEpollfd;

	/**
     * 最大连接数
	 */
	int	_max_connections;

	/**
     * 事件集
	 */
	struct epoll_event *_pevs;

    /**
     * 是否是ET模式
     */
    bool _et;
};

}
#endif

