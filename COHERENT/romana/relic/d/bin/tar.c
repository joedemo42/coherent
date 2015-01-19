/*
 * Tape archive
 * tar [0-7bcflmrtuvwx]+ [blocks] [archive] pathname*
 */

#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <canon.h>
#include <sys/stat.h>
#include <sys/dir.h>

#define	S_PERM	07777		/* should be in stat.h */

/* Possible actions for do_checksum_error().  */
#define CLEAR 0	/* Print all pending messages.  */
#define ADD 1	/* Add another pending message.  */

#define	MAXBLK	20
#define	roundup(n, r)	(((n)+(r)-1)/(r))

typedef	struct	dirhd_t	{
	dev_t	t_dev;
	ino_t	t_ino;
	unsigned short	t_nlink,
			t_mode,
			t_uid,
			t_gid;
	fsize_t	t_size;
	time_t	t_mtime;
	char	*t_name;
	struct	dirhd_t	*t_cont[];
} dirhd_t;

typedef	union	tarhd_t	{
	struct {
		char	th_name[100],
			th_mode[8],
			th_uid[8],
			th_gid[8],
			th_size[12],
			th_mtime[12],
			th_check[8],
			th_islink,
			th_link[100],
			th_pad[255];
	} th_info;
	char	th_data[BUFSIZ];
} tarhd_t;

typedef	unsigned short	flag_t;		/* fastest type for machine */

flag_t	linkmsg = 0,			/* message if not all links found */
	modtime = 1,			/* restore modtimes */
	verbose = 0;
	unixbug = 0;			/* avoid bug in U**X tar */
FILE	*whether = (FILE *)NULL,	/* ask about each file */
	*tarfile;
char	tapedev[10] = '\0';
char	*archive = &tapedev[0];
unsigned short	blocking = 1;		/* blocking factor */
time_t	oldtime[2],			/* for utime */
	recently,			/* set to 6 months ago for tv key */
	time();

dirhd_t	*newdirhd(),
	*update(),
	*research();
tarhd_t	*readhdr(),
	*readblk(),
	*writeblk();
char	*havelink();
long	getoctl();
extern char	*malloc();
extern char	*ctime();

