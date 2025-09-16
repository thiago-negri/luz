CSTATICFLAGS := -I../include -fsanitize=address -g -flto ../lib/libluz.a
CSHAREDFLAGS := -I../include -L../lib -lluz -fsanitize=address -g -flto
RUNSHAREDFLAGS := LD_LIBRARY_PATH=../lib

CFILES = $(wildcard *.test.c)
STATIC_TARGETS = $(patsubst %.test.c,%.static,$(CFILES))
SHARED_TARGETS = $(patsubst %.test.c,%.shared,$(CFILES))

.PHONY: all
all: $(STATIC_TARGETS) $(SHARED_TARGETS)

.PHONY: clean
clean:
	rm -rf $(STATIC_TARGETS) $(SHARED_TARGETS)

%.static: %.test.c ../.deps/%.d
	$(CC) $< $(CSTATICFLAGS) -o $@
	./$@

%.shared: %.test.c ../.deps/%.d
	$(CC) $< $(CSHAREDFLAGS) -o $@
	$(RUNSHAREDFLAGS) ./$@


include $(wildcard ../.deps/*.d)
