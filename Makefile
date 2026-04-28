CC?=cc
CFLAGS?=-std=c89 -Wall -O2 -Isrc
LDFLAGS?=-lm
all: main test_core
main: main.c src/rune_core.c src/rune_core.h src/undying_allocator.c src/undying_allocator.h
	$(CC) $(CFLAGS) -o main main.c src/rune_core.c src/undying_allocator.c $(LDFLAGS)
test_core: tests/test_core.c src/rune_core.c src/rune_core.h src/undying_allocator.c src/undying_allocator.h
	$(CC) $(CFLAGS) -o test_core tests/test_core.c src/rune_core.c src/undying_allocator.c $(LDFLAGS)
check: all
	./test_core
clean:
	rm -f main test_core
