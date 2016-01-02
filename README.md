# Robot drivers #

A set of (user space) drivers for raspberry pi providing an abstract access to the robot hardware, mainly via the I2C bus.
In addition, it provides a mechanism against I2C bus overload, freeing the programmer from these considerations

### Installation ###

* First, you need [wiringPi](http://wiringpi.com/download-and-install/).

* Once wiringPi is installed, get back the sources :
`git clone git@github.com:TelecomParistoc/RobotDriver.git`

* cd to the root of the repository and enter
`make` and `sudo make install`