main(argc, argv)
int	argc;
char	*argv[];
{
	char	*key,
		unit = '\0',
		function = 0,
		deffunc,
		prefix[101] = '\0';
	unsigned short	arg = 2;
	dirhd_t	*args;

	if (argc < 2) {
		fprintf(stderr,
	"Usage: %s [crtux][0-7bflmvw] [blocks] [archive] pathname*\n",
			argv[0]);
		exit(-1);
	}
	for (key = argv[1];  *key != '\0';  key++) switch (*key) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
		unit = *key;
		deffunc = 't';
		continue;
	case 'b':
		if (argc <= arg)
			fatal("missing blocking factor");
		else if ((blocking = atoi(argv[arg++])) <= 0
			 || blocking > MAXBLK)
			fatal("illegal blocking factor");
		deffunc = 'r';
		continue;
	case 'f':
		if (argc <= arg)
			fatal("missing archive name");
		else
			archive = argv[arg++];
		deffunc = 't';
		continue;
	case 'l':
		linkmsg = 1;
		deffunc = 'r';
		continue;
	case 'm':
		modtime = 0;
		deffunc = 'x';
		continue;
	case 'v':
		verbose = 1;
		deffunc = 't';
		continue;
	case 'U':
		unixbug = 1;
		deffunc = 't';
		continue;
	case 'w':
		whether = fopen("/dev/tty", "r+w");
		deffunc = 'x';
		continue;
	case 'c':
	case 'r':
	case 't':
	case 'u':
	case 'x':
		if (function)
			fatal("keys c, r, t, u, x are mutually exclusive");
		else
			function = *key;
		continue;
	default:
		fatal("illegal key '%c'", *key);
	}
	if (!function)
		function = deffunc;
	oldtime[0] = time((time_t *)0);
	if (verbose && function=='t')
		recently = oldtime[0] - 6*((time_t)30*24*60*60);

	/* construct descriptors for args given */

	args = (dirhd_t *) malloc(sizeof (dirhd_t)
				 + (argc-arg)*sizeof (dirhd_t *));
	if (args == NULL)
		fatal("out of memory");
	args->t_mode = S_IFDIR;
	args->t_nlink = 0;
	for (;  arg < argc;  arg++) {
		dirhd_t	*argp;

		if ((argp = newdirhd(argv[arg], strlen(argv[arg]))) == NULL)
			fprintf(stderr, "Tar: %s: out of memory\n", argv[arg]);
		else
			args->t_cont[args->t_nlink++] = argp;
	}

	/* open archive file */

	if (*archive == '\0')
		sprintf(archive, "/dev/%smt%c", blocking==1 ? "" : "r", unit);
	if (function=='t' || function=='x')
		if (strcmp(archive, "-")==0)
			tarfile = stdin;
		else
			tarfile = fopen(archive, "r");
	else if (function=='c')
		if (strcmp(archive, "-")==0)
			tarfile = stdout;
		else
			tarfile = fopen(archive, "w");
	else
		tarfile = fopen(archive, "r+w");
	if (tarfile==NULL) {
		exit(perror("Tar: 1: %s", archive));
	}
	setbuf(tarfile, NULL);

	/* perform required function */

	switch (function) {
	case 't':
		table(args);
		break;
	case 'x':
		extract(args);
		break;
	case 'r':
		while (readblk() != NULL)
			;
		goto doappend;
	case 'u':
		if ((args = update(prefix, args, readhdr())) == NULL)
			break;
	case 'c':
	doappend:
		append(prefix, args);
		flushtar();
	}
	/* Flush any remaining checksum messages.  */
	do_checksum_error(NULL, CLEAR);

	exit(errno);
}

fatal(args)
char	*args;
{
	/* Flush any remaining checksum messages.  */
	do_checksum_error(NULL, CLEAR);

	fprintf(stderr, "tar: %r\n", &args);
	exit(-1);
}

dirhd_t *
newdirhd(name, len)
register char	*name;
unsigned short	len;
{
	register dirhd_t *dp;

	if ((dp = (dirhd_t *) malloc(sizeof (dirhd_t))) != NULL
	 && (dp->t_name = malloc(len+1)) == NULL) {
		free(dp);
		dp = NULL;
	}
	if (dp != NULL) {
		dp->t_nlink = 0;
		strncpy(dp->t_name, name, len);
		dp->t_name[len] = '\0';
	}
	return (dp);
}
int
perror(args)
char	*args;
{
	register int	err;

	if ((err=errno) >= sys_nerr)
		fprintf(stderr, "%r: Bad error number\n", &args);
	else if (err)
		fprintf(stderr, "%r: %s\n", &args, sys_errlist[err]);
	return (err);
}

