#ifndef __TC_LOGGER_H
#define __TC_LOGGER_H

#include <streambuf>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <time.h>
#include <sys/syscall.h>
#include <iomanip>
#include "util/tc_ex.h"
#include "util/tc_autoptr.h"
#include "util/tc_common.h"
#include "util/tc_file.h"
#include "util/tc_thread.h"
#include "util/tc_monitor.h"
#include "util/tc_thread_pool.h"
#include "util/tc_timeprovider.h"

using namespace std;

namespace taf
{
/////////////////////////////////////////////////
// 说明: 日志类
// Author : j@syswin.com              
// Modify : j@syswin.com   
// Modify : j@syswin.com               
/////////////////////////////////////////////////
/*****************************************************************************
说明:
	可扩展的日志类,可以自定义滚动策略和写方法.
	提供了两种方式的滚动方式:
	1 按文件大小,文件数目滚动;
	2 按时间滚动(按天/小时/分钟等)
	上述两种默认的策略中,都是默认写入文件的,当没有初始化,则默认写入到cout

	自定义滚动方式的步骤如下:
	1 实现自定义模板类,继承于TC_LoggerRoll,实现roll函数,该函数实现写逻辑, 例如:
	template<typename WriteT>
	class MyRoll : public TC_LoggerRoll, public TC_ThreadMutex
	2 模板中的WriteT是具体的写入逻辑,提供缺省的:TC_DefaultWriteT
	3 可以自定义实现WriteT, 例如:实现发送到网络的逻辑.
	4 在MyRoll中实现内部类RollWrapperI, 并继承RollWrapperBase<MyRoll<WriteT> >
	5 RollWrapperI主要就是提供MyRoll的初始化接口, 这些接口最后会直接反应到TC_Logger中.
	6 RollWrapperI构造时, 构造MyRoll对象(采用智能指针),并且接口的操作直接转发到MyRoll对象中.

	实现这些逻辑后, 采用模板方式定义出类型:
	typedef TC_Logger<MyWriteT, MyRoll> MyLogger;

	TAF的远程log实现例子,可以参见src/libtaf/taf_logger.h
*****************************************************************************/
/**
* 日志异常类
*/
	struct TC_Logger_Exception : public TC_Exception
	{
		TC_Logger_Exception(const string &buffer) : TC_Exception(buffer){};
		TC_Logger_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
		~TC_Logger_Exception() throw(){};
	};

/**
 * 写操作
 */
	class TC_DefaultWriteT
	{
	public:
		void operator()(ostream &of, const deque<pair<int, string> > &ds)
		{
			deque<pair<int, string> >::const_iterator it = ds.begin();
			while (it != ds.end())
			{
				of << it->second;
				++it;
			}
			of.flush();
		}
	};

	class TC_LoggerThreadGroup;

//////////////////////////////////////////////////////////////////////////////
/**
 * 具体写日志基类
 */
	class TC_LoggerRoll : public TC_HandleBase
	{
	public:
		/**
		 * 构造函数
		 */
		TC_LoggerRoll() : _pThreadGroup(NULL)
		{
		}

		/**
		 * 实时记日志, 并且滚动
		 * 不同的策略日志类,实现不同的逻辑
		 * @param buffer
		 */
		virtual void roll(const deque<pair<int, string> > &ds) = 0;

		/**
		 * 安装线程
		 * @param ltg
		 */
		void setupThread(TC_LoggerThreadGroup *pThreadGroup);

		/**
		 * 取消线程
		 */
		void unSetupThread();

		/**
		 * 写到日志
		 * @param buffer
		 */
		void write(const pair<int, string> &buffer);

		/**
		 * 刷新缓存到文件
		 */
		void flush();

		/**
		 * 设置染色是否生效
		 * @param bEnable
		 */
		void enableDyeing(bool bEnable) 
		{
			TC_LockT<TC_ThreadMutex> lock(_mutexDyeing);

			if (bEnable)
			{
				_setThreadID.insert(pthread_self());
			}
			else
			{
				_setThreadID.erase(pthread_self());
			}

			_bDyeingFlag = (_setThreadID.size() > 0);
		}

	protected:

		/**
		 * buffer
		 */
		TC_ThreadQueue<pair<int, string> >  _buffer;

