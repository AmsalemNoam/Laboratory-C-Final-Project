#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include "../Headers/globals.h"
#include "../Headers/pre_assemblar.h"
#include "../Headers/Errors.h"
#include "../Headers/util.h"

int execute_macr(char file_name[]) {

	node* head;
	char* new_file2, * final_file;

	/*Cheking if the file is Null*/
	if (file_name == NULL) {
		return 0;
	}

	head = NULL; /*Starting the linked list*/

	/* Step 1: Scanning and saving all the macros in the file in a linked list of macros*/

	   /*If it failed close the file and free the list*/
	if (!macr_list(file_name, &head)) {
		free_list(head);
		close_files(2, "%s", file_name);
		return 0;
	}

	/* Step 2: Removing all macro declarations from the file and saving in a new file*/
	new_file2 = remove_mcros_decl(file_name);

	/*cheking if there is a problem if so free and close*/
	if (new_file2 == NULL) {
		free_list(head);
		close_files(2, "%s", new_file2);
		print_in_error(ERROR_CODE_15);
		return 0;
	}
	/* Free allocated memory for the name of the first file*/
	
	free(file_name);

	/* Step 3: Replacing all macro calls with the content we saved at the linked list*/
	final_file = paste_all_macrs(new_file2, head);

	/*cheking if there is a problem if so free and close*/
	if (final_file == NULL) {
		free_list(head);
		close_files(4, "%s", new_file2);
		print_in_error(ERROR_CODE_15);
		return 0;
	}

	/* Step 4: Freeing memory */
	remove(new_file2);
	free(new_file2);
	free(final_file);
	free_list(head);
	return 1;
} /*end of macro execute*/


const char** get_illegal_macro_names() {
	static const char* illegal_macro_names[ILLEGAL_MACRO_NAMES_COUNT] = {
		"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp",
		"bne", "red", "prn", "jsr", "rts", "stop", "r0", "r1", "r2", "r3",
		"r4", "r5", "r6", "r7", "macr", "endmacr", "data", "string", "entry",
		"extern"
	};
	return illegal_macro_names;
}

int is_valid_macro_name(char* str, char** name, int line_count, char* file_name) {

	/* assumes "mcro " has been encountered right before the function was called */

	const char** illegal_macro_names = get_illegal_macro_names();
	char* token;
	int i;

	/* Get the macro name checks if empty name*/
	token = strtok(str, " \t\n");

	if (token == NULL) {
		print_ext_error(ERROR_CODE_9, file_name, line_count);
		return 0;
	}

	/* Check if the name is illegal word */
	for (i = 0; i < ILLEGAL_MACRO_NAMES_COUNT; i++) {
		if (strcmp(illegal_macro_names[i], token) == 0) {
			print_ext_error(ERROR_CODE_16, file_name, line_count);
			return 0;
		}
	}

	/* Check if the name starts with a letter */
	if (!isalpha(token[0])) {
		print_ext_error(ERROR_CODE_16, file_name, line_count);
		return 0;
	}

	/* Check if the remaining characters are valid */
	for (i = 1; token[i] != '\0'; i++) {
		if (!isalnum(token[i]) && token[i] != '_') {
			print_ext_error(ERROR_CODE_16, file_name, line_count);
			return 0;
		}
	}
	/*Check if there are more words after macro name*/
	if (strtok(NULL, " \n") != NULL) {
		print_ext_error(ERROR_CODE_10, file_name, line_count);
		return 0;
	}
	*name = check_malloc((strlen(token) + 1) * sizeof(char));
	strcpy(*name, token);

	return 1;
}

char* copy_macr_content(FILE* fp, int* line_count)
{
	char* content = NULL;
	char line[MAX_LINE_LENGTH];
	size_t size = 0;
	size_t len;
	char* new_content;


	while (fgets(line, sizeof(line), fp) != NULL) {
		(*line_count)++;
		/* Check if the line contains "endmcro" */
		if (strncmp(line, "endmacr", 7) == 0) {
			/* Check if there is any extra text after "endmcro" */
			if (strtok(line + 7, " \t\n") != NULL) {
				print_in_error(ERROR_CODE_12);
			}
			break; /* Exit the loop, "endmcro" line should not be saved */
		}

		len = strlen(line);
		new_content = realloc(content, size + len + 1);
		if (!new_content) {
			print_in_error(ERROR_CODE_1);
			free(content); /* Free the original content in case of realloc failure */
			return NULL;
		}

		content = new_content;
		strcpy(content + size, line);
		size += len;
	}
	content[size - 1] = '\0'; /* Remove trailing newline characters */
	return content;
}

