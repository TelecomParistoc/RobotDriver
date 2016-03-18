/* This file contains the constants that may change from one robot to another.
 * Use 'make big' to build for the big robot (with this config), and 'make small'
 * when compiling for the small robot (see config.small.h)
 */

#ifndef CONFIG_H
#define CONFIG_H

// distance between coding wheels in mm
#define WHEEL_DISTANCE 190 // !!!! WARNING : WRONG VALUE !!!!

// coefficient for motor battery level
#define MOTOR_LEVEL_COEFF 7840.0

// coefficient for motor battery level
#define LOGIC_LEVEL_COEFF 0.014

#define BIG_ROBOT

#endif
