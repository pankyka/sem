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

#include <nan.h>
#include <semaphore.h>

#include "sem.h"
#include "sem_acquireworker.h"

using std::string;
using v8::Local;
using v8::Value;
using v8::String;
using v8::Function;
using v8::FunctionTemplate;
using Nan::New;
using Nan::Set;
using Nan::To;
using Nan::ObjectWrap;
using Nan::ThrowError;
using Nan::ErrnoException;
using Nan::Callback;
using Nan::Persistent;
using Nan::NewInstance;
using Nan::GetFunction;
using Nan::SetPrototypeMethod;

NAN_MODULE_INIT(SemaphoreFactory::Init)
{
    Local<FunctionTemplate> tpl = New<FunctionTemplate>(SemaphoreFactory::Instance);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    SetPrototypeMethod(tpl, "acquire", SemaphoreFactory::Acquire);
    SetPrototypeMethod(tpl, "release", SemaphoreFactory::Release);

    constructor().Reset(GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(SemaphoreFactory::CreateInstance)
{
    Local<Function> ctor = New<Function>(SemaphoreFactory::constructor());

    String::Utf8Value utf8(info[0]->ToString());
    string name(*utf8);
    name = SEM_NAME(name);

    const int argc = 1;

    Local<Value> argv[1] = {New(name).ToLocalChecked()};
    info.GetReturnValue().Set(NewInstance(ctor, argc, argv).ToLocalChecked());
}

NAN_METHOD(SemaphoreFactory::Instance)
{
    String::Utf8Value utf8(info[0]->ToString());
    string name(*utf8);

    if (info.IsConstructCall())
    {
        SemaphoreFactory *obj = new SemaphoreFactory(name);
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    }
    else
    {
        const int argc = 1;

        Local<Value> argv[argc] = {New(name).ToLocalChecked()};
        Local<Function> ctor = New(SemaphoreFactory::constructor());
        info.GetReturnValue().Set(NewInstance(ctor, argc, argv).ToLocalChecked());
    }
}

inline Persistent<Function> &SemaphoreFactory::constructor()
{
    static Persistent<Function> ctor;
    return ctor;
}

NAN_METHOD(SemaphoreFactory::Acquire)
{
    long timeout_millisec = -1;

    if (info.Length() == 0 && !info[info.Length() - 1]->IsFunction())
    {
        ThrowError("Callback function missing.");
    }

    if (info.Length() > 1)
    {
        timeout_millisec = (long)info[0]->NumberValue();
    }

    Callback *callback = new Callback(info[info.Length() - 1].As<Function>());

    SemaphoreFactory *obj = ObjectWrap::Unwrap<SemaphoreFactory>(info.Holder());

#ifdef _WIN32
    //todo win
#else
    obj->mutex = sem_open(obj->semaphore_name.c_str(), O_CREAT, S_IRUSR | S_IWUSR, 1);
#endif

    if (obj->mutex == SEM_FAILED)
    {
        ThrowError(ErrnoException(
            errno,
            "sem_open",
            strerror(errno)));
    }

    auto acquire_worker = new AcquireWorker(callback, obj->mutex, timeout_millisec);
    AsyncQueueWorker(acquire_worker);
}

NAN_METHOD(SemaphoreFactory::Release)
{
    SemaphoreFactory *obj = ObjectWrap::Unwrap<SemaphoreFactory>(info.Holder());

#ifdef _WIN32

#else
    int result = sem_post(obj->mutex);
#endif

    if (result < 0)
    {
        ThrowError(ErrnoException(
            errno,
            "sem_post",
            strerror(errno)));
        return;
    }
}

NAN_METHOD(SemaphoreFactory::Unlink)
{
    if (info[0]->IsString())
    {
        String::Utf8Value utf8(info[0]->ToString());
        string name(*utf8);

#ifdef _WIN32
        //todo win
#else
        sem_unlink(name.c_str());
#endif
    }
}

NAN_MODULE_INIT(Init)
{
    SemaphoreFactory::Init(target);
    Set(target,
        New<String>("create").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(SemaphoreFactory::CreateInstance)).ToLocalChecked());
    Set(target,
        New<String>("unlink").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(SemaphoreFactory::Unlink)).ToLocalChecked());
}

NODE_MODULE(sem, Init)
