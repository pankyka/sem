# sem
Native, interprocess semaphore module for Node.js

* [x] OSX
* [x] POSIX
* [ ] Win32

## install

## usage

```javascript
const semaphore = require('sem');

const sem1 = semaphore.create('somename');  // Creates a named semaphore

sem1.acquire(function () {});               // Waits till the semaphore is available
sem1.acquire(5000, function () {});         // Waits till available or the timeout (in ms) expired
sem1.release();                             // Releases the semaphore

semaphore.unlink('somename');               // Removes the named semaphore from the system
```
Be careful with `unlink()`. The semaphore removed immediately, all other processes that have the semaphore open close it.

## common error codes

```
EACCES          The caller does not have permission to unlink this semaphore.
EACCES          The semaphore exists, but the caller does not have permission to open it.
ENAMETOOLONG    Name was too long.
ETIMEDOUT       Specified timeout (in ms) expired.
```
For more error code see linux man pages.