table(args)
register dirhd_t *args;
{
	register tarhd_t *header;

	for (;  (header = readhdr()) != NULL;  skipfile(header)) {
		if (argcont(args, header->th_info.th_name, 0) < 0)
			continue;
		if (verbose) {
			register unsigned short	mode = getoct(header->th_info.th_mode);
			unsigned short	uid = getoct(header->th_info.th_uid),
					gid = getoct(header->th_info.th_gid);
			fsize_t	size = getoctl(header->th_info.th_size);
			time_t	mtime = getoctl(header->th_info.th_mtime);
			char	*timestr = ctime(&mtime);
	
			if (header->th_info.th_name[strnlen(header->th_info.th_name, 100)-1]
					== '/')
				putchar('d');
			else
				putchar('-');
			printf("%c%c%c",
				mode&S_IREAD  ? 'r' : '-',
				mode&S_IWRITE ? 'w' : '-',
				mode&S_ISUID  ? 's' :
				mode&S_IEXEC  ? 'x' : '-');
			printf("%c%c%c",
				mode&S_IREAD>>3  ? 'r' : '-',
				mode&S_IWRITE>>3 ? 'w' : '-',
				mode&S_ISGID     ? 's' :
				mode&S_IEXEC>>3  ? 'x' : '-');
			printf("%c%c%c",
				mode&S_IREAD>>6  ? 'r' : '-',
				mode&S_IWRITE>>6 ? 'w' : '-',
				mode&S_ISVTX     ? 't' :
				mode&S_IEXEC>>6  ? 'x' : '-');
			printf("%3d %3d %6ld %.11s%.5s ",
				gid,
				uid,
				size,
				timestr,
				mtime > recently ? timestr+11 : timestr+19);
		}
		printf("%.100s", header->th_info.th_name);
		if (header->th_info.th_islink)
			if (verbose)
				printf("\n%33s link to %s\n", "",
					header->th_info.th_link);
			else
				printf(" link to %s\n", header->th_info.th_link);
		else
			putchar('\n');
	}
}

extract(args)
register dirhd_t *args;
{
	register tarhd_t *header;
	short	skipping = 0;

	while ((header = readhdr()) != NULL) {
		register char	name[101];
		unsigned short	namelen = strnlen(header->th_info.th_name, 100),
				mode,
				uid,
				gid;

		if (!skipping || contains(name, header->th_info.th_name) >= 0) {
			strncpy(name, header->th_info.th_name, namelen);
			name[namelen--] = '\0';
			if ((skipping = argcont(args, name, 0)) >= 0)
				skipping = disallow('x', name);
		}
		switch (skipping) {
		case -1:
			skipping = 0;
		case 1:
			skipfile(header);
			continue;
		}
		if (verbose)
			printf("x %s\n", name);
		mode = getoct(header->th_info.th_mode);
		uid = getoct(header->th_info.th_uid);
		gid = getoct(header->th_info.th_gid);
		oldtime[1] = getoctl(header->th_info.th_mtime);
		if (name[namelen] == '/') {
			name[namelen] = '\0';
			makepath(name);
			chmod(name, mode);
			name[namelen] = '/';
		} else if (header->th_info.th_islink == '\0') {
			int	fd = recreate(name, mode);
			fsize_t	size = getoctl(header->th_info.th_size);
	
			for (;  size > 0;  size -= sizeof (tarhd_t)) {
				int	my_size;

				/* Handle unexpected EOF - mods by vlad@kiev.mwc.com */
				if ((header = readblk()) == NULL) {
					fprintf(stderr, 
					 "Unexpected end of the file %s\n",
						name);
					break;
				}
				if (size > sizeof(tarhd_t))
					my_size = sizeof(tarhd_t);
				else
					my_size = (int) size;
				if (fd >= 0 && (write(fd, header->th_data,
						my_size) != my_size))

					perror("Tar: 2: %s", name);
			}
			if (fd >= 0)
				close(fd);
		} else {
			struct	stat	statbuf;
			flag_t	xlink = 1;
	
			if (stat(header->th_info.th_link, &statbuf) < 0)
				close(recreate(header->th_info.th_link, mode));
			else if (havelink(statbuf.st_dev, statbuf.st_ino, 0)
					!= NULL)
				xlink = 0;
			if (xlink)
				fprintf(stderr, "Tar: Must extract %s\n",
					header->th_info.th_link);
			unlink(name);
			mkparent(name);
			if (link(header->th_info.th_link, name) < 0)
				fprintf(stderr, "Tar: Can't link %s to %s\n",
					name, header->th_info.th_link);
		}
		if (modtime)
			utime(name, oldtime);
		chown(name, uid, gid);
	}
}

