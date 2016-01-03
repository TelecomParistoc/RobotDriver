var ffi = require("ffi");
var voidPtr = require("ref").refType('void');

function mCallback(func) {
    return ffi.Callback('void', [ voidPtr ], func);
}

module.exports = [
    {
        title: "simple speed change test",
        inertia: false,
        duration: 3,
        run: function (motion, log) {
            motion.initMotionController();
            motion.queueSpeedChange(0.5, mCallback(function() { log("rise callback"); }) );
            motion.queueSpeedChange(0, mCallback(function() { log("fall callback"); }) );
        }
    }
];