		/**
		 * 锁
		 */
		TC_ThreadMutex          _mutex;

		/**
		 * 线程组
		 */
		TC_LoggerThreadGroup    *_pThreadGroup;

		/**
		 * 是否已经染色的标志
		 */
		static bool 			_bDyeingFlag;

		/**
		 * 染色用锁
		 * 
		 * @author kevintian (2010-10-9)
		 */
		static TC_ThreadMutex	_mutexDyeing;

		/**
		 * 染色的线程ID集合
		 * 
		 * @author kevintian (2010-10-9)
		 */
		static set<pthread_t> 	_setThreadID;
	};

	typedef TC_AutoPtr<TC_LoggerRoll> TC_LoggerRollPtr;

//////////////////////////////////////////////////////////////////////////////
//
/**
 * 写日志线程组
 * 关键点:注册日志后,会保存职能指针
 * 保证日志对象一直存在
 */
	class TC_LoggerThreadGroup : public TC_ThreadLock
	{
	public:
		/**
		 * 构造函数
		 */
		TC_LoggerThreadGroup();

		/**
		 * 析够函数
		 */
		~TC_LoggerThreadGroup();

		/**
		 * 启动线程
		 * @param iThreadNum
		 */
		void start(size_t iThreadNum);

		/**
		 * 注册logger对象
		 * @param l
		 */
		void registerLogger(TC_LoggerRollPtr &l);

		/**
		 * 卸载logger对象
		 * @param l
		 */
		void unRegisterLogger(TC_LoggerRollPtr &l);

		/**
		 * 刷新所有的数据
		 */
		void flush();

	protected:

		/**
		 * 写日志
		 */
		void run();

		/**
		 * 指针比较
		 */
		struct KeyComp
		{
			bool operator()(const TC_LoggerRollPtr& p1, const TC_LoggerRollPtr& p2) const
			{
				return p1.get() <  p2.get();
			}
		};

		typedef set<TC_LoggerRollPtr, KeyComp>  logger_set;

	protected:

		/**
		 * 结束
		 */
		bool            _bTerminate;

		/**
		 * 写线程
		 */
		TC_ThreadPool   _tpool;

		/**
		 * logger对象
		 */
		logger_set      _logger;

	};

/**
 * 自定义logger buffer
 */
	class LoggerBuffer : public std::basic_streambuf<char>
	{
	public:
		//定义最大的buffer大小(10M)
		enum
		{
			MAX_BUFFER_LENGTH = 1024*1024*10,
		};

		/**
		 * 
		 */
		LoggerBuffer();

		/**
		 * 构造函数
		 * @param logger
		 */
		LoggerBuffer(TC_LoggerRollPtr roll, size_t buffer_len);

		/**
		 * 析构函数
		 */
		~LoggerBuffer();

	protected:

		/**
		 * 分配空间
		 * @param n
		 */
		void reserve(std::streamsize n);

		/**
		 * 放数据
		 * @param s
		 * @param n
		 * 
		 * @return streamsize
		 */
		virtual streamsize xsputn(const char_type* s, streamsize n);

		/**
		 * buffer满了, 具体写数据
		 * @param c
		 * 
		 * @return int_type
		 */
		virtual int_type overflow(int_type c);

		/**
		 * 读空间empty了(不实现)
		 * 
		 * @return int_type
		 */
		virtual int_type underflow()    { return std::char_traits<char_type>::eof();}

		/**
		 * 具体写逻辑
		 * 
		 * @return int
		 */
		virtual int sync();

	protected:
		LoggerBuffer(const LoggerBuffer&);
		LoggerBuffer& operator=(const LoggerBuffer&);

	protected:
		/**
		 * 写日志
		 */ 
		TC_LoggerRollPtr    _roll;

		/**
		 * 缓冲区
		 */
		char*                       _buffer;

		/**
		 * 缓冲区大小
		 */
		std::streamsize     _buffer_len;
	};

/**
 * 临时类, 析够的时候写日志
 */
	class LoggerStream
	{
	public:
		/**
		 * 构造
		 * @param stream
		 * @param mutex
		 */
		LoggerStream(ostream *stream, ostream *estream, TC_ThreadMutex &mutex) : _stream(stream), _estream(estream), _mutex(mutex)
		{
		}