dirhd_t *
update(name, args, header)
char	*name;
register dirhd_t *args;
tarhd_t	*header;
{
	unsigned short	namelen = strlen(name);
	flag_t	donedir = 0;

	if (header == NULL)
		return (args);
	if (args->t_nlink == 0 && (args = research(name, args)) == NULL)
		return (NULL);
	do switch (args->t_mode&S_IFMT) {
		register short	arg;
		register dirhd_t *argp;

	case S_IFREG:
		if (args->t_mtime <= getoctl(header->th_info.th_mtime))
			args->t_nlink = 0;
		skipfile(header);
		continue;
	case S_IFDIR:
		if (namelen != 0 && !donedir) {
			name[namelen] = '/';
			donedir++;
		}
		if ((arg = argcont(args, header->th_info.th_name+namelen+donedir, -1))
				>= 0) {
			strcpy(name+namelen+donedir,
				(argp=args->t_cont[arg])->t_name);
			if ((args->t_cont[arg] = update(name, argp, header))
					== NULL)
				args->t_cont[arg]
					= args->t_cont[--args->t_nlink];
		} else
			skipfile(header);
		name[namelen+donedir] = '\0';
	} while ((header=readhdr()) != NULL
		&& contains(name, header->th_info.th_name));
	if (header != NULL)
		ungetblk();
	if (args->t_nlink == 0) {
		if (namelen != 0)
			free(args->t_name);
		free((char *) args);
		return (NULL);
	} else
		return (args);
}

append(name, args)
char	*name;
register dirhd_t *args;
{
	unsigned short	namelen;
	register unsigned short	arg;
	register dirhd_t *argp;

	if ((namelen = strlen(name)) != 0 && disallow('a', name))
		return;
	if (args->t_nlink == 0 && (args = research(name, args)) == NULL)
		return;
	switch (args->t_mode&S_IFMT) {
	case S_IFDIR:
		if (namelen != 0) {
			name[namelen++] = '/'; name[namelen] = '\0';
			writehdr(name, args, "");
		}
		for (arg = 0;  arg < args->t_nlink;  arg++) {
			strcpy(name+namelen, (argp=args->t_cont[arg])->t_name);
			append(name, argp);
		}
		break;
	case S_IFREG:
		if (args->t_nlink != 1) {
			char	*link;

			if ((link=havelink(args->t_dev,
					args->t_ino, 1))!=NULL) {
				writehdr(name, args, link);
				break;
			} else {
				filelink(args->t_dev,
					 args->t_ino,
					 args->t_nlink,
					 name);
			}
		}
		writehdr(name, args, "");
		writefil(name, args->t_size);
	}
	if (namelen != 0)
		free(args->t_name);
	free((char *) args);
}

