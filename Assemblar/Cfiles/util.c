#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "../Headers/util.h"
#include "../Headers/Errors.h"
#include "../Headers/globals.h"
#include "../Headers/symbol_table.h"
#include "../Headers/first_pass_helper.h"
#include "../Headers/code_helper.h"
#include "../Headers/link_list_strct.h"


void* check_malloc(long memory_size) {
	void* memory_ptr = malloc(memory_size);
	if (memory_ptr == NULL) {
		print_in_error(ERROR_CODE_1);
	}
	return memory_ptr;
}


char* concatStrings(char* file_name, char* ending) {
	char* dot;
	char* new_file_name;

	/* Allocate memory for the new file name */
	new_file_name = check_malloc(MAX_LINE_LENGTH * sizeof(char));

	/* Copy the original file name to the new file name */
	strcpy(new_file_name, file_name);

	/* Find the '.' in the file name and cut off the string if found */
	dot = strchr(new_file_name, '.');
	if (dot != NULL) {
		*dot = '\0';  /* Remove the old extension */
	}

	/* Append the new ending to the file name */
	strcat(new_file_name, ending);

	return new_file_name;
}

void close_files(int num_args, ...) {
	FILE* fp;
	char* identifier;
	int i;
	va_list args;

	/* Initialize the variable argument list */
	va_start(args, num_args);

	/* Process each argument pair */
	for (i = 0; i < num_args; i++) {
		/* Check if the identifier is "%s" for strings */
		if (strcmp(va_arg(args, char*), "%s") == 0) {
			i++;  /* Move to the string argument */
			identifier = va_arg(args, char*);

			/* Remove the file associated with the string name and free the memory */
			remove(identifier);   /* Attempt to remove the file from the filesystem */
			free(identifier);     /* Free the allocated memory for the string */
		}
		/* Otherwise, assume the identifier is "file" for file pointers */
		else {
			fp = va_arg(args, FILE*);

			/* Close the open file pointer */
			fclose(fp);
		}
	}

	/* Clean up the variable argument list */
	va_end(args);
}

/*This function helps to allocate strings*/
char* allocate_string(char* source) {
	char* dest = malloc(strlen(source) + 1);
	if (dest) {
		strcpy(dest, source);
	}
	return dest;
}


int extra_text() {
	char* token;
	token = strtok(NULL, "\n");

	/* If a token exists There is More Text. */
	if (token != NULL) {
		return 1;
	}
	return 0;
}

int allocate_first_pass(ent_ext_label** externs, ent_ext_label** entries, code_help** code, code_help** data) {

	/* Allocate memory for 'extern' */
	*externs = check_malloc(sizeof(ent_ext_label));
	if (*externs == NULL) {
		print_in_error(ERROR_CODE_1);
		return 1;
	}

	/* Allocate memory for 'entries' */
	*entries = check_malloc(sizeof(ent_ext_label));
	if (*entries == NULL) {
		print_in_error(ERROR_CODE_1);
		return 1;
	}

	/* Allocate memory for 'code' */
	*code = check_malloc(sizeof(code_help));
	if (*code == NULL) {
		print_in_error(ERROR_CODE_1);
		return 1;
	}
	(*code)[0].label = NULL;
	(*code)[0].short_num = 0;

	/* Allocate memory for 'data' */
	*data = check_malloc(sizeof(code_help));
	if (*code == NULL) {
		print_in_error(ERROR_CODE_1);
		return 1;
	}

	/*Return 0 done succsesfully*/
	return 0;
}

void free_project(code_help* code, label_address* label_table, ent_ext_label* entries, ent_ext_label* externs, \
	int code_count, int label_table_line, int entries_count, int externs_count) {
	free_code(code, code_count);
	free_label_table(label_table, label_table_line);
	free_another_table(entries, entries_count);
	free_another_table(externs, externs_count);
}

int long_lines(char* file_name) {

	char line[MAX_LINE_LENGTH + 3];
	int am_file_line = 0;
	char* am_file_name = file_name;
	int line_too_long_found = 0;

	FILE* file = fopen(file_name, "r");
	if (file == NULL) {
		print_ext_error(ERROR_CODE_8, am_file_name, am_file_line);
		return -1;
	}

	while (fgets(line, sizeof(line), file) != NULL) {
		if (strlen(line) > MAX_LINE_LENGTH) {
			print_ext_error(ERROR_CODE_6, am_file_name, am_file_line);
			line_too_long_found = 1;
		}
		am_file_line++;
	}

	fclose(file);
	return line_too_long_found;
}

int write_in_octal(code_help* code, int count, char* file_name, int IC, int DC) {
	FILE* output_file;
	int i, address;
	char* output_file_name;

	/* Create a new file name with the ".ob" extension */
	output_file_name = concatStrings(file_name, ".ob");

	/* Open the file for writing */
	output_file = fopen(output_file_name, "w");
	if (output_file == NULL) {
		print_in_error(ERROR_CODE_8);
		free(output_file_name);
		return 0;
	}

	/* Write the IC and DC values at the top of the file */
	fprintf(output_file, "%d %d\n", IC, DC);

	/* Write the address and content of each word in the required format */
	for (i = 0; i < count; i++) {
		address = DEFAULT_IC + i;  /* Calculate the address of the word */

		/* Print the address in decimal with leading zeros, and the content in octal with leading zeros */
		fprintf(output_file, "%04d %05o\n", address, code[i].short_num);
	}

	/* Close the file */
	fclose(output_file);
	free(output_file_name);

	return 1;
}
