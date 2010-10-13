/*
   CITS1210 Project 2
   Names:		Jonathan Chua			Angeline Honggowarsito
   Student numbers:	10996944	20640062
   Date:		<date of submission>
*/

/* Comment out the next line when you have filled in your details above */
#error Did you fill in your name and student number?

#ifndef _MYSYNC_H_
#define _MYSYNC_H_

/****************************************************************/
/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

/* An explicit declaration of the very useful strdup function */
extern char *strdup(const char *);

/****************************************************************/
/* Constants and structure definitions */

/* Structure to hold information about a file */
typedef struct {
	/* The name of the file */
	char *name;
	/* An array of modification times, one for each top-level
	   directory, indexed in order of top-level directory.
	   The corresponding entry in the array is zero if the top-level
	   directory does not contain this file */
	time_t *modtimes;
	/* An array of permission modes, one for each top-level directory,
	   indexed in order of top-level directory.  The corresponding
	   entry in the array is zero if the top-level directory does
	   not contain this file */
	mode_t *modes;
} FILEINFO;

/* Structure to hold information about a directory */
typedef struct _directoryinfo {
	/* The name of the directory */
	char *name;
	/* An array of Booleans, one for each top-level directory,
	   indexed in order of top-level directory.  The corresponding
	   entry in the array indicates whether the top-level directory
	   contains this directory or not */
	bool *exists;
	/* The number of regular files in the directory */
	int nFiles;
	/* Information about each file in the directory */
	FILEINFO **files;
	/* The number of sub-directories in the directory */
	int nSubdirs;
	/* Information about each sub-directory in the directory */
	struct _directoryinfo **subdirs;
} DIRECTORYINFO;

/* Structure to hold information about the top-level directories that
   need to be synchronised */
typedef struct {
	/* The number of top-level directories */
	int nDirs;
	/* The provided path-names of the top-level directories */
	char **pathnames;
} TOPLEVELS;

/* Structure to hold information about command-line switches and
   arguments to the program */
typedef struct
{
	/* Boolean toggle to indicate whether hidden files should be
	   processed or not */
	bool processHiddens;
	/* Integer number of seconds such that files with modification
	   times that differ by less than this value are considered the
	   same */
	int timeResolution;
	/* Boolean toggle to indicate whether the actual synchronisation
	   (file creation and overwriting) should be performed or not */
	bool performSync;
	/* Boolean toggle to indicate whether subdirectories should be
	   recursively processed or not */
	bool recursive;
	/* Boolean toggle to indicate whether to print the combined
	   directory structure or not */
	bool print;
	/* Boolean toggle to indicate whether modification times and
	   permissions should be updated or not */
	bool updateStatus;
	/* Boolean toggle to indicate whether verbose output should be
	   printed or not */
	bool verbose;
	/* The name of the executable */
	char *programname;
} OPTIONS;

/****************************************************************/
/* Global variable definitions */
/* The sample solution contains 0 global variables */

/****************************************************************/
/* Declarations for the required functions */

extern DIRECTORYINFO *processTopLevelDirectories(TOPLEVELS *, char **, int, OPTIONS);
extern void printFileSystem(DIRECTORYINFO *, TOPLEVELS);
extern void syncFiles(DIRECTORYINFO *, TOPLEVELS, OPTIONS);

/****************************************************************/

#endif
