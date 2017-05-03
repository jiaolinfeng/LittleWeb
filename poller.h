#ifndef _POLLER_H_
#define _POLLER_H_

#include <fcntl.h>
#include "common.h"


class IPoller;

class PollHandler
{
public:
    virtual void handleInput(IPoller *poller, fd_t fd)
    {
        
    }
    
    virtual void handleOutput(IPoller *poller, fd_t fd)
    {
        
    }

    virtual void handleError(IPoller *poller, fd_t fd) 
    {

    }

    virtual ~PollHandler()
    {

    }
};

class IPoller
{
public:
    enum POLL_TYPE
    {
        POLL_READ  = 0x1,
        POLL_WRITE = 0x2
    };

    IPoller() : handler_(0)
    {

    }

    virtual ~IPoller()
    {

    }

    virtual int poll(int timeout = -1) = 0;

    virtual int addFd(fd_t fd, int flag) = 0;

    virtual int rmFd(fd_t fd) = 0;

    virtual int setEvents(fd_t fd, int flag) = 0;

    void setHandler(PollHandler *handler)
    {
        handler_ = handler;
    }

    int setnonblocking(int fd);
protected:
    PollHandler *handler_;
};

class EPoller : public IPoller
{
public:
    enum EPOLL_TYPE
    {
        EPOLL_ET = 0x4,
        EPOLL_ONESHOT = 0x8
    };

    EPoller();

    virtual int addFd(fd_t fd, int flag);

    virtual int rmFd(fd_t fd);

    virtual int setEvents(fd_t fd, int flag);

    virtual int poll(int timeout = -1);

    virtual ~EPoller();
private:
    fd_t epoll_fd_;
};

#endif
