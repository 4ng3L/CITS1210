/*
   CITS1210 Project 2
   Names:		Jonathan Chua			Angeline Honggowarsito
   Student numbers:	10996944	20640062
   Date:		29/10/10
*/

#include <unistd.h>
#include "mysync.h"

#define OPTLIST "am:npruv"
#define DEFAULT_MOD_TIMES 1

int main(int argc, char *argv[])
{
	OPTIONS options;
	TOPLEVELS *tls = malloc(BUFSIZ);
	DIRECTORYINFO *d;
	int opt;
	char **paths;
	int nDirs = 0;

	/*
	 * Initialise default without options input.
	 * Done the long way to be more verbose.
	 */
	optind = 1;
	opterr = 0;
	options.processHiddens = false;
	options.timeResolution = DEFAULT_MOD_TIMES;
	options.performSync = true;
	options.recursive = false;
	options.print = false;
	options.updateStatus = true;
	options.verbose = false;
	options.programname = argv[0];

	/*
	 * Processing options.
	 */
	while ((opt = getopt(argc, argv, OPTLIST)) != -1)
	{
		switch (opt)
		{
			case 'a':
				options.processHiddens = !(options.processHiddens);
				break;
			case 'm':
				options.timeResolution = atoi(optarg);
				break;
			case 'n':
				options.performSync = !(options.performSync);
				break;
			case 'p':
				options.print = !(options.print);
				break;
			case 'r':
				options.recursive = !(options.recursive);
				break;
			case 'u':
				options.updateStatus = !(options.updateStatus);
				break;
			case 'v':
				options.verbose = !(options.verbose);
				break;
			default:
				argc = -1;
		}
	}

	if (argc - optind + 1 < 3)
	{
		fprintf(stderr, "Usage: %s [-amnpruv] dir1 dir2 [dirn ... ]\n", options.programname);
		exit(EXIT_FAILURE);
	}
	else
	{
		/*
		 * Processing the paths array for top level directory.
		 */
		paths = &argv[optind];

		/*
		 * Counting number of directories supplied.
		 */
		char **tempPaths = paths;
		while (*tempPaths != NULL)
		{
			nDirs++;
			tempPaths++;
		}

		d = processTopLevelDirectories(tls, paths, nDirs, options);
		
		TOPLEVELS tmp = *tls;
		if (options.print)
		{
			printf("Pre-synchronisation file structure:\n");
			printFileSystem(d, tmp);
		}
		syncFiles(d, tmp, options);
		if (options.print)
		{
			printf("Post-synchronisation file structure:\n");
			printFileSystem(d, tmp);
		}
		exit(EXIT_SUCCESS);
	}

	return 0;
}
