CC      = gcc
CFLAGS  = -Wall -Wextra -std=gnu11 -g
BINDIR  = bin

SRCS  = $(wildcard src/*.c) $(wildcard challenges/*.c)
PROGS = $(patsubst src/%.c,$(BINDIR)/%,$(wildcard src/*.c)) \
        $(patsubst challenges/%.c,$(BINDIR)/%,$(wildcard challenges/*.c))

all: $(PROGS)

$(BINDIR)/%: src/%.c | $(BINDIR)
	$(CC) $(CFLAGS) $< -o $@

$(BINDIR)/%: challenges/%.c | $(BINDIR)
	$(CC) $(CFLAGS) $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(BINDIR) build

.PHONY: all clean
