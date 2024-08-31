#ifndef FINAL_PROJECT_LINK_LIST
#define FINAL_PROJECT_LINK_LIST


/* Node */
typedef struct node {
    char *name;         /* Name associated with the node */
    char *content;      /* Content stored in the node */
    int line;           /* Line number associated with the content */
    struct node *next;  /* Pointer to the next node in the linked list */
} node;
/**
 * @brief Creates a new node.
 *
 * @param name The name of the macro(string)
 * @param content The content inside the macro (excluding endmcro)
 * @param line_num the line number in the source file where the macro was defined
 * @return a new node of a macro
 */
node *create_node(char *name, char *content, int line_num);

/**
 * @brief Searches for a node in a linked list.
 *
 * This function searches for a name of a spesific macro in the list
 * @param head a pointer to the head of the list of macros
 * @param name The name of the macro we are searching(String)
 * @param found a pointer to a number that indicates if the macro was found or not
 * @return the node of the macro if it was found, NULL if not
 */
node *search_list(node *head, char *name);

/**
 * @brief Adds a node to a linked list.
 *
 * This function adds a new node of a macro to the end of the linked list
 * @param head a pointer to the head of the list of macros
 * @param name a string with the name of the new macro
 * @param content a string with the content of the new macro
 * @param line_num the line number in the source file where the macro was defined
 */
void add_to_list(node **head, char *name, char *content, int line_num);

/**
 * @brief Frees memory allocated for a node and its contents.
 *
 * This function deallocates memory from a node of a macro and the content of it
 * @param node a pointer to the node to be deallocated
 */
void free_node(node *node);

/**
 * @brief Frees the memory allocated for all the list.
 *
 * This function deallocates memory from the entire linked list
 * @param a pointer to the head of the list of macros
 */
void free_list(node *head);

#endif
