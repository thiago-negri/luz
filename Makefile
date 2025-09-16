LIBNAME := luz

SRCDIR := src
SRCGENDIR := src-gen
INCLUDEDIR := include

OBJDIR := .objs
DEPDIR := .deps
HEADERDIR := $(INCLUDEDIR)/$(LIBNAME)
GENDIR := $(HEADERDIR)/gen
LIBDIR := lib

SHAREDLIB := lib$(LIBNAME).so
STATICLIB := lib$(LIBNAME).a

INSTALLDIR := /usr/local

CFLAGS := $(shell cat compile_flags.txt)
CFLAGS += -flto
LDFLAGS += -flto

HEADER := $(wildcard $(HEADERDIR)/*.h)
SRC := $(wildcard $(SRCDIR)/*.c)
SRCGEN := $(wildcard $(SRCGENDIR)/*.c)
DEP := $(patsubst $(SRCDIR)/%.c, $(DEPDIR)/%.d, $(SRC))
DEPPIC := $(patsubst $(SRCDIR)/%.c, $(DEPDIR)/%.pic.d, $(SRC))
GEN := $(patsubst $(SRCGENDIR)/%.c, $(GENDIR)/%.h, $(SRCGEN))
OBJ := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
OBJPIC := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.pic.o, $(SRC))
LIB := $(LIBDIR)/$(SHAREDLIB) $(LIBDIR)/$(STATICLIB)

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

build: $(LIB)

install: $(LIB) $(INSTALLDIR)/$(LIBDIR) $(INSTALLDIR)/$(HEADERDIR) $(INSTALLDIR)/$(GENDIR)
	cp $(LIB) $(INSTALLDIR)/$(LIBDIR)/
	cp $(HEADER) $(INSTALLDIR)/$(HEADERDIR)/
	cp $(GEN) $(INSTALLDIR)/$(GENDIR)/

uninstall:
	rm -f $(INSTALLDIR)/$(LIBDIR)/$(SHAREDLIB)
	rm -f $(INSTALLDIR)/$(LIBDIR)/$(STATICLIB)
	rm -rf $(INSTALLDIR)/$(INCLUDEDIR)/$(LIBNAME)/

$(DEPDIR) $(OBJDIR) $(GENDIR) $(LIBDIR) $(INSTALLDIR)/$(LIBDIR) $(INSTALLDIR)/$(HEADERDIR) $(INSTALLDIR)/$(GENDIR):
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
