#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string>
#include <memory.h>
#include <boost/shared_ptr.hpp>
#include "server.h"

Server::Server() : server_(0), start_(false)
{

}

Server::~Server()
{
    if (start_)
        stop();
}

int Server::start()
{
    int ret;
    try {
        server_ = socket(AF_INET, SOCK_STREAM, 0);
        if (server_ == -1)
            throw std::string("Server::start() : create socket failed");
        int reuse = 1;
        setsockopt(server_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(80);
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        ret = bind(server_, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (ret == -1)
            throw std::string("Server::start() : bind failed");
        ret = listen(server_, CLIENT_NUM_MAX);
        if (ret == -1)
            throw std::string("Server::start() : listen failed");
        ret = 0;
        start_ = true;
    } catch(std::string)
    {
        ret = -1;
        if (server_ > 0)
            close(server_);
    }
    return ret;
}

void Server::stop()
{
    if (server_ > 0)
        close(server_);
    start_ = false;
}

void Server::work()
{
    boost::shared_ptr<IPoller> poller(new EPoller);
    poller->setHandler(this);
    poller->addFd(server_, IPoller::POLL_READ | EPoller::EPOLL_ET);
    while (start_)
    {
        poller->poll();
    }
}

void Server::handleInput(IPoller *poller, fd_t fd)
{
    if (fd == server_)
    {
        int acceptor = acceptor
    }
}
