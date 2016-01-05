var motion = require("../JSbinding/motioncontroller.js");
var tests = require("./tests.js");
var express = require('express');
var path = require('path');
var jade = require('jade');
require('sugar');

var app = express();

app.use(express.static(path.join(__dirname, 'public')));

var server = app.listen(3003, function () {
  var host = server.address().address;
  var port = server.address().port;

  console.log('MotorTest listening at http://%s:%s', host, port);
});

var timeStep = 0.01;

function stringifyFunctionBody(func) {
    var text = func.toString().split('{');
    text.shift();
    text = text.join("{").split('}');
    text.pop();
    return text.join("}");
}
function runTest(test) {
    var time = 0;
    var robot = require("./robotmodel.js")();
    if(test.inertia)
        robot.useInertialModel(test.inertia);
    //provide a function to show logs
    test.output = "";
    var log = function(text) {
        test.output += Math.round(time*1000)/1000 + " : " + text + "\n";
    };
    test.run(motion, log, robot.block);
    if(!robot.assertInitialized())
        log("WARNING : driver not initialized");
    test.labels = [0];
    test.data = [[0],[0]];
    test.distances = [[0], [0]];
    while(time < test.duration) {
        time += timeStep;
        robot.nextStep(timeStep);
        test.labels.push(time);
        test.data[0].push(robot.speed());
        test.data[1].push(robot.diff());
        test.distances[0].push(robot.distance());
        test.distances[1].push(robot.heading());
    }
    test.run = stringifyFunctionBody(test.run);
    return test;
}
for(var i in tests)
    tests[i] = runTest(tests[i]);
var index = jade.compileFile("templates/index.jade");

app.get("/", function (req, res) {
    res.send(index({tests: tests}));
});
app.get("/tests", function (req, res) {
    res.send(JSON.stringify(tests));
});
