#include "motordriver.h"
#include "i2c-cache.h"
#include "i2c-functions.h"
#include "imudriver.h"

#define MOTOR_ADDR 0x23
// PIC registers
#define I2CMOTOR_KP 0x41
#define I2CMOTOR_KI 0x42
#define I2CMOTOR_KD 0x43
#define I2CMOTOR_SPEEDL 0x83
#define I2CMOTOR_SPEEDR 0x84
#define I2CMOTOR_CODL 0x85
#define I2CMOTOR_CODR 0x86
// COMMANDS INDEX
#define KP_CMD 0
#define KI_CMD 1
#define KD_CMD 2
#define SPEEDL_CMD 0
#define SPEEDR_CMD 1
#define CODL_CMD 2
#define CODR_CMD 3

struct device_cache *cache;

int initMotorDriver() {
    int error = 0;

    cache = initCache(MOTOR_ADDR, 3, 4, 3, 4);
    cache->r8_cmds[KP_CMD] = I2CMOTOR_KP; cache->w8_cmds[KP_CMD] = I2CMOTOR_KP;
    cache->r8_cmds[KP_CMD] = I2CMOTOR_KP; cache->w8_cmds[KP_CMD] = I2CMOTOR_KP;
    cache->r8_cmds[KP_CMD] = I2CMOTOR_KP; cache->w8_cmds[KP_CMD] = I2CMOTOR_KP;

    cache->r16_cmds[SPEEDR_CMD] = I2CMOTOR_SPEEDR; cache->w16_cmds[SPEEDR_CMD] = I2CMOTOR_SPEEDR;
    cache->r16_cmds[SPEEDL_CMD] = I2CMOTOR_SPEEDL; cache->w16_cmds[SPEEDL_CMD] = I2CMOTOR_SPEEDL;
    cache->r16_cmds[CODR_CMD] = I2CMOTOR_CODR; cache->w16_cmds[CODR_CMD] = I2CMOTOR_CODR;
    cache->r16_cmds[CODL_CMD] = I2CMOTOR_CODL; cache->w16_cmds[CODL_CMD] = I2CMOTOR_CODL;

    error = initIMU();

    //check for the two I2C devices
    if(i2c_init(MOTOR_ADDR)<0) {
        error += -2;
    }
    return error;
}

int getLdistance() {
    return c_read16(cache, CODL_CMD)/1.52;
}
int getRdistance() {
    return c_read16(cache, CODR_CMD)/1.52;
}
void setLdistance(int distance) {
    distance = distance*1.52;
    c_write16(cache, CODL_CMD, distance);
}
void setRdistance(int distance) {
    distance = distance*1.52;
    c_write16(cache, CODR_CMD, distance);
}
double getLspeed() {
    double result = c_read16(cache, SPEEDL_CMD)/172.34;
    return result;
}
double getRspeed() {
    double result = c_read16(cache, SPEEDR_CMD)/172.34;
    return result;
}
void setLspeed(double speed) {
    speed = speed*172.34;
    c_write16(cache, SPEEDL_CMD, (int) speed);
}
void setRspeed(double speed) {
    speed = speed*172.34;
    c_write16(cache, SPEEDR_CMD, (int) speed);
}

int getKp() { return c_read8(cache, KP_CMD); }
int getKi() { return c_read8(cache, KI_CMD); }
int getKd() { return -c_read8(cache, KD_CMD); }
void setKp(int8_t coeff) { c_write8(cache, KP_CMD, coeff); }
void setKi(int8_t coeff) { c_write8(cache, KI_CMD, coeff); }
void setKd(int8_t coeff) { c_write8(cache, KD_CMD, -coeff); }

double getRobotHeading() { return getHeading(); }
void setRobotHeading(double heading) { setHeading(heading); }
void setMotorDriverUpdateFreq(int max_freq) { setUpdateFrequency(max_freq); }
int getMotorDriverUpdateFreq() { return getUpdateFrequency(); }
