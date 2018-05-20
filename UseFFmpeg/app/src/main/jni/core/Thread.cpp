//
// Created by weiersyuan on 2017/5/10.
//

#include <sys/prctl.h>
#include "core/Thread.h"
Thread::Thread()
        : mThread((pthread_t) -1), mLock("TestAtlas:mLock"), mExitPending(false),
          running_(false) {
}

Thread::~Thread() {

}


int Thread::readyToRun() {
    return 0;
}

int Thread::finishRun() {
    return 0;
}

int Thread::run(const char *name, int32_t priority, size_t stack) {
    Mutex::Autolock _l(mLock);

    if (running_) {
        return -1;
    }

    mExitPending = false;
    mThread = (pthread_t) -1;
    running_ = true;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    //线程分离状态
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
#if DEBUG
    int policy;
    pthread_attr_getschedpolicy(&attr,&policy);
    LOGD("thread sched policy : %d",policy);
#endif

    if (name) {
        mThreadName = mThreadName.assign(name);
    }

    if (stack) {
        pthread_attr_setstacksize(&attr, stack);
    }

    sched_param sched;
    pthread_attr_getschedparam(&attr, &sched);
#if DEBUG
    LOGD("thread inited sched_priority : %d",sched.sched_priority);
#endif
    sched.sched_priority = priority;
    pthread_attr_setschedparam(&attr, &sched);

    int result = pthread_create(&mThread, &attr, &Thread::_threadLoop, (void *) this);

    pthread_attr_destroy(&attr);
    if (result != 0) {
        running_ = false;
        mThread = (pthread_t) -1;
        return -2;
    }
    return 0;
}

void *Thread::_threadLoop(void *user) {
    Thread *const self = static_cast<Thread *>(user);
    prctl(PR_SET_NAME, self->mThreadName.c_str());
    bool first = true;

    do {

        bool result = false;
        if (first) {
            first = false;
            self->readyToRun();

            if (!self->exitPending()) {
                result = self->threadLoop();
            }
        } else {
            result = self->threadLoop();
        }

        {
            Mutex::Autolock _l(self->mLock);
            if (!result || self->mExitPending) {
                self->running_ = false;
                self->mThread = (pthread_t) -1;
                break;
            }
        }

    } while (self->running_);
    pthread_detach(pthread_self());
    self->finishRun();
    return nullptr;
}

void Thread::requestExit() {
    Mutex::Autolock _l(mLock);
    mExitPending = true;
}

bool Thread::exitPending() const {
    Mutex::Autolock _l(mLock);
    return mExitPending;
}