dirhd_t *
research(name, args)
char	*name;
register dirhd_t *args;
{
	unsigned short	namelen;
	register short	nfile;
	int	fd;
	struct	stat	statbuf;

	if ((namelen = strlen(name)) == 0)
		name = ".";
	if (stat(name, &statbuf) < 0) {
		perror("Tar: 3: %s", name);
		return (args);
	}
	args->t_dev = statbuf.st_dev;
	args->t_ino = statbuf.st_ino;
	args->t_nlink = statbuf.st_nlink;
	args->t_uid = statbuf.st_uid;
	args->t_gid = statbuf.st_gid;
	args->t_mtime = statbuf.st_mtime;
	switch ((args->t_mode = statbuf.st_mode)&S_IFMT) {
	case S_IFREG:
		args->t_size = statbuf.st_size;
		return (args);
	default:
		args->t_size = 0;
		return (args);
	case S_IFDIR:
		args->t_size = 0;
	}
	if ((nfile = (short) (statbuf.st_size/sizeof (struct direct))) > 2) {
		args = (dirhd_t *) realloc((char *)args,
			sizeof (dirhd_t) + (nfile-2)*sizeof (dirhd_t *));
		if (args == NULL) {
			fprintf(stderr, "Tar: %s: out of memory\n", name);
			return (NULL);
		}
	}
	args->t_nlink = 0;
	if ((fd = open(name, 0)) < 0) {
		perror("Tar: 4: %s", name);
		return (args);
	}
	for (;  nfile > 0;  --nfile) {
		struct	direct	dir_ent;
		unsigned short	arglen;
		dirhd_t	*argp;

		switch (read(fd, (char *)&dir_ent, sizeof (struct direct))) {
		case -1:
			perror("Tar: 5: %s", name);
		case 0:
			break;
		default:
			if (dir_ent.d_ino == 0
			 || strcmp(dir_ent.d_name, ".") == 0
			 || strcmp(dir_ent.d_name, "..") == 0)
				continue;
			else if ((arglen=strnlen(dir_ent.d_name, DIRSIZ))
				 + namelen > 99)
				fprintf(stderr,
					"Tar: %s/%.*s: name too long\n",
					name, DIRSIZ, dir_ent.d_name);
			else if ((argp = newdirhd(dir_ent.d_name, arglen))
				 == NULL)
				fprintf(stderr,
					"Tar: %s/%.*s: out of memory\n",
					name, DIRSIZ, dir_ent.d_name);
			else
				args->t_cont[args->t_nlink++] = argp;
			continue;
		}
		break;
	}
	close(fd);
	return (args);
}

int
argcont(args, name, ret)
register dirhd_t *args;
char	*name;
int	ret;
{
	register unsigned short	arg;

	if (args->t_nlink == 0)
		return (ret);
	else for (arg = 0;  arg < args->t_nlink;  arg++)
		if (contains(args->t_cont[arg]->t_name, name))
			return (arg);
	return (-1);
}

int
strnlen(str, maxlen)
register char	*str;
register unsigned short	maxlen;
{
	register unsigned short	len = 0;

	while (maxlen && *str++ != '\0') {
		--maxlen; ++len;
	}
	return (len);
}

/*
 * Create file system paths
 */

int
makepath(pathname)
char	*pathname;
{
	struct	stat	statbuf;
	register int	err;

	if (stat(pathname, &statbuf) == 0)
		if ((statbuf.st_mode&S_IFMT) == S_IFDIR)
			return (0);
		else
			errno = ENOTDIR;
	else if (errno == ENOENT) {
		errno = 0;
		if ((err=mkparent(pathname)) != 0)
			return err;
		return mkdir(pathname);
	}
	return (perror("Tar: 6: %s", pathname));
}

int
mkparent(pathname)
register char	*pathname;
{
	register char	*pathend = &pathname[strlen(pathname)];

	while (pathend > pathname && *--pathend != '/')
		;
	if (pathend > pathname) {
		*pathend = '\0';
		errno = makepath(pathname);
		*pathend = '/';
	} else
		errno = 0;
	return (errno);
}

mkdir(pathname)
char	*pathname;
{
	int	status;

	switch(fork()) {
	case -1:
		break;
	case 0:
		close(2);
		execl("/bin/mkdir", "mkdir", pathname, NULL);
		exit(errno);
	default:
		wait(&status);
		errno = status>>8;
	}
	return (perror("Tar: 7: %s", pathname));
}

int
recreate(pathname, mode)
char	*pathname;
unsigned short	mode;
{
	int	fd;

	if ((fd = create(pathname, mode)) < 0
	&& (errno != ENOENT
	  || mkparent(pathname) == 0 && (fd = create(pathname, mode)) < 0))
		perror("Tar: 8: %s", pathname);
	errno = 0;
	return (fd);
}

int
create(pathname, mode)
char	*pathname;
unsigned short	mode;
{
	int	fd;

	unlink(pathname);
	if ((fd = creat(pathname, mode)) >= 0) {
		struct	stat	statbuf;

		fstat(fd, &statbuf);
		filelink(statbuf.st_dev, statbuf.st_ino,
			statbuf.st_nlink, pathname);
	}
	return (fd);
}