		/**
		 * 析构
		 */
		~LoggerStream()
		{
			if (_stream)
			{
				_stream->flush();
				_mutex.unlock();
			}
		}

		/**
		* 重载<<
		*/
		template <typename P>
		LoggerStream& operator << (const P &t)  { if (_stream) *_stream << t;return *this;}

		/**
		 * endl,flush等函数
		 */
		typedef ostream& (*F)(ostream& os);
		LoggerStream& operator << (F f)         { if (_stream) (f)(*_stream);return *this;}

		/**
		 * hex等系列函数
		 */
		typedef ios_base& (*I)(ios_base& os);
		LoggerStream& operator << (I f)         { if (_stream) (f)(*_stream);return *this;}

		/**
		 * 字段转换成ostream类型
		 *
		 * @return ostream&
		 */
		operator ostream&()                     
		{ 
			if (_stream)
			{
				return *_stream; 
			}

			return *_estream; 
		}

		//不实现
		LoggerStream(const LoggerStream& lt);
		LoggerStream& operator=(const LoggerStream& lt);

	protected:

		/**
		 * 输出流
		 */
		std::ostream    *_stream;

		/**
		 * 
		 */
		std::ostream    *_estream;

		/**
		 * 锁
		 */
		TC_ThreadMutex  &_mutex;
	};

/**
 * 日志基类
 */
	template<typename WriteT, template<class> class RollPolicy>
	class TC_Logger : public RollPolicy<WriteT>::RollWrapperI
	{
	public:

		/**
		 * 设置显示标题
		 */
		enum
		{
			HAS_TIME   = 0x01,	//秒级别时间
			HAS_PID    = 0x02,	//进程ID
			HAS_LEVEL  = 0x04,	//日志等级
			HAS_MTIME  = 0x08	//毫秒级别时间(不建议使用, 会影响性能)
		};

		/**
		* 枚举类型,定义日志的四种等级
		*/
		enum
		{
			NONE_LOG    = 1,	//所有的log都不写
			ERROR_LOG   = 2,	//写错误log
			WARN_LOG    = 3,	//写错误,警告log
			DEBUG_LOG   = 4,	//写错误,警告,调试log
			INFO_LOG    = 5		//写错误,警告,调试,Info log
		};

		/**
		 * 日志级别名称
		 */
		static const string LN[6];

		/**
		 * 构造函数
		 */
		TC_Logger()
		: _flag(HAS_TIME)
		, _level(DEBUG_LOG)
		, _buffer(TC_LoggerRollPtr::dynamicCast(this->_roll), 1024)
		, _stream(&_buffer)
		, _ebuffer(NULL, 0)
		, _estream(&_ebuffer)
		{
		}

		/**
		 * 析够函数
		 */
		~TC_Logger()
		{
		}

		/**
		 * 修改标题
		 * @param flag
		 * @param add
		 */
		void modFlag(int flag, bool add = true)
		{
			if (add)
			{
				_flag |= flag;
			}
			else
			{
				_flag &= ~flag;
			}
		}

		/**
		 * 是否有某标示
		 *
		 * @return bool
		 */
		bool hasFlag(int flag) const    { return _flag & flag;}

		/**
		 * 获取flag
		 *
		 * @return int
		 */
		int getFlag() const             { return _flag;}

		/**
		* 获取日志等级
		* @return int 等级
		*/
		int getLogLevel() const         { return _level;}

		/**
		* 设置日志等级
		* @param level : 等级
		* @return int
		*/
		int setLogLevel(int level)
		{
			if (!isLogLevelValid(level))
			{
				return -1;
			}

			_level = level;
			return 0;
		}

		/**
		 * 设置等级
		 * @param level
		 * @param int
		 */
		int setLogLevel(const string &level)
		{
			if (level == "ERROR")
			{
				return setLogLevel(ERROR_LOG);
			}
			else if (level == "WARN")
			{
				return setLogLevel(WARN_LOG);
			}
			else if (level == "DEBUG")
			{
				return setLogLevel(DEBUG_LOG);
			}
			else if (level == "NONE")
			{
				return setLogLevel(NONE_LOG);
			}
			else if (level == "INFO")
			{
				return setLogLevel(INFO_LOG);
			}
			return -1;
		}

		/**
		* DEBUG记日志
		*/
		LoggerStream info()     { return stream(INFO_LOG);}

