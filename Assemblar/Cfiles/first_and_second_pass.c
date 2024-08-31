#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Headers/Errors.h"
#include "../Headers/globals.h"
#include "../Headers/util.h"
#include "../Headers/first_and_second_pass.h"
#include "../Headers/first_pass_helper.h"
#include "../Headers/code_helper.h"

/*
 * Executes the first pass of the assembler on the provided assembly file.
 * This pass includes reading the file, parsing labels, commands,
 * and data instructions, and building initial structures for the second pass.
 * Returns 0 if successful, or 1 if any errors are found.
 */

int execute_first_pass(char* file_name) {

    /* Variables for instruction counter, data counter, label table line, extern and entry counters */
    int IC, DC, label_table_line, ext_number, ent_number;
    int isGuideLine = 1; /* Flag to indicate if a new guidance/directive line is found */
    int found_error = 0; /* Flag indicating if any error is found */
    int am_file_line;
    char* am_file_name;
    code_help* code, * data;
    ent_ext_label* externs;
    ent_ext_label* entries;
    label_address* label_table;
    command_parts* command;
    inst_parts* inst = NULL;
    char str[MAX_LINE_LENGTH];
    char str_copy[MAX_LINE_LENGTH];
    FILE* fp;
    int regBinVal;

    /* Initialize instruction counter and data counter */
    IC = 0;
    DC = 0;

    /* Check if any line in the file exceeds the maximum length */
    if (long_lines(file_name) == 1 || long_lines(file_name) == -1) {
        found_error = 1;
    }

    /* Open the assembly file */
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        /* Failed to open file */
        print_in_error(ERROR_CODE_8);
        found_error = 1;
    }

    am_file_name = file_name;
    am_file_line = 0;

    /* Initialize label table and counters */
    label_table_line = ext_number = ent_number = 0;
    label_table = NULL;

    /* Allocate memory for externs, entries, code, and data tables */
    found_error = allocate_first_pass(&externs, &entries, &code, &data);

    /* Start reading the file line by line until end of file or maximum instruction count */
    while (fgets(str, MAX_LINE_LENGTH, fp) != NULL && IC + DC <= MAX_MEM - DEFAULT_IC) {
        am_file_line++;
        isGuideLine = 0;

        /* Skip empty lines */
        if (strcmp(str, "\n") == 0) {
            continue;
        }

        /* Skip comment lines */
        if (str[0] == ';') {
            continue;
        }

        /* Handle lines containing instructions or directives */
        if (strchr(str, '.')) {

            strcpy(str_copy, str);

            /* Handle .data or .string directives */
            if (strstr(str_copy, ".data") != NULL || strstr(str_copy, ".string") != NULL) {
                isGuideLine = 1;
                inst = read_directive(str_copy);

                /* Insert label into label table if present */
                if (inst->label != NULL) {
                    insert_label_table(&label_table, ++label_table_line, inst->label, DC, am_file_line, 1);
                }

            }
            /* Handle .entry or .extern directives */
            else if (strstr(str_copy, ".entry") || strstr(str_copy, ".extern")) {

                inst = read_entry_or_extern(str_copy);

                /* Handle .entry directive */
                if (inst->is_extern == 0) {
                    insert_another_labels(&entries, ++ent_number, inst, am_file_line);
                }
                /* Handle .extern directive */
                else if (inst->is_extern == 1) {
                    insert_another_labels(&externs, ++ext_number, inst, am_file_line);
                }
            }

            /* If an error is found, print the error and skip the instruction */
            if (found_error != 0) {
                print_ext_error(ERROR_CODE_50, am_file_name, am_file_line);
                if (isGuideLine) {
                    free(inst);
                }
                continue;
            }

            /* Process the data if it's a directive line */
            if (isGuideLine) {
                if (add_data_bin_code(&data, inst, &DC, am_file_line) != 1) {
                    found_error = 1;
                    continue;
                }
                /* Free the instruction structure after processing */
                if (inst->nums) {
                    free(inst->nums);
                }
                free(inst);
            }
        }
        /* Handle lines containing commands */
        else {
            command = read_command(str);

            if (found_error == 0) {
                IC++;

                /* Insert label into label table if present */
                if (command != NULL && command->label != NULL) {
                    insert_label_table(&label_table, ++label_table_line, command->label, IC, am_file_line, 0);
                }
            }
            else {
                free(command);
                continue;
            }

            /* Add command to the code help structure */
            if (add_code_help(&code, command_to_short(command), NULL, &IC, am_file_line) == 0) {
                free(command);
                found_error = 1;
                continue;
            }

            /* Add additional code help structures */
            if ((command->srcOpType == 2 || command->srcOpType == 3) &&
                (command->destOpType == 2 || command->destOpType == 3)) {
                regBinVal = reg_and_pointer_to_short(command);
                IC++;
                /* Add the machine code line for the register representation */
                if (add_code_help(&code, regBinVal, NULL, &IC, am_file_line) == 0) {
                    return 0;
                }
            }
            else if (add_more_code_help(&code, command, &IC, 1, am_file_line) == 0 ||
                add_more_code_help(&code, command, &IC, 0, am_file_line) == 0) {
                free(command);
                found_error = 1;
                continue;
            }

            /* Free the command structure */
            free(command);
        }
    }

    printf("Start Second Pass\n");

    /* Execute the second pass. If it fails, set found_error to 1 */
    if (execute_second_pass(file_name,
        label_table,
        IC,
        DC,
        label_table_line,
        ext_number,
        ent_number,
        code,
        data,
        externs,
        entries,
        found_error) == 0) {
        found_error = 1;
    }

    /* Close the file pointer */
    fclose(fp);

    /* Return 1 if any errors were found, otherwise return 0 */
    return found_error;
}


