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

#define SEM_NAME(name) "/" + name

class SemaphoreFactory : public Nan::ObjectWrap
{
  public:
    static NAN_MODULE_INIT(Init);
    static NAN_METHOD(CreateInstance);
    static NAN_METHOD(Unlink);

  private:
    explicit SemaphoreFactory(std::string semaphore_name) : semaphore_name(semaphore_name) {};
    ~SemaphoreFactory() {};
    std::string semaphore_name;
    sem_t *mutex;
    static NAN_METHOD(Instance);
    static NAN_METHOD(Acquire);
    static NAN_METHOD(Release);    
    static inline Nan::Persistent<v8::Function> &constructor();
};
