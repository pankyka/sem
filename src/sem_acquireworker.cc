/*
Copyright 2017 Gábor Pankotay (mrpanky@gmail.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <errno.h>
#include <time.h>
#include <semaphore.h>
#include <iostream>

#ifdef __APPLE__
#include "sem_timedwait_osx.h"
#endif

#include "sem_acquireworker.h"

using v8::Value;
using v8::String;
using v8::Local;
using v8::Object;
using Nan::Null;
using Nan::New;
using Nan::To;
using Nan::Callback;
using Nan::HandleScope;
using Nan::AsyncWorker;
using Nan::Persistent;
using Nan::Utf8String;
using Nan::ThrowError;
using Nan::ObjectWrap;
using Nan::ErrnoException;

AcquireWorker::AcquireWorker(Callback *callback, sem_t *mutex, long timeout_millisec)
    : AsyncWorker(callback), mutex(mutex), timeout_millisec(timeout_millisec) {}

void AcquireWorker::Execute()
{
    if (timeout_millisec > 0)
    {
#ifdef _WIN32
        //todo win
#elif __APPLE__
        sem_timedwait_mach(mutex, timeout_millisec);
#else
        timespec ts;

        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec = timeout_millisec / 1000;
        ts.tv_nsec = (timeout_millisec % 1000) * 1000000;

        sem_timedwait(mutex, &ts);
#endif
    }
    else
    {
#ifdef _WIN32
        //todo win
#else
        sem_wait(mutex);
#endif
    }

    err_no = errno;
}

void AcquireWorker::WorkComplete()
{
    HandleScope scope;

    if (err_no > 0)
    {
        HandleErrorCallback();
    }
    else
    {
        HandleOKCallback();
    }
}

void AcquireWorker::HandleErrorCallback()
{
    HandleScope scope;

    Local<Value> argv[] = {ErrnoException(
        err_no,
        timeout_millisec > 0 ? "sem_timedwait" : "sem_wait",
        strerror(err_no))};

    callback->Call(1, argv);
}
