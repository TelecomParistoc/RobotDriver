var ffi = require('ffi');

var lib = ffi.Library('../build/librobotdriver', {
  'initMotorDriver': [ 'int', [] ],
  'assertInitialized': [ 'int' , []],
  'getLdistance': [ 'int', [] ],
  'getRdistance': [ 'int', [] ],
  'getRspeed': [ 'double', [] ],
  'getLspeed': [ 'double', [] ],
  'setLdistance': [ 'void', ['int'] ],
  'setRdistance': [ 'void', ['int'] ],
  'setRspeed': [ 'void', ['double'] ],
  'setLspeed': [ 'void', ['double'] ],
  'getRobotHeading': [ 'double', [] ],
  'setRobotHeading': [ 'void', ['double'] ],
  'nextStep': ['void', []]
});

module.exports = {
    init: lib.initMotorDriver,
    assertInitialized: lib.assertInitialized,
    nextStep: lib.nextStep,
    speed: function() {
        return (lib.getLspeed() + lib.getRspeed())/2;
    },
    diff: function() {
        return (lib.getRspeed() - lib.getLspeed())/2;
    },
    distance: function(distance) {
        if(typeof distance != "undefined") {
            lib.setLdistance(distance);
        } else {
            return lib.getLdistance();
        }
    },
    heading: function(value) {
        if(typeof value != "undefined") {
            lib.setRobotHeading(value);
        } else {
            return lib.getRobotHeading();
        }
    }
};
