#ifndef _HTTP_H_
#define _HTTP_H_

#include <map>
#include <string>

enum CheckState 
{ 
    REQUEST_LINE = 0, 
    HEADER 
};
enum LineStatus 
{ 
    LINE_OK = 0, 
    LINE_BAD, 
    LINE_OPEN 
};

enum HttpCode 
{
    NO_REQUEST = 0,
    GET_REQUEST = 100, 
    BAD_REQUEST = 400,
    FORBIDDEN_REQUEST = 403,
    INTERNAL_ERROR = 500
};

class Http
{
public:
    Http();

    int processRequest();
    int response();
     
    int getLen() { return REQ_MAX_LEN - read_idx_; };
    char* getPtr() { return req_ + read_idx_; }
    void incPos(int nread) { read_idx_ += nread;}

    HttpCode parseRequest();

    int printRequest(); //debug

private:

    enum { REQ_MAX_LEN = 20000 };

    int checked_idx_;
    int read_idx_;
    int start_line_;
    char *line_ptr_;
    CheckState check_state_;
    char req_[REQ_MAX_LEN+1];
    
    std::map< std::string, std::string > headers;

    LineStatus parseLine();

    HttpCode parseReqLine();

    HttpCode parseHeader();


};

#endif
