TARGET = librobotdriver.dylib
SRCS = i2c-cache.c imudriver.c motordriver.c i2c-functions.c queue.c motioncontroller.c headingcontroller.c controllerutils.c speedcontroller.c
HEADERS = $(addprefix src/, ${SRCS:.c=.h})
OBJECTS = $(addprefix build/,${SRCS:.c=.o})
CC=gcc
CFLAGS = -O2 -Wall -Werror -fpic
LDFLAGS= -shared -lwiringPi
PREFIX = /usr/local
VPATH = build/

vpath %.c src/ tests/
vpath %.h src/
-include $(subst .c,.d,$(SRCS))

.PHONY: all clean test update

all: build/$(TARGET)

build/%.o: %.c build/%.d
	$(CC) -c -o $@ $< $(CFLAGS)
build/%.d : %.c
	$(CC) $(CFLAGS) -MM -MF $@ -MP $<

build/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

test:
	make SRCS="motordriver_mock.c queue.c motioncontroller.c headingcontroller.c controllerutils.c speedcontroller.c" LDFLAGS=-shared
	cd tests; node testrunner.js

update:
	git pull
	make
	make install

clean:
	rm -f build/*.o build/*.so build/*.d

install: build/$(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/include/robotdriver
	cp build/$(TARGET) $(DESTDIR)$(PREFIX)/lib/
	cp $(HEADERS) $(DESTDIR)$(PREFIX)/include/robotdriver/
	chmod 0755 $(DESTDIR)$(PREFIX)/lib/$(TARGET)
	ldconfig
	ldconfig -p | grep robotdriver