/*
 * Ask about current action
 */

int
disallow(function, pathname)
char	function,
	*pathname;
{
	while (whether) {
		register int	c1,
				c;

		fprintf(whether, "%c %s? ", function, pathname);
		for (c1=c=getc(whether);  c!=EOF && c!='\n';  c=getc(whether))
			;
		switch (c1) {
		case EOF:
		case 'x':
			if (function == 'a')
				flushtar();
			exit(errno);
		case '\n':
		case 'n':
		case 'N':
			return (1);
		case 'y':
		case 'Y':
			return (0);
		}
	}
	return (0);
}

/*
 * High level I/O routines
 */

tarhd_t *
readhdr()
{
	register tarhd_t *header;

	while ((header = readblk()) != NULL) {
		if (header->th_info.th_name[0] == '\0')
			if (unixbug) {
				ungetblk();
				return (NULL);
			} else
				continue;
		else {
			int	check = getoct(header->th_info.th_check);
	
			strncpy(header->th_info.th_check, "        ",
				sizeof(header->th_info.th_check));
			if (checksum(header->th_data, sizeof(tarhd_t)) != check){
				do_checksum_error(header->th_info.th_name, ADD);
			} else {
				do_checksum_error(NULL, CLEAR);
				break;
			}
		}
	}
	/* We shouldn't have to check EVERY header, but if we do it here
	 * it gets done for every routine.
	 * Newer tar programs use the first eight bytes of the
	 * pad field as magic to indicate the type of archive.
	 * With old tar archives, these eight characters should be
	 * NUL.  We'll only check the first one.
	 */
	/* Check to see if this is a v7 style tar archive.  */
	if ((char)0 != header->th_info.th_pad[0]) {
		fatal("%.8s archive, not v7 archive.",
			header->th_info.th_pad);
		
	}
	return (header);
}

skipfile(header)
tarhd_t	*header;
{
	register fsize_t	size;

	if (header->th_info.th_islink)
		return;
	for (size = getoctl(header->th_info.th_size);
		size > 0 && readblk() != NULL;
		size -= sizeof (tarhd_t))
		;
}

writehdr(name, args, link)
char	*name;
register dirhd_t *args;
char	*link;
{
	register tarhd_t *header = writeblk();

	if (verbose)
		fprintf(stderr, "a %s", name);
	strncpy(header->th_info.th_name, name, sizeof(header->th_info.th_name));
	putoct(header->th_info.th_mode, args->t_mode&S_PERM);
	putoct(header->th_info.th_uid, args->t_uid);
	putoct(header->th_info.th_gid, args->t_gid);
	putoctl(header->th_info.th_size, args->t_size);
	putoctl(header->th_info.th_mtime, args->t_mtime);
	strncpy(header->th_info.th_check, "        ", sizeof(header->th_info.th_check));
	if (*link == '\0') {
		header->th_info.th_islink = 0;
		if (verbose)
			if (args->t_size == 0)
				putc('\n', stderr);
			else
				fprintf(stderr, " %ld block%s\n",
					roundup(args->t_size, BUFSIZ),
					args->t_size <= BUFSIZ ? "" : "s");
	} else {
		header->th_info.th_islink = '1';
		if (verbose)
			fprintf(stderr, " link to %s\n", link);
	}
	strncpy(header->th_info.th_link, link, sizeof(header->th_info.th_link));
	strncpy(header->th_info.th_pad, "", sizeof(header->th_info.th_pad));
	putoct(header->th_info.th_check, checksum(header->th_data, sizeof(tarhd_t)));
}

