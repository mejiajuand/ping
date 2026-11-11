# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2026 Juan Mejia

CC := cc

SRC := main.c icmp.c ping.c

TARGET := ping

WARNINGS := -Wall -Wextra -Wpedantic -Wformat -Wformat=2 \
-Wformat-security -Wconversion -Wsign-conversion -Wshadow \
-Wundef -Wstrict-prototypes -Wmissing-prototypes -Wcast-align \
-Wwrite-strings -Werror=format-security -Werror=implicit \
-Werror=incompatible-pointer-types -Werror=int-conversion

COMMON := -std=iso9899:1999 $(WARNINGS) -fstack-protector-strong \
-fstack-clash-protection -ftrivial-auto-var-init=zero -fno-common

PRODUCTION := -O2 -pipe -mtune=generic -fPIE -fvisibility=hidden \

HARDENED := -pie -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack \
-Wl,-z,separate-code -Wl,--as-needed

DEBUG := $(COMMON) -O0 -g3 -fno-omit-frame-pointer

RELEASE := $(COMMON) $(PRODUCTION)

.PHONY: debug release

all:
	mkdir -p ../bin/debug
	$(CC) $(SRC) -o ../bin/debug/ping -lm

debug:
	mkdir -p ../bin/debug
	$(CC) $(DEBUG) $(SRC) -o ../bin/debug/ping -lm

release:
	mkdir -p ../bin/release
	$(CC) $(RELEASE) $(SRC) -o ../bin/release/ping -lm

run:
	./../bin/debug/ping

clean:
	rm -f ../bin/debug/ping ../bin/release/ping
