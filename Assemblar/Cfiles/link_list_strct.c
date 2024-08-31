#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Headers/link_list_strct.h"
#include "../Headers/util.h"
#include "../Headers/Errors.h"

/* Function to create a new node in the linked list */
node* create_node(char* name, char* content, int line_num) {
	node* temp;

	/* Check if memory allocation for the node succeeded */
	temp = check_malloc(sizeof(node));

	temp->name = name;        /* Set the name of the node */
	temp->content = content;  /* Set the content string of the node */
	temp->line = line_num;    /* Set the line number associated with the content */
	temp->next = NULL;        /* Initialize the next pointer to NULL */

	return temp;  /* Return a pointer to the newly created node */
}

/* Function to search for a node in the linked list by name */
node* search_list(node* head, char* name) {
	node* current = head;
	int found;
	/* Check if the head is NULL */
	if (head == NULL) {
		return NULL;
	}

	/* Traverse the list to find the node with the given name */
	while (current != NULL) {
		found = 0; /* Initialize found to 0 */
		if ((strcmp(current->name, name) == 0) && (found == 0)) {
			found = 1;  /* Node found, set found to 1 */
			printf("Node %s already exists in the list\n", name);
			return current;  /* Return the found node */
		}
		current = current->next;
	}
	return NULL;  /* Return NULL if node is not found */
}

/* Function to add a new node to the linked list */
void add_to_list(node** head, char* name, char* content, int line_num) {
	int found = 0;  /* Initialize found to 0 */
	node* new_node;
	node* temp;
	node* current;

	/* Search for the macro name in the list */
	temp = search_list(*head, name);

	/* If the name is found but the content is different, skip this macro */
	if (found && strcmp(temp->content, content) != 0) {
		print_in_error(ERROR_CODE_13);
		free(name);
		free(content);
		return;
	}

	/* Create a new node */
	new_node = create_node(name, content, line_num);

	/* If the list is empty, set the new node as the head */
	if (*head == NULL) {
		*head = new_node;
		return;
	}

	/* Traverse to the end of the list and add the new node */
	current = *head;
	while (current->next != NULL) {
		current = current->next;
	}
	current->next = new_node;
}

void free_node(node* node1) {
	/* Free memory allocated for the name, content and node */
	free(node1->name);
	free(node1->content);
	free(node1);
}

void free_list(node* head) {
	/* Free memory for the whole list */
	while (head != NULL) {
		node* temp = head;
		head = head->next;
		free_node(temp);
	}
}
