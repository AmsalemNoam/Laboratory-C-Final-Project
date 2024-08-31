#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Headers/code_helper.h"
#include "../Headers/Errors.h"
#include "../Headers/globals.h"
#include "../Headers/symbol_table.h"
#include "../Headers/util.h"
#include "../Headers/first_pass_helper.h"

unsigned short command_to_short(command_parts* command) {
	unsigned short res = 0;
	int destOpType = -1;
	int srcOpType = -1;

	/* Check if the source is a numerical value and set the corresponding bits. */
	if (is_num(command->source))
		srcOpType = 0;
	else if (is_pointer(command->source))
		srcOpType = 2;
	else if (find_reg(command->source) >= 0)
		srcOpType = 3;
	else if (legal_label(command->source))
		srcOpType = 1;

	if (is_num(command->dest))
		destOpType = 0;
	else if (is_pointer(command->dest))
		destOpType = 2;
	else if (find_reg(command->dest) >= 0)
		destOpType = 3;
	else if (legal_label(command->dest))
		destOpType = 1;

	res |= command->opcode << OPCODE_BITS_SHIFT;

	if (srcOpType != -1)
		res |= 1 << (SOURCE_BITS_SHIFT + srcOpType);

	if (destOpType != -1)
		res |= 1 << (DEST_BITS_SHIFT + destOpType);

	/* Combine the source, opcode, and destination bits to obtain the final unsigned short representation. */
	return res;
}

unsigned short reg_and_pointer_to_short(command_parts* command)
{
	unsigned short res;
	int regVal;
	res = 4;

	if (command->srcOpType == 2 || command->srcOpType == 3)
	{
		regVal = find_reg(command->source);
		res |= regVal << SOURCE_BITS_SHIFT_REG_POINTER;
	}

	if (command->destOpType == 2 || command->destOpType == 3)
	{
		regVal = find_reg(command->dest);
		res |= regVal << DEST_BITS_SHIFT_REG_POINTER;
	}
	return res;
}

int increse_memory(code_help** code, int size) {
	code_help* new_code;

	/* Attempt to reallocate memory for the code_help array */
	new_code = realloc(*code, size * sizeof(code_help));

	/* Check if memory allocation was successful */
	if (new_code == NULL) {
		/* Memory allocation failed */
		print_in_error(ERROR_CODE_1);
		return 0; /* Indicate failure */
	}

	/* Update the code pointer with the newly allocated memory */
	*code = new_code;

	return 1; /* Indicate success */
}


int add_code_help(code_help** code, unsigned short num, char* str, int* IC, int am_file_line) {
	/* Increase memory allocation for the code_help array if necessary */
	if (!increse_memory(code, *IC + 1)) {
		return 0;  /* Return 0 if memory allocation fails */
	}

	/* Set the numerical value for the current instruction */
	(*code)[*IC].short_num = num;

	/* Set the assembly file line number */
	(*code)[*IC].assembly_line = am_file_line;

	/* Handle label if present */
	if (str == NULL) {
		(*code)[*IC].label = NULL;  /* No label provided; set label to NULL */
	}
	else {
		(*code)[*IC].label = check_malloc((strlen(str) + 1) * sizeof(char));
		if ((*code)[*IC].label == NULL) {
			return 0;  /* Return 0 if memory allocation for the label fails */
		}
		strcpy((*code)[*IC].label, str);  /* Copy the label to the allocated memory */
	}

	return 1;  /* Return 1 to indicate success */
}


int add_more_code_help(code_help** code, command_parts* command, int* IC, int is_src, int am_file_line) {
	unsigned short num;
	char* ptr;
	int opType;
	char* opStr;
	/* Determine the argument based on whether the source or destination operand is being processed */
	if (is_src)
	{
		opType = command->srcOpType;
		opStr = command->source;
	}
	else
	{
		opType = command->destOpType;
		opStr = command->dest;
	}

	/*Check if its a pointer or registar*/
	if (opType == 2 || opType == 3)
	{
		num = reg_and_pointer_to_short(command);
		(*IC)++;
		/* Add the machine code line for the register representation */
		if (add_code_help(code, num, NULL, IC, am_file_line) == 0)
			return 0;
	}
	/* Checks if it's a legal label */
	else if (opType == 1) {
		(*IC)++;

		if (add_code_help(code, 0, opStr, IC, am_file_line) == 0) {
			return 0;
		}
	}

	/* Check if the argument is a numerical value */
	else if (opType == 0) {
		(*IC)++;

		/* representing number in bits so move from the ARE bits */
		if (add_code_help(code, strtol(opStr + 1, &ptr, 10) << ARE_BITS, NULL, IC, am_file_line) == 0) {
			return 0;
		}
	}

	/* Dobe Successfully  */
	return 1;

}

int add_data_bin_code(code_help** data, inst_parts* inst, int* counter, int am_file_line) {
	int i;
	int inst_len;

	/* Check if inst pointer and inst->nums are valid (not NULL) */
	if (inst == NULL || inst->nums == NULL) {

		return 0;
	}

	inst_len = inst->len;


	/* Check if memory allocation for the code_help array succeeded */
	if (increse_memory(data, *counter + inst_len) == 0) {

		return 0;
	}

	for (i = 0; i < inst_len; i++) {
		(*data)[*counter].short_num = inst->nums[i];
		(*data)[*counter].label = NULL;
		(*data)[*counter].assembly_line = am_file_line;
		(*counter)++;
	}

	return 1;
}

int merge_data(code_help** code, code_help* data, int IC, int DC) {
	int i;
	code_help* ptr;
	ptr = *code;

	/* Check if memory allocation for the code_help array succeeded */
	if (increse_memory(code, IC + DC + 1) == 0) {
		free(ptr);
		free(data);
		return 0;
	}
	/* Coping the info from the data lines into the end of the command code lines */
	for (i = 0; i < DC; i++) {
		(*code)[IC + i + 1].label = data[i].label;
		(*code)[IC + i + 1].assembly_line = data[i].assembly_line;
		(*code)[IC + i + 1].short_num = data[i].short_num;
	}
	free(data); /* No need anymore for the code from the data */
	return 1; /* Return 1 to indicate successful merge of machine code and data */
}

void free_code(code_help* code, int code_count) {
	int i;

	/* Free every label memory */
	for (i = 0; i <= code_count; i++) {
		if (code[i].label != NULL) {
			free(code[i].label);
		}
	}
	/* Free memory for the entire code_help array */
	free(code);
}

