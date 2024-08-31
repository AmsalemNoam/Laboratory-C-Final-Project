#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "../Headers/Errors.h"

/* This is array for error codes numbers and the error message.
 *  I left some blank for future error codes.
*/

Error errors[] = {
	/*Errors 1-19 are macro/general errors*/
	{ERROR_CODE_0, "No Error"},
	{ERROR_CODE_1, "Faild Allocating Memory"},
	{ERROR_CODE_2, "File .as name Is Missing"},
	{ERROR_CODE_3, "File Name Too Long"},
	{ERROR_CODE_4, "Name of file Doesn't Exist"},
	{ERROR_CODE_5, "File .am is Missing"},
	{ERROR_CODE_6, "Line Too Long"},
	{ERROR_CODE_7, "Failed Opening New File For Writing"},
	{ERROR_CODE_8, "Failed Opening File For Reading"},
	{ERROR_CODE_9, "Macro Without Name"},
	{ERROR_CODE_10, "Extra Text after macro Name"},
	{ERROR_CODE_11, "Failed Setting a position in a file"},
	{ERROR_CODE_12, "The endmcro has extra text"},
	{ERROR_CODE_13, "Macro Has More Than One Definition"},
	{ERROR_CODE_14, "Failed To Copy The File While macros expansion"},
	{ERROR_CODE_15, "Failed macros expansion in .as File"},
	{ERROR_CODE_16, "Illegal name for macro"},
	/* Skip for future macro/general errors */
	{-1,            ""},
	{-1,            ""},
		{-1,            ""},
		/* Errors 20-49 are related to reading commands */
		{ERROR_CODE_20, "Illegal opcode"},
		{ERROR_CODE_21, "Illegal argument"},
		{ERROR_CODE_22, "Illegal register name."},
		{ERROR_CODE_23, "Illegal label declaration"},
		{ERROR_CODE_24, "Illegal argument"},
		{ERROR_CODE_25, "Missing argument"},
		{ERROR_CODE_26, "Line has extra text"},
		{ERROR_CODE_27, "Missing ',' between arguments"},
		{ERROR_CODE_28, "Too much ','"},
		{ERROR_CODE_29, "Illegal ',' in opcode"},
		{ERROR_CODE_30, "Wrong place for ','"},
		{ERROR_CODE_31, "Illegal label for .entry"},
		{ERROR_CODE_32, "Label defined more than once"},
		{ERROR_CODE_33, "Label defined as .extern and defined in file"},
		{ERROR_CODE_34, "Label not defined in the assembly file"},
		{ERROR_CODE_35, "IC is too big"},
		{ERROR_CODE_36, ""},
		{ERROR_CODE_37, ""},
		{ERROR_CODE_38, ""},
		{ERROR_CODE_39, ""},
		/* Skip for future Command reading errors */
		{-1,            ""},
			{-1,            ""},
			{-1,            ""},
			{-1,            ""},
			{-1,            ""},
			{-1,            ""},
			{-1,            ""},
			{-1,            ""},
			{-1,            ""},
			{-1,            ""},
			/*Errors 50-59 are related to reading instruction .data or .string lines errors */
			{ERROR_CODE_50, "Illegal '.data' instruction"},
			{ERROR_CODE_51, "Comma in the end of '.data' instruction"},
			{ERROR_CODE_52, "Not a number in '.data' instruction"},
			{ERROR_CODE_53, "Number is out of range for '.data' instruction"},
			{ERROR_CODE_54, "Missing '\"' after '.string'"},
			{ERROR_CODE_55, "Extra text after the string end in '.string' line"},
			{ERROR_CODE_56, "Null String"},
			{ERROR_CODE_57, ""},
			{ERROR_CODE_58, ""},
			{ERROR_CODE_59, "NOTICEMENOTICEMENOTICEMENOTICEME"},
};

/*This is for problems in the file we are checking*/
void print_in_error(int error_code) {
	/* Print the error code number and the error message to stdout */
	printf("ERROR ID:%d In The File | %s\n", error_code, errors[error_code].error_msg);
}
/*This is for problems if we created another file or such*/
void print_ext_error(int error_code, char* file_name, int line_number) {
	/* Print the error code number, file name, assembly line number and the error message to
	stdout*/
	printf("Error %d: In This File '%s' at line %d | %s\n", error_code, file_name, line_number, errors[error_code].error_msg);
}
