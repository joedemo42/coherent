# Simple makefile for 'mkdev' under Coherent. This doesn't change very often,
# so keeping the .o's around doesn't seem like a win.

TMPDIR = .

SOURCES = assign.c buildobj.c ehand.c input.c lex.c mdev.c mkconf.c mkdev.c \
		mklink.c mktune.c mtune.c read.c sdev.c stune.c symbol.c

OBJS = $(TMPDIR)/assign.o $(TMPDIR)/buildobj.o $(TMPDIR)/ehand.o \
	$(TMPDIR)/input.o $(TMPDIR)/lex.o $(TMPDIR)/mdev.o $(TMPDIR)/mkconf.o \
	$(TMPDIR)/mkdev.o $(TMPDIR)/mklink.o $(TMPDIR)/mktune.o \
	$(TMPDIR)/mtune.o $(TMPDIR)/read.o $(TMPDIR)/sdev.o $(TMPDIR)/stune.o \
	$(TMPDIR)/symbol.o

mkdev: $(SOURCES)
	cc -o mkdev $(SOURCED)
	rm -f drvbld.mak

clean:
	rm -f $(OBJS)

$(TMPDIR)/assign.o : assign.c
	$(CC) $(CFLAGS) -o $(TMPDIR)/assign.o -c assign.c

$(TMPDIR)/buildobj.o : buildobj.c
	$(CC) $(CFLAGS) -o $(TMPDIR)/buildobj.o -c buildobj.c

$(TMPDIR)/ehand.o : ehand.c
	$(CC) $(CFLAGS) -o $(TMPDIR)/ehand.o -c ehand.c

$(TMPDIR)/input.o : input.c
	$(CC) $(CFLAGS) -o $(TMPDIR)/input.o -c input.c

$(TMPDIR)/lex.o : lex.c
	$(CC) $(CFLAGS) -o $(TMPDIR)/lex.o -c lex.c

$(TMPDIR)/mdev.o : mdev.c
	$(CC) $(CFLAGS) -o $(TMPDIR)/mdev.o -c mdev.c

$(TMPDIR)/mkconf.o : mkconf.c
	$(CC) $(CFLAGS) -o $(TMPDIR)/mkconf.o -c mkconf.c

$(TMPDIR)/mkdev.o : mkdev.c
	$(CC) $(CFLAGS) -o $(TMPDIR)/mkdev.o -c mkdev.c

$(TMPDIR)/mklink.o : mklink.c
	$(CC) $(CFLAGS) -o $(TMPDIR)/mklink.o -c mklink.c

$(TMPDIR)/mktune.o : mktune.c
	$(CC) $(CFLAGS) -o $(TMPDIR)/mktune.o -c mktune.c

$(TMPDIR)/mtune.o : mtune.c
	$(CC) $(CFLAGS) -o $(TMPDIR)/mtune.o -c mtune.c

$(TMPDIR)/read.o : read.c
	$(CC) $(CFLAGS) -o $(TMPDIR)/read.o -c read.c

$(TMPDIR)/sdev.o : sdev.c
	$(CC) $(CFLAGS) -o $(TMPDIR)/sdev.o -c sdev.c

$(TMPDIR)/stune.o : stune.c
	$(CC) $(CFLAGS) -o $(TMPDIR)/stune.o -c stune.c

$(TMPDIR)/symbol.o : symbol.c
	$(CC) $(CFLAGS) -o $(TMPDIR)/symbol.o -c symbol.c

