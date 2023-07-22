# A simplistic Makefile used to illustrate the very basics of make(1).
# The following Makefiles build on this and show more realistic usage.
#
# Run 'make', then 'touch ls.h' and 'make' again.

CC=gcc
CFLAGS = -Wall -Werror -Wextra
all: command

showvars:
	@echo "CC=${CC}"
	@echo "CFLAGS=${CFLAGS}"

command: main.o command.o 	
	${CC} command.o main.o -o command

command.o:
	${CC} -c command.c

main.o: command.h
	${CC} -c main.c

clean:
	rm -f command *.o
