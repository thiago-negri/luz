CFLAGS := -I../include -L../lib -lluz -fsanitize=address -g
LDFLAGS := LD_LIBRARY_PATH=../lib

CFILES = $(wildcard *.c)
TARGETS = $(patsubst %.c,%,$(CFILES))

%: %.c
	$(CC) $< $(CFLAGS) -o $@
	$(LDFLAGS) ./$@

