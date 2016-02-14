TARGET = librobotdriver.so
SRCS = i2c-cache.c imudriver.c motordriver.c i2c-functions.c queue.c motioncontroller.c headingcontroller.c controllerutils.c speedcontroller.c toolboxdriver.c
HEADERS = $(addprefix src/, ${SRCS:.c=.h})
OBJECTS = $(addprefix build/,${SRCS:.c=.o})
EXAMPLES = examples/example1 examples/example2 examples/example3
CC=gcc
CFLAGS = -O2 -std=gnu99 -Wall -Werror -fpic
LDFLAGS= -shared -lwiringPi -lm
PREFIX = /usr/local
VPATH = build/

vpath %.c src/ tests/ examples
vpath %.h src/

.PHONY: clean test update small

small: small_conf build build/$(TARGET)

small_conf:
	@echo "\n\nYou are compiling this lib for the SMALL ROBOT. If you're on the big robot, use 'make big'\n\n"
	cp src/config.small.h  src/config.h

big: big_conf build build/$(TARGET)

big_conf:
	@echo "\n\nYou are compiling this lib for the BIG ROBOT. If you're on the small robot, use 'make small'\n\n"
	cp src/config.big.h  src/config.h

build:
	mkdir -p build
build/%.o: %.c build/%.d
	$(CC) -c -o $@ $< $(CFLAGS)
build/%.d : %.c
	$(CC) $(CFLAGS) -MM -MF $@ -MP $<

build/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

examples: LDFLAGS=-lrobotdriver
examples: $(EXAMPLES)

mock:
	make SRCS="motordriver_mock.c queue.c motioncontroller.c headingcontroller.c controllerutils.c speedcontroller.c" LDFLAGS=-shared
test: mock
	cd tests; node testrunner.js

testinstall:
	cd tests; npm install
	cd JSbinding; npm install

update:
	git pull
	make
	make install

clean:
	rm -f build/*.o build/*.so build/*.d
	rm -f src/config.h

install: build/$(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/include/robotdriver
	cp build/$(TARGET) $(DESTDIR)$(PREFIX)/lib/
	cp $(HEADERS) $(DESTDIR)$(PREFIX)/include/robotdriver/
	chmod 0755 $(DESTDIR)$(PREFIX)/lib/$(TARGET)
	ldconfig
	ldconfig -p | grep robotdriver

-include $(subst .c,.d,$(SRCS))
