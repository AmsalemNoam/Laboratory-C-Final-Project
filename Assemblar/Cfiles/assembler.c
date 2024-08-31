#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "../Headers/pre_assemblar.h"
#include "../Headers/first_and_second_pass.h"
#include "../Headers/util.h"
#include "../Headers/globals.h"

/**
 * This is the main function that processes the input files.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return Returns 0 on successful completion.
 */
int main(int argc, char* argv[]) {
	char* as_file, * am_file;
	while (--argc > 0) {
		
		/* Generate a new file with the ending ".as" */
		as_file = concatStrings(argv[argc], ".as");
		
		printf("Starting The Assemblar For This File: %s\n",as_file);
		
		printf("Start The Pre Assemblar\n");
		/*Execute the macro preprocessor on the ".as" file.*/

		if (!execute_macr(as_file)) {
			/*If it failed, move to the next file.*/
			continue;
		}

		printf("Start First Pass\n");
		/* Generate a new file with the ending ".am" */
		am_file = concatStrings(argv[argc], ".am");
		/*Execute the first pass, and then the second*/
		if (execute_first_pass(am_file)) {
			/*If it failed, move to the next file.*/
			continue;
		}

		/*Free allocated memory*/
		free(am_file);
	}

	printf("Finished The Assembler!\n");
	return 0;
}
