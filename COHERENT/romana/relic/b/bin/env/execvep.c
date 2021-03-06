/*
 * Version of execv that mimics the
 * actions of the shell in using search
 * rules and running a shell file.
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define	NFNAME	100		/* Largest filename */

static	char	srch[] = ":/bin";
static	char	shell[] = "/bin/sh";

extern	int	errno;

execvep(name, argp, envp)
char *name;
char *argp[];
char *envp[];
{
	char *getenv();
	register char *p1, *p2;
	register char *sp;
	char fname[NFNAME];

	if ((sp = getenv("PATH")) == NULL)
		sp = srch;
	for (;;) {
		p2 = fname;
		while (*sp!='\0' && *sp!=':')
			*p2++ = *sp++;
		p1 = name;
		if (p2 != fname)
			*p2++ = '/';
		while (*p1 != '\0')
			*p2++ = *p1++;
		*p2 = '\0';
		if (strchr(name, '/') != NULL) {
			execve(name, argp, envp);
			if (errno != ENOEXEC)
				break;
		} else
			execve(fname, argp, envp);
		if (errno == ENOEXEC) {
			register char *sv1, *sv2;

			sv1 = argp[-1];
			sv2 = argp[0];
			argp[-1] = shell;
			argp[0] = fname;
			execve(shell, argp-1, envp);
			argp[-1] = sv1;
			argp[0] = sv2;
			break;
		}
		if (*sp == '\0')
			break;
		if (*sp == ':')
			sp++;
	}
	return (-1);
}
