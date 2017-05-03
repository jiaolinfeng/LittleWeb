#include <stdio.h>
#include "server.h"

int main()
{
    Server server;
    server.start();
    server.work();
    server.stop();
    return 0;
}
