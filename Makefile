CC ?= gcc
LD := $(CC)

PREFIX ?= /usr/local

LLVM_CFLAGS := $(shell llvm-config --cflags | sed -e "s/-DNDEBUG//g")
LLVM_LIBS := $(shell llvm-config --libs)
LLVM_LDFLAGS := $(shell llvm-config --ldflags)

CFLAGS += -g
CFLAGS += -Iinclude
CFLAGS += -Isrc
CFLAGS += -O3
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wshadow
CFLAGS += -std=c99
CFLAGS += -D_GNU_SOURCE=1
CFLAGS += $(LLVM_CFLAGS)

LDFLAGS += -lstdc++
LDFLAGS += -lreadline
LDFLAGS += $(LLVM_LDFLAGS)

LIBS += $(LLVM_LIBS)

OBJS += src/eval.o
OBJS += src/expr.o
OBJS += src/gc.o
OBJS += src/huron.o
OBJS += src/llvm.o
OBJS += src/object.o
PROG := huron

ifeq ($(strip $(V)),)
	E := @echo
	Q := @
else
	E := @\#
	Q :=
endif

all: $(PROG)
.PHONY: all

clean:
	$(E) "  CLEAN    "
	$(Q) rm -f $(OBJS) $(PROG)
.PHONY: clean

install: $(PROG)
	$(E) "  INSTALL  " $<
	$(Q) install $< $(PREFIX)/bin/
.PHONY: install

$(PROG): $(OBJS)
	$(E) "  LINK     " $@
	$(Q) $(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(E) "  COMPILE  " $@
	$(Q) $(CC) $(CFLAGS) -c -o $@ $<
