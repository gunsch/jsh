# Makefile for jsh
#
# Current assumptions to remove:
# - D8 output directory (native)
# - Probably all of the dependence on D8 in general
# - D8 has been compiled with output files

CC=g++
CFLAGS=-g
INCLUDES=-I/home/jesse/src/v8/include -I/home/jesse/src/v8/src
SHELL=/bin/bash
LINKS=-lreadline -lrt
D8OUTDIR=/home/jesse/src/v8/out/native/obj.target/d8/src

all: jsh

jsh: environment.o main.o
	$(CC) $(CFLAGS) $(INCLUDES) \
	    main.o environment.o \
	    $(D8OUTDIR)/d8-posix.o \
	    $(D8OUTDIR)/d8-readline.o \
	    -o jsh -Wl,--start-group /home/jesse/src/v8/out/native/obj.target/{tools/gyp/libv8_{base.x64,snapshot},third_party/icu/libicu{uc,i18n,data}}.a -Wl,--end-group $(LINKS)

environment.o: environment.cc
	$(CC) $(CFLAGS) $(INCLUDES) -c environment.cc

main.o: main.cc
	$(CC) $(CFLAGS) $(INCLUDES) -c main.cc

clean:
	rm -rf *.o jsh
