#ifndef _SERVER_H_
#define _SERVER_H_
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
};

#endif
