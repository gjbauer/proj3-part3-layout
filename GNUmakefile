CFLAGS = -g
RM_FILES = cache_test my.img

UNAME_S := $(shell uname -s)

# Linux-specific flags
ifeq ($(UNAME_S), Linux)
    CFLAGS += -lbsd
endif

# macOS-specific flags
ifeq ($(UNAME_S), Darwin)
    RM_FILES += cache_test.dSYM
endif

all:
	clang $(CFLAGS) -o cache_test *.c
	dd if=/dev/zero of=my.img bs=1M count=2

sanitize:
	clang $(CFLAGS) -fsanitize=address -O0 -o cache_test *.c
	dd if=/dev/zero of=my.img bs=1M count=2


clean:
	rm -rf $(RM_FILES)

open:
	gedit *.h *.c

.PHONY: clean open
