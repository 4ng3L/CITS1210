/*
   CITS1210 Project 2
   Name:		<name>			<name>
   Student number:	<student number>	<student number>
   Date:		<date of submission>
*/

/* Comment out the next line when you have filled in your details above */
#error Did you fill in your name and student number?

#include "mysync.h"

/****************************************************************/

/*
   processTopLevelDirectories(tls, paths, nDirs, opts) updates tls
   and returns a pointer to a DIRECTORYINFO structure that contains
   information about the files and sub-directories found in the nDirs
   top-level directories specified by the paths array, using the options
   in opts to guide the process.
*/
DIRECTORYINFO *processTopLevelDirectories(TOPLEVELS *tls, char **paths, int nDirs, OPTIONS opts)
{
	return NULL;
}

/* 
   printFileSystem(d, tls) prints the directory structure represented
   by d using the top-level pathnames from tls.
*/
void printFileSystem(DIRECTORYINFO *d, TOPLEVELS tls)
{
}

/*
   syncFiles(d, tls, opts) synchronises the files contained within d
   using the top-level pathnames from tls and the options in opts to
   guide the process.
*/
void syncFiles(DIRECTORYINFO *d, TOPLEVELS tls, OPTIONS opts)
{
}
