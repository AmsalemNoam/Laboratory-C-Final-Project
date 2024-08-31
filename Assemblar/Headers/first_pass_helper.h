#ifndef FINAL_PROJECT_FIRST_PASS_HELPER
#define FINAL_PROJECT_FIRST_PASS_HELPER

/* Represents an operation code with its associated opcode and argument count */
typedef struct op_code {
	char* opcode;    /* The opcode name */
	int arg_num;     /* The number of arguments for the opcode */
} op_code;

/* Represents different parts of a command */
typedef struct command_parts {
	char* label;     /* The label with the command */
	int opcode;      /* The opcode of the command */
	char* source;    /* The source operand of the command*/
	char* dest;      /* The destination operand of the command */
	int srcOpType;
	int destOpType;
} command_parts;

/* Represents different parts of a data(.data or .string) lines */
typedef struct inst_parts {
	char* label;       /* The label associated with the instruction */
	short* nums;       /* An array of short integers of the instruction data */
	int len;           /* The number of data in *nums  */
	char* arg_label;   /* The label with the argument, if there is one */
	int is_extern;     /* If its extern it will equal 1 if its not 0 */
} inst_parts;

/**
 * @brief This function returns the index of the opcode in the opcode list.
 *
 * @param str A pointer to a character string of the opcode to check.
 * @return Returns an int that is the index of the opcode in the `OPCODES` array.
 *         If the string does not match any opcode or is NULL, it returns -1.
 **/
int find_opcode(char* str);

/**
 * @brief This function returns the index of the register in the register list.
 *
 * @param str A pointer to a character string of the register to check.
 *
 * @return Returns an int that is the index of the opcode in the `REGS` array.
 *         If the string does not match any opcode or is NULL, it returns -1.
 */
int find_reg(char* str);

/**
 * @brief Checks if the string is an instruction.
 *
 * This function compares the string with a list of instructions.
 * If we found it, the function will return 1. If not found, it returns 0.
 *
 * @param str A pointer to a character string of the instruction to check.
 *
 * @return Returns an integer index. If we found the instruction, it returns 1.
 *          if the string does not match any instruction or is NULL, it returns 0.
 */
int find_instr(char* str);

/**
 * @brief This function checks if the provided string is a valid label declaration.
 *
 * This function checks if the given label declaration don't surpass rules:
 * 1. The string is not be NULL.
 * 2. The first character of the label must be alphabetic.
 * 3. The total length of the label must be less than or equal to MAX_LABEL_LENGTH.
 * 4. The label can't be a register or an opcode.
 * 5. After the first character, the label can contain alphabetic or numeric characters.
 * 6. The label must end with a ':' character, followed by a null character '\0'.
 *
 * If all these conditions are good, it returns 1. Otherwise, it returns 0.
 *
 * @param str A pointer to a character string of the label to check.
 *
 * @return Returns 1 If the string is a valid label declaration, Otherwise, it returns 0.
 */
int legal_label_decl(char* str);

/**
 * @brief This function checks if the provided string is a valid label.
 *
 * This function checks the given string as per the rules:
 * 1. The string mustn't be NULL.
 * 2. The first character of the label must be alphabetic.
 * 3. The total length of the label must be less than or equal to MAX_LABEL_LENGTH.
 * 4. The label must not be an opcode or an instruction.
 * 5. After the first character, the label can contain alphabetic or numeric characters until a space character or a null character '\0' is found.
 *
 * If all the conditions met it returns 1. Otherwise, it returns 0.
 *
 * @param str A pointer to a character string of the label to check.
 *
 * @return Returns 1 If the string is a good, Otherwise, it returns 0.
 */

int legal_label(char* str);

/**
 * @brief Checks if its a legel number
 *
 * @param *str pointer to the string that checks
 * @return 1 if its a number 0 if its not
 */

int is_num(char* str);

/**
 * @brief Checks if its a legel pointer
 *
 * @param *str pointer to the string that checks
 * @return 1 if its a number 0 if its not
 */

int is_pointer(char* str);

