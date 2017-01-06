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

#ifdef __APPLE__
#include <cstdio>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>
#include "sem_timedwait_osx.h"

// Mac OSX timedwait impl
int sem_timedwait_mach(sem_t *mutex, long timeout_millisec)
{
    const long SLEEP_INTERVAL = 50;
    int counter = timeout_millisec;
    int retval;

    do
    {
        retval = sem_trywait(mutex);

        if (retval == 0)
        {
            break;
        }
        else
        {
            if (counter <= 0)
            {
                retval = -1;
                errno = ETIMEDOUT;
                break;
            }
            counter -= SLEEP_INTERVAL;
            usleep(SLEEP_INTERVAL * 1000); // sleep for 50 millisec
        }

    } while (errno == EAGAIN || retval < 0);

    return retval;
}

#endif