int macr_list(char* file_name, node** head) {
	FILE* fp;
	char line[MAX_LINE_LENGTH];
	int line_count = 0;
	char* name, * content;
	int success = 1;

	fp = fopen(file_name, "r");
	if (fp == NULL) {
		print_in_error(ERROR_CODE_8); /*check if file opened*/
		success = 0;
		return success;
	}

	while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) { /*Checking getline is succesful*/
		line_count++;
		if (strcmp(strtok(line, " "), "macr") == 0) {
			/*If read in a line "macr " include the space there is a macro*/

			if (!is_valid_macro_name(line + 5, &name, line_count, file_name)) {
				success = 0;
				continue; /*checking if macro name is good*/

			}

			content = copy_macr_content(fp, &line_count);
			if (content == NULL) { /*if failed*/
				success = 0;
				continue;
			}

			add_to_list(head, name, content, line_count);
		}
	}
	fclose(fp);

	return success;
} /*end of macr_list*/

char* remove_mcros_decl(char file_name[]) {
	FILE* src, * dest;
	char line[MAX_LINE_LENGTH];
	char* new_file_name = concatStrings(file_name, ".temp");
	int in_macro = 0;

	/* Open the source file for reading */
	src = fopen(file_name, "r");
	if (src == NULL) {
		print_in_error(ERROR_CODE_8);
		free(new_file_name);
		return NULL;
	}

	/* Open the destination file for writing */
	dest = fopen(new_file_name, "w");
	if (dest == NULL) {
		print_in_error(ERROR_CODE_7);
		fclose(src);
		free(new_file_name);
		return NULL;
	}

	/* Read each line from the source file */
	while (fgets(line, MAX_LINE_LENGTH, src) != NULL) {

		/* Check if the line starts with "macr ", indicating the start of a macro */
		if (strncmp(line, "macr ", 5) == 0) {
			in_macro = 1; /* Enter macro definition block */
		}
		/* Check if the line starts with "endmacr", indicating the end of a macro */
		else if (strncmp(line, "endmacr", 7) == 0) {
			in_macro = 0; /* Exit macro definition block */
		}
		/* If not inside a macro definition, write the line to the destination file */
		else if (!in_macro) {
			fprintf(dest, "%s", line);
		}
	}

	/* Close both source and destination files */
	fclose(src);
	fclose(dest);
	return new_file_name;
}


char* paste_macr(char* str, node* mcro) {
	char* pos, * result;
	size_t prefix_len, suffix_len, new_len;
	/* Find the first occurrence of the macro name in the input string*/
	pos = strstr(str, mcro->name);
	if (pos == NULL) {
		result = check_malloc(strlen(str) + 1);
		strcpy(result, str);
		return result;
	}
	/* Calculate the length of the new string after replacement*/
	prefix_len = pos - str;
	suffix_len = strlen(pos + strlen(mcro->name));
	new_len = prefix_len + strlen(mcro->content) + suffix_len + 1;
	/* Allocate memory for the new string*/
	result = check_malloc(new_len * sizeof(char));

	strncpy(result, str, prefix_len);
	result[prefix_len] = '\0';
	strcat(result, mcro->content);
	strcat(result, pos + strlen(mcro->name));

	return result; /* Return the resulting string*/
}

char* paste_all_macrs(char file_name[], node* head) {
	FILE* src = NULL, * dest = NULL;
	char line[MAX_LINE_LENGTH];
	char* final_file = NULL;
	char* modified_line = NULL;
	node* mcro;
	char* temp_line;

	final_file = concatStrings(file_name, ".am");

	src = fopen(file_name, "r");
	if (src == NULL) {
		print_in_error(ERROR_CODE_8);
		close_files(2, "%s", final_file);
		return NULL;
	}

	dest = fopen(final_file, "w");
	if (dest == NULL) {
		print_in_error(ERROR_CODE_7);
		close_files(4, "%s", final_file, "file", src);
		return NULL;
	}

	while (fgets(line, MAX_LINE_LENGTH, src) != NULL) {

		modified_line = malloc(strlen(line) + 1);
		if (!modified_line) {
			print_in_error(ERROR_CODE_9);
			close_files(6, "file", src, "file", dest, "%s", final_file);
			return NULL;
		}

		strcpy(modified_line, line);
		mcro = head;

		while (mcro) {

			temp_line = paste_macr(modified_line, mcro);

			if (temp_line) {
				free(modified_line);
				modified_line = temp_line;
			}

			mcro = mcro->next;
		}

		fprintf(dest, "%s", modified_line);
		free(modified_line);
	}

	close_files(2, "file", src, "file", dest);
	return final_file;
}
