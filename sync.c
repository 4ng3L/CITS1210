/*
   CITS1210 Project 2
   Names:		Jonathan Chua			Angeline Honggowarsito
   Student numbers:	10996944	20640062
   Date:		29/10/10
*/
   
#include "mysync.h"   
#include <sys/stat.h>   
#include <time.h>
#include <utime.h>   
      
/****************************************************************/

/*
 * copyContent(input, output), copying content from one file
 * to another file.
 */
static void copyContent(char *input, char *output)
{
	FILE	*fp_input = fopen(input, "r");
	FILE 	*fp_output = fopen(output, "w");
	char buffer[BUFSIZ];
	size_t 	read, write;
	
	if(fp_input == NULL)
	{
		perror("fp_input");
		exit(EXIT_FAILURE);	
	}
	if(fp_output == NULL)
	{
		perror("fp_output");
		exit(EXIT_FAILURE);
	}
	
	while( (read = fread(buffer, 1, sizeof buffer, fp_input)) > 0)
	{
		write = fwrite(buffer, 1, read, fp_output);
		if(write != read)
		{
			perror("copyContent");
			exit(EXIT_FAILURE);
		}
	}	
	fclose(fp_input);
	fclose(fp_output);
}

/*
 * findLastModifiedIndex(*modTimes, nDirs, timeResolution)
 * Comparing the modification time largest than timeResolution that 
 * is contained in each dir specified in nDirs.
 */
static int findLastModifiedIndex(time_t *modTimes, int nDirs, int timeResolution)
{
	int lastModIndex = 0;
	for(int i = 1; i < nDirs; i++){
		if(modTimes[i] == 0)
		{
			continue;		
		}
		// difftime == POSIX function in <time.h>
		// difftime(TIM1, TIM2) --> Return TIM1 - TIM2
		if( ((int)difftime(modTimes[i], modTimes[lastModIndex])) >= timeResolution )
		{
			lastModIndex = i;
 		}
	}
	return lastModIndex;
}

/*
 * Updating each top level dir with the latest version of file or subdir
 * Pointer of d here represent the first top level dir.
 * 
 * Need improvement:
 * - change bool to OPT -- cause malloc error..
 * - make it flexible to subdir 
 */
static DIRECTORYINFO *updateDir(TOPLEVELS tls, char *fullname,  DIRECTORYINFO *d, int lastModifiedIndex, int fileIndex, int nDirs, bool subdirs, OPTIONS optsSubdir)
{
	DIRECTORYINFO *d_copy = d;
	DIRECTORYINFO *d_lastModified = d + lastModifiedIndex ; 
	char tempName[BUFSIZ];
	//char *subdirName;
	char fullNameDLastModified[BUFSIZ];
	struct utimbuf timeMod;
	
	sprintf(fullNameDLastModified, "%s/%s", tls.pathnames[lastModifiedIndex], d_lastModified->files[fileIndex]->name);
		
	for(int i = 0; i < nDirs; i++)
	{
		if(i == lastModifiedIndex)
		{
			continue;
		}
		//copyContent(input, output)
		sprintf(tempName, "%s/%s", tls.pathnames[i], d_copy->files[fileIndex]->name);
		
		// OPTIONS -v 
		if(optsSubdir.verbose)
		{
			printf("cp %s %s\n", fullNameDLastModified, tempName);	
		}
		
		// OPTIONS -n
		if(!optsSubdir.performSync)
		{
			printf("cp %s %s\n", fullNameDLastModified, tempName);
			continue;	
		}
		
		copyContent(fullNameDLastModified, tempName);
		if(optsSubdir.updateStatus)
		{
			//changeMod(ouput pathname, change mode to)
			chmod(tempName, d_lastModified->files[fileIndex]->modes[lastModifiedIndex]);
			//change Mod Time -- setting new struct of utimbuf 
			timeMod.actime = d_lastModified->files[fileIndex]->modtimes[lastModifiedIndex];
			timeMod.modtime = d_lastModified->files[fileIndex]->modtimes[lastModifiedIndex];
			utime(tempName, &timeMod);	
		}
	}
	return d;	
}

/*
 * For each File in each Dir, find the latest version one
 * Delegate work to "Update Dir" to update the content of all dirs.
 */
