#ifndef FINAL_PROJECT_PRE_ASSEMBLAR
#define FINAL_PROJECT_PRE_ASSAMBLAR

#include <stdio.h>
#include "link_list_strct.h"



/**
 * @brief Returns an array of illegal macro names.
 *
 * This function returns a pointer to an array of illegal macro names.
 * @return A pointer to an array of illegal macro names.
 */
const char** get_illegal_macro_names();


/**
 * @brief Checks if the macro name is valid.
 *
 * This function checks if the given name is a valid macro name.
 * @param str The input string containing the macro name.
 * @param name A double pointer to a string with the name of the macro read from
 * the input string.
 * @param line_count The line number from which we are reading the macro content
 * in the source file.
 * @param file_name A string with the name of the source file.
 * @return 1 if the name is valid, 0 otherwise.
 */

int is_valid_macro_name(char* str, char** name, int line_count, char* file_name);

char* copy_macr_content(FILE* fp, int* line_count);



/**
 * @brief Adding all the macros to the Linked List
 *
 * This function adds all the macros in the files to the linked list struct that we created.
 *
 * @param fp file pointer to a source file
 * @param pos the position where the content of the macro begins
 * @param line_count a pointer to the line number of the macros content
 * @return a string with the macro content
 */

int macr_list(char* file_name, node** head);

/**
 * @brief Copy Macros Content
 *
 * This function copies the content of a macro
 *
 * @param fp file pointer to a source file
 * @param pos the position where the content of the macro begins
 * @param line_count a pointer to the line number of the macros content
 * @return a string with the macro content
 */

char* copy_macr(FILE* fp, long pos, int* line_count);

/**
 * @brief This Function pastes a macro name with the macro's content.
 *
 * @param str  a string with the content of the line where a macro call is present
 * @param mcro  A pointer to a node that has the macro's name and content.
 * @return A newly allocated string with the first occurrence of the macro name replaced by its content.
 *         If the macro name is not found in the input string or if memory allocation fails, the function returns NULL.
 *
 */

char* paste_macr(char* str, node* mcro);
/**
 * @brief Pastes all macros
 *
 * Pastes all macros in the input file and creates a new file with all the macros content.
 *
 * @param file_name a string with the name of the source file
 * @param head a pointer to head of a linked list where the macros were saved
 * @return a string to the name of a new file after the macros has been expanded
 */

char* paste_all_macrs(char file_name[], node* head);

/**
 * Executes the macro expansion process for the input file.
 *
 * This function performs the macro expansion process on the input file, which includes several steps:
 * 1. Scanning and saving all the macros in the input file in a linked list of macros.
 * 2. Removing the declaration of the macros from the input file and saving the result in a new temp file.
 * 3. Replacing all macro calls with their definitions as saved in the linked list and saving the result in a new file.
 * 4. 	Freeing memory.
 *
 * @param file_name The name of the source file to perform macro expansion on.
 * @return Returns 1 if the macro expansion process is successful, 0 otherwise.
 */

int execute_macr(char file_name[]);


/**
 * @brief Deletes all macro declarations
 *
 * This function deletes all macro declarations from a file and create a new file without them
 *
 * @param file_name a string with the name of the source file
 * @return a string to the name of a new file after the macros definition were deleted
 */

char* remove_mcros_decl(char file_name[]);

#endif
