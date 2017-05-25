TARGET = librobotdriver.so
SRCS = i2c-functions.c timing.c ax12driver.c ax-comm.c motordriver.c movecontroller.c roof.c
HEADERS = $(addprefix src/, ${SRCS:.c=.h}) src/driver.h
OBJECTS = $(addprefix build/,${SRCS:.c=.o})
TESTS = tests/timing tests/AX12position tests/AXcomm tests/AXmove
JSBINDINGS := $(wildcard JSbinding/*.js)
CC=gcc
CFLAGS = -O2 -std=gnu99 -Wall -Werror -fpic
LDFLAGS= -shared -lwiringPi -lm -lpthread
PREFIX = /usr/local
VPATH = build/

vpath %.c src/ tests/
vpath %.h src/

.PHONY: all build clean tests AX12console jsinstall

all: build build/$(TARGET)

build:
	@mkdir -p build
build/%.o: %.c build/%.d
	@echo "$<"
	@$(CC) -c -o $@ $< $(CFLAGS)
build/%.d : %.c
	@$(CC) $(CFLAGS) -MM -MF $@ -MP $<

build/$(TARGET): $(OBJECTS)
	@echo "\nLinking target $@"
	@$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

tests: LDFLAGS=-lrobotdriver
tests: $(TESTS)

tests/%: tests/%.o $(OBJECTS)
	$(CC) $(LDFLAGS_EXE) $^ -o $@

clean:
	rm -f build/*.o build/*.so build/*.d
	rm -f $(TESTS)
	rm -f tests/*.o

jsinstall: $(JSBINDINGS) JSbinding/package.json
	mkdir -p $(DESTDIR)$(PREFIX)/lib/node_modules/robotdriver
	cp -r JSbinding/* $(DESTDIR)$(PREFIX)/lib/node_modules/robotdriver
	cd $(DESTDIR)$(PREFIX)/lib/node_modules/robotdriver; npm install
AX12console: AX12console/app.js AX12console/package.json AX12console/AX12
	mkdir -p $(DESTDIR)$(PREFIX)/lib/node_modules/AX12console
	cp -r AX12console/* $(DESTDIR)$(PREFIX)/lib/node_modules/AX12console
	cd $(DESTDIR)$(PREFIX)/lib/node_modules/AX12console; npm install
	cp AX12console/AX12 $(DESTDIR)$(PREFIX)/bin/
	chmod a+x $(DESTDIR)$(PREFIX)/bin/AX12

install: build/$(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/include/robotdriver
	cp build/$(TARGET) $(DESTDIR)$(PREFIX)/lib/
	cp $(HEADERS) $(DESTDIR)$(PREFIX)/include/robotdriver/
	chmod 0755 $(DESTDIR)$(PREFIX)/lib/$(TARGET)
	ldconfig
	ldconfig -p | grep robotdriver

-include $(subst .c,.d,$(SRCS))
