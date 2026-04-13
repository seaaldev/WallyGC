CC = gcc
LIB_CFLAGS = -Wall -Wextra -Werror -pedantic -c
LINKS = -lwgc -L./lib/

lib : wvm.o warena.o wmalloc.o  
	ar rcs ./lib/libwgc.a ./build/wvm.o ./build/warena.o ./build/wmalloc.o

warena.o : folders ./src/warena.c ./src/wvm.h
	${CC} ${LIB_CFLAGS} ./src/warena.c -o ./build/warena.o
	
wvm.o : folders ./src/wvm.c
	${CC} ${LIB_CFLAGS} ./src/wvm.c -o ./build/wvm.o

wmalloc.o : folders ./src/wmalloc.c ./src/warena.h
	${CC} ${LIB_CFLAGS} ./src/wmalloc.c -o ./build/wmalloc.o

# ----- TEST SECTION ------

testrunners = test-warena-api.o

tests: ${testrunners}
	${CC} -Wall -Werror -Wextra -pedantic ./build/tests/test-warena-api.o \
		-o ./build/tests/testrun 

test-warena-api.o : folders wvm.o warena.o
	${CC} ${LIB_CFLAGS} ./tests/test-warena-api.c ./build/wvm.o ./build/warena.o -o ./build/tests/test-warena-api.o
	./build/tests/test-warena-api.o

folders : 
	mkdir -p ./build ./build/tests ./lib

clean:
	rm -rf ./build ./lib


