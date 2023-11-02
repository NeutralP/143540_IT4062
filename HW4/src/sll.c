#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../lib/sll.h" // Assuming your header file for singly-linked list is in "../lib/sll.h"

// Function to create a singly linked list
void createSinglyLinkedList(singlyLinkedList *list)
{
    list->root = list->prev = list->cur = list->tail = NULL;
}

// Function to delete a singly linked list and free memory
void deleteSinglyList(singlyLinkedList *list)
{
    while (list->root != NULL)
    {
        node *current = list->root;
        list->root = list->root->next;
        free(current);
    }
}

// Function to create a new node
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

// Function to insert a new node at the end of the list
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

// Function to print the element in the last node
void printEndNode(singlyLinkedList *list)
{
    if (list->tail != NULL)
    {
        printf("\n%s %s %d", list->tail->element.username, list->tail->element.password, list->tail->element.status);
    }
}

// Function to write the list to a file
void writeFile(singlyLinkedList *list)
{
    FILE *fp = fopen("sample.txt", "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }

    for (list->cur = list->root; list->cur != NULL; list->cur = list->cur->next)
    {
        fprintf(fp, "%s %s %d\n", list->cur->element.username, list->cur->element.password, list->cur->element.status);
    }

    fclose(fp);
}

// Function to read data from a file and populate the list
void readFile(singlyLinkedList *list)
{
    FILE *fp;
    char username[50], password[50];
    int status;

    fp = fopen("sample.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }

    while (fscanf(fp, "%s %s %d", username, password, &status) != EOF)
    {
        elementtype e;
        strcpy(e.username, username);
        strcpy(e.password, password);
        e.status = status;
        insertEnd(list, e);
    }

    fclose(fp);
}

// Function to search for a user by username and return the node if found
node *searchUserByUsername(singlyLinkedList *list, const char *username)
{
    if (list == NULL || username == NULL)
        return NULL;

    node *current = list->root;

    while (current != NULL)
    {
        if (strcmp(current->element.username, username) == 0)
            return current;
        current = current->next;
    }

    return NULL;
}

// Function to search for a user by username and password and return the node if found
node *searchUser(singlyLinkedList *list, const char *username, const char *password)
{
    // Input validation: Ensure list, username, and password are not NULL and have non-empty values
    if (list == NULL || username == NULL || password == NULL || username[0] == '\0' || password[0] == '\0')
        return NULL;

    node *current = list->root;

    while (current != NULL)
    {
        // Compare the username and password of the current node with the input values
        if (strcmp(current->element.username, username) == 0 &&
            strcmp(current->element.password, password) == 0)
        {
            // User found with matching username and password
            return current;
        }

        current = current->next;
    }

    // User not found, return NULL
    return NULL;
}

// Function to print user
void printUser(node *e)
{
    if (e != NULL)
    {
        printf("\n%s %s %d", e->element.username, e->element.password, e->element.status);
    }
}

// Function to display the entire singly linked list
void displaySinglyLinkedList(singlyLinkedList *list)
{
    if (list->root == NULL)
        return;

    printf("\n-----------------------------------");
    list->cur = list->root;
    do
    {
        printf("\n%s %s %d", list->cur->element.username, list->cur->element.password, list->cur->element.status);
        list->cur = list->cur->next;
    } while (list->cur != NULL);
    printf("\n-----------------------------------");
    fflush(stdout);
}
