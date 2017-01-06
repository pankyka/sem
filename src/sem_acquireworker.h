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

class AcquireWorker : public Nan::AsyncWorker
{
  public:
    AcquireWorker(Nan::Callback *callback, sem_t *mutex, long timeout_millisec);
    void Execute();
    void WorkComplete();
    void HandleErrorCallback();

  private:
    sem_t *mutex;
    int err_no = 0;
    long timeout_millisec;
};
