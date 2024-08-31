#ifndef FINAL_PROJECT_UTIL
#define FINAL_PROJECT_UTIL

#include "symbol_table.h"
#include "code_helper.h"

/**
 * @brief Check if malloc didn't failed
 * This function use malloc and cheks the errors that might occur
 * @param memory_size the amount of memory to allocate
 * @return a void pointer of the allocated memory, or NULL if the allocation failed
 */
void *check_malloc(long memory_size);


/**
 * @brief Created new file and change the ending.
 * This function creates a new file name by replacing the extension of the provided file name.
 * If the original file name does not have an extension, the new ending is simply appended.
 * @param file_name The original file name as a string.
 * @param ending The new file extension or ending to be added.
 * @return A string containing the new file name with the updated extension.
 */
char *concatStrings(char *file_name, char *ending);

/**
 * @brief Copies the contents of one file to another.
 *
 * This function reads from the source file line by line and writes each line to the destination file.
 *
 * @param num_args The number of arguments
 * @param ... pairs of arguments with the file or string identifier
 * @return 1 if the copying process succeeded, 0 if not.
 */
void close_files(int num_args, ...);


/**
 * @brief Alloctes Strings.
 *
 * This function gets a string and allocate memory for it and checks if it failed.
 *
 * @param source The string
 * @return dest which is the allocted string
 */

char* allocate_string(char* source);


/**
 * @brief Checks if there extra text.
 *
 * @return 0 No extra text, 1 There is extra text
 */

int extra_text();


/**
 * Handles memory allocation for data structures used in the first pass.
 *
 * @param externs A double pointer to the externs table.
 * @param entries A double pointer to the entries table.
 * @param code A double pointer to the code_help struct.
 * @param data A double pointer to the code_help struct for data.
 *
 * @return An integer 0 if done succsesfully 1 if something failed
 */
int allocate_first_pass(ent_ext_label **externs, ent_ext_label **entries, code_help **code, code_help **data);

/**
 * @brief This function checks if any line in the file is longer than the maximum lenght.
 *
 * if it encounters a line longer than `MAX_LINE_LENGTH`,
 * it prints an external error using `ERROR_CODE_6`
 *
 * @param file_name A pointer to a character string that is the file name.
 *
 * @return An int: 0 if all lines are shorter than the max lenght and 1 if there is a line longer than it -1 if the file did not open
 */
int long_lines(char *file_name);

/**
 * @brief This function frees all the memory from the project
 *
 * @param code A code_help struct to free
 * @param label_table A label_address to free
 * @param entries A pointer to entries to free
 * @param externs A pointer ro externs to free
 * @param code_count int the number of code struct
 * @param label_table_line int the number of lines to free
 * @param entries_count int the number of entries
 * @param externs_count int the number of externs
 *
 * @return An int: 0 if all lines are shorter than the max lenght and 1 if there is a line longer than it -1 if the file did not open
 */
void free_project(code_help *code, label_address *label_table, ent_ext_label *entries, ent_ext_label *externs, int code_count, int label_table_line, int entries_count, int externs_count);

/**
* Convert the binary code to octal format and write it to the output file with the end ".ob"
*
* @param code        A pointer to the code_help array containing the binary code that represent to assembler code.
* @param count       The total number of instructions and data in the 'code' array.
* @param file_name   The name of the source file being processed (used for creating output files).
* @param IC          The Instruction Counter.
* @param DC          The Data Counter.
* @return Returns 1 if the conversion and writing to the output file are successful, 0 otherwise.
*/
int write_in_octal(code_help *code, int count, char *file_name, int IC, int DC);

#endif

