#include <stdio.h>
#include "server.h"

int main()
{
    Server server;
    server.start();
    char ch = getchar();
    server.stop();
    return 0;
}
