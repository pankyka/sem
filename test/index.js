const assert = require('assert');
const async = require('async');
const semaphore = require('../build/Release/sem');
const SEM_NAME = "somethingspecial";

const job = function (jobId, timeout, cb) {

    if (!cb) {
        cb = timeout;
        timeout = -1;
    }

    const sem = semaphore.create(SEM_NAME);

    console.log(`Job [${jobId}] try acquire`);

    sem.acquire(timeout, err => {

        if (err) {
            console.error(`Job [${jobId}] has error`, err);
            // don't have to release
            cb(jobId);
        } else {

            console.log(`Job [${jobId}] acquired`);
            setTimeout(() => {

                sem.release();
                console.log(`Job [${jobId}] released`);
                cb(jobId);
            }, 2000);
        }
    });
};

const callback = function (arr, cb) {
    return function (jobId) {
        arr.push(jobId);
        cb();
    };
};

describe('Test parallel workers', () => {

    it('Workers should be synchronized', done => {

        const jobOrder = [];

        async.parallel([
            cb => job("001", callback(jobOrder, cb)),
            cb => job("002", 500, callback(jobOrder, cb)),
            cb => job("003", callback(jobOrder, cb))
        ], (err, results) => {

            if (err) {
                console.error("parallel error:", err);
            }

            semaphore.unlink(SEM_NAME);
            // second job finished first due to timeout
            assert.deepEqual(jobOrder, ["002", "001", "003"]);     
            done();
        });
    }).timeout(10000);
});