		/**
		* DEBUG记日志
		*/
		LoggerStream debug()    { return stream(DEBUG_LOG);}

		/**
		* WARNING记日志
		*/
		LoggerStream warn()     { return stream(WARN_LOG);}

		/**
		* ERROR记日志
		*/
		LoggerStream error()    { return stream(ERROR_LOG);}

		/**
		* 记所有日志, 与等级无关
		*/
		LoggerStream any()      { return stream(-1);}

	protected:

		/**
		 * 头部信息
		 * @param c
		 * @param len
		 * @param level
		 */
		void head(char *c, int len, int level)
		{
			size_t n = 0;

			if (hasFlag(TC_Logger::HAS_MTIME))
			{
				struct timeval t;
				TC_TimeProvider::getInstance()->getNow(&t);
				//gettimeofday(&t, NULL);

				tm tt;
				localtime_r(&t.tv_sec, &tt);
				n += snprintf(c + n, len-n, "%04d-%02d-%02d %02d:%02d:%02d.%03ld|", 
							  tt.tm_year + 1900, tt.tm_mon+1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec, t.tv_usec/1000);
			}
			else if (hasFlag(TC_Logger::HAS_TIME))
			{
				time_t t = TC_TimeProvider::getInstance()->getNow();
				tm tt;
				localtime_r(&t, &tt);
				n += snprintf(c + n, len-n, "%04d-%02d-%02d %02d:%02d:%02d|", 
							  tt.tm_year + 1900, tt.tm_mon+1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec);
			}

			if (hasFlag(TC_Logger::HAS_PID))
			{
				n += snprintf(c + n, len - n, "%ld%s", syscall(SYS_gettid), "|");
			}

			if (hasFlag(TC_Logger::HAS_LEVEL))
			{
				n += snprintf(c + n, len - n, "%s%s", TC_Logger::LN[level].c_str(), "|");
			}
		}

		/**
		 * 流
		 * @param level
		 * 
		 * @return LoggerStream
		 */
		LoggerStream stream(int level)
		{
			ostream *ost = NULL;

			if (level <= _level)
			{
				char c[128] = "\0";
				head(c, sizeof(c) - 1, level);

				_mutex.lock(); 
				ost = &_stream;
				_stream.clear();
				_stream << c;
			}

			return LoggerStream(ost, &_estream, _mutex);
		}

		/**
		* 进程等级是否有效
		* @param level : 进程等级
		* @return bool,等级是否有效
		*/
		bool isLogLevelValid(int level)
		{
			switch (level)
			{
			case NONE_LOG:
			case ERROR_LOG:
			case WARN_LOG:
			case DEBUG_LOG:
			case INFO_LOG:
				return true;
				break;
			}

			return false;
		}

	protected:
		/**
		 * 显示头
		 */
		int             _flag;

		/**
		* 日志最高等级
		*/
		int             _level;

		/**
		 * buffer
		 */
		LoggerBuffer    _buffer;

		/**
		 * logger临时流
		 */
		std::ostream    _stream;

		/**
		 * 空buffer
		 */
		LoggerBuffer   _ebuffer;

		/**
		 * 空流
		 */
		std::ostream   _estream;

		/**
		 * 锁
		 */
		TC_ThreadMutex  _mutex;
	};

	template<typename WriteT, template<class> class RollPolicy>
	const string TC_Logger<WriteT, RollPolicy>::LN[6] = {"", "", "ERROR", "WARN", "DEBUG", "INFO"};

////////////////////////////////////////////////////////////////////////////////

	class RollWrapperInterface
	{
	public:

		virtual ~RollWrapperInterface() {}

		/**
		* DEBUG记日志
		*/
		virtual LoggerStream info() = 0;

		/**
		* DEBUG记日志
		*/
		virtual LoggerStream debug() = 0;

		/**
		* WARNING记日志
		*/
		virtual LoggerStream warn() = 0; 

		/**
		* ERROR记日志
		*/
		virtual LoggerStream error() = 0;

		/**
		* 记所有日志, 与等级无关
		*/
		virtual LoggerStream any() = 0;

		/**
		 * 如果是异步调用，则马上进行刷新
		 */
		virtual void flush() = 0;
	};


	template<typename RollPolicyWriteT>
	class RollWrapperBase : public RollWrapperInterface
	{
	public:

