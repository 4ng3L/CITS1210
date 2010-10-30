/*
   CITS1210 Project 2
   Names:		Jonathan Chua			Angeline Honggowarsito
   Student numbers:	10996944	20640062
   Date:		29/10/10
*/

#include "mysync.h"
#include <time.h>
#include <utime.h>   
/****************************************************************/

/*
 * findLastModifiedIndexPrint(*modTimes, nDirs) loop through the number
 * of dirs and compare the modTime for file in each dir.
 */
static int findLastModifiedIndexPrint(time_t *modTimes, int nDirs)
{
	int lastModIndex = 0;
	int total = 1;
	for(int i = 1; i < nDirs; i++){
		if(modTimes[i] == 0)
		{
			continue;		
		}
		// difftime == POSIX function in <time.h>
		// difftime(TIM1, TIM2) --> Return TIM1 - TIM2
		if( ((int)difftime(modTimes[i], modTimes[lastModIndex])) > 0 )
		{
			lastModIndex = i;
 		}
 		if( ((int)difftime(modTimes[i], modTimes[lastModIndex])) == 0 )
		{
			total++;
 		}
	}
	if(total == nDirs)
	{
		lastModIndex = nDirs;
	}
	return lastModIndex;
}

static void trimLine(char str[])
{
	int i = 0;

	while (str[i] != '\0')
	{
		if (str[i] == '\r' || str[i] == '\n')
		{
			str[i] = '\0';
			break;
		}
		i++;
	}
}

void printFilesInEachDir(DIRECTORYINFO *d, TOPLEVELS tls)
{
	DIRECTORYINFO *d_copy = d;
	
	for(int i = 0; i < d_copy->nFiles; i++)
	{
		printf("  File: %s\n", d_copy->files[i]->name);
		int lastModified = findLastModifiedIndexPrint(d_copy->files[i]->modtimes, tls.nDirs);
		for(int j = 0; j < tls.nDirs; j++)
		{
			if(lastModified == tls.nDirs)
			{
				char str[BUFSIZ];
				str[0] = '\0';
				strcat(str, ctime(&(d_copy->files[i]->modtimes[j])));
				trimLine(str);
				printf("    %s: %s *\n", tls.pathnames[j], str);
				str[0] = '\0';
			}
			else 
			{
				if(d_copy->files[i]->modtimes[j] == 0) continue;
				else if(lastModified == j)
				{
					char str[BUFSIZ];
					strcat(str, ctime(&(d_copy->files[i]->modtimes[j])));
					trimLine(str);
					printf("    %s: %s *\n", tls.pathnames[j], str);
					str[0] = '\0';
					continue;
				}
				else printf("    %s: %s", tls.pathnames[j], ctime(&(d_copy->files[i]->modtimes[j])));
			}
		}
	}
}
/* 
   printFileSystem(d, tls) prints the directory structure represented
   by d using the top-level pathnames from tls.
*/
      
void printFileSystem(DIRECTORYINFO *d, TOPLEVELS tls)
{
	DIRECTORYINFO *d_copy = d;
	if(d_copy->nFiles > 0)
	{
		printFilesInEachDir(d_copy, tls);
	}
}
