CC ?= gcc
LD := $(CC)

PREFIX ?= /usr/local

CFLAGS += -g
CFLAGS += -Isrc
CFLAGS += -O3
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wshadow
CFLAGS += -pedantic
CFLAGS += -std=c99

OBJS += src/huron.c
PROG := huron

E := @echo
Q := @

all: $(PROG)
.PHONY: all

clean:
	$(E) "  CLEAN    "
	$(Q) rm -f $(OBJS) $(PROG)
	$(Q) find . -name *.tmp | xargs rm -fr
.PHONY: clean

install: $(PROG)
	$(E) "  INSTALL  " $<
	$(Q) install $< $(PREFIX)/bin/
.PHONY: install

$(PROG): $(OBJS)
	$(E) "  LINK     " $@
	$(Q) $(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(E) "  COMPILE  " $@
	$(Q) $(CC) $(CFLAGS) -c -o $@ $<
