#include "toolboxdriver.h"
#include "i2c-cache.h"
#include "i2c-functions.h"
#include "config.h"
#include <wiringPi.h>
#include <stdio.h>

// toolbox controller I2C address
#define TOOLBOX_ADDR 0x42
// writable 8 bit registers
#define TB_PWM1 0x41
#define TB_PWM2 0x42
#define TB_PWM3 0x43
#define TB_PWM4 0x44
#define TB_LEDS 0x45
#define TB_LEDSSO 0x46
#define TB_LEDSCO 0x47
#define TB_COLLMASK 0x48
#define TB_SENMASK 0x49
#define AX_SETACTIVEWHEEL 0x4A
#define AX_SETACTIVEDEFAULT 0x4B
// readable 8 bit registers
#define TB_INTERRUPT_STATUS 0x40
#define TB_MOTOR_POWER_LEVEL 0x41
#define TB_LOGIC_POWER_LEVEL 0x42
#define TB_BUTTONS 0x43
#define TB_SENSORS 0x44
#define TB_COLLISIONS 0x45
// writable 16 bit registers
#define AX_SETSPEED 0x81
#define AX_SETPOSITION 0x82
#define AX_SETTORQUE 0x83
// readable 16 bit registers
#define AX_GETPOSITION 0x81
// input pins
#define TB_INT 7
#define TB_BT4 2
#define TB_BT5 3
// interrupt flags
#define AX12_FINISHED_MOVE 0x01
#define AX12_FORCING 0x02
#define SENSOR_CHANGE 0x04
#define COLLISION_CHANGE 0x08

static struct device_cache *cache = NULL;

static uint8_t sensorsMask = 0x1F;
static uint8_t collisionsMask = 0x1F;

static void (*sensorsCallback)(void) = NULL;
static void (*collisionsCallback)(void) = NULL;

static void invalidateCache(int command) {
    cache->r8_flags[command&0x0F] = CACHE_NOT_VALID;
}

static volatile int axFinishedMove;
static volatile int axForcing;

static uint8_t axCurrentId;
static int axPositions[150];

static void interruptManager() {
    if(digitalRead(TB_INT)) {
        uint8_t flags = I2Cread8(TOOLBOX_ADDR, TB_INTERRUPT_STATUS);
	if(flags & AX12_FINISHED_MOVE) {
		if(getAxPosition() == I2Cread16(TOOLBOX_ADDR, AX_GETPOSITION))
			axFinishedMove = 1;
		else
			axFinishedMove = 2;
	}
	if(flags & AX12_FORCING) {
		axForcing = 1;
	}
        if(flags & SENSOR_CHANGE) {
            invalidateCache(TB_SENSORS);
            if(sensorsCallback != NULL)
                sensorsCallback();
        }
        if(flags & COLLISION_CHANGE) {
            invalidateCache(TB_COLLISIONS);
            if(collisionsCallback != NULL)
                collisionsCallback();
        }
    }
}

int initToolboxDriver() {
    cache = initCache(TOOLBOX_ADDR, 6, 2, 10, 4);
    cache->w8_cmds[TB_PWM1&0x0F] = TB_PWM1;
    cache->w8_cmds[TB_PWM2&0x0F] = TB_PWM2;
    cache->w8_cmds[TB_PWM3&0x0F] = TB_PWM3;
    cache->w8_cmds[TB_PWM4&0x0F] = TB_PWM4;
    cache->w8_cmds[TB_LEDS&0x0F] = TB_LEDS;
    cache->w8_cmds[TB_LEDSSO&0x0F] = TB_LEDSSO;
    cache->w8_cmds[TB_LEDSCO&0x0F] = TB_LEDSCO;
    cache->w8_cmds[TB_COLLMASK&0x0F] = TB_COLLMASK;
    cache->w8_cmds[TB_SENMASK&0x0F] = TB_SENMASK;
    cache->w8_cmds[AX_SETACTIVEWHEEL&0x0F] = AX_SETACTIVEWHEEL;
    cache->w8_cmds[AX_SETACTIVEDEFAULT&0x0F] = AX_SETACTIVEDEFAULT;

    cache->r8_cmds[TB_INTERRUPT_STATUS&0x0F] = TB_INTERRUPT_STATUS;
    cache->r8_cmds[TB_MOTOR_POWER_LEVEL&0x0F] = TB_MOTOR_POWER_LEVEL;
    cache->r8_cmds[TB_LOGIC_POWER_LEVEL&0x0F] = TB_LOGIC_POWER_LEVEL;
    cache->r8_cmds[TB_BUTTONS&0x0F] = TB_BUTTONS;
    cache->r8_cmds[TB_SENSORS&0x0F] = TB_SENSORS;
    cache->r8_cmds[TB_COLLISIONS&0x0F] = TB_COLLISIONS;

    cache->w16_cmds[AX_SETSPEED&0x0F] = AX_SETSPEED;
    cache->w16_cmds[AX_SETPOSITION&0x0F] = AX_SETPOSITION;
    cache->w16_cmds[AX_SETTORQUE&0x0F] = AX_SETTORQUE;

    cache->r16_cmds[AX_GETPOSITION&0x0F] = AX_GETPOSITION;

    cache->updateCallback = interruptManager;

    // setup input pins
    wiringPiSetup();

    pinMode(TB_INT, INPUT);
    pinMode(TB_BT4, INPUT);
    pinMode(TB_BT5, INPUT);
    pullUpDnControl(TB_INT, PUD_DOWN);
    pullUpDnControl(TB_BT4, PUD_DOWN);
    pullUpDnControl(TB_BT5, PUD_DOWN);

    //check for the I2C connection
    if(i2c_init(TOOLBOX_ADDR)<0) {
        printf("initToolboxDriver : ERROR cannot initialize Toolbox Controller\n");
        return -1;
    } else
        return 0;
}