writefil(name, size)
char	*name;
register fsize_t	size;
{
	int	fd;
	register tarhd_t *header;

	if ((fd = open(name, 0)) < 0) {
		perror("Tar: 9: %s", name);
	}
	for (;  size > 0;  size -= sizeof (tarhd_t)) {
		header = writeblk();
		if (fd >= 0)
			read(fd, header->th_data, sizeof (tarhd_t));
		else
			strncpy(header->th_data, "", sizeof (tarhd_t));
	}
	if (fd >= 0)
		close(fd);
}

/*
 * Tape I/O, with record blocking if specified
 */

tarhd_t	buffer[MAXBLK],
	*current = &buffer[0];

tarhd_t *
readblk()
{
	static unsigned short	blocks = 0;

	if (feof(tarfile))
		return (NULL);
	if (current == &buffer[blocks]) {
		current = &buffer[0];
		blocks = fread((char *) buffer,
				sizeof (tarhd_t), MAXBLK, tarfile);
		if (ferror(tarfile)) {
			exit(perror("Tar: 10: %s", archive));
		} else if (feof(tarfile)) {
			/* This is a hack.  We should actually find
			 * out why returning NULL is not good enough.
			 *
			 * Really a pain without a symbolic debugger!
			 */
			exit(0);
		}
	}
	return (current++);
}

ungetblk()
{
	--current;
}

tarhd_t *
writeblk()
{
	if (current == &buffer[blocking]) {
		current = &buffer[0];
		fwrite((char *)buffer, sizeof (tarhd_t), blocking, tarfile);
		if (ferror(tarfile) || feof(tarfile)) {
			exit(perror("Tar: 11: %s", archive));
		}
	}
	return (current++);
}

flushtar()
{
	register tarhd_t *header;

	while ((header=writeblk()) != &buffer[0])
		strncpy(header->th_data, "", sizeof (tarhd_t));
	if (linkmsg)
		misslink();
}

/*
 * Keep track of files by ino in hash table
 */

typedef	struct	link_t	{
	dev_t	t_dev;
	ino_t	t_ino;
	unsigned short	t_nlink;
	struct	link_t	*t_next;
	char	t_link[];
} link_t;

#define	NHASH	64

link_t	*linklist[NHASH];

filelink(dev, ino, nlink, link)
dev_t	dev;
ino_t	ino;
unsigned short	nlink;
char	*link;
{
	unsigned short	id = ino%NHASH;
	register link_t	*lp;

	lp = (link_t *) malloc(sizeof (link_t) + strlen(link) + 1);
	if (lp == NULL)
		fprintf(stderr, "Tar: %s: note: link info lost\n", link);
	else {
		lp->t_next = linklist[id];
		linklist[id] = lp;
		lp->t_dev = dev;
		lp->t_ino = ino;
		lp->t_nlink = nlink;
		strcpy(lp->t_link, link);
	}
}

char *
havelink(dev, ino, flag)
dev_t	dev;
ino_t	ino;
flag_t	flag;
{
	register link_t	*lp;

	for (lp = linklist[ino%NHASH];  lp != NULL;  lp = lp->t_next) {
		if (lp->t_ino == ino && lp->t_dev == dev) {
			if (flag)
				--lp->t_nlink;
			return (lp->t_link);
		}
	}
	return (NULL);
}

misslink()
{
	unsigned short	ino;

	for (ino = 0;  ino < NHASH;  ino++) {
		register link_t	*lp;

		for (lp = linklist[ino];  lp != NULL;  lp=lp->t_next) {
			register short	nlink;

			if (nlink = lp->t_nlink - 1)
				fprintf(stderr,
					"Tar: missed %d link%s to %s\n",
					nlink, nlink==1 ? "" : "s",
					lp->t_link);
		}
	}
}

/*
 * Read unsigned octal numbers
 */

int
getoct(cp)
register char	*cp;
{
	register int	val = 0;
	register char	c;

	while (*cp == ' ')
		cp++;
	while ((c = *cp++ - '0') >= 0 && c <= 7)
		val = val<<3 | c;
	return (val);
}

