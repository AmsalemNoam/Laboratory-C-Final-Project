#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../Headers/first_pass_helper.h"
#include "../Headers/globals.h"
#include "../Headers/Errors.h"
#include "../Headers/util.h"


/* Define the opcodes */
op_code OPCODES[] = {
		{"mov",  2},
		{"cmp",  2},
		{"add",  2},
		{"sub",  2},
		{"lea",  2},
		{"clr",  1},
		{"not",  1},
		{"inc",  1},
		{"dec",  1},
		{"jmp",  1},
		{"bne",  1},
		{"red",  1},
		{"prn",  1},
		{"jsr",  1},
		{"rts",  0},
		{"stop", 0}
};

/* Define the registers */
char* REGS[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7" };

/* Define the instructions */
char* INSTUCTIONS[] = { ".data", ".string", ".extern", ".entry" };


int find_opcode(char* str) {
	int i;

	/* Return -1 if the string is NULL */
	if (str == NULL) {
		return -1;
	}

	/* Sreaches for the str in the OPCODES arr */
	for (i = 0; i < OPCODES_COUNT; i++) {
		if (strcmp(str, OPCODES[i].opcode) == 0) {
			return i; /* Return the index of the matching opcode */
		}
	}
	return -1; /* Return -1 if the string does not match any known opcodes */
}



int find_reg(char* str) {
	int i;

	/* Return -1 if the string is NULL */
	if (str == NULL) {
		return -1;
	}

	/* Sreaches for the str in the REGS arr */
	for (i = 0; i < REG_COUNT; i++) {
		if (strcmp(str, REGS[i]) == 0) {
			return i; /* Return the index of the matching register */

		}
	}

	return -1; /* Return -1 if the string does not match any known registers */
}

int find_instr(char* str) {
	int i;

	/* Return 0 if the string is NULL */
	if (str == NULL) {
		return 0;
	}

	/* Sreaches for the str in the INSTRUCTIONS arr */
	for (i = 0; i < INSTRUCTIONS_COUNT; i++) {
		if (strcmp(str, INSTUCTIONS[i]) == 0) {
			return 1; /* Return 1 if the string matches an instruction */
		}
	}

	return 0; /* Return 0 if the string is not a valid instruction */
}


int legal_label_decl(char* str) {

	if (str == NULL) {
		return 0;
	}
	/*if we got just an opcode and not a label*/
	if (find_opcode(str) >= 0) {
		return 0;
	}

	/* Cheking if label name is legel */
	if (strlen(str) > MAX_LABEL_LENGTH || !isalpha(*str) || find_reg(str) >= 0) {
		return 0;
	}

	while (*(++str) != ' ' && (isalpha(*str) || isdigit(*str))) {
		;
	}
	if (*(str) == ':') {
		return 1;
	}
	else {
		print_in_error(ERROR_CODE_23);
	}

	return 0;
}

int legal_label(char* str) {

	if (str == NULL) {
		return 0;
	}

	if (isalpha(*str) && strlen(str) <= MAX_LABEL_LENGTH && find_opcode(str) < 0 && !find_instr(str)) {
		while (*(++str) != '\0' && *(++str) != ' ' && (isalpha(*str) || isdigit(*str)))
		{
			;
		}
	}
	/* If we reached this point the string ends or has space
	 * then it means that all the characters were legal and the label is legal */
	if (*str == '\0')
		return 1;

	/*found space or something illegal*/
	return 0;
}


int is_num(char* str) {
	char* ptr;

	/* Find the '#' character in the string */
	if (str != NULL && strchr(str, '#') == str) {
		strtol(str + 1, &ptr, 10); /* Skip the '#' character and convert the rest */

		/* Check if the rest of the string is a valid number */
		if (*ptr == '\0' || *ptr == ' ') {
			return 1; /* The string is a valid number with '#' prefix */
		}
	}
	return 0; /* The string is not a valid number with '#' prefix */
}

int is_pointer(char* str) {
	/* Find the '*' character in the string */
	if (str != NULL && strchr(str, '*') == str) {
		/* Check if the rest is a valid register */
		if (find_reg(str + 1) != -1) {
			return 1;
		}
	}

	return 0;
}


int getOpType(char* str) {
	/* Check if the string is a valid register or  valid label or if it's a valid number or a pointer */
	if (find_reg(str) != -1)
		return 3;

	if (is_pointer(str))
		return 2;

	if (legal_label(str))
		return 1;

	if (is_num(str))
		return 0;

	return -1;

}

int occurrence_count(char* str, char ch) {
	int count;
	char* ptr;
	ptr = str;
	count = 0;

	while ((*ptr != '\0') && (ptr = strchr(ptr, ch)) != NULL) {
		if (*ptr == ch) {
			count++;
		}
		ptr++;
	}

	return count;
}

int has_white_space(char* str) {
	while (*str) {
		if (isspace(*str)) {
			return 1; /* Found a whitespace character */
		}
		str++;
	}
	return 0; /* No whitespace characters found */
}

void find_reg_error(char* str) {

	char temp_str[MAX_LINE_LENGTH];
	strcpy(temp_str, str);

	/* Check if it has white spaces */
	if (has_white_space(str))
		print_in_error(ERROR_CODE_24);

	else if (atoi(strtok(temp_str, "r"))) {
		print_in_error(ERROR_CODE_22); /* Illegal register name */
	}
	else
		print_in_error(ERROR_CODE_24); /* Illegal argument */
}

int check_get_arg(char* str) {
	/* Calculate the length of the first argument substring */

	/* Extract the first argument substring */
	char first_arg[MAX_LINE_LENGTH];
	int index;

	index = strcspn(str, " \t");

	strncpy(first_arg, str, index);
	first_arg[index] = '\0';

	/* Check if the extracted first argument is a valid register, label, or number or pointer*/
	return getOpType(first_arg);
}

int legal_arg(char* str, command_parts* command) {
	char* str1 = NULL, * str2 = NULL;
	int srcOpType = -1, destOpType = -1;

	/* Check if the argument is empty */
	if (str == NULL && OPCODES[command->opcode].arg_num != 0) {
		print_in_error(ERROR_CODE_25);
		return 0;
	}

	command->source = NULL;
	command->dest = NULL;
	command->srcOpType = -1;
	command->destOpType = -1;

	/* Handle opcodes with no arguments */
	if (OPCODES[command->opcode].arg_num == 0)
	{
		if (extra_text()) {
			print_in_error(ERROR_CODE_26);
			return 0;
		}
		return 1;
	}

	/* Handle opcodes with two arguments */
	if (OPCODES[command->opcode].arg_num == 2)
	{
		if (strstr(str, ",") == NULL) {
			print_in_error(ERROR_CODE_27);
			return 0;
		}
		else if (occurrence_count(str, ',') > 1) {
			print_in_error(ERROR_CODE_28);
			return 0;
		}

		str1 = strtok(str, ",");
		srcOpType = check_get_arg(str1);
		if (srcOpType == -1) {
			print_in_error(ERROR_CODE_24);
			return 0;
		}
		str2 = strtok(NULL, " \t\n");
		if (str2 == NULL)
		{
			print_in_error(ERROR_CODE_25); /* Missing argument */
			return 0;
		}

		destOpType = check_get_arg(str2);
		if (destOpType == -1) {
			print_in_error(ERROR_CODE_24);
			return 0;
		}

		command->source = str1;
		command->dest = str2;

		if (extra_text()) {
			print_in_error(ERROR_CODE_26);
			return 0;
		}

	}
	/* Handle opcodes with one argument */
	else if (OPCODES[command->opcode].arg_num == 1)
	{
		str1 = strtok(str, " \t\n");
		if (str1 == NULL)
		{
			print_in_error(ERROR_CODE_25); /* Missing argument */
			return 0;
		}

		destOpType = check_get_arg(str1);
		if (destOpType == -1) {
			print_in_error(ERROR_CODE_24);
			return 0;
		}

		command->dest = str1;

		if (extra_text()) {
			print_in_error(ERROR_CODE_26);
			return 0;
		}
	}

	command->srcOpType = srcOpType;
	command->destOpType = destOpType;

	switch (command->opcode) {
		/* source addressing code is 0,1,2,3 and dest addressing code is 1,2,3 */
		/*case 0 2 3; src: Everything dest: label/ pointer/ registar*/
	case 0:
	case 2:
	case 3:

		if (srcOpType < 0 || srcOpType > 3 || destOpType < 1 || destOpType > 3)
			return 0;

		break;
		/* source addressing code is 0,1,2,3 and dest addressing code is 0,1,2,3 */
		/*case 1; src: Everything dest: Everything*/
	case 1:
		if (srcOpType < 0 || srcOpType > 3 || destOpType < 0 || destOpType > 3)
			return 0;
		break;
		/* source addressing code is 1 and dest addressing code is 1,2,3 */
		/*case 4; src: label dest: label/ ponter/ registar*/
	case 4:
		if (srcOpType != 1 || destOpType < 1 || destOpType > 3)
			return 0;
		break;
		/* dest addressing code is 1,2,3 and no source */
		/*case 5 6 7 8 11; src: nothing dest: label/ ponter/ registar*/
	case 5:
	case 6:
	case 7:
	case 8:
	case 11:
		if (srcOpType != -1 || destOpType < 1 || destOpType > 3)
			return 0;
		break;
		/* dest addressing code is 1,2 and no source */
		/*case 9 10 13; src: nothing dest: label/ ponter*/
	case 9:
	case 10:
	case 13:
		if (srcOpType != -1 || destOpType < 1 || destOpType > 2)
			return 0;
		break;
		/* dest addressing code is 0,1,2,3 and no source */
		/*case 12; src: nothing dest: everything*/
	case 12:
		/* No source and dest already dealt with*/
		if (srcOpType != -1 || destOpType < 0 || destOpType > 3)
			return 0;
		break;
	default:
		print_in_error(ERROR_CODE_24);
		return 0;
	}

	return 1; /*end of swithc*/
}   /*end of function*/

int inc_array(inst_parts* inst, int len)
{
	short* new_ptr;
	new_ptr = realloc(inst->nums, len * sizeof(short));
	if (new_ptr == NULL) {
		print_in_error(ERROR_CODE_1); /* Internal error: Memory allocation failed */
		free(inst->nums);
		return 0;
	}
	inst->nums = new_ptr;
	return 1;
}




int is_data_legal(char* str) {
	int state = 0; /*0: expecting number 1: expecting comma or space*/
	char* ptr;

	if (str == NULL) return 0;
	ptr = str;

	while (*ptr != '\0') {

		if (state == 0) {
			/*Expecting a number (may be negative or with '+' before it)*/

			if (*ptr == ' ') {
				ptr++;
			}
			if (*ptr == '-') {
				ptr++;
			}
			if (*ptr == '+') {
				ptr++;
			}

			if (isdigit(*ptr)) {

				while (isdigit(*ptr)) {
					ptr++;
				}
				state = 1;
			}
			else {

				return 0; /*Found Invalid Charcter*/
			}
		}
		else if (state == 1) {
			/*Need Comma or space*/
			if (*ptr == ',' || *ptr == ' ') {
				ptr++;
				state = 0;
			}
			else {

				return 0; /*Found Invalid Charcter*/
			}

		}
	}

	/*If the last character was part of a valid  number, it's legal*/
	return (state == 1);
}


int add_space_after_colon(char** str) {
	char* colon_ptr = NULL;

	/* Find the first occurrence of a colon in the string */
	colon_ptr = strchr(*str, ':');

	/* If a colon is found in the string */
	if (colon_ptr) {
		/* Check if the character after the colon is already a space */
		if (*(colon_ptr + 1) == ' ') {
			return 1; /* No modification needed */
		}

		/* If the colon is not followed by a space, add a space */
		/* Check if the string is already at its maximum length */
		if (strlen(*str) == MAX_LINE_LENGTH) {
			char* temp_ptr = *str;

			/* Reallocate memory to accommodate one more character */
			*str = realloc(*str, MAX_LINE_LENGTH + 1);
			if (*str == NULL) {
				/* If reallocation fails, print an error message and return 0 */
				print_in_error(ERROR_CODE_1);
				free(temp_ptr);
				return 0;
			}
		}

		/* Move the content after the colon one position to the right to make space for the new character */
		colon_ptr = strchr(*str, ':'); /* Re-find the colon position (in case reallocation changed the pointer) */
		colon_ptr++; /* Move to the character immediately after the colon */
		memmove(colon_ptr + 1, colon_ptr, strlen(colon_ptr) + 1); /* Shift the content to the right */
		*colon_ptr = ' '; /* Insert a space after the colon */

		return 1; /* Modification successful */
	}

	/* Return 1 if no colon was found or no changes were necessary */
	return 1;
}

int capture_nums(char* str, char* token_copy, inst_parts* inst) {
	char* token;
	int len, number;
	len = 0;

	/* Ensure a space follows any colon in the token_copy */
	if (!add_space_after_colon(&token_copy)) {
		return 0;
	}

	/* Check if the first token is ".data" */
	token = strtok(token_copy, " \n");

	/* If the first token is not ".data", skip it and get the next one */
	if (strcmp(token, ".data") != 0) {
		token = strtok(NULL, " \n");
	}

	/* Process each token separated by commas */
	while ((token = strtok(NULL, ", \n")) != NULL) {

		if (is_data_legal(token)) {

			number = (short)(atoi(token));

			if (number > MAX_NUM || number < MIN_NUM) {
				print_in_error(ERROR_CODE_53);
				return 0;
			}
			else if (inc_array(inst, ++len) == 0) {
				return 0;
			}

			inst->nums[len - 1] = number;

		}
		else {

			if (strcmp(token, "\n") == 0) {
				/* comma AFTER the last number */
				print_in_error(ERROR_CODE_51);
				return 0;
			}
			print_in_error(ERROR_CODE_52);
			return 0;
		}
	}

	inst->len = len;

	return 1;
}



int capture_string(char* str, inst_parts* inst) {
	int flag;   /* If at least one char was saved */
	char* token;
	int len;
	int tklength;
	len = inst->len = 0;

	if (str == NULL) {
		return 0;
	}
	token = strchr(str, '"');

	if (token == NULL) {
		print_in_error(ERROR_CODE_54); /* There was no '"' at all*/
		return 0;
	}

	/* Check for the opening double-quote */
	if (*token != '"') {
		print_in_error(ERROR_CODE_54); /* There was no opening*/
		return 0;
	}

	tklength = strlen(token);

	/* Check for the closing double-quote */
	if (token[tklength - 2] != '"') {
		print_in_error(ERROR_CODE_54); /* There was no closing */
		return 0;
	}


	flag = 0;
	token++;
	while (token[len] != '"') {

		/* Save each character as a short integer in the instruction's nums array */
		if (inc_array(inst, ++len) == 0) {
			return 0;
		}
		inst->nums[len - 1] = (short)(token[len - 1]);
		flag = 1;
	}

	/* Check for extra text after the end of the string */
	if (!(token[tklength - 1] != '\0' || token[tklength] != '\n')) {
		/* Extra text after the string end */
		print_in_error(ERROR_CODE_55);
		if (flag == 1) {
			free(inst->nums);
		}
		return 0;
	}
	/* Add '\0' as the end of the string */
	if (inc_array(inst, ++len) == 0) {
		return 0;
	}

	inst->len = len;

	return 1;
}

inst_parts* read_entry_or_extern(char* str) {
	inst_parts* inst;
	char* ptr, * token;

	/* Find the first occurrence of '.' in the string */
	ptr = strchr(str, '.');
	token = strtok(ptr, " ");

	/* Allocate memory for the inst_parts structure */
	inst = check_malloc(sizeof(inst_parts));
	if (inst == NULL) {
		print_in_error(ERROR_CODE_1);
		return NULL;
	}

	/* Initialize inst_parts structure */
	inst->label = NULL;
	inst->nums = NULL;
	inst->is_extern = 0;
	inst->len = 0;

	/* Check if the token is ".extern" and set the is_extern*/
	if (strcmp(token, ".extern") == 0) {
		inst->is_extern = 1;
	}

	/* Tokenize and handle the label */
	token = strtok(NULL, " \n");
	if (legal_label(token)) {
		inst->arg_label = token;
	}
	else {
		print_in_error(ERROR_CODE_23);
	}

	/* Check for extra text after parsing */
	if (extra_text()) {
		print_in_error(ERROR_CODE_26);
	}

	return inst; /* Return the inst_parts structure */
}

void find_opcode_err(char* str) {
	char* c;

	/* Check if a comma is present in the string */
	if ((c = strchr(str, ',')) != NULL) {
		print_in_error(ERROR_CODE_29); /* Comma-related opcode error */
	}
	print_in_error(ERROR_CODE_21); /* Illegal opcode */
}


inst_parts* read_directive(char* str) {
	inst_parts* inst;
	char* token;
	char token_copy[MAX_LINE_LENGTH];

	strcpy(token_copy, str);

	/* Find the first dot if there are dots */
	if (strstr(str, ".") == NULL) {
		return NULL;
	}

	/* Add a space after colon and handle error if needed */
	if (!add_space_after_colon(&str)) {
		return NULL;
	}

	/* Tokenize the string */
	token = strtok(str, " \n");

	/* Allocate memory for the inst_parts structure */
	inst = check_malloc(sizeof(inst_parts) + 1);
	if (inst == NULL) {
		print_in_error(ERROR_CODE_1);
		return NULL;
	}

	/* Initialize the new fields of the inst_parts structure */
	inst->label = NULL;
	inst->nums = NULL;
	inst->len = 0;
	inst->arg_label = NULL;
	inst->is_extern = 0;

	/* Handle different cases based on the token */
	if (legal_label_decl(token)) {
		inst->label = token;
		token = strtok(NULL, " \n");

	}

	/*If we didn't found any label*/
	else if (strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0 || \
		strcmp(token, ".entry") == 0 || strcmp(token, ".extern") == 0) {

		inst->label = NULL;
	}

	/* Check what we found */
	if (strcmp(token, ".data") == 0) {
		capture_nums(token_copy, token_copy, inst);
	}
	else if (strcmp(token, ".string") == 0) {
		capture_string(token_copy, inst);
	}
	else if (strcmp(token, ".entry") == 0) {
		token = strtok(NULL, " \n");
		if (legal_label(token)) {
			inst->label = NULL; /* ignore label in the beginning */
			inst->len = -1; /* not a data line at all */
			inst->arg_label = token;
			inst->nums = 0;
			inst->is_extern = 0;
		}
		else {
			print_in_error(ERROR_CODE_31);
			free(inst);
			return NULL;
		}
	}
	else if (strcmp(token, ".extern") == 0) {
		token = strtok(NULL, " \n");
		if (legal_label(token)) {
			inst->label = NULL; /* ignore label in the beginning */
			inst->len = -1; /* not a data line at all */
			inst->arg_label = token;
			inst->nums = 0;
			inst->is_extern = 1;
		}
	}
	else {
		if (strchr(token, ',')) {
			print_in_error(ERROR_CODE_30); /* Comma after directive */
			return NULL;
		}

		print_in_error(ERROR_CODE_50); /* Illegal data line  */

		return NULL;
	}

	return inst; /* Return the inst_parts structure */
}

command_parts* read_command(char* str) {
	char* token;
	int found_label_decl = 0;
	int found_error = 0;

	/* Allocate memory for the command_parts structure */
	command_parts* command = check_malloc(sizeof(command_parts) + 1);
	if (command == NULL) {
		return command;
	}

	/* Add a space after colon  */
	if (!add_space_after_colon(&str)) {
		return command;
	}
	/* Tokenize the string */
	token = strtok(str, " \n");

	/* Check for a legal label declaration */
	if (legal_label_decl(token)) {

		found_label_decl = 1;
		command->label = token;

		/* Get the opcode and check if it's valid */
		token = strtok(NULL, " \n");

		if ((command->opcode = find_opcode(token)) == -1) {
			find_opcode_err(token);
			found_error = found_error + 1;
			command->opcode = -1;
			return command;
		}

		/* Handle arguments based on the opcode's argument count */
		if (OPCODES[command->opcode].arg_num == 0) {

			if (extra_text()) {
				print_in_error(ERROR_CODE_26);
				found_error = found_error + 1;
			}
			else {
				command->source = command->dest = NULL;
			}
		}
		else {

			token = strtok(NULL, "\n");
			if (legal_arg(token, command) == 0) {

				return command;
			}
		}
	}

	/* Command line with a legal opcode without a label */
	else {
		/*Found an error*/
		if (found_error > 0)
			return command;
	}

	/* Handle the case where no legal_label_decl was visited */
	if (!found_label_decl) {

		if ((command->opcode = find_opcode(token)) != -1) {
			command->label = NULL;
			token = strtok(NULL, "\n");
			legal_arg(token, command);
			return command;
		}
		else {
			find_opcode_err(token);
			command->opcode = -1;
			return command;
		}
	}
	else {
		return command;
	}
}
