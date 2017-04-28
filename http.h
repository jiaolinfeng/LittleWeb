#ifndef _HTTP_H_
#define _HTTP_H_

#include <string>

class Http
{
public:
    int parseRequest(std::string req);
    int processRequest();
    int response();
private:
    
};

#endif
