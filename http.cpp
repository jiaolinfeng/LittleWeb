#include "http.h"
#include <stdio.h>
#include <string.h>

Http::Http() : checked_idx_(0), read_idx_(0), start_line_(0), line_ptr_(NULL), check_state_(REQUEST_LINE)
{
    memset(req_, 0, sizeof(req_));
}

LineStatus Http::parseLine()
{
    for (; checked_idx_ <= read_idx_; checked_idx_ ++)
    {
        int tmp = checked_idx_;
        if (req_[tmp] == '\r')
        {
            if (tmp + 1 > read_idx_)
                return LINE_OPEN;
            else if (req_[tmp + 1] == '\n')
            {
                req_[tmp] = '\0';
                req_[tmp + 1] = '\0';
                return LINE_OK;
            }
            else
                return LINE_BAD;
        }
        else if (req_[tmp] == '\n')
        {
            if (tmp > 0 && req_[tmp - 1] == '\r')
            {
                req_[tmp] = '\0';
                req_[tmp - 1] = '\0';
                return LINE_OK;
            }
            else
                return LINE_BAD;
        }
    }  
    return LINE_OPEN;
}

HttpCode Http::parseReqLine()
{
    char *url = strpbrk(line_ptr_, " \t");
    if (!url)
    {
        return BAD_REQUEST;
    }
    *url++ = '\0';
    char *method = line_ptr_;
    if (strcasecmp(method, "GET") && strcasecmp(method, "POST"))
    {
        return BAD_REQUEST; 
    }
    url += strspn(url, " \t");
    char *version = strpbrk(url, " \t");
    if (!version)
    {
        return BAD_REQUEST;
    }
    *version++ = '\0';
    version += strspn(version, " \t");
    if (strcasecmp(version, "HTTP/1.1"))
    {
        return BAD_REQUEST;
    }
    if (!strncasecmp(url, "http://", 7))
    {
        url += 7;
        url = strchr(url, '/');
    }
    if (!url || url[0] != '/')
    {
        return BAD_REQUEST;
    }
    printf("%s\n", url);
    check_state_ = HEADER;
    return NO_REQUEST;
}

HttpCode Http::parseHeader()
{
    if (line_ptr_[0] == '\0')
    {
        return GET_REQUEST;
    }
    char* key = line_ptr_;
    char* value = strpbrk(line_ptr_, " \t");
    if (!value)
    {
        return BAD_REQUEST;
    }
    *value++ = '\0';
    value += strspn(value, " \t");
    headers[key] = value;
    //headers.insert(std::pair<string, string>(key, value));
    return NO_REQUEST;
}

HttpCode Http::parseRequest()
{
    LineStatus line_status;
    while ((line_status = parseLine()) == LINE_OK)
    {
        line_ptr_ = req_ + start_line_;
        start_line_ = checked_idx_;
        HttpCode httpRet;
        switch (check_state_)
        {
            case REQUEST_LINE:
                httpRet = parseReqLine();
                if (httpRet == BAD_REQUEST)
                    return httpRet;
                break;
            case HEADER:
                httpRet = parseHeader();
                if (httpRet == BAD_REQUEST || httpRet == GET_REQUEST)
                    return httpRet;
                break;
            default:
                return INTERNAL_ERROR;
        }
    }
    if (line_status == LINE_OPEN)
        return NO_REQUEST;
    else
        return BAD_REQUEST;
}


int Http::printRequest()
{
//    char tmp;
    for (unsigned i = 0; i < strlen(req_); i++)
    {
        if (req_[i] == '\r') 
        {
            printf("\\r");
        }
        else if (req_[i] == '\n')
        {
            printf("\\n\n");
        }
        else
            printf("%c", req_[i]);
    }
    return 0;
}
