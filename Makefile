TARGET = libwalkingdriver.so
SRCS = ax12driver.c ax-comm.c motorDriver.c
HEADERS = $(addprefix src/, ${SRCS:.c=.h}) src/driver.h
OBJECTS = $(addprefix build/,${SRCS:.c=.o})
TESTS = tests/motorDriver tests/AX12position tests/AXcomm tests/AXmove
JSBINDINGS := $(wildcard JSbinding/*.js)
CC=gcc
CFLAGS = -O2 -g -std=gnu99 -Wall -Werror -fpic
LDFLAGS= -shared -lwiringPi -lm -lwalkingdriver
LDFLAGS_EXE = -lwiringPi -lm -lwalkingdriver
PREFIX = /usr/local
VPATH = build/

vpath %.c src/ tests/ examples/
vpath %.h src/

.PHONY: all build clean tests AX12console jsinstall

all: build build/$(TARGET) tests

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

tests: $(TESTS)

tests/motorDriver: tests/motorDriver.o src/motorDriver.o
	$(CC) $(LDFLAGS_EXE) $^ -o $@

clean:
	rm -f build/*.o build/*.so build/*.d
	rm -f $(TESTS)
	rm -f tests/*.o

jsinstall: $(JSBINDINGS) JSbinding/package.json
	mkdir -p $(DESTDIR)$(PREFIX)/lib/node_modules/walkingdriver
	cp -r JSbinding/* $(DESTDIR)$(PREFIX)/lib/node_modules/walkingdriver
	cd $(DESTDIR)$(PREFIX)/lib/node_modules/walkingdriver; npm install
AX12console: AX12console/app.js AX12console/package.json AX12console/AX12
	mkdir -p $(DESTDIR)$(PREFIX)/lib/node_modules/AX12console
	cp -r AX12console/* $(DESTDIR)$(PREFIX)/lib/node_modules/AX12console
	cd $(DESTDIR)$(PREFIX)/lib/node_modules/AX12console; npm install
	cp AX12console/AX12 $(DESTDIR)$(PREFIX)/bin/
	chmod a+x $(DESTDIR)$(PREFIX)/bin/AX12

install: build/$(TARGET) jsinstall AX12console
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/include/walkingdriver
	cp build/$(TARGET) $(DESTDIR)$(PREFIX)/lib/
	cp $(HEADERS) $(DESTDIR)$(PREFIX)/include/walkingdriver/
	chmod 0755 $(DESTDIR)$(PREFIX)/lib/$(TARGET)
	ldconfig
	ldconfig -p | grep walkingdriver

-include $(subst .c,.d,$(SRCS))