static void syncFilesEachDir(int nFiles,  DIRECTORYINFO *d, TOPLEVELS tls, char *fullname, bool subdirs, OPTIONS opts)
{
	DIRECTORYINFO *d_i = d;
	int timeResolution = 1;
	int lastModifiedIndex;
	
	for (int i = 0; i < d_i->nFiles; i++)
	{	
		//check for hidden files
		/*
		if(opts.processHiddens == true)
		{
			//if true
			if(processHiddenFiles(d_i->files[nFilesEachDir]->name))
			{
				nFilesEachDir++;
				continue;
			}
		}		
		*/
		
		// if there is TIME DIFFERENCES set (OPT -m i)
		if(opts.timeResolution > 0)
		{
			timeResolution = opts.timeResolution;		
		}
			
		lastModifiedIndex = findLastModifiedIndex(d_i->files[i]->modtimes, tls.nDirs, timeResolution);
		d = updateDir(tls, fullname, d, lastModifiedIndex, i, tls.nDirs, subdirs, opts);
	}
}

/*
 * Synchronizing subdir
 */ 
/*static void syncSubdir(int nSubdirs, DIRECTORYINFO *parentDir, TOPLEVELS tls, int currentTlsIndex, char *fullname, OPTIONS optsSubdir)
{
	DIRECTORYINFO *parentDir_copy = parentDir;
	//DIRECTORYINFO *start = parentDir;
	char subDirPathName[BUFSIZ];
	//char parentName[BUFSIZ];
	char tempName[BUFSIZ];
	
	for(int i = 0; i < nSubdirs; i++)
	{
		//parentDir_copy = realloc(parentDir_copy, sizeof(DIRECTORYINFO) * (parentDir_copy->nSubdirs + 1));
		sprintf(subDirPathName, "%s/%s", fullname, parentDir_copy->subdirs[i]->name);
		printf("SubdirPathName1 %s\n", subDirPathName);
		//ensure all tlsDir has this subDirs
		for(int j = 0; j < tls.nDirs; j++)
		{	
			if(j == currentTlsIndex)
			{
				continue;
			}

			//if it's not exists, make dir
			if(parentDir_copy->subdirs[i]->exists[j] == false)
			{
				sprintf(tempName, "%s/%s", tls.pathnames[j], parentDir_copy->subdirs[i]->name);
				printf("SubdirPathName %s\n", tempName);
				//parentDir_copy = realloc(parentDir_copy, sizeof(DIRECTORYINFO) * (parentDir_copy->nSubdirs + 1));
				mkdir(tempName,parentDir_copy->subdirs[i]->files[i]->modes[i]);
			}
		}

		if(parentDir_copy->subdirs[i]->nFiles >= 1)
		{
			syncFilesEachDir(parentDir_copy->subdirs[i]->nFiles, parentDir_copy->subdirs[i], tls, subDirPathName, true, optsSubdir);	
		}
		
		if(parentDir_copy->subdirs[i]->nSubdirs > 0)
		{
			syncSubdir(parentDir_copy->subdirs[i]->nSubdirs, parentDir_copy->subdirs[i], tls, currentTlsIndex, subDirPathName, optsSubdir);
		}
	}
}*/

/*
   syncFiles(d, tls, opts) synchronises the files contained within d
   using the top-level pathnames from tls and the options in opts to
   guide the process.
*/
void syncFiles(DIRECTORYINFO *d, TOPLEVELS tls, OPTIONS opts)
{
	DIRECTORYINFO *d_i = d;
	//DIRECTORYINFO *start = d;
	char fullname[BUFSIZ];
	
	//loop through number of top level directory
	for(int i = 0; i < tls.nDirs; i++)
	{
		//checking whether dir exists in tls or not
		//if not exists
		if (d_i->exists[i] == false)
		{			
			perror("name");
			exit(EXIT_FAILURE);
		}
		sprintf(fullname, "%s", tls.pathnames[i]);
 		
		if(d_i->nFiles >= 1)
		{
			syncFilesEachDir(d_i->nFiles, d_i, tls, fullname, false, opts);		
		}		
		// sync subdir -- hasn't finish the implementation.
		if(opts.recursive == true)
		{
			if(d_i->nSubdirs > 0)
			{
				//syncSubdir(d_i->nSubdirs,d_i, tls, i, fullname, opts);
			}
		}
	}			
}
