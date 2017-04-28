#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>

typedef void* (*ThreadFunc) (void*);

class Thread
{
public:
    static pid_t currentThread();
    explicit
    Thread(ThreadFunc func = NULL, void *arg = NULL) : 
        start_(false), func_(func), arg_(arg)
    {

    }
    virtual ~Thread() {}

    int start();
    void join();

protected:
    virtual void run() {
    }

protected:
    bool start_;
    ThreadFunc func_;
    void *arg_;
    pthread_t thread_;
private:
    static void* _thread_func_(void *arg);
};

#endif
