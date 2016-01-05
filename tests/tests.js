var ffi = require("ffi");
var voidPtr = require("ref").refType('void');
var funcs = [];

function cCallback(func) {
    var cbck = ffi.Callback('void', [ voidPtr ], func);
    funcs.push(cbck);
    return cbck;
}

module.exports = [
    {
        title: "verify speed changes with limited acceleration",
        inertia: false,
        duration: 3,
        run: function (motion, log, block) {
            motion.initMotionController();
            motion.queueSpeedChange(0.5, cCallback(function() { log("rise callback"); }) );
            motion.queueSpeedChange(0, cCallback(function() { log("fall callback"); }) );
        }
    },
    {
        title: "verify robot stops at the right distance",
        inertia: false,
        duration: 3,
        run: function (motion, log, block) {
            motion.initMotionController();
            motion.queueSpeedChange(0.5, cCallback(function() {
                log("rise callback at " + Math.round(motion.getRobotDistance()) + " mm");
            }));
            motion.queueSpeedChangeAt(1000, 0, cCallback(function() {
                log("fall callback at " + Math.round(motion.getRobotDistance()) + " mm");
            }));
        }
    },
    {
        title: "verify robot stops at the right distance with inertia",
        inertia: 0.05,
        duration: 4,
        run: function (motion, log, block) {
            motion.initMotionController();
            motion.queueSpeedChange(0.5, cCallback(function() {
                log("rise callback at " + Math.round(motion.getRobotDistance()) + " mm");
            }));
            motion.queueStopAt(1000, cCallback(function() {
                log("stop callback at " + Math.round(motion.getRobotDistance()) + " mm");
            }));
        }
    },
    {
        title: "verify complex move is executed correctly",
        inertia: false,
        duration: 4,
        run: function (motion, log, block) {
            motion.initMotionController();
            motion.queueSpeedChange(0.5, cCallback(function() {
                log("rise callback at " + Math.round(motion.getRobotDistance()) + " mm");
            }));
            motion.queueSpeedChangeAt(700, 0.2, cCallback(function() {
                log("slow down callback at " + Math.round(motion.getRobotDistance()) + " mm");
            }));
            motion.queueStopAt(1000, cCallback(function() {
                log("stop callback at " + Math.round(motion.getRobotDistance()) + " mm");
            }));
        }
    },
    {
        title: "verify max acceleration can be changed",
        inertia: 0.02,
        duration: 4,
        run: function (motion, log, block) {
            motion.initMotionController();
            motion.setMaxAcceleration(1);
            motion.queueSpeedChange(0.5, cCallback(function() {
                log("rise callback at " + Math.round(motion.getRobotDistance()) + " mm");
            }));
            motion.queueSpeedChangeAt(700, 0.2, cCallback(function() {
                log("slow down callback at " + Math.round(motion.getRobotDistance()) + " mm");
            }));
            motion.queueStopAt(1000, cCallback(function() {
                log("stop callback at " + Math.round(motion.getRobotDistance()) + " mm");
            }));
        }
    },
    {
        title: "verify robot can turn 90",
        inertia: false,
        duration: 4,
        run: function (motion, log, block) {
            motion.initMotionController();
            motion.setMaxAcceleration(0.5);
            motion.turnOf(90, cCallback(function() {
                log("turn finished");
            }));
        }
    },
    {
        title: "verify robot can turn 180°",
        inertia: false,
        duration: 4,
        run: function (motion, log, block) {
            motion.initMotionController();
            motion.turnOf(180, cCallback(function() {
                log("turn finished");
            }));
        }
    },
    {
        title: "verify blocking detected",
        inertia: 0.05,
        duration: 2,
        run: function (motion, log, block) {
            motion.initMotionController();
            // a wall will stop the robot at 300mm
            block(300);
            motion.setBlockingCallback(cCallback(function() {
                log("robot blocked !");
                // when we hit the wall, stop the motors and go backward
                motion.fastSpeedChange(0);
                motion.queueSpeedChange(-0.5, cCallback(function() { log("going back"); }));
                //block(232323);
            }));
            motion.queueSpeedChange(0.5, cCallback(function() { log("rise callback"); }) );
        }
    }
];
