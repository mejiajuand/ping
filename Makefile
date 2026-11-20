CC=cc
WARNINGS=-Wall -Wextra -Wpedantic -Wshadow -Wconversion \
-Wsign-conversion -Wformat -Wformat=2 -Wformat-security \
-Wstrict-prototypes -Wmissing-prototypes -Wcast-align \
-Wundef -Wwrite-strings -Werror=format-security \
-Werror=implicit -Werror=incompatible-pointer-types \
-Werror=int-conversion
CFLAGS=$(WARNINGS) -fstack-protector-strong -fno-common \
-fstack-clash-protection -ftrivial-auto-var-init=zero
SRC=src/ping.c src/icmp.c
DBG=bin/debug
REL=bin/release
BIN=ping

all: $(DBG) $(DBG)/$(BIN)

$(DBG):
	mkdir -p $(DBG)

$(DBG)/$(BIN): $(SRC)
	$(CC) $(CFLAGS) -g3 -O0 -fno-omit-frame-pointer \
	$(SRC) -o $(DBG)/$(BIN) -lm

$(REL):
	mkdir -p $(REL)

release: $(REL) $(SRC)
	$(CC) $(CFLAGS) -O2 -DNDEBUG -pipe -fPIE -pie \
	-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 $(SRC) -o $(REL)/$(BIN) \
	-lm

run:
	./$(DBG)/$(BIN) 127.0.0.1

clean:
	rm -f $(DBG)/$(BIN) $(REL)/$(BIN)

.PHONY: all release run clean
