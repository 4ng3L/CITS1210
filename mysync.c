/*
   CITS1210 Project 2
   Names:		Jonathan Chua			Angeline Honggowarsito
   Student numbers:	10996944	20640062
   Date:		29/10/10
*/

#include "source_collection.h"

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		//TODO change this to fit sample output
		fprintf(stderr, "Please enter at least 2 files to be synced.\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		/*
		 * TODO
		 * Basically read in 2 top_lvl_dirs and options, then parses them.
		 * Calls the three main function to do all the work.
		 * So only thing required here is to prepare the stuff required.
		 */
		option_a();

		exit(EXIT_SUCCESS);
	}

	return 0;
}