/**
 * @brief This function counts how many times a given character appears in a string.
 *
 * @param str A pointer to the character string in which the character is to be counted.
 * @param ch The character whose occurrences are to be counted.
 *
 * @return Returns an integer representing the number of times the character appears in the string.
 */
int occurrence_count(char* str, char ch);

/**
 * @brief This function checks if there white space in the str
 *
 * @param str A pointer to the character string
 *
 * @return Returns 1 if there is white space 0 if no
 */

int has_white_space(char* str);

/**
 * @brief This function checks what error there is in the registar
 *
 *This function print out what error there is in the registar
 *
 * @param str A pointer to the character string
 */
void find_reg_error(char* str);

/**
 * @brief This function checks if the first argument is reg label or num or pointer
 *
 * @param *str A pointer to the character string
 * @param *ptr A pointer to the first space in the arguments
 *
 * @return Return using the function "is_num_or_label_or_reg_or_pointer"
 */
int check_get_arg(char* str);

/**
 * @brief This function checks what error there is in the opcode
 *
 *This function print out what error there is in the opcode
 *
 * @param str A pointer to the character string
 */
void find_opcode_err(char* str);

/**
 * @brief This function checks if the str is num or label or registar or pointer
 *
 * @param str A pointer to the character string
 */
int getOpType(char* str);

/**
 * @brief This function checks if the str is a legel arguement
 *
 * The function is checking how many arguement there is in the line and deal with it
 * and checks what opcode is it and depdens on the opcode there is it deals with it and change the command_parts
 *
 * @param str A pointer to the character string
 * @param command A pointer to the command_parts
 *
 * @return 0 if the arg is not legal 1 if the arg is legal
 */
int legal_arg(char* str, command_parts* command);


/**
 * @brief This function increse the size of array and reallocate memory for it
 *
 * @param inst A double pointer to the character inst_parts
 * @param len A int that is the size of the array
 *
 * @return 0 if failed 1 if succeed
 */
int inc_array(inst_parts* inst, int len);
 /**
  * @brief This function checks if the data given is legal
  *
  * @param str A pointer that is the string to check
  *
  * @return 0 if not legal 1 if legal
  */
int is_data_legal(char* str);

/*
 * Function: add_space_after_colon
 * -------------------------------
 * This function checks for the presence of a colon (':') in the given string.
 * If a colon is found, it ensures that there is a space immediately following it.
 * If the string length is at its maximum allowed length, the function reallocates memory
 * to accommodate the additional space character.
 *
 * Parameters:
 * @param str: A pointer to the string (char*) where the colon may need to be followed by a space.
 *        The string is passed as a pointer to a pointer (char**) because it may need to be
 *        reallocated within the function.
 *
 * Returns:
 * @return 1 if the modification was successful or if no modification was necessary.
 * - 0 if memory reallocation fails.
 */
int add_space_after_colon(char** str);

/**
 * @brief This function take all the numbers
 *
 * @param str A pointer to the string
 * @param token_copy A string that is declared in another function used just for this function
 * @param inst A inst_parts to access nums and len
 *
 * @return 0 if failed 1 if succeed
 */
int capture_nums(char* str, char* token_copy, inst_parts* inst);

/**
 * @brief This function take all the string
 *
 * @param str A pointer to the string
 * @param inst A inst_parts to access nums and len
 *
 * @return 0 if failed 1 if succeed
 */
int capture_string(char* str, inst_parts* inst);

/**
 * @brief This function checks if the str is extren or entry if it is extren activte in inst_parts "is_extren" and change the inst_parts accordinly
 *
 * @param str A pointer to the string
 *
 * @return NULL if there is a problem, inst if there are no problems
 */
inst_parts* read_entry_or_extern(char* str);

/**
 * @brief This function reads and search for an instructions
 *
 * @param str A pointer to the string
 *
 * @return NULL if there is a problem, inst if there are no problems
 */
inst_parts* read_directive(char* str);

/**
 * @brief This function reads and search for a the opcodes and change the command_parts accordinly
 *
 * @param str A pointer to the string
 *
 * @return command
 */
command_parts* read_command(char* str);

#endif
