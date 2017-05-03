#include <sys/epoll.h>
#include <string>
#include "poller.h"

int IPoller::setnonblocking(fd_t fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

EPoller::EPoller() : epoll_fd_(0)
{
    epoll_fd_ = epoll_create(1);
    if (!epoll_fd_)
        throw std::string("EPoller() epoll_create() failed");
}

int EPoller::addFd(fd_t fd, int flag)
{
    if (fd < 0) return -1;
    struct epoll_event event;
    event.data.fd = fd;
    if (flag & POLL_READ)
        event.events |= EPOLLIN;
    if (flag & POLL_WRITE)
        event.events |= EPOLLOUT;
    if (flag & EPOLL_ET)
    {
        event.events |= EPOLLET;
        setnonblocking(fd);
    }
    event.events |= (EPOLLERR | EPOLLHUP);
    return epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event);
}

int EPoller::setEvents(fd_t fd, int flag)
{
    if (fd < 0) return -1;
    struct epoll_event event;
    event.data.fd = fd;
    if (flag & POLL_READ)
        event.events |= EPOLLIN;
    if (flag & POLL_WRITE)
        event.events |= EPOLLOUT;
    if (flag & EPOLL_ET)
        event.events |= EPOLLET;
    event.events |= (EPOLLERR | EPOLLHUP);
    return epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &event);
}

int EPoller::rmFd(fd_t fd)
{
    if (fd < 0) return -1;
    return epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, NULL);
}

int EPoller::poll(int timeout)
{
    if (!handler_)
        return -1;
    epoll_event events[EPOLL_WAIT_MAX];
    int ret = epoll_wait(epoll_fd_, events, EPOLL_WAIT_MAX, timeout);
    if (ret < 0)
        return -1;
    for (int i = 0; i < ret; ++i)
    {
        fd_t fd = events[i].data.fd;
        unsigned event = events[i].events;
        if ((event & EPOLLERR) || (event & EPOLLHUP))
            handler_->handleError(this, fd);
        else if (event & EPOLLIN)
            handler_->handleInput(this, fd);
        else 
            handler_->handleOutput(this, fd);
    }
    return 1;
}

EPoller::~EPoller()
{
    if (epoll_fd_)
        close(epoll_fd_);
}
