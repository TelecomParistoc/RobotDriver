var driver = require('../motordriver.js');

console.log("initializing driver : " + driver.init());
setInterval(function() {
    console.log("heading : "+ driver.heading() + "°");
}, 200);
