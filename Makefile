PREFIX ?= /usr/local
LIBDIR ?= /lib
INCLDIR ?= /include

CFLAGS = -I include
CFLAGS += -Wunused-function -Wunused-variable -Wno-prototype
CFLAGS += -Wconversion -Wsign-compare -Werror

ifeq "$(DEBUG)" "1"
CFLAGS += -Og -g
else
CFLAGS += -O2
endif

ifeq "$(ASSERT_ARGS)" "1"
CFLAGS += -DLIBLIST_ASSERT_ARGS=1
endif

all: build/liblist.so build/liblist.a build/test

clean:
	rm -rf build

build:
	mkdir build

build/liblist.a: src/list.c include/list.h liblist.api | build
	$(CC) -o build/tmp.o src/list.c $(CFLAGS) -r
	objcopy --keep-global-symbols=liblist.api build/tmp.o build/fixed.o
	ar rcs $@ build/fixed.o

build/liblist.so: src/list.c include/list.h liblist.lds | build
	$(CC) -o $@ src/list.c $(CFLAGS) -shared -Wl,-version-script liblist.lds

build/test: src/test.c build/liblist.a | build
	$(CC) -o $@ $^ -I include

install:
	install -m644 include/list.h -t "$(DESTDIR)$(PREFIX)$(INCLDIR)"
	install -m755 build/liblist.a -t "$(DESTDIR)$(PREFIX)$(LIBDIR)"
	install -m755 build/liblist.so -t "$(DESTDIR)$(PREFIX)$(LIBDIR)"

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)$(INCLDIR)/list.h"
	rm -f "$(DESTDIR)$(PREFIX)$(LIBDIR)/liblist.a"
	rm -f "$(DESTDIR)$(PREFIX)$(LIBDIR)/liblist.so"

.PHONY: all clean install uninstall
