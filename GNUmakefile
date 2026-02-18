CFLAGS = -g

UNAME_S := $(shell uname -s)

# Linux-specific flags
ifneq ($(UNAME_S), Darwin)
    CFLAGS += -lbsd
endif

all:
	clang $(CFLAGS) -o cache_test *.c
	dd if=/dev/zero of=my.img bs=1M count=2

sanitize:
	clang $(CFLAGS) -fsanitize=address -O0 -o cache_test *.c
	dd if=/dev/zero of=my.img bs=1M count=2


clean:
	rm cache_test my.img

open:
	gedit *.h *.c

.PHONY: clean open
