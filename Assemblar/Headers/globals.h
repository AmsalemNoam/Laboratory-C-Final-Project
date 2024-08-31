#ifndef FINAL_PROJECT_GLOBALS
#define FINAL_PROJECT_GLOBALS


/*The File contain all the global values in the program*/


/* Maximum length of a single command line including \n */
#define MAX_LINE_LENGTH 81

/* All the illegal macro names  */
#define ILLEGAL_MACRO_NAMES_COUNT 30

/* Maximum length of a Lable */
#define MAX_LABEL_LENGTH 31

/* Maximum size of IC */
#define MAX_MEM 4095 

/* Number of opcode */
#define OPCODES_COUNT 16

/* Number of registers */
#define REG_COUNT 8

/* Number of Instructions */
#define INSTRUCTIONS_COUNT 4

/*Word max len(bits)*/
#define WORD_LEN 15

/*Max Number possible with number of bits*/
#define MAX_NUM ((1 << (WORD_LEN-1)) - 1)

/*Min Number possible with number of bits*/
#define MIN_NUM (-(1 << (WORD_LEN-1)))

/* Default size of IC */
#define DEFAULT_IC 100 

/* number of shift left needed to reach the dest operand 3-6 */
#define DEST_BITS_SHIFT 3 

/* number of shift left needed to reach the source operand 7-10 */
#define SOURCE_BITS_SHIFT 7 

/* number of shift left needed to reach the opcode 11-14 */
#define OPCODE_BITS_SHIFT 11 

/* A,R,E for addressing methods values */
#define DIRECT_ADDRESSING 0
#define LABEL_ADDRESSING 1
#define POINTER_ADDRESSING 2
#define REG_ADDRESSING 3

/* number of shift left needed to reach the source register operand 6-8 */
#define SOURCE_BITS_SHIFT_REG_POINTER 6 

/* number of shift left needed to reach the dest register operand 3-5 */
#define DEST_BITS_SHIFT_REG_POINTER 3 

/* Value that shows we already dealt with both registers or pointers to avoid doing it twice */
#define DOUBLE_REGS_VALUE 9999

/*The number of ARE bits*/
#define ARE_BITS 3

#endif







