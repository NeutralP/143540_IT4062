#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../lib/sll.h"
typedef struct
{
    char username[255];
    char password[255];
    int status;
    char homepage[255];
} elementtype;

/*
    Khai báo singly linked list
*/
/*
    Singly linked list node
*/
typedef struct node node;
typedef struct node
{
    elementtype element;
    node *next;
} node;

/*
    Singly linked list
*/
typedef struct singlyLinkedList
{
    node *root;
    node *cur;
    node *prev;
    node *tail;
} singlyLinkedList;

/*
    Hàm khởi tạo sll
*/
void createSinglyLinkedList(singlyLinkedList *list)
{
    list->root = list->prev = list->cur = list->tail = NULL;
}

/*
    Hàm xóa sll
*/
void deleteSinglyList(singlyLinkedList *list)
{
    while (list->root != NULL)
    {
        node *current = list->root;
        list->root = list->root->next;
        free(current);
    }
}

/*
    Tạo node mới
*/
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

/*
    Thêm vào cuối sll
*/
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

/*
    Hiển thị ds
*/
void displaySinglyLinkedList(singlyLinkedList list)
{
    if (list.root == NULL)
        return;
    printf("\n-----------------------------------");
    list.cur = list.root;
    while (list.cur != NULL)
    {
        printf("\n%s %s %d %s", list.cur->element.username, list.cur->element.password, list.cur->element.status, list.cur->element.homepage);
        list.cur = list.cur->next;
    }
}
singlyLinkedList list;
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
        fprintf(fp, "%s %s %d %s\n", list->cur->element.username, list->cur->element.password, list->cur->element.status, list->cur->element.homepage);
        list->cur = list->cur->next;
    }
    fclose(fp);
}


void readFile()
{
    // Read file using single linked list
    if (list.root != NULL)
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
    list.cur = list.root;
    while (1)
    {
        int result = fscanf(fp, "%s %s %d %s", tmp.username, tmp.password, &tmp.status, tmp.homepage);

        // Check if fscanf successfully read all three values
        if (result == 4)
        {
            insertEnd(&list, tmp);
        }
        else
            break;
    }
    fclose(fp);
    printf("\nREAD FILE\n");
    displaySinglyLinkedList(list);
}

node *searchUser(singlyLinkedList *list, const char *username, const char *password)
{
    // Search user and return the user if found, null otherwise
    if (list == NULL || username == NULL || password == NULL)
    {
        // Input validation: Ensure list, username, and password are not NULL
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
