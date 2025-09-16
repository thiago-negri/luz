LIBNAME := luz

SRCDIR := src
SRCGENDIR := src-gen
INCLUDEDIR := include

OBJDIR := .objs
DEPDIR := .deps
GENDIR := include/$(LIBNAME)/gen
LIBDIR := lib

SHAREDLIB := lib$(LIBNAME).so
STATICLIB := lib$(LIBNAME).a

INSTALLDIR := /usr/local

CFLAGS := $(shell cat compile_flags.txt)
CFLAGS += -flto
LDFLAGS += -flto

SRC := $(wildcard $(SRCDIR)/*.c)
SRCGEN := $(wildcard $(SRCGENDIR)/*.c)
DEP := $(patsubst $(SRCDIR)/%.c, $(DEPDIR)/%.d, $(SRC))
DEPPIC := $(patsubst $(SRCDIR)/%.c, $(DEPDIR)/%.pic.d, $(SRC))
GEN := $(patsubst $(SRCGENDIR)/%.c, $(GENDIR)/%.h, $(SRCGEN))
OBJ := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
OBJPIC := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.pic.o, $(SRC))

.PHONY: default clean codegen deps build install test

default: codegen deps build test

test:
	$(MAKE) -C $@

clean:
	rm -rf $(OBJDIR)
	rm -rf $(DEPDIR)
	rm -rf $(GENDIR)
	rm -rf $(LIBDIR)
	$(MAKE) -C test clean

codegen: $(GEN)

deps: codegen $(DEP) $(DEPPIC)

build: $(LIBDIR)/$(SHAREDLIB) $(LIBDIR)/$(STATICLIB)

install: $(LIBDIR)/$(SHAREDLIB) $(LIBDIR)/$(STATICLIB)
	cp $(LIBDIR)/$(SHAREDLIB) $(INSTALLDIR)/$(LIBDIR)/$(SHAREDLIB)
	cp $(LIBDIR)/$(STATICLIB) $(INSTALLDIR)/$(LIBDIR)/$(STATICLIB)
	cp -r $(INCLUDEDIR)/$(LIBNAME)/ $(INSTALLDIR)/$(INCLUDEDIR)/

uninstall:
	rm -f $(INSTALLDIR)/$(LIBDIR)/$(SHAREDLIB)
	rm -f $(INSTALLDIR)/$(LIBDIR)/$(STATICLIB)
	rm -rf $(INSTALLDIR)/$(INCLUDEDIR)/$(LIBNAME)/

$(DEPDIR) $(OBJDIR) $(GENDIR) $(LIBDIR):
	mkdir -p $@

$(DEPDIR)/%.d: $(SRCDIR)/%.c | $(DEPDIR)
	$(CC) -I$(INCLUDEDIR)/ -MT '$(OBJDIR)/$(@F:.d=.o)' -M -o $@ $<

$(DEPDIR)/%.pic.d: $(SRCDIR)/%.c | $(DEPDIR)
	$(CC) -I$(INCLUDEDIR)/ -MT '$(OBJDIR)/$(@F:.d=.o)' -M -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPDIR)/%.d | $(OBJDIR)
	$(CC) $(CFLAGS) -c -O -o $@ $<

$(OBJDIR)/%.pic.o: $(SRCDIR)/%.c $(DEPDIR)/%.d | $(OBJDIR)
	$(CC) $(CFLAGS) -fPIC -c -O -o $@ $<

$(GENDIR)/%.h: $(SRCGENDIR)/%.c | $(GENDIR)
	$(CC) $(CFLAGS) -o $(@D)/$(@F:.h=.o) $<
	$(@D)/$(@F:.h=.o) > $@
	rm $(@D)/$(@F:.h=.o)

$(LIBDIR)/$(SHAREDLIB): $(OBJPIC) | $(LIBDIR)
	$(CC) $(LDFLAGS) -shared $^ -o $@

$(LIBDIR)/$(STATICLIB): $(OBJ) | $(LIBDIR)
	ar rcs $@ $^

include $(wildcard $(DEP))
include $(wildcard $(DEPPIC))
