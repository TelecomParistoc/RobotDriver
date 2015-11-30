#include "i2c-functions.h"
#include <wiringPi.h>
#include <sys/ioctl.h>
#include <stdio.h>

int i2c_device = NULL;

int i2c_init(uint8_t addr) {
    i2c_device = wiringPiI2CSetup(addr);
    return i2c_device;
}

int8_t I2Cread8(uint8_t addr, uint8_t reg) {
    if(i2c_device == NULL || i2c_device < 0) {
        if(i2c_init(addr) < 0) {
            printf("I2Cread8 : ERROR cannot open device\n");
            return -1;
        }
    } else if(ioctl(i2c_device, I2C_SLAVE, addr)<0) {
        printf("I2Cread8 : ERROR cannot connect to selected device at address 0x%x\n", addr);
        return -2;
    }
    return wiringPiI2CReadReg8 (i2c_device, reg);
}
int16_t I2Cread16(uint8_t addr, uint8_t reg) {
    if(i2c_device == NULL || i2c_device < 0) {
        if(i2c_init(addr) < 0) {
            printf("I2Cread16 : ERROR cannot open device\n");
            return -1;
        }
    } else if(ioctl(i2c_device, I2C_SLAVE, addr)<0) {
        printf("I2Cread16 : ERROR cannot connect to selected device at address 0x%x\n", addr);
        return -2;
    }
    return wiringPiI2CReadReg16 (i2c_device, reg);
}
int I2Cwrite8(uint8_t addr, uint8_t reg, uint8_t value) {
    if(i2c_device == NULL || i2c_device < 0) {
        if(i2c_init(addr) < 0) {
            printf("I2Cwrite8 : ERROR cannot open device\n");
            return -1;
        }
    } else if(ioctl(i2c_device, I2C_SLAVE, addr)<0) {
        printf("I2Cwrite8 : ERROR cannot connect to selected device at address 0x%x\n", addr);
        return -2;
    }
    return wiringPiI2CWriteReg8 (i2c_device, reg, value);
}
int I2Cwrite16(uint8_t addr, uint8_t reg, uint16_t value) {
    if(i2c_device == NULL || i2c_device < 0) {
        if(i2c_init(addr) < 0) {
            printf("I2Cwrite16 : ERROR cannot open device\n");
            return -1;
        }
    } else if(ioctl(i2c_device, I2C_SLAVE, addr)<0) {
        printf("I2Cwrite16 : ERROR cannot connect to selected device at address 0x%x\n", addr);
        return -2;
    }
    return wiringPiI2CWriteReg16(i2c_device, reg, value);
}
