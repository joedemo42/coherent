# Version number for var.c.
VERSION=V4.2.2

CFLAGS= -O2 -DDIRECTORY_STACK=1
CC=gcc
LDFLAGS=-nostdlib /lib/crts0.o
LIBFLAGS=/lib/libc.a

OBJ=	alloc.o eval.o exec1.o exec2.o exec3.o extern.o \
	glob.o lex.o main.o tab.o trap.o var.o parse.o buildobj.o io.o

SRCS=	sh.h parse.y alloc.c eval.c exec1.c exec2.c exec3.c \
	extern.c glob.c lex.c main.c tab.c trap.c var.c buildobj.c io.c

# Primary targets.

all: sh printf

sh: $(OBJ)
	$(CC) -o sh $(LDFLAGS) $(OBJ) $(LIBFLAGS)

# Secondary targets.

alloc.o:	sh.h alloc.c
eval.o:		sh.h eval.c
exec1.o:	sh.h exec1.c
exec2.o:	sh.h exec2.c
exec3.o:	sh.h exec3.c
extern.o:	sh.h extern.c
glob.o:		sh.h glob.c
lex.o:		sh.h lex.c parse.o
main.o:		sh.h main.c
tab.o:		sh.h tab.c
trap.o:		sh.h trap.c
var.o:		var.c sh.h
	$(CC) -c $(CFLAGS) -DVERSION=\"$(VERSION)\" $*.c
parse.o:	parse.y sh.h
io.o:		sh.h io.c

clean:
	rm -f $(OBJ) sh printf y.tab.h