int execute_second_pass(char* file_name, label_address* label_table,
    int IC, int DC, int label_table_line, int externs_count,
    int entries_count, code_help* code, code_help* data, ent_ext_label* externs,
    ent_ext_label* entries, int found_error) {

    /* Validate if the Instruction Counter (IC) exceeds the maximum allowable memory size */
    if (IC > MAX_MEM) {
        print_in_error(ERROR_CODE_35);
        found_error = 1;
    }

    /* Ensure each label in the label table is unique and not duplicated */
    if (!check_label(label_table, label_table_line, file_name)) {
        found_error = 1;
    }

    /* Ensure that no extern labels are improperly defined in the assembly file */
    if (!is_extern_defined(externs, externs_count, label_table, label_table_line, file_name)) {
        found_error = 1;
    }

    /* Adjust the data segment addresses in the label table based on the final IC */
    add_symbol_table_ic(label_table, label_table_line, IC);

    /* Update the addresses in the label table after combining the code and data segments */
    fix_address(label_table, label_table_line);

    /* Combine the code and data arrays into a single machine code array */
    if (!merge_data(&code, data, IC, DC)) {
        found_error = 1;
    }

    /* Replace all extern label references in the code with their appropriate machine code (short_num = 1) */
    replace_externs(code, externs, externs_count, IC + DC, file_name);

    /* Replace all label references in the code array with their corresponding addresses from the label table */
    if (replace_labels(code, label_table, label_table_line, IC, file_name)) {
        found_error = 1;
    }

    /* If no errors were encountered, proceed to generate the output files */
    if (!found_error) {
        /* Convert the assembled machine code to octal format and write it to the output file */
        write_in_octal(code, IC + DC, file_name, IC, DC);

        /* Write the extern label references and their addresses to the '.ext' output file */
        paste_ext(code, IC + DC, externs, externs_count, file_name);

        /* Write the entry label references and their addresses to the '.ent' output file */
        paste_ent(label_table, label_table_line, entries, entries_count, file_name);
    }

    /* Free all allocated memory and resources used during the second pass */
    free_project(code, label_table, entries, externs, IC + DC, label_table_line, entries_count, externs_count);

    /* Return 1 if the second pass completes without errors, otherwise return 0 */
    return !found_error;
}
