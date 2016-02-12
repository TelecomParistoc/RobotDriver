/*
 * Motor driver : provides high level access to the robot's main motors
 * through the I2C bus.
 * functionnalities provided :
 *  > read and write the distance travelled by each wheel
 *    (quantum is ~1.5 mm)
 *  > read the actual speed and write the target speed (which will be PID
 *    controlled)
 *  > read/write PID coefficients
 *  > get/set heading of the robot
 *  Note : in order to free the programmer from I2C ressource sharing
 * considerations, read may be cached and write may be delayed when write/read
 * are made too often and might saturate the I2C bus.
 * See setMotorDriverUpdateFreq() for more details.
 */

#ifndef I2CMOTORDRIVER_H
#define I2CMOTORDRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* initialize the driver, needs to be called before any other operations
 * returns -1 if it was unable to init IMU, -2 if it was unable to init
 * motor controller and -3 if it was unable to init both */
int initMotorDriver();

/* get the distance travelled by the left wheel, in mm
 * forward count as positive and backward as negative
 * Warning : the distance needs to be reset before reaching ~12000 */
double getLdistance();
/* get the distance travelled by the right wheel, in mm
 * forward count as positive and backward as negative
 * Warning : the distance needs to be reset before reaching ~12000 */
double getRdistance();
/* set the left distance to a given value, in mm */
void setLdistance(double distance);
/* set the right distance to a given value, in mm */
void setRdistance(double distance);

/* get the actual speed of the left wheel, in m/s
 * forward is positive speed and backward is negative */
double getLspeed();
/* get the actual speed of the right wheel, in m/s
 * forward is positive speed and backward is negative */
double getRspeed();
/* set the target speed of the left wheel, in m/s */
void setLspeed(double speed);
/* set the target speed of the right wheel, in m/s */
void setRspeed(double speed);

/* ##### PID coefficients access #####
 * the coefficients are stored the motor driver's non-volatile memory,
 * so there's no need to set them at startup. */
/* get Kp : PID proportionnal coefficient */
uint8_t getKp();
/* get Ki : PID integral coefficient */
uint8_t getKi();
/* get Kd : PID derivative coefficient */
uint8_t getKd();
/* set Kp : PID proportionnal coefficient */
void setKp(uint8_t coeff);
/* set Ki : PID integral coefficient */
void setKi(uint8_t coeff);
/* set Kd : PID derivative coefficient */
void setKd(uint8_t coeff);

/* ##### Heading angle #####
 * get/set the heading of the robot (relative to power-on position)
 * in degrees (quantum = 1/16°) from 0 (included) to 360 (excluded)
 * Note : setting heading WON'T MOVE the robot, it will offset the heading
 * angle
 * to access the other angles (pitch and roll), see imudriver.h */
double getRobotHeading();
void setRobotHeading(double heading);

/* ##### Update frequency #####
 * a read/write doesn't always cause an I2C transaction. I2C
 * transactions are throttled to a max frequency, reads are cached and writes
 * are delayed if the get/set functions are called too often.
 * > the logic is as follows :
 *  + if no transaction was made in at least an 1/max_freq period, do an
 *    actual read or write on the bus.
 *  + for read : if there is any data fresher than 1/max_freq, return data
 *    otherwise update and return the new value
 *  + for write : if any transaction has been sent in the last 1/max_freq period,
 *    delay write */

 /* set max frequency (in Hz) : set by default to 100Hz*/
 void setMotorDriverUpdateFreq(int max_freq);
 /* get max frequency (in Hz)*/
 int getMotorDriverUpdateFreq();

 /* set a function called everytime the data is updated
  * the function is called just after new data has been read and before data
  * is sent.
  * callback function should be like : void myCallback() {...} and SHOULD
  * NOT BLOCK (no wait, filesystem access or any system call)*/
 void setMotorUpdateCallback(void (*updateCallback)(void));

 #ifdef __cplusplus
 }
 #endif
 
#endif
