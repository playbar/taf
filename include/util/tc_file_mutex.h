#ifndef __TC_FILE_MUTEX_H
#define __TC_FILE_MUTEX_H

#include "util/tc_lock.h"

namespace taf
{
/////////////////////////////////////////////////
// 说明: 文件锁类
// Author : j@syswin.com              
/////////////////////////////////////////////////
/**
* 异常类
*/
struct TC_FileMutex_Exception : public TC_Lock_Exception
{
    TC_FileMutex_Exception(const string &buffer) : TC_Lock_Exception(buffer){};
    TC_FileMutex_Exception(const string &buffer, int err) : TC_Lock_Exception(buffer, err){};
    ~TC_FileMutex_Exception() throw() {};
};

/**
 * 文件锁
 * 注意:只能在进程间加锁.
 */
class TC_FileMutex
{
public:
	/**
     * 构造函数
	 */
	TC_FileMutex();

	/**
     * 析够函数
	 */
	virtual ~TC_FileMutex();

	/**
	 * 初始化文件锁
	 * @param filename
	 */
	void init(const std::string& filename);

    /**
    * 加读锁
    *@return int
    */
    int rlock();

    /**
    * 解读锁
    * @return int
    */
    int unrlock();

    /**
    * 尝试读锁
    * @throws TC_FileMutex_Exception
    * @return bool : 加锁成功则返回false, 否则返回false
    */
    bool tryrlock();

    /**
    * 加写锁
    * @return int
    */
    int wlock();

    /**
    * 解写锁
    */
    int unwlock();

    /**
    * 尝试写锁
    * @throws TC_FileMutex_Exception
    * @return bool : 加锁成功则返回false, 否则返回false
    */
    bool trywlock();

    /**
    * 写锁
    * @return int, 0 正确
    */
    int lock(){return wlock();};

    /**
    * 解写锁
    */
    int unlock();

    /**
    * 尝试解锁
    * @throws TC_FileMutex_Exception
    * @return int, 0 正确
    */
    bool trylock() {return trywlock();};

protected:
	/**
     * 设置锁
	 * @param fd
	 * @param cmd
	 * @param type
	 * @param offset
	 * @param whence
	 * @param len
	 *
	 * @return int
	 */
	int lock(int fd, int cmd, int type, off_t offset, int whence, off_t len);

	/**
     * 是否被其他进程锁了
	 * @param fd
	 * @param type
	 * @param offset
	 * @param whence
	 * @param len
	 *
	 * @return bool
	 */
	bool hasLock(int fd, int type, off_t offset, int whence, off_t len);

private:
	int _fd;
};

}

#endif

