#ifndef FINAL_PROJECT_ERRORS
#define FINAL_PROJECT_ERRORS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* Represents an error ID and Message*/
typedef struct Error {
	int error_num;     /* Identifier for Errors */
	char* error_msg;  /* Error message With The Error */
} Error;


/* Represents error codes for various types of errors */
typedef enum ERROR_CODES {
	/*Errors 1-19 are macro/general errors*/
	ERROR_CODE_0 = 0,
	ERROR_CODE_1,
	ERROR_CODE_2,
	ERROR_CODE_3,
	ERROR_CODE_4,
	ERROR_CODE_5,
	ERROR_CODE_6,
	ERROR_CODE_7,
	ERROR_CODE_8,
	ERROR_CODE_9,
	ERROR_CODE_10,
	ERROR_CODE_11,
	ERROR_CODE_12,
	ERROR_CODE_13,
	ERROR_CODE_14,
	ERROR_CODE_15,
	ERROR_CODE_16,
	ERROR_CODE_17,
	/* Skip for future macro/general errors */

	/* Errors 20-49 are related to reading command assembly lines */
	ERROR_CODE_20 = 20,
	ERROR_CODE_21,
	ERROR_CODE_22,
	ERROR_CODE_23,
	ERROR_CODE_24,
	ERROR_CODE_25,
	ERROR_CODE_26,
	ERROR_CODE_27,
	ERROR_CODE_28,
	ERROR_CODE_29,
	ERROR_CODE_30,
	ERROR_CODE_31,
	ERROR_CODE_32,
	ERROR_CODE_33,
	ERROR_CODE_34,
	ERROR_CODE_35,
	ERROR_CODE_36,
	ERROR_CODE_37,
	ERROR_CODE_38,
	ERROR_CODE_39,
	/* Skip for future Command reading Errors */

	/*Errors 50-59 are related to reading instruction .data or .string lines errors */
	ERROR_CODE_50 = 50,
	ERROR_CODE_51,
	ERROR_CODE_52,
	ERROR_CODE_53,
	ERROR_CODE_54,
	ERROR_CODE_55,
	ERROR_CODE_56,
	ERROR_CODE_57,
	ERROR_CODE_58,
	ERROR_CODE_59
} ERROR_CODES;



/**
 * @brief Prints an external error message.
 *
 * This function prints to stdout an error that is related to an error in the source file
 * with error number and the error location in the source file.
 *
 * @param error_code The error number.
 * @param file_name The name of the file where the error occurred.
 * @param line_number The line number in the file where the error occurred.
 */
void print_ext_error(int error_code, char* file_name, int line_number);


/**
 * @brief Prints an internal error message.
 *
 * This function prints to stdout an error that is a result of an Error In This File and not in The Source File
 * @param error_code This is the error number
 */
void print_in_error(int error_code);

#endif
