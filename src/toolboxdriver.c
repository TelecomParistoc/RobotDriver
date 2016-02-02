#include "toolboxdriver.h"
#include "i2c-cache.h"
#include "i2c-functions.h"
#include <stdio.h>

#define TOOLBOX_ADDR 0x42

static struct device_cache *cache = NULL;

void initToolboxDriver() {
    cache = initCache(TOOLBOX_ADDR, 3, 4, 3, 4);

}
