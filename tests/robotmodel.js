var motordriver = require("./motordriver_mock.js");
var motion = require("../JSbinding/motioncontroller.js");

var robotDiameter = 140;

module.exports = function robot() {
    var distance = 0;
    var heading = 0;
    var computeReaction;
    useSimpleModel();

    function nextStep(timeStep) {
        var deltas = computeReaction(motordriver.speed(), motordriver.diff(), timeStep);
        distance += deltas.deltaDistance;
        heading += deltas.deltaHeading;
        heading = (heading + 360) % 360;
        motordriver.distance(distance);
        motordriver.heading(heading);
        motordriver.nextStep();
    }
    function assertInitialized() {
        return motordriver.assertInitialized();
    }
    function useInertialModel() {
        var diffs = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
        var speeds = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
        function mean(buff) {
            var sum = 0;
            for(var val in buff)
                sum += val;
            return sum/buff.length;
        }
        computeReaction = function(speed, diff, time) {
            diffs.shift();
            diffs.push(diff);
            speeds.shift();
            speeds.push(speed);
            return {
                deltaDistance: mean(speeds)*time,
                deltaHeading: mean(diffs)*time*360/(Math.PI*robotDiameter)
            };
        };
    }
    function useSimpleModel() {
        computeReaction = function(speed, diff, time) {
            return {
                deltaDistance: speed*time,
                deltaHeading: diff*time*360/(Math.PI*robotDiameter)
            };
        };
    }
    return {
        useInertialModel: useInertialModel,
        useSimpleModel: useSimpleModel,
        nextStep: nextStep,
        assertInitialized: assertInitialized,
        speed: function() { return motordriver.speed(); },
        diff: function() { return motordriver.diff(); }
    };
};
