#include <sys/syscall.h>
#include <string>
#include <stdio.h>
#include "thread.h"

pid_t Thread::currentThread()
{
    return (pid_t)syscall(SYS_gettid);
}

void* Thread::_thread_func_(void *arg)
{
    Thread *p_self = (Thread*)arg;
    if (p_self)
    {
        p_self->run();
        p_self->start_ = false;
    }
    else
    {
        throw std::string("Thread::_thread_func_ arg(this) is NULL");
    }
    pthread_exit(NULL);
}

int Thread::start()
{
    int res = 0;
    if (start_)
        return -1;
    if (func_)
        res = pthread_create(&thread_, NULL, func_, arg_);
    else
        res = pthread_create(&thread_, NULL, _thread_func_, this);
    
    if (res)
        return -1;
    start_ = true;
    return 0;
}

void Thread::join()
{
    if (start_)
        pthread_join(thread_, NULL);
}
