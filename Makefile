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

SRC := $(wildcard $(SRCDIR)/*.c)
SRCGEN := $(wildcard $(SRCGENDIR)/*.c)
DEP := $(patsubst $(SRCDIR)/%.c, $(DEPDIR)/%.d, $(SRC))
GEN := $(patsubst $(SRCGENDIR)/%.c, $(GENDIR)/%.h, $(SRCGEN))
OBJ := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
OBJPIC := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.pic.o, $(SRC))

.PHONY: default clean codegen deps build install

default: codegen deps build

clean:
	rm -rf $(OBJDIR)
	rm -rf $(DEPDIR)
	rm -rf $(GENDIR)
	rm -rf $(LIBDIR)

codegen: $(GEN)

deps: $(DEP)

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
	$(CC) -I$(GENDIR)/ -MT $@ -MM -MP -MF $@ $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPDIR)/%.d | $(OBJDIR)
	$(CC) $(CFLAGS) -c -O -o $@ $<

$(OBJDIR)/%.pic.o: $(SRCDIR)/%.c $(DEPDIR)/%.d | $(OBJDIR)
	$(CC) $(CFLAGS) -fPIC -c -O -o $@ $<

$(GENDIR)/%.h: $(SRCGENDIR)/%.c | $(GENDIR)
	$(CC) $(CFLAGS) -o $(@D)/$(@F:.h=.o) $<
	$(@D)/$(@F:.h=.o) > $@
	rm $(@D)/$(@F:.h=.o)

$(LIBDIR)/$(SHAREDLIB): $(OBJPIC) | $(LIBDIR)
	$(CC) -shared $^ -o $@

$(LIBDIR)/$(STATICLIB): $(OBJ) | $(LIBDIR)
	ar rcs $@ $^

include $(wildcard $(DEP))