long
getoctl(cp)
register char	*cp;
{
	register long	val = 0;
	register char	c;

	while (*cp==' ')
		cp++;
	while ((c = *cp++ - '0') >= 0 && c <= 7)
		val = val<<3 | c;
	return (val);
}

/*
 * Write unsigned octal numbers
 * in fixed format
 */

putoct(str, val)
char	*str;
unsigned short	val;
{
	register char	*cp;

	*(cp = &str[7]) = '\0';
	*--cp = ' ';
	*--cp = (val&07) + '0';
	while (cp != str)
		*--cp = (val>>=3) ? (val&07) + '0' : ' ';
}

putoctl(str, val)
char	*str;
register unsigned long	val;
{
	register char	*cp;

	*(cp = &str[11]) = ' ';
	*--cp = (val&07) + '0';
	while (cp != str)
		*--cp = (val>>=3) ? (val&07) + '0' : ' ';
}

/*
 * Compute checksum of string
 */
int
checksum(str, len)
register char	*str;
register int	len;
{
	register int	check = 0;

	if (len) do
		check += *str++;
	while (--len);
	return (check);
}

/*
 * Return -1 if dname is a directory prefix of fname
 *	0 if no match
 *	1 if complete match
 */
int
contains(dname, fname)
register char	*dname,
		*fname;
{
	if (*dname=='\0')
		return (-1);
	while (*dname!='\0')
		if (*dname++ != *fname++)
			return (0);
	if (*fname=='\0')
		return (1);
	else if (*fname=='/' || *--fname=='/')
		return (-1);
	else
		return (0);
}

/* Handle the processing of a checksum error.
 * This is special because sometimes end of archive will look like
 * a lot of checksum errors.
 */
int
do_checksum_error(name, action)
	char *name;
	int action;
{
#define MSG_SIZE (100 + sizeof("Tar: %.100s: bad checksum\n") + 1)
#define MAX_CHECKERR	5 /* Give up after 5 consecutive checksum errors.  */

	static error_count = 0;	/* Number of checksums we've seen.  */
	static char *msg_list = NULL; /* Error message we've queued.  */
	char tmp_buf[MSG_SIZE];	/* Someplace to build new errors. */

	
	switch (action) {
	case ADD:
		/* If there have been too many checksum errors, or if
		 * we can't allocate more memory for more error messages,
		 * throw away the existing messages, and look for another
		 * valid block.
		 */
		if (++error_count > MAX_CHECKERR ||
		    (msg_list = (char *)realloc(msg_list,
					strlen(msg_list) + MSG_SIZE +1)) ==
		    NULL) {
			fprintf(stderr,
			    "Tar: %s: This doesn't look like a tar archive.\n",
			    archive);
			free(msg_list);
			msg_list = 0;
			error_count = 0;
			fprintf(stderr, "Tar: Scanning for next file.\n");
			scan_for_next();
		}

		sprintf(tmp_buf, "Tar: %.100s: bad checksum\n",
			name);
		strcat(msg_list, tmp_buf);
		break;
	case CLEAR:
		if (NULL != msg_list) {
			fprintf(stderr, "%s", msg_list);
			free(msg_list);
			msg_list = NULL;
		}
		error_count = 0;
		break;
	} /* switch (action) */
} /* do_checksum_error() */

/* Find the next valid block.  */
int
scan_for_next()
{
	int	check;
	tarhd_t	*header;

	while ( (header = readblk()) != NULL ) {
		check = getoct(header->th_info.th_check);

		strncpy(header->th_info.th_check, "        ",
			sizeof(header->th_info.th_check));
		if (checksum(header->th_data, sizeof(tarhd_t)) == check){
			/* Found a good header!  */
			ungetblk();
			return;
		}
	} /* while (more blocks to read) */
	/* If we got here, it means we ran out of things to read.  */
	fatal("no more valid blocks");

} /* scan_for_next() */
