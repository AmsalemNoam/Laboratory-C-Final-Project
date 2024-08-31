#ifndef FINAL_PROJECT_SYMBOL_TABLE
#define FINAL_PROJECT_SYMBOL_TABLE

#include "first_pass_helper.h"
#include "globals.h"
#include "code_helper.h"


typedef struct label_address {
	char* label_name;
	int address;
	int assembly_line;
	int data_line;
} label_address;



typedef struct ent_ext_label {
	char* label_name;
	int assembly_line;
} ent_ext_label;


/**
 * @brief Frees the memory allocated for the another_table structure.
 *
 * @param table The another_table structure to be freed.
 * @param count The number of elements in the another_table.
 */
void free_another_table(ent_ext_label* table, int count);


/**
 * @brief Frees the memory allocated for the label_table structure.
 *
 * @param label_table The label_table structure to be freed.
 * @param label_table_line The number of elements in the label_table.
 */
void free_label_table(label_address* label_table, int label_table_line);


/**
 * @brief This function inserts a label entry into the label_table.
 *
 * It reallocates memory for the label_table to accommodate the new label.
 * It updates the is_data_line, address, assembly_line, and label_name fields for the new label entry.
 *
 * @param label_table: Double Pointer to the label_table.
 * @param lines: Number of lines.
 * @param label: The label name.
 * @param counter: The counter value.
 * @param am_file_line: The line in the assembly file.
 * @param is_data_line: Flag indicating whether it is a data label.
 *
 * @return Returns 1 if the label is inserted successfully, or 0 if memory allocation fails.
 */
int insert_label_table(label_address** label_table, int lines, char* label, int counter, int am_file_line, int is_data_line);

/**
 * @brief This function inserts other labels into the another_table.
 *
 * It updates the assembly line and label name for the new label entry in the table.
 * It then reallocates memory for the table to accommodate the new entry.
 *
 * @param table: Double Pointer to the another_table.
 * @param count: Count of labels.
 * @param inst: The inst_parts structure containing the label information.
 * @param am_file_line: The line in the assembly file.
 *
 * @return Returns 1 if the label is inserted successfully, or 0 if memory allocation fails.
 */
int insert_another_labels(ent_ext_label** table, int count, inst_parts* inst, int am_file_line);

/**
 * @brief This function checks each label in the label_table to make sure there are no duplicates.
 *
 * It uses two for loops to compare every two spots in the symbol table if there are two that are the same it show warning
 * and return 0 if there aren't retrun 1
 *
 * @param label_table: Pointer to the label_table.
 * @param table_lines: Number of lines in the label_table.
 * @param file_name: The name of the assembly file.
 *
 * @return Returns 1 if there are no duplicates, or 0 if found a duplicates.
 */
int check_label(label_address* label_table, int table_lines, char* file_name);

/**
 * @brief This function updates the addresses in the label_table for data lines.
 *
 * Check all the spots in the symbol table and if found is_data_line add to IC + 1
 *
 * @param label_table: Pointer to the label_table.
 * @param table_lines: Number of lines in the label_table.
 * @param IC: The instruction counter.
 */
void add_symbol_table_ic(label_address* label_table, int table_lines, int IC);

/**
 * @brief This function adds to the addresses DEFAULT_IC.
 *
 * The function adds to every address DEFAULT_IC (which is equals to 100) to "fix" or "reset" them
 *
 * @param label_table: Pointer to the label_table.
 * @param table_lines: Number of lines in the label_table.
 */
void fix_address(label_address* label_table, int table_lines);

/**
 * @brief This function replaces all the labels
 *
 * @param code pointer to code_help
 * @param label_table pointer to label_address
 * @param label_table_line int
 * @param IC_len int that is the IC lenght
 * @param file_name pointer to char
 *
 * @return
 */
int replace_labels(code_help* code, label_address* label_table, int label_table_line, int IC_len, char* file_name);

/**
 * @brief This function checks if there are externs
 *
 *It checks if there is an extern defined in this file change extern_defined flag to 1 and print out error
 *
 * @param externs: Pointer to the externs table.
 * @param externs_count: Number of externs in the externs table.
 * @param label_table: Pointer to the label table.
 * @param label_table_line: Number of lines in the label table.
 * @param file_name: Name of the file being processed.
 *
 * @return 1 if an extern is defined in the label table, 0 otherwise.
 */
int is_extern_defined(ent_ext_label* externs, int externs_count, label_address* label_table, int label_table_line, char* file_name);

/**
 * @brief This function replaces the externs in the code with the appropriate values.
 *
 * Searches a label if the label is the same as an entry in the table it chaanges short_num to show its extren
 *
 * @param code: Pointer to the code array.
 * @param externs: Pointer to the externs table.
 * @param externs_count: Number of externs in the externs table.
 * @param count: Number of elements in the code array.
 * @param file_name: Name of the file being processed.
 */
void replace_externs(code_help* code, ent_ext_label* externs, int externs_count, int count, char* file_name);

/**
 * @brief This function pastes extren
 *
 * @param code pointer to code_help
 * @param count int
 * @param externs pointer to another_table
 * @param externs_count int
 * @param file_name pointer to char
 *
 * @return 1 if succed 0 if failed
 */
int paste_ext(code_help* code, int count, ent_ext_label* externs, int externs_count, char* file_name);

/**
 * @brief This function pastes entrie
 *
 * @param label_address pointer to label_address
 * @param clabel_table_line int
 * @param entries pointer to another_table
 * @param entries_count int
 * @param file_name pointer to char
 *
 * @return 1 if succed 0 if failed
 */
int paste_ent(label_address* label_table, int label_table_line, ent_ext_label* entries, int entries_count, char* file_name);
#endif
