const semaphore = require("../build/Release/sem");

semaphore.unlink("somename");
const sem1 = semaphore.create("somename");
const sem2 = semaphore.create("somename");
const sem3 = semaphore.create("somename");

console.log("sem1 try acquire");
sem1.acquire(function (err) {

    if (err) {
        console.error("sem1 error", err);
        return;
    }
    
    console.log("sem1 acquired");

    setTimeout(function () {

        sem1.release();
        console.log("sem1 released");
    }, 2000);
});

console.log("sem2 try acquire");
sem2.acquire(function (err) {

    if (err) {
        console.error("sem2 error", err);
        return;
    }

    console.log("sem2 acquired");

    sem2.release();
    console.log("sem2 released");
});

console.log("sem3 try acquire");
sem3.acquire(100, function (err) {

    if (err) {
        console.error("sem3 error", err);
        return;
    }

    console.log("sem3 acquired");

    sem3.release();
    console.log("sem3 released");
});
