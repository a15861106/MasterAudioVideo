//
// Created by weiersyuan on 2017/5/8.
//

#ifndef TESTATLAS_THREAD_H
#define TESTATLAS_THREAD_H

#include <cstdint>
#include <string>
#include "Mutex.h"

using std::string;

class Thread {
public:
    Thread();

    virtual ~Thread();

    virtual int run(const char *name = 0, int32_t priority = 0, size_t stack = 0);

    virtual void requestExit();

protected:
    bool exitPending() const;

    virtual int readyToRun();

    virtual int finishRun();

private:
    virtual bool threadLoop() = 0;

    Thread &operator=(const Thread &);

    static void *_threadLoop(void *user);

protected:
    mutable Mutex mLock; //线程锁

private:
    pthread_t mThread;
    string mThreadName;
    volatile bool mExitPending; //是否退出线程循环
    volatile bool running_;
};

#endif //TESTATLAS_THREAD_H
