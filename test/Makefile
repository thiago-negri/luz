CFLAGS := -I../include -L../lib -lluz -fsanitize=address -g
LDFLAGS := LD_LIBRARY_PATH=../lib

CFILES = $(wildcard *.test.c)
TARGETS = $(patsubst %.test.c,%,$(CFILES))

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	rm -rf $(TARGETS)

%: %.test.c ../.deps/%.d
	$(CC) $< $(CFLAGS) -o $@
	$(LDFLAGS) ./$@

include $(wildcard ../.deps/*.d)
