/*
   CITS1210 Project 2
   Names:		Jonathan Chua			Angeline Honggowarsito
   Student numbers:	10996944	20640062
   Date:		29/10/10
*/

#include "mysync.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/param.h>

#define TEST1() printf("test1\n")
#define TEST2() printf("test2\n")
#define TEST3() printf("test3\n")
#define TEST4() printf("test4\n")

/****************************************************************/

/*
 * Calculate the total number of files in all the directories.
 */
static int calculateNFiles(char** paths, OPTIONS opts)
{
	int nFiles = 0;
	char** tmp_paths = paths;
	char* filenames = malloc(MAXPATHLEN);

	DIR* dir;
	struct dirent* dp;

	while (*tmp_paths != NULL)
	{
		char fullname[MAXPATHLEN];

		dir = opendir(*tmp_paths);
		if (dir == NULL)
		{
			perror(opts.programname);
			exit(EXIT_FAILURE);
		}

		while ((dp = readdir(dir)) != NULL)
		{
			struct stat stat_buffer;

			sprintf(fullname, "%s/%s", *tmp_paths, dp->d_name);

			if (stat(fullname, &stat_buffer) != 0)
				perror(opts.programname);

			if (S_ISREG(stat_buffer.st_mode))
			{
				char *tmp_filenames = filenames;
				while (strcmp(tmp_filenames, dp->d_name))
				{
					tmp_filenames = strdup(dp->d_name);
					nFiles++;

					tmp_filenames++;
				}
				tmp_filenames = filenames;
			}
		}

		closedir(dir);
		tmp_paths++;
	}

	return nFiles;
}

/*
 * Calculate the total number of subdirectories in all the directories.
 */
static int calculateNSubdirs(char** paths, OPTIONS opts)
{
	int nSubdirs;
	char** tmp_paths = paths;
	char* filenames = malloc(MAXPATHLEN);

	DIR* dir;
	struct dirent* dp;

	while (*tmp_paths != NULL)
	{
		char fullname[MAXPATHLEN];

		dir = opendir(*tmp_paths);
		if (dir == NULL)
		{
			perror(opts.programname);
			exit(EXIT_FAILURE);
		}

		while ((dp = readdir(dir)) != NULL)
		{
			struct stat stat_buffer;

			sprintf(fullname, "%s/%s", *tmp_paths, dp->d_name);

			if (stat(fullname, &stat_buffer) != 0)
				perror(opts.programname);

			if (S_ISDIR(stat_buffer.st_mode))
			{
				char *tmp_filenames = filenames;
				while (strcmp(tmp_filenames, dp->d_name))
				{
					tmp_filenames = strdup(dp->d_name);
					nSubdirs++;

					tmp_filenames++;
				}
				tmp_filenames = filenames;
			}
		}

		closedir(dir);
		tmp_paths++;
	}

	return nSubdirs;
}

/*
   processTopLevelDirectories(tls, paths, nDirs, opts) updates tls
   and returns a pointer to a DIRECTORYINFO structure that contains
   information about the files and sub-directories found in the nDirs
   top-level directories specified by the paths array, using the options
   in opts to guide the process.
*/
DIRECTORYINFO *processTopLevelDirectories(TOPLEVELS *tls, char **paths, int nDirs, OPTIONS opts)
{
	TOPLEVELS* tl = tls;
	DIRECTORYINFO* d = malloc(BUFSIZ);
	DIRECTORYINFO* tmp_d = d;
	DIR* dir;
	struct dirent* dp;

	//Updating TOPLEVELS
	tl->pathnames = paths;
	tl->nDirs = nDirs;

	//Initialising DIRECTORYINFO
	char** tmp_paths = paths;
	tmp_d->name = ".";
	tmp_d->exists = malloc(nDirs*sizeof(bool));
	bool* tmp_exists = tmp_d->exists;
	int nFiles = calculateNFiles(paths, opts);
	int nSubdirs = calculateNSubdirs(paths, opts);
	tmp_d->nFiles = nFiles;
	tmp_d->nSubdirs = nSubdirs;

	//Since all tls would contain the dir "." set all d->exists to true
	for (int i = 0; i < nDirs; i++)
	{
		*tmp_exists = true;
		tmp_exists++;
	}
	tmp_exists = tmp_d->exists;

	tmp_d->files = malloc(nFiles*sizeof(FILEINFO));
	tmp_d->subdirs = malloc(nSubdirs*sizeof(DIRECTORYINFO));

	//Initialising FILEINFO
	FILEINFO** tmp_files = tmp_d->files;
	//To keep track of which dir we're at for modtimes and modes
	FILEINFO** tmp_files2 = tmp_d->files;
	(*tmp_files)->name = malloc(MAXPATHLEN);

	//Initialising subDIRECTORYINFO
	DIRECTORYINFO** tmp_subd = d->subdirs;

	while (*tmp_paths != NULL)
	{
		char fullname[MAXPATHLEN];

		dir = opendir(*tmp_paths);
		if (dir == NULL)
		{
			perror(opts.programname);
			exit(EXIT_FAILURE);
		}

		while ((dp = readdir(dir)) != NULL)
		{
			struct stat stat_buffer;

			sprintf(fullname, "%s/%s", *tmp_paths, dp->d_name);

			if (stat(fullname, &stat_buffer) != 0)
				perror(opts.programname);

			//Processing subdirs
			if (S_ISDIR(stat_buffer.st_mode))
			{
				while (strcmp((*tmp_subd)->name, dp->d_name) != 0)
				{
					DIRECTORYINFO* subd = malloc(sizeof(DIRECTORYINFO));

					subd->name = dp->d_name;
					*subd->exists = true;

					*tmp_d->subdirs = subd;
					free(subd);
					tmp_subd++;
				}

				//To set the array of exists
				if (strcmp((*tmp_subd)->name, dp->d_name) == 0)
				{
					*(*tmp_subd)->exists = true;
					(*tmp_subd)->exists++;
				}
			}
			tmp_subd = d->subdirs;

			//Processing files
			if (S_ISREG(stat_buffer.st_mode))
			{
				while (strcmp((*tmp_files)->name, dp->d_name) != 0)
				{
					(*tmp_files)->name = strdup(dp->d_name);
					*(*tmp_files2)->modtimes = stat_buffer.st_mtime;
					(*tmp_files2)->modtimes++;
					*(*tmp_files2)->modes = stat_buffer.st_mode;
					(*tmp_files2)->modes++;
					tmp_files++;
				}

				//To set the array of modtimes and modes.
				if (strcmp((*tmp_files)->name, dp->d_name) == 0)
				{
					*(*tmp_files2)->modtimes = stat_buffer.st_mtime;
					(*tmp_files2)->modtimes++;
					*(*tmp_files2)->modes = stat_buffer.st_mode;
					(*tmp_files2)->modes++;
				}
			}
			(*tmp_files2)->modtimes++;
			(*tmp_files2)->modes++;
			//Points tmp_files back to the first file.
			tmp_files = tmp_d->files;
		}

		closedir(dir);
		tmp_paths++;
	}

	return d;
}