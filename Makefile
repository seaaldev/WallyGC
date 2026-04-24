CC := gcc

RELEASE_FLAGS = -std=gnu11 -O2 -Wall -Wextra -Werror
DEBUG_FLAGS   = -std=gnu11 -O0 -g3 -Wall -Wextra -Werror -Wshadow -Wstrict-prototypes -fsanitize=address,undefined

ifeq ($(DEBUG_FLAGS), 0)
	CFLAGS := ${RELEASE_FLAGS}
else
	CFLAGS := ${DEBUG_FLAGS}
endif

LINKS := -lwgc -L./lib/

.PHONY : lib

lib : wvm.o warena.o wmalloc.o  
	ar rcs ./lib/libwgc.a ./build/wvm.o ./build/warena.o ./build/wmalloc.o

warena.o : folders ./src/warena.c ./src/wvm.h ./src/wlist.h ./src/wcontainer_of.h
	${CC} ${CFLAGS} -c ./src/warena.c -o ./build/warena.o
	
wvm.o : folders ./src/wvm.c
	${CC} ${CFLAGS} -c ./src/wvm.c -o ./build/wvm.o

wmalloc.o : folders ./src/wmalloc.c ./src/warena.h
	${CC} ${CFLAGS} -c ./src/wmalloc.c -o ./build/wmalloc.o

# ----- TEST SECTION ------

.tests: test-warena-api test-wlist 

test-warena-api : folders ./tests/test-warena-api.c ./src/wvm.c ./src/warena.c
	${CC} ${CFLAGS} ./src/warena.c ./src/wvm.c ./tests/test-warena-api.c -o ./build/tests/test-warena-api

test-wlist : folders ./tests/test-wlist.c
	${CC} ${CFLAGS} ./tests/test-wlist.c -o ./build/tests/test-wlist

folders : 
	mkdir -p ./build ./build/tests ./lib

clean:
	rm -rf ./build ./lib


