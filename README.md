# Robot drivers #

A library providing high level access to the robot capabilities.
This library controls robot's modules over I2C, and provides an advanced bus
ressource sharing mecanism.

It provides (user space) drivers for :

* the IMU (BNO055)
* the motor module, controlling robot's motors with speed PID
* the toolbox module, controlling AX-12 and providing access to sensors and robot's buttons and LEDs

In addition, it provides high level function to control robot's motions.

## Installation ##

This library is designed for Raspberry Pi with Raspbian (We use Raspbian RT, a
real time-oriented version of Raspbian).

* First, you need [wiringPi](http://wiringpi.com/download-and-install/).

* Once wiringPi is installed, get back the sources :
`git clone git@github.com:TelecomParistoc/RobotDriver.git`

* cd to the root of the repository and enter `make small` on the small robot or
`make big` on the big robot

* finally enter `sudo make install`

## Usage ##

Don't forget to compile (actually, link) your C/C++ app with `-lrobotdriver` option.

### Motor module driver ###

To access the motors, including controlling robot's speed and accessing the distance
traveled by each wheel, include motordriver.h :

```c
#include "robotdriver/motordriver.h"
```

See src/motordriver.h for API.

### IMU driver ###

Heading access is provided by motor module driver, but access to pitch and roll
is available through imudriver.h. Include the header with :

```c
#include "robotdriver/imudriver.h"
```

See src/imudriver.h for API.

### Toolbox module driver ###

The toolbox module provides :
* AX-12 control, supporting callback on the end of moves
* 5 sensors inputs, supporting callback on change
* 5 collision detector inputs, supporting callback on change
* 3 button inputs
* 4 LEDs ouputs
* 4 PWM outputs
* voltage measurements of both batteries

To use these functions, include the header :

```c
#include "robotdriver/toolboxdriver.h"
```

For more info on toolbox module API, see [toolboxdriver.h](https://github.com/TelecomParistoc/RobotDriver/blob/master/src/toolboxdriver.h).

### Motion control library ###

The goal is to provide easy programming of complex move, including acceleration
limiting mecanism and an event system based on callbacks (JS style, yeah !)

To access high level function to control speed and heading, include the following
headers :

```c
#include "robotdriver/motioncontroller.h"
#include "robotdriver/speedcontroller.h"
#include "robotdriver/headingcontroller.h"
```

See src/motioncontroller.h for general use functions API.

See src/speedcontroller.h for speed change API.

See src/headingcontroller.h for heading change API.

## Examples ##

Some examples about motion control are given in examples/.

To run the examples, run `make examples` from the root of the repository, and run
the programs created in examples/ (on a Raspberry Pi with IMU and motor module connected).

## Tests ##

A test test environnement has been created to test motion controller without the
actual robot.

To run tests :

* install node ([nodejs.org](https://nodejs.org/en/download/))
* run `make testinstall`, then `make test`
* access [localhost:3003](http://localhost:3003) to see results.
