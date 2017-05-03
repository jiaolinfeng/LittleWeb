#ifndef _SERVER_H_
#define _SERVER_H_

#include <list>
#include "poller.h"

class Server : public PollHandler
{
public:
    Server();

    ~Server();

    int start();

    void stop();

    void work();

    void handleInput(IPoller *poller, fd_t fd);

private:
    fd_t server_;

    bool start_;

    typedef std::list<fd_t> ClientContainer;
    ClientContainer clients_;
};

#endif