double getMotorPowerLevel() {
    uint8_t val = c_read8(cache, TB_MOTOR_POWER_LEVEL&0x0F);
    double voltage = 0;
    if(val) {
        int period = (val+300)*2;
        voltage = MOTOR_LEVEL_COEFF/period;
    }
    return voltage;
}

double getLogicPowerLevel() {
    uint8_t val = c_read8(cache, TB_LOGIC_POWER_LEVEL&0x0F);
    return (val*2 + 448)*LOGIC_LEVEL_COEFF;
}

int getButton(int number) {
    if(number < 1 || number > 5) {
        printf("Button %d doesn't exist !\n", number);
        return -1;
    }

    if(number == 4)
        return digitalRead(TB_BT4);
    if(number == 5)
        return digitalRead(TB_BT5);

    uint8_t val = c_read8(cache, TB_BUTTONS&0x0F);
    if(val & (0x01 << (number-1)))
        return 1;
    else
        return 0;
}

int getSensor(int number) {
    if(number < 1 || number > 5) {
        printf("SENSOR %d doesn't exist !\n", number);
        return 0;
    }
    uint8_t val = c_read8(cache, TB_SENSORS&0x0F);
    if(val & (0x01 << (number-1)))
        return 1;
    else
        return 0;
}

int getCollisionDetector(int number) {
    if(number < 1 || number > 5) {
        printf("Collision detector %d doesn't exist !\n", number);
        return -1;
    }
    uint8_t val = c_read8(cache, TB_COLLISIONS&0x0F);
    if(val & (0x01 << (number-1)))
        return 1;
    else
        return 0;
}

void enableSensorCallback(int number) {
    if(number < 1 || number > 5) {
        printf("SENSOR %d doesn't exist !\n", number);
        return;
    }
    sensorsMask |= 1 << (number - 1);
    c_write8(cache, TB_SENMASK&0x0F, sensorsMask);
}
void disableSensorCallback(int number) {
    if(number < 1 || number > 5) {
        printf("SENSOR %d doesn't exist !\n", number);
        return;
    }
    sensorsMask &= 0xFF ^ (1 << (number - 1));
    c_write8(cache, TB_SENMASK&0x0F, sensorsMask);
}
void enableCollisionCallback(int number) {
    if(number < 1 || number > 5) {
        printf("Collision detector %d doesn't exist !\n", number);
        return;
    }
    collisionsMask |= 1 << (number - 1);
    c_write8(cache, TB_COLLMASK&0x0F, collisionsMask);
}
void disableCollisionCallback(int number) {
    if(number < 1 || number > 5) {
        printf("Collision detector %d doesn't exist !\n", number);
        return;
    }
    collisionsMask &= 0xFF ^ (1 << (number - 1));
    c_write8(cache, TB_COLLMASK&0x0F, collisionsMask);
}

void setPWM(int number, uint8_t value) {
    double dc = value*0.754;
    if(number < 1 || number > 4) {
        printf("the PWM out %d doesn't exist !\n", number);
        return;
    }
    c_write8(cache, number, (uint8_t) dc);
}

void setLED(int number, int state) {
    if(number < 1 || number > 4) {
        printf("LED%d doesn't exist !\n", number);
        return;
    }
    if(state) {
        // if some data is already in cache
        if(cache->w8_flags[TB_LEDSSO&0x0F]==CACHE_VALID)
            cache->w8_cache[TB_LEDSSO&0x0F] |= 1 << (number - 1);
        else
            c_write8(cache, TB_LEDSSO&0x0F, 1 << (number - 1));
    } else {
        // if some data is already in cache
        if(cache->w8_flags[TB_LEDSCO&0x0F]==CACHE_VALID)
            cache->w8_cache[TB_LEDSCO&0x0F] |= 1 << (number - 1);
        else
            c_write8(cache, TB_LEDSCO&0x0F, 1 << (number - 1));
    }
}

int getAxPosition() {
	int val = c_read16(cache, AX_GETPOSITION&0x0F);
	return val;
}

void setAxActiveWheel(uint8_t id) {
	I2Cwrite8(TOOLBOX_ADDR, AX_SETACTIVEWHEEL, id);
	axCurrentId = id;
	delayMilli(10);
}


void setAxActiveDefault(uint8_t id) {
	I2Cwrite8(TOOLBOX_ADDR, AX_SETACTIVEDEFAULT, id);
	axCurrentId = id;
	delayMilli(10);
}

void setAxSpeed(int speed) {
	I2Cwrite16(TOOLBOX_ADDR, AX_SETSPEED, speed);
	delayMilli(10);
}

void setAxPosition(int position) {
	I2Cwrite16(TOOLBOX_ADDR, AX_SETPOSITION, position);
	axPositions[axCurrentId] = position;
	delayMilli(10);
}

void setAxTorque(int torque) {
	I2Cwrite16(TOOLBOX_ADDR, AX_SETTORQUE, torque);
	delayMilli(10);
}

int axHasFinishedMove() {
	if (axFinishedMove) {
		tmp = axFinishedMove;
		axFinishedMove = 0;
		return tmp;
	}
	return 0;
}

int axIsForcing() {
	if (axForcing) {
		printf("Forcing\n");
		axForcing = 0;
		return 1;
	}
	return 0;
}

void setCollisionsCallback(void (*callback)(void)) {
    collisionsCallback = callback;
}

void setSensorsCallback(void (*callback)(void)) {
    sensorsCallback = callback;
}
