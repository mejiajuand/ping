# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2026 Juan Mejia

CC=cc
SRC=main.c ping.c icmp.c
WARNINGS=-Wall -Wextra -Wpedantic -Wformat -Wformat=2
WARNINGS+=-Wformat-security -Wconversion -Wsign-conversion -Wshadow
WARNINGS+=-Wundef -Wstrict-prototypes -Wmissing-prototypes -Wcast-align
WARNINGS+=-Wwrite-strings -Werror=format-security -Werror=implicit
WARNINGS+=-Werror=incompatible-pointer-types -Werror=int-conversion
CFLAGS=$(WARNINGS) -fstack-protector-strong -fstack-clash-protection
CFLAGS+=-ftrivial-auto-var-init=zero -fno-common
DEBUGFLAGS=$(CFLAGS) -O0 -g3 -fno-omit-frame-pointer
RELEASEFLAGS=$(CFLAGS) -O2 -pipe -fPIE -fvisibility=hidden
RELEASEFLAGS+=-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3

.PHONY: all debug release clean

all:
	mkdir -p ../bin/debug ../bin/release
	make release

debug:
	$(CC) $(DEBUGFLAGS) $(SRC) -o ../bin/debug/ping -lm

release:
	$(CC) $(RELEASEFLAGS) $(SRC) -o ../bin/release/ping -lm

clean:
	rm -f ../bin/debug/ping ../bin/release/ping
