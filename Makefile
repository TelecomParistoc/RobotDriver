TARGET := librobotdriver.so
SRCS = i2c-cache.c imudriver.c motordriver.c i2c-functions.c
HEADERS := $(addprefix src/, ${SRCS:.c=.h})
OBJS := $(addprefix build/, ${SRCS:.c=.o})
CC=gcc
CFLAGS = -O2 -Wall
LDFLAGS= -shared
PREFIX = /usr/local

.PHONY: all

all: build/$(TARGET)

build/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

build/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

clean:
	rm -f build/*.o
	rm -f build/*.a

install: build/$(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/include/robotdriver
	cp $(TARGET) $(DESTDIR)$(PREFIX)/lib/
	cp HEADERS $(DESTDIR)$(PREFIX)/include/robotdriver/