		typedef TC_AutoPtr<RollPolicyWriteT>     RollPolicyWriteTPtr;

		/**
		 * 构造
		 */
		RollWrapperBase()
		{
			_roll = new RollPolicyWriteT;
		}

		/**
		 * 安装线程
		 * @param ltg
		 */
		void setupThread(TC_LoggerThreadGroup *ltg) { _roll->setupThread(ltg);}

		/**
		 * 取消线程
		 */
		void unSetupThread()                        { _roll->unSetupThread();}

		/**
		 * 获取写对象
		 *
		 * @return WriteT&
		 */
		typename RollPolicyWriteT::T &getWriteT()   { return _roll->getWriteT();}

		/**
		 * 写日志
		 * @param buffer
		 */
		void roll(const pair<int, string> &buffer)  { _roll->write(buffer);}

		/**
		* 获取roll实例
		 * 
		 * @return RollPolicyWriteTPtr&
		 */
		RollPolicyWriteTPtr & getRoll()          	{return _roll;}

		/**
		 * 异步刷新
		 */
		void flush()								{ _roll->flush(); }

	protected:

		/**
		 * 具体写日志操作类
		 */
		RollPolicyWriteTPtr      _roll;

	};

////////////////////////////////////////////////////////////////////////////////
/**
 * 日志滚动方法, 根据日志大小滚动
 */
	template<typename WriteT>
	class TC_RollBySize : public TC_LoggerRoll, public TC_ThreadMutex
	{
	public:
		typedef WriteT T;

		/**
		 * 封装类(提供接口)
		 */
		class RollWrapperI : public RollWrapperBase<TC_RollBySize<WriteT> >
		{
		public:
			/**
			 * 初始化
			 * @param path
			 * @param iMaxSize, 字节
			 * @param iMaxNum
			 */
			void init(const string &path, int iMaxSize = 5000000, int iMaxNum = 10)
			{
				this->_roll->init(path, iMaxSize, iMaxNum);
			}

			/**
			 * 日志路径
			 *
			 * @return string
			 */
			string getPath()                    { return this->_roll->getPath();}

			/**
			 * 设置文件路径
			 */
			void setPath(const string &path)    { this->_roll->setPath(path);}

			/**
			 * 获取最大大小
			 *
			 * @return int
			 */
			int getMaxSize()                    { return this->_roll->getMaxSize();}

			/**
			 * 设置最大大小
			 * @param maxSize
			 */
			void setMaxSize(int maxSize)        { this->_roll->setMaxSize(maxSize);}

			/**
			 * 最大个数
			 *
			 * @return int
			 */
			int getMaxNum()                     { return this->_roll->getMaxNum();}

			/**
			 * 设置文件个数
			 * @param maxNum
			 */
			void setMaxNum(int maxNum)          { this->_roll->setMaxNum(maxNum);}
		};

		/**
		 * 构造函数
		 */
		TC_RollBySize() : _iUpdateCount(0), _lt(time(NULL))
		{
		}

		/**
		 * 析够
		 */
		~TC_RollBySize()
		{
			if (_of.is_open())
			{
				_of.close();
			}
		}

		/**
		 * 初始化
		 * @param path, 文件名
		 * @param iMaxSize, 最大大小
		 * @param iMaxNum, 最大个数
		 */
		void init(const string &path, int iMaxSize = 5000000, int iMaxNum = 10)
		{
			TC_LockT<TC_ThreadMutex> lock(*this);

			_path   = path;
			_maxSize= iMaxSize;
			_maxNum = iMaxNum;
		}

		/**
		 * 日志路径
		 *
		 * @return string
		 */
		string getPath()                    { TC_LockT<TC_ThreadMutex> lock(*this); return _path;}

		/**
		 * 设置路径
		 */
		void setPath(const string &path)    { TC_LockT<TC_ThreadMutex> lock(*this); _path = path;}

		/**
		 * 获取最大大小
		 *
		 * @return int
		 */
		int getMaxSize()                    { TC_LockT<TC_ThreadMutex> lock(*this); return _maxSize;}

		/**
		 * 设置最大大小
		 * @param maxSize
		 *
		 * @return void
		 */
		void setMaxSize(int maxSize)        { TC_LockT<TC_ThreadMutex> lock(*this); return _maxSize = maxSize;}

