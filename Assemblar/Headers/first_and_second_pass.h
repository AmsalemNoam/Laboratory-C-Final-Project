#ifndef FINAL_PROJECT_FIRST_AND_SECOND_PASS
#define FINAL_PROJECT_FIRST_AND_SECOND_PASS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "Errors.h"
#include "globals.h"
#include "util.h"
#include "first_and_second_pass.h"
#include "first_pass_helper.h"
#include "code_helper.h"
#include "symbol_table.h"


/*
 * Function: execute_first_pass
 * ----------------------------
 * This function performs the first pass of the assembler process.
 * It processes the assembly file line by line, handling instructions,
 * directives, labels, and comments. The function builds the label table,
 * and counts the instructions and data while identifying any errors.
 *
 * Parameters:
 * @param file_name: The name of the assembly file to be processed (without extension).
 *
 * Returns:
 * @return 1 if any errors are found during the first pass.
 * - 0 if no errors are found.
 */
int execute_first_pass(char* file_name);


/*
 * Function: execute_second_pass
 * -----------------------------
 * This function performs the second pass of the assembler process. 
 * It finalizes the machine code by resolving labels, handles externs and entries, 
 * and generates the required output files in the correct format.
 * 
 * Parameters:
 * @param file_name: The name of the assembly file being processed (without extension).
 * @param label_table: The table containing all labels and their addresses.
 * @param IC: Instruction Counter, representing the total number of machine code instructions.
 * @param DC: Data Counter, representing the total number of data words.
 * @param label_table_line: The number of entries in the label table.
 * @param externs_count: The number of extern labels in the assembly file.
 * @param entries_count: The number of entry labels in the assembly file.
 * @param code: The array holding the machine code.
 * @param data: The array holding the data section of the machine code.
 * @param externs: The array containing information about extern labels.
 * @param entries: The array containing information about entry labels.
 * @param found_error: A flag indicating whether any errors were encountered during the first pass.
 * 
 * Returns:
 * @return 1 if the second pass completes successfully without errors.
 * - 0 if errors are found during the second pass.
 */
int execute_second_pass(char* file_name, label_address* label_table, int IC, int DC,
	int label_table_line, int externs_count, int entries_count, code_help* code,
	code_help* data, ent_ext_label* externs, ent_ext_label* entries, int error_found);

#endif

