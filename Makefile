CC=g++
V8_DIR=/Users/jesse/src/v8/include
V8_DEPS=/Users/jesse/src/v8/out/x64.release/libv8_base.x64.a /Users/jesse/src/v8/out/x64.release/libv8_snapshot.a
CFLAGS=-lpthread

all: jsh

debug: CC += -DDEBUG -g
debug: jsh

jsh: test.cc
	${CC} ${CFLAGS} -I${V8_DIR} test.cc -o jsh ${V8_DEPS} 

clean:
	rm jsh

