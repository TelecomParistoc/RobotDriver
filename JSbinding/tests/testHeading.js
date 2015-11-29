var driver = require('../motordriver.js');

setInterval(function() {
    console.log("heading : "+ driver.heading() + "°");
}, 200);
