#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Headers/symbol_table.h"
#include "../Headers/Errors.h"
#include "../Headers/util.h"
#include "../Headers/globals.h"
#include "../Headers/first_pass_helper.h"


void free_another_table(ent_ext_label* table, int count) {
	int i;
	for (i = 0; i < count; i++) {
		free((table + i)->label_name);
	}
	free(table);
}

void free_label_table(label_address* label_table, int label_table_line) {
	int i;
	for (i = 0; i < label_table_line; i++) {
		free((label_table + i)->label_name);
	}
	free(label_table);
}

int insert_label_table(label_address** label_table, int lines, char* label, int counter, int am_file_line, int is_data_line) {
	label_address* temp;

	/*Reallocate memory to accommodate new entry*/
	temp = realloc(*label_table, lines * sizeof(label_address));
	if (!temp) {
		print_in_error(ERROR_CODE_1);
		free(temp);
		return 0;
	}

	*label_table = temp;

	/* Assign values to the new entry */

	(*label_table + lines - 1)->label_name = malloc((strlen(label) + 1) * sizeof(char));
	if ((*label_table + lines - 1)->label_name == NULL) {
		print_in_error(ERROR_CODE_1);
		return 0;
	}
	strcpy((*label_table + lines - 1)->label_name, label);

	(*label_table)[lines - 1].address = counter;
	(*label_table)[lines - 1].assembly_line = am_file_line;
	(*label_table)[lines - 1].data_line = is_data_line;

	return 1;
}

int insert_another_labels(ent_ext_label** table, int count, inst_parts* inst, int am_file_line) {
	ent_ext_label* ptr;
	int label_length;

	if (inst->arg_label == NULL)
		return 0;

	/* Set 'ptr' to the current table */
	ptr = *table;

	/* Update the assembly_line field of the last entry in the table */
	(*table + count - 1)->assembly_line = am_file_line;

	/* Calculate the length of the label_name */
	label_length = strlen(inst->arg_label) + 1;

	/* Allocate memory for the label_name and copy the label from 'inst' */
	(*table + count - 1)->label_name = check_malloc(label_length * sizeof(char));
	if ((*table + count - 1)->label_name == NULL) {
		print_in_error(ERROR_CODE_1); /*Print error code if failed*/
		return 0;
	}
	/*Copy the data into the table*/
	strcpy((*table + count - 1)->label_name, inst->arg_label);

	/* Resize the table to accommodate the new entry */
	*table = realloc(*table, (count + 1) * sizeof(ent_ext_label));
	if (table == NULL) {
		/*Failed to allocate memory*/
		print_in_error(ERROR_CODE_1); /*Print error code if failed*/
		free(ptr);
		return 0;
	}

	/* Return 1 to indicate successful insertion */

	return 1;
}

int check_label(label_address* label_table, int table_lines, char* file_name) {
	int i, j;

	for (i = 0; i < table_lines; i++) {
		for (j = i + 1; j < table_lines; j++) {
			if (strcmp(label_table[i].label_name, label_table[j].label_name) == 0) {
				print_ext_error(ERROR_CODE_32, file_name, label_table[i].assembly_line);
				print_ext_error(ERROR_CODE_32, file_name, label_table[j].assembly_line);
				return 0;
			}
		}
	}

	return 1;
}

void add_symbol_table_ic(label_address* label_table, int table_lines, int IC) {
	int i;

	for (i = 0; i < table_lines; i++) {
		if (label_table[i].data_line) {
			label_table[i].address += (IC + 1); /*adding to IC + 1*/
		}
	}
}

void fix_address(label_address* label_table, int table_lines) {
	int i;

	for (i = 0; i < table_lines; i++) {
		label_table[i].address += DEFAULT_IC; /*adding 100 to all addresses since our assembler starts from 100*/
	}
}

int is_extern_defined(ent_ext_label* externs, int externs_count, label_address* label_table, int label_table_line, char* file_name) {
	int i, j;
	int am_file_line = 0;
	char* am_file_name = file_name;
	int found_ext = 1;

	for (i = 0; i < externs_count; i++) {
		for (j = 0; j < label_table_line; j++) {
			if (strcmp(externs[i].label_name, label_table[j].label_name) == 0) {
				am_file_line = (label_table + j)->assembly_line;
				print_ext_error(ERROR_CODE_33, am_file_name, am_file_line);
				found_ext = 0;
			}
		}
	}

	return found_ext;
}

void replace_externs(code_help* code, ent_ext_label* externs, int externs_count, int count, char* file_name) {
	int i, j, found;

	for (i = 0; i < count; i++) {
		found = 0;
		if (code[i].label != NULL) {
			for (j = 0; j < externs_count && found == 0; j++) {
				if (strcmp(code[i].label, externs[j].label_name) == 0) {
					code[i].short_num = 1; /*Marking as extern*/
					code[i].is_external = 1; /*Marking as extern*/
					found = 1;
				}
			}
		}
	}
}

int replace_labels(code_help* code, label_address* label_table, int label_table_line, int IC_len, char* file_name) {
	int i, j, found_error, found;

	found_error = 0;

	for (i = 0; i <= IC_len; i++) {

		found = 0;

		if (code[i].label != NULL && code[i].is_external != 1) {

			for (j = 0; j < label_table_line; j++) {

				if (strncmp(code[i].label, label_table[j].label_name, strlen(code[i].label)) == 0) {
					code[i].short_num = 2; /* 010 ARE */
					code[i].short_num |= label_table[j].address << ARE_BITS;
					found = 1;
					break;
				}
			}

			if (found == 0) { /*Label not found*/

				print_in_error(ERROR_CODE_34);
				found_error = 1;
			}
		}
	}

	return found_error;
}

int paste_ext(code_help* code, int count, ent_ext_label* externs, int externs_count, char* file_name) {
	FILE* ext_file;
	int i, j, found;
	int externs_found = 0;
	char* temp;

	temp = concatStrings(file_name, ".ext");
	ext_file = fopen(temp, "w");

	if (!ext_file) {
		print_in_error(ERROR_CODE_7);
		return 0;
	}

	for (i = 0; i <= count; i++) {
		found = 0;
		if (code[i].label != NULL) {
			for (j = 0; j < externs_count && found == 0; j++) {
				if (strcmp(code[i].label, externs[j].label_name) == 0) {
					/*Copy and paste the extern*/
					fprintf(ext_file, "%s\t%d\n", externs[j].label_name, DEFAULT_IC + i);
					found = 1;
					externs_found = 1;
				}
			}
		}
	}

	fclose(ext_file);

	if (!externs_found) {
		remove(temp); /*Remove the file if no externs found*/
	}
	free(temp);

	return 1;
}

int paste_ent(label_address* label_table, int label_table_line, ent_ext_label* entries, int entries_count, char* file_name) {
	FILE* ent_file;
	int i, j, found;
	int entries_found = 0;
	char* temp;
	found = 0;

	temp = concatStrings(file_name, ".ent");
	ent_file = fopen(temp, "w");

	if (!ent_file) {
		print_in_error(ERROR_CODE_7);
		return 0;
	}

	for (i = 0; i < label_table_line; i++) {
		found = 0;
		for (j = 0; j < entries_count && found == 0; j++) {
			if (strncmp(label_table[i].label_name, entries[j].label_name, strlen(entries[j].label_name)) == 0) {
				fprintf(ent_file, "%s\t%d\n", entries[j].label_name, label_table[i].address);
				found = 1;
				entries_found = 1;

			}
		}
	}

	fclose(ent_file);

	if (!entries_found) {
		remove(temp); /*Remove the file if no entries found*/
	}
	free(temp);

	return 1;
}

