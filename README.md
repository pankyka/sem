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
sem1.acquire(5000, function () {});         // Waits till available or the timeout expired
sem1.release();                             // Releases the semaphore

semaphore.unlink('somename');               // Removes the named semaphore from the system
```
