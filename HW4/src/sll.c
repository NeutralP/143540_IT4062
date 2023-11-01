#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../lib/sll.h"

void createSinglyLinkedList(singlyLinkedList *list)
{
    list->root = list->prev = list->cur = list->tail = NULL;
}

void deleteSinglyList(singlyLinkedList *list)
{
    while (list->root != NULL)
    {
        node *current = list->root;
        list->root = list->root->next;
        free(current);
    }
}

node *makeNewNode(elementtype e)
{
    node *new = (node *)malloc(sizeof(node));
    if (new == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new->element = e;
    new->next = NULL;
    return new;
}

void insertEnd(singlyLinkedList *list, elementtype e)
{
    node *new = makeNewNode(e);
    if (list->root == NULL)
    {
        list->root = list->tail = new;
    }
    else
    {
        list->tail->next = new;
        list->tail = new;
    }
}



void writeFile(singlyLinkedList *list)
{
    // Write file using single linked list
    FILE *fp;
    fp = fopen("sample.txt", "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }
    list->cur = list->root;
    while (list->cur != NULL)
    {
        fprintf(fp, "%s %s %d\n", list->cur->element.username, list->cur->element.password, list->cur->element.status);
        list->cur = list->cur->next;
    }
    fclose(fp);
}

void readFile(singlyLinkedList *list)
{
    // Read file using single linked list
    if (list->root != NULL)
    {
        return;
    }
    FILE *fp;
    elementtype tmp;
    fp = fopen("sample.txt", "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }
    list->cur = list->root;
    while (1)
    {
        int result = fscanf(fp, "%s %s %d", tmp.username, tmp.password, &tmp.status);

        // Check if fscanf successfully read all three values
        if (result == 3)
        {
            insertEnd(list, tmp);
        }
        else
            break;
    }
    fclose(fp);
    printf("\nREAD FILE\n");
    // displaySinglyLinkedList(*list);
}

void displaySinglyLinkedList(singlyLinkedList list)
{
    if (list.root == NULL)
        return;
    printf("\n-----------------------------------");
    list.cur = list.root;
    while (list.cur != NULL)
    {
        printf("\n%s %s %d", list.cur->element.username, list.cur->element.password, list.cur->element.status);
        list.cur = list.cur->next;
    }
    printf("\n-----------------------------------");
}

node *searchUser(singlyLinkedList *list, const char *username, const char *password)
{
    // Search user and return the user if found, null otherwise
    if (list == NULL || username == NULL || password == NULL)
    {
        // Input validation: Ensure list, username, and password are not NULL
        printf("\nsearchUser has something null\n");
        return NULL;
    }

    node *current = list->root;

    while (current != NULL)
    {
        if (strcmp(current->element.username, username) == 0 &&
            strcmp(current->element.password, password) == 0)
        {
            // User found with matching username and password
            return current;
        }
        current = current->next;
    }

    // User not found
    return NULL;
}

node *searchUserByUsername(singlyLinkedList *list, const char *username)
{
    // Search user and return the user if found, null otherwise
    if (list == NULL || username == NULL)
    {
        // Input validation: Ensure list, username, and password are not NULL
        return NULL;
    }

    node *current = list->root;

    while (current != NULL)
    {
        if (strcmp(current->element.username, username) == 0)
        {
            // User found with matching username and password
            return current;
        }
        current = current->next;
    }

    // User not found
    return NULL;
}

void freeList(singlyLinkedList *list)
{
    // Free list
    node *current = list->root;
    while (current != NULL)
    {
        node *tmp = current;
        current = current->next;
        free(tmp);
    }
}


int split(char *buffer, char *only_number, char *only_string)
{

	// Only number in buffer converts to string only_number
	strcpy(only_string, buffer);
	int k = 0;
	strcpy(only_number, buffer);
	int j = 0;

	// if number, copy to only_number
	// if character, copy to only_string
	int m = 0;
	for (int i = 0; i < 100; i++)
	{
		char ch = only_number[i];
		if (ch == '\0')
			break;
		if (ch >= '0' && ch <= '9')
		{
			only_number[j] = ch;
			j++;
		}
		else if ((ch >= 'a' && ch <= 'z') || (ch == ' '))
		{
			only_string[k] = ch;
			k++;
		}
		else
		{
			return 0;
		}
	}
	only_number[j] = '\0';
	only_string[k] = '\0';
	return 1;
}