		/**
		 * 最大个数
		 *
		 * @return int
		 */
		int getMaxNum()                     { TC_LockT<TC_ThreadMutex> lock(*this); return _maxNum;}

		/**
		 * 最大个数
		 * @param maxNum
		 */
		void setMaxNum(int maxNum)          { TC_LockT<TC_ThreadMutex> lock(*this); return _maxNum = maxNum;}

		/**
		 * 获取写示例
		 *
		 * @return T&
		 */
		WriteT &getWriteT()                 { return _t;}

		/**
		 * 函数对象
		 *
		 * @param string
		 */
		void roll(const deque<pair<int, string> > &buffer)
		{
			TC_LockT<TC_ThreadMutex> lock(*this);

			if (_path.empty())
			{
				_t(cout, buffer);
				return;
			}

			time_t t = TC_TimeProvider::getInstance()->getNow();
			time_t tt= t - _lt;
			//每隔5, 重新打开一次文件, 避免文件被删除后句柄不释放
			if (tt > 5 || tt < 0)
			{
				_lt = t;
				_of.close();
			}

			//检查日志文件是否打开
			if (!_of.is_open())
			{
				string sLogFileName = _path + ".log";
				_of.open(sLogFileName.c_str(), ios::app);

				string sLogFilePath = TC_File::extractFilePath(_path);

				if (!TC_File::isFileExist(sLogFilePath,S_IFDIR))
				{
					TC_File::makeDirRecursive(sLogFilePath);
				}

				if (!_of)
				{
					//抛异常前继续进入_t 以便打远程日志
					_t(_of, buffer);

					throw TC_Logger_Exception("[TC_RollBySize::roll]:fopen fail: " + sLogFileName, errno);
				}

			}

			_t(_of, buffer);

			if (tt <= 5)
			{
				return;
			}

			//文件大小小于限制, 直接返回
			if (_of.tellp() < _maxSize)
			{
				return;
			}

			//文件大小超出限制,删除最后一个文件
			string sLogFileName = _path + TC_Common::tostr(_maxNum - 1) + ".log";
			if (access(sLogFileName.c_str(), F_OK) == 0)
			{
				if (remove(sLogFileName.c_str()) < 0 )
				{
					return;
				}
			}

			//将log文件命名shift, xxx1.log=>xxx2.log,第一文件还是为xxx.log
			for (int i = _maxNum-2; i >= 0; i--)
			{
				if (i == 0)
				{
					sLogFileName = _path + ".log";
				}
				else
				{
					sLogFileName = _path + TC_Common::tostr(i) + ".log";
				}

				if (access(sLogFileName.c_str(), F_OK) == 0)
				{
					string sNewLogFileName = _path + TC_Common::tostr(i + 1) + ".log";
					rename(sLogFileName.c_str(), sNewLogFileName.c_str());
				}
			}

			_of.close();
			_of.open(sLogFileName.c_str(), ios::app);
			if (!_of)
			{
				throw TC_Logger_Exception("[TC_RollBySize::roll]:fopen fail: " + sLogFileName, errno);
			}
		}

	protected:

		/**
		 * 文件路径
		 */
		string      _path;

		/**
		* 日志文件的最大大小
		*/
		int         _maxSize;

		/**
		* log文件最大数log文件命名为:xxx.log xxx1.log ...
		* xxx[_maxNum-1].log,_maxNum<=1 ?xxx.log
		*/
		int         _maxNum;

		/**
		 * 日志文件
		 */
		ofstream    _of;

		/**
		 * 具体写操作
		 */
		WriteT      _t;

		/**
		 * 多长时间检查一次文件大小
		 */
		short       _iUpdateCount;

		/**
		 * 多长时间检查一次文件大小
		 */
		time_t      _lt;
	};

	typedef TC_Logger<TC_DefaultWriteT, TC_RollBySize> TC_RollLogger;

/**
 * 根据时间滚动日志
 */
	template<typename WriteT>
	class TC_RollByTime : public TC_LoggerRoll, public TC_ThreadMutex
	{
	public:

		typedef WriteT T;

		/**
		 * 封装类(接口类)
		 */
		class RollWrapperI : public RollWrapperBase<TC_RollByTime<WriteT> >
		{
		public:

