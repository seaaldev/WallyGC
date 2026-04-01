CC = gcc
LIB_CFLAGS = -Wall -Wextra -Werror -pedantic -c
LIBFILES = ./src/wmalloc.c
LIBOBJFILES = ./build/wmalloc.o
LIBOUT = ./lib/libwgc.a

TEST_CFLAGS = -Wall -Wextra -Werror -pedantic
TESTFILES = ./tests/test-libary-linkage.c
TESTOUT = ./build/test
LINKS = -lwgc -L./lib/

all: lib tests

lib: $(LIBOBJFILES)
	ar rcs $(LIBOUT) $(LIBOBJFILES)

$(LIBOBJFILES): $(LIBFILES)
	mkdir -p ./build ./lib
	$(CC) $(LIB_CFLAGS) $(LIBFILES) -o $(LIBOBJFILES)

tests: lib
	$(CC) $(TEST_CFLAGS) $(TESTFILES) -o $(TESTOUT) $(LINKS)

clean:
	rm -rf ./build ./lib
