#include "servant/FDReactor.h"
#include "servant/TransceiverHandle.h"
#include "log/taf_logger.h"

using namespace std;

namespace taf
{

FDReactor::FDReactor()
: _terminate(false)
{
    _ep.create(1024);

    _shutdown.createSocket();

    _ep.add(_shutdown.getfd(), _shutdown.getfd(), EPOLLIN);
}

FDReactor::~FDReactor()
{
}

void FDReactor::terminate()
{
    _terminate = true;
    //通知epoll响应
    _ep.mod(_shutdown.getfd(), _shutdown.getfd(), EPOLLOUT);
}

void FDReactor::notify(int fd)
{
    //通知epoll响应
    _ep.mod(fd, fd, EPOLLOUT|EPOLLIN);
}

void FDReactor::handle(int fd, int events)
{
    try
    {
        TransceiverHandle * hd;

        {
            TC_ThreadLock::Lock lock(*this);

            map<int, TransceiverHandle*>::iterator it = _handles.find(fd);

            if (it == _handles.end())
            {
                _ep.del(fd, fd, 0);
                return;
            }

            hd = it->second;
        }

        if (events & (EPOLLERR | EPOLLHUP))
        {
            hd->handleExcept(fd);
        }
        else if (events != 0)
        {
            hd->handle(fd, events);
        }
    }
    catch (exception& e)
    {
        LOG->error() << "[TAF][FDReactor:handle exception:" << e.what() << "]" << endl;
    }
    catch (...)
    {
        LOG->error() << "[TAF][FDReactor:handle exception.]" << endl;
    }
}

void FDReactor::run()
{
    while (!_terminate)
    {
        try
        {
            int num = _ep.wait(20000);

            if(_terminate) break;

            //先处理epoll的网络事件
            for (int i = 0; i < num; ++i)
            {
                epoll_event ev = _ep.get(i);

                int fd = ev.data.u64;
                
                if(fd == _shutdown.getfd()) continue;

                int event = (ev.events & EPOLLIN) ? TransceiverHandle::R : 0;

                event = event | ((ev.events & EPOLLOUT) ? TransceiverHandle::W : 0);
                
                handle(fd, event);
            }
            
        }
        catch (exception& e)
        {
            LOG->error() << "[TAF][FDReactor:run exception:" << e.what() << "]" << endl;
        }
        catch (...)
        {
            LOG->error() << "[TAF][FDReactor:run exception.]" << endl;
        }
    }
}

void FDReactor::registerHandle(int fd, uint32_t events, TransceiverHandle* handle)
{
    TC_ThreadLock::Lock lock(*this);

    handle->setFDReactor(this);
    
    _handles[fd] = handle;

    _ep.del(fd, fd, 0);

    _ep.add(fd, fd, events);
}

void FDReactor::unregisterHandle(int fd, uint32_t event, TransceiverHandle* handle)
{
    TC_ThreadLock::Lock lock(*this);

    _ep.del(fd, fd, 0);

    _handles.erase(_handles.find(fd));
}
//////////////////////////////////////////////////////////////////////////////////
}
