var ffi = require('ffi');
var ref = require('ref');

var voidPtr = ref.refType('void');
var simpleCallback = ffi.Function('void', []);
var motionCallback = ffi.Function('void', [voidPtr]);

var lib = ffi.Library('./../build/librobotdriver', {
    // motioncontroller
    'initMotionController': [ 'int', [] ],
    'getMaxAcceleration': [ 'double', [] ],
    'setMaxAcceleration': [ 'void', ['double'] ],
    'setBlockingCallback': [ 'void', [simpleCallback] ],
    // speedcontroller :
    'queueSpeedChange': [ 'void', ['double', motionCallback]],
    'queueSpeedChangeAt': [ 'void', ['int', 'double', motionCallback]],
    'queueStopAt': [ 'void', ['int', motionCallback] ],
    'clearMotionQueue': ['void', []],
    'getDistanceTolerance': [ 'int', [] ],
    'setDistanceTolerance': [ 'void', ['int'] ],
    // headingcontroller :
    'getTargetHeading': [ 'double', [] ],
    'setTargetHeading': [ 'void', ['double', simpleCallback] ],
    'turnOf': [ 'void', ['double', simpleCallback] ],
    'getCurrentHeading': [ 'double', [] ],
    'getHeadingTolerance': [ 'double', [] ],
    'setHeadingTolerance': [ 'void', ['double'] ],
    'enableHeadingControl' : ['void', ['int'] ]
});

module.exports = lib;