			/**
			 * 初始化
			 * @param path
			 * @param iMaxSize
			 * @param iMaxNum
			 */
			void init(const string &path, const string &format = "%Y%m%d")
			{
				this->_roll->init(path, format);
			}

			/**
			 * 日志路径
			 *
			 * @return string
			 */
			string getPath()                        { return this->_roll->getPath();}

			/**
			 * 设置文件路径
			 */
			void setPath(const string &path)        { this->_roll->setPath(path);}

			/**
			 * 获取格式
			 *
			 * @return string
			 */
			string getFormat()                      { return this->_roll->getFormat();}

			/**
			 * 设置格式
			 * @param format
			 */
			void setFormat(const string &format)    { this->_roll->setFormat(format);}
		};

		/**
		 * 构造
		 */
		TC_RollByTime() : _lt(TC_TimeProvider::getInstance()->getNow())
		{
		}

		/**
		 * 析够
		 */
		~TC_RollByTime()
		{
			if (_of.is_open())
			{
				_of.close();
			}
		}

		/**
		 * 初始化
		 * @param path, 文件路径
		 * @param format, 格式
		 */
		void init(const string &path, const string &format = "%Y%m%d")
		{
			TC_LockT<TC_ThreadMutex> lock(*this);

			_path       = path;
			_format     = format;
			_currentTime= TC_Common::tm2str(TC_TimeProvider::getInstance()->getNow(), _format);
		}

		/**
		 * 日志路径
		 *
		 * @return string
		 */
		string getPath()                    { TC_LockT<TC_ThreadMutex> lock(*this); return _path;}

		/**
		 * 设置文件路径
		 */
		void setPath(const string &path)    { TC_LockT<TC_ThreadMutex> lock(*this); _path = path;}

		/**
		 * 获取格式
		 *
		 * @return string
		 */
		string getFormat()                  { TC_LockT<TC_ThreadMutex> lock(*this); return _format;}

		/**
		 * 设置格式
		 * @param format
		 */
		void setFormat(const string &format){ TC_LockT<TC_ThreadMutex> lock(*this); _format = format;}

		/**
		 * 获取写示例
		 *
		 * @return WriteT&
		 */
		WriteT &getWriteT()                 { return _t;}

		/**
		 * 函数对象
		 * @param path
		 * @param of
		 *
		 * @return string
		 */
		void roll(const deque<pair<int, string> > &buffer)
		{
			TC_LockT<TC_ThreadMutex> lock(*this);

			if (_path.empty())
			{
				_t(cout, buffer);
				return;
			}

			time_t t = TC_TimeProvider::getInstance()->getNow();

			string nowTime = TC_Common::tm2str(t, _format);

			//检查时间是否跨时间了, 跨时间后关闭文件
			if (_currentTime != nowTime)
			{
				_currentTime = nowTime;
				_of.close();
			}

			//每隔10s, 重新打开一次文件, 避免文件被删除后句柄不释放
			if (t - _lt > 10 || t - _lt < 0)
			{
				_lt = t;
				_of.close();
			}

			if (!_of.is_open())
			{
				string sLogFileName = _path + "_" + nowTime + ".log";

				string sLogFilePath = TC_File::extractFilePath(_path);
				if (!TC_File::isFileExist(sLogFilePath,S_IFDIR))
				{
					TC_File::makeDirRecursive(sLogFilePath);
				}

				_of.open(sLogFileName.c_str(), ios::app);

				if (!_of)
				{
					//抛异常前继续进入_t 以便打远程日志
					_t(_of, buffer);
					throw TC_Logger_Exception("[TC_RollByTime::roll]:fopen fail: " + sLogFileName, errno);
				}
			}

			//写
			_t(_of, buffer);
		}

	protected:
		/**
		 * 文件路径
		 */
		string      _path;

		/**
		 * 时间格式
		 */
		string      _format;

		/**
		 * 上次roll时的时间
		 */
		string      _currentTime;

		/**
		 * 日志文件
		 */
		ofstream    _of;

		/**
		 * 具体写操作
		 */
		WriteT      _t;

		/**
		 * 多长时间检查一次文件大小
		 */
		time_t      _lt;
	};

	typedef TC_Logger<TC_DefaultWriteT, TC_RollByTime> TC_DayLogger;

}

#endif

