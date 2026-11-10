CC := clang
SRC := $(wildcard *.c)
CPPFLAGS := -D_FORTIFY_SOURCE=3 -D_POSIX_C_SOURCE=200809L
WARNINGS := -Wall -Wextra -Wpedantic -Wformat=2 -Wformat-security \
-Wconversion -Wsign-conversion -Wshadow -Wstrict-prototypes \
-Wmissing-prototypes -Wundef -Wcast-align -Wwrite-strings
COMMON := -std=iso9899:1999 $(WARNINGS) -fstack-protector-strong \
-fstack-clash-protection -ftrivial-auto-var-init=zero -fno-common
PRODUCTION := -O2 -pipe -march=x86-64 -mtune=generic -fPIE \
-fvisibility=hidden -fcf-protection=full
LINK_HARDENING := -fuse-ld=lld -pie -Wl,-z,relro -Wl,-z,now \
-Wl,-z,noexecstack -Wl,-z,separate-code -Wl,--as-needed

ifeq ($(PROFILE),release)
	TARGET := ../bin/release/ping
	OBJDIR := ../bin/release/build
	OBJS := $(SRC:%.c=$(OBJDIR)/%.o)
	CFLAGS := $(COMMON) $(PRODUCTION)
else ifeq ($(PROFILE),tsan)
	TARGET := ../bin/debug/ping
	OBJDIR := ../bin/debug/build
	OBJS := $(SRC:%.c=$(OBJDIR)/%.o)
	CFLAGS := $(COMMON) -O1 -g3 -fno-omit-frame-pointer \
	-fsanitize=thread
	SANITIZERS := -fsanitize=thread
else ifeq ($(PROFILE),asan)
	TARGET := ../bin/debug/ping
	OBJDIR := ../bin/debug/build
	OBJS := $(SRC:%.c=$(OBJDIR)/%.o)
	CFLAGS := $(COMMON) -O1 -g3 -fno-omit-frame-pointer \
	-fsanitize=address,undefined,integer -fno-sanitize-recover=all
	SANITIZERS := -fsanitize=address,undefined,integer
else
	TARGET := ../bin/debug/ping
	OBJDIR := ../bin/debug/build
	OBJS := $(SRC:%.c=$(OBJDIR)/%.o)
	CFLAGS := $(COMMON) -O0 -g3 -fno-omit-frame-pointer
endif

LDFLAGS := $(LINK_HARDENING) $(SANITIZERS)

.PHONY: all clean run debug asan tsan release

all: $(TARGET)

debug:
	mkdir -p ../bin/debug
	$(MAKE) PROFILE=debug all

asan:
	mkdir -p ../bin/debug
	$(MAKE) PROFILE=asan all

tsan:
	mkdir -p ../bin/debug
	$(MAKE) PROFILE=tsan all

release:
	mkdir -p ../bin/release
	$(MAKE) PROFILE=release all


$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

run:
	./$(TARGET)

clean:
	rm -rf ../bin/debug/build ../bin/debug/ping \
	../bin/release/build ../bin/release/ping
