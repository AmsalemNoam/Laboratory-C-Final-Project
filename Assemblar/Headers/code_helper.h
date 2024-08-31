#ifndef FINAL_PROJECT_CODE_HELPER
#define FINAL_PROJECT_CODE_HELPER

#include "first_pass_helper.h"
#include "globals.h"

typedef struct code_help {
	unsigned short short_num; /* The numerical representation of the converted line */
	char* label; /* An optional label associated with the line */
	int assembly_line; /* The corresponding assembly line number */
	int is_external;
} code_help;



/**
 * @brief This function converts a command_parts structure to an unsigned short representing the machine code.
 *
 * Look into bit representations of the dest/source/opcode.
 * Then the converted values are combined to the final machine code,
 * which is returned as an unsigned short.
 *
 * @param command: A pointer to the command_parts structure containing the command information.
 *
 * @return Returns an unsigned short representing the machine code of the command.
 */
unsigned short command_to_short(command_parts* command);



/**
 * @brief This function converts registers or pointers operands to their bit representation in the machine code.
 *
 * If register_src is false and the done flag is not set, the function converts only the destination register
 * to its bit representation and returns the value.
 * The done flag is used to ensure that the function won't do it twice
 *
 * @param command: A pointer to the command_parts structure containing the command information.
 * @param register_src: Checks if both source and destination registers should be converted.
 * @return Returns the bit representation of the register/s and/or pointer/s in the machine code.
 */
unsigned short reg_and_pointer_to_short(command_parts* command);

 /**
  * @brief This function reallocate the memory for a code_help array.
  *
  * @param code: A pointer to a pointer to the code_help array.
  * @param counter: The desired new size of the array.
  *
  * @return Returns 1 if the memory allocation was successful, and 0 otherwise.
  */
int increse_memory(code_help** code, int counter);

/**
 * Function: add_code_help
 * -----------------------
 * @brief Adds a new entry to the `code_help` array, which stores machine code information.
 *
 * Parameters:
 * @param  code - A double pointer to the array of `code_help` structures.
 * @param  num - The numerical value representing the assembly instruction.
 * @param  str - A string representing the label, if any. If no label is present, this should be NULL.
 * @param  IC - A pointer to the instruction counter, representing the current position in the `code_help` array.
 * @param  am_file_line - The line number in the assembly file corresponding to the instruction.
 *
 * Returns:
 * @return  1 on success, indicating the `code_help` structure was added successfully.
 *   0 on failure, indicating an error occurred.
 */
int add_code_help(code_help** cod, unsigned short num, char* str, int* IC, int am_file_line);

/**
 * @brief This function adds more machine code lines to the code_help array.
 *
 * This function converts the optional second and third code words from a command line and stores them in the array.
 *
 * @param code a double pointer to the array
 * @param command a pointer to a command_parts struct
 * @param IC the machine code counter
 * @param is_src 1 if the the converted information originates from the source part of the command, 0 if from the target
 * @param am_file_line int which represnete the line in the file
 *
 * @return 1 if the conversion and store of the machine word succeeded, 0 if not
 */
int add_more_code_help(code_help** code, command_parts* command, int* IC, int is_src, int am_file_line);

/**
 * @brief This function adds machine code data to the data array and increse DC if needed.
 *
 * @param data: A double pointer to the code_help array.
 * @param inst: The inst_parts structure containing the data values.
 * @param DC: A pointer to DC variable.
 * @param am_file_line the number of the line
 *
 * @return Returns 1 if data was added successfully, and 0 if failed.
 */
int add_data_bin_code(code_help** data, inst_parts* inst, int* DC, int am_file_line);

/**
 * @brief This function merges the code data arrays into a single code array.
 *
 * It increse the code array to save the data array and copies the data
 * into the code array starting from the IC position.
 *
 * @param code: A double pointer to the code_help array.
 * @param data: The code_help data array to be merged.
 * @param IC: The instruction counter (IC).
 * @param DC: The data counter (DC).
 * @return Returns 1 if the merge was successful, and 0 otherwise.
 */
int merge_data(code_help** code, code_help* data, int IC, int DC);

/**
 * @brief This function frees the memory allocated for the code_help array.
 *
 * @param code: The code_help code array to be freed.
 * @param code_count: The number of things in the array.
 */
void free_code(code_help* code, int code_count);

#endif
