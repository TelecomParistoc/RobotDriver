var motordriver = require("./motordriver_mock.js");
var motion = require("../JSbinding/motioncontroller.js");

var robotDiameter = 0.140;

module.exports = function robot() {
    var distance = 0;
    var heading = 0;
    var computeReaction;
    useSimpleModel();
    motordriver.distance(0);
    motordriver.heading(0);

    function nextStep(timeStep) {
        var deltas = computeReaction(motordriver.speed(), motordriver.diff(), timeStep);
        distance += deltas.deltaDistance;
        heading += deltas.deltaHeading;
        heading = (heading + 360) % 360;
        motordriver.distance(distance*1000);
        motordriver.heading(heading);
        motordriver.nextStep();
    }
    function assertInitialized() {
        return motordriver.assertInitialized();
    }
    function useInertialModel(delay) {
        function createArray(size) {
            var arr = [];
            for(var i=0; i<size; i++)
                arr.push(0);
            return arr;
        }
        function mean(buff) {
            var sum = 0;
            for(var i in buff)
                sum += buff[i];
            return sum/buff.length;
        }
        var diffs = createArray(Math.round(delay/0.01));
        var speeds = createArray(Math.round(delay/0.01));

        computeReaction = function(speed, diff, time) {
            diffs.shift();
            diffs.push(diff);
            speeds.shift();
            speeds.push(speed);
            return {
                deltaDistance: speeds[0]*time,
                deltaHeading: diffs[0]*time*360/(Math.PI*robotDiameter)
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
        speed: motordriver.speed,
        diff: motordriver.diff,
        distance: motordriver.distance,
        heading: motordriver.heading
    };
};
