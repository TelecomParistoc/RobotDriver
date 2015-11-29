var driver = require('../motordriver.js');

setInterval(function() {
    console.log("distances :  R: "+ driver.Rdistance() + "mm, L: "+ driver.Ldistance()+"mm");
    console.log("speeds :  R: "+ driver.Rspeed() + "m/s, L: "+ driver.Lspeed()+"m/s");
}, 200);
