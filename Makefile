CFLAGS = -I src -I include
LDLIBS =
DEPFLAGS = -MT $@ -MMD -MP -MF build/$*.d

_SRCS = list.c
SRCS = $(_SRCS:%=src/%)
OBJS = $(_SRCS:%.c=build/%.o)
DEPS = $(_SRCS:%.c=build/%.d)
PI_OBJS = $(_SRCS:%.c=build/%.pi.o)

.PHONY: all clean

all: build/liblist.so build/liblist.a build/test

clean:
	rm -rf build

build:
	mkdir build

build/%.o: src/%.c build/%.d | build
	$(CC) -c -o $@ $< $(DEPFLAGS) $(CFLAGS)

build/%.pi.o: src/%.c build/%.d | build
	$(CC) -c -o $@ $< $(DEPFLAGS) $(CFLAGS) -fPIC

build/%.d: | build;

include $(DEPS)

build/liblist.a: $(OBJS) | build
	$(CC) -o build/tmp.o $(OBJS) $(CFLAGS) -r
	objcopy --keep-global-symbols=liblist.api build/tmp.o build/fixed.o
	ar rcs $@ build/fixed.o

build/liblist.so: $(PI_OBJS) | build
	$(CC) -o $@ $(PI_OBJS) $(CFLAGS) -shared -Wl,-version-script liblist.lds

build/test: src/test.c build/liblist.a | build
	$(CC) -o $@ $^ -I include

