/*      /\
 *     /  \		(C) Copyright 2009 Parliament Hill Computers Ltd.
 *     \  /		All rights reserved.
 *      \/
 *       .		Author: Alain Williams, <addw@phcomp.co.uk> 2009
 *       .
 *        .		SCCS: @(#)fsync.c	1.3 11/14/11 22:46:01
 *          .
 *
 */

#include "config.h"

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


extern	int	optind;
extern	char*	optarg;

char*	progname;	/* What this program is called - name of binary */
int	exit_c;		/* Exit status to return to O/S */
int	datasync;	/* Use fdatasync */
int	quiet;		/* Do not report errors */
int	verbose;	/* Chatty mode */
char*	flushname;	/* Name of how we flush */
int (*flushfn)(int);	/* Pointer to system call to use */

/* Usage message.
 */
char*	usagem[] = {
	"Ensure that named files are flushed to disk",
	"Usage: %s [options] file ...",
	"-d	do not flush metadata (fdatasync())",
	"-q	do not report errors (eg file open fails), still set exit code",
	"-v	verbose",
	"-x	eXplain",
	"Version: 1.3 11/14/11",
	(char*)NULL	/* Must end the list */
};

/* Functions defined in this module, in alphabetic order */
void	read_opts(int argc, char* argv[]);
void	usage(int exitc);
void	process(const char* file);

int	main(int argc, char* argv[])
{
	read_opts(argc, argv);

	flushname = datasync ? "fdatasync" : "fsync";
	flushfn = datasync ? fdatasync : fsync;

	argv += optind;
	while(*argv)
		process(*argv++);

	return(exit_c);
}

/* Interpret '-' arguments -options.
 * If there is an error in the arguments, print the options list and exit.
 */
void	read_opts(int argc, char* argv[])
{
	register int	c;
	register int	error;

	error = 0;
	progname = argv[0];

	while((c = getopt(argc, argv, "dqvx")) != EOF)
		switch(c) {
		case 'd':	/* Use fdatasync() */
			datasync = 1;
			break;
		case 'q':	/* Quiet */
			quiet = 1;
			break;
		case 'v':	/* Verbose */
			verbose = 1;
			break;
		case 'x':	/* eXplain */
			usage(0);
			/*NOTREACHED*/
		case '?':
			error = 1;
			break;
		}

	if(error)
		usage(2);
}

/* Output a usage message and end with exit status exitc.
 */
void	usage(int exitc)
{
	register char**	mes;
	
	for(mes = usagem; *mes != (char*)NULL; mes++) {
		(void) fprintf(stderr, *mes, progname);
		fputc('\n', stderr);
	}
	
	exit(exitc);
	/*NOTREACHED*/
}

/* Flush the named file to disk -- use the fsync or fdatasync system call
 */
void	process(const char* file)
{
	int	fd;

	if(verbose)
		fprintf(stderr, "%s: about to %s '%s'\n", progname,
			flushname, file);

	/* Need to open read/write for fsync to work.
	 * But we can't open directories to write to, so try to open one read only.
	 * Linux (2.6.18 anyway) doesn't seem to check that the fd to fsync is for
	 * writing - but that is what the manual says it must be.
	 */
	if((fd = open(file, O_WRONLY)) == -1 &&
	   (errno == EISDIR && (fd = open(file, O_RDONLY)) == -1)) {
		if( ! quiet)
			fprintf(stderr, "%s: Cannot open '%s' as: %s\n", progname, file,
				strerror(errno));
		exit_c = 1;
		return;
	}

	if((*flushfn)(fd) == -1) {
		if( ! quiet)
			fprintf(stderr, "%s: Cannot %s '%s' as: %s\n", progname, flushname, file,
				strerror(errno));
		exit_c = 1;
	}

	if(close(fd)) {
		if( ! quiet)
			fprintf(stderr, "%s: Error on close of '%s' as: %s\n", progname, file,
				strerror(errno));
		exit_c = 1;
	}
}

/* end */
