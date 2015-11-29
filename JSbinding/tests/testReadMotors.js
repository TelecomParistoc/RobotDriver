var driver = require('../motordriver.js');

console.log("initializing driver : " + driver.initMotorDriver());
setInterval(function() {
    console.log("distances :  R: "+ driver.Rdistance() + "mm, L: "+ driver.Ldistance()+"mm");
    console.log("speeds :  R: "+ driver.Rspeed() + "m/s, L: "+ driver.Lspeed()+"m/s");
}, 200);
