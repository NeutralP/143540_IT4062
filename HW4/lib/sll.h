#ifndef SLL_H
#define SLL_H

typedef struct
{
    char username[255];
    char password[255];
    int status;
    char homepage[255];
} elementtype;

/**
 * Singly linked list node
*/
typedef struct node node;
typedef struct node
{
    elementtype element;
    node *next;
} node;

/**
 * Singly linked list
*/
typedef struct singlyLinkedList
{
    node *root;
    node *cur;
    node *prev;
    node *tail;
} singlyLinkedList;

/**
 * Hàm khởi tạo danh sách
*/
void createSinglyLinkedList(singlyLinkedList *list);

/**
 * Hàm xóa danh sách
*/
void deleteSinglyList(singlyLinkedList *list);

/**
 * Hàm tạo node mới
*/
node *makeNewNode(elementtype e);

/**
 * Hàm thêm node vào cuối danh sách
*/
void insertEnd(singlyLinkedList *list, elementtype e);

/**
 * Hàm hiển thị danh sách
*/
void displaySinglyLinkedList(singlyLinkedList *list);

/**
 * Hàm viết file
*/
void writeFile(singlyLinkedList *list);

/**
 * Hàm đọc file
*/
void readFile(singlyLinkedList *list);

/**
 * Hàm tìm kiếm user theo username
*/
node *searchUserByUsername(singlyLinkedList *list, const char *username);

/**
 * Hàm tìm kiếm user theo username và password, return NULL nếu không tìm thấy
*/
node *searchUser(singlyLinkedList *list, const char *username, const char *password);

/**
 * Hàm in ra node cuối cùng
*/
void printEndNode(singlyLinkedList *list);

/**
 * Hàm in ra node hiện tại
*/
void printUser(node *e);

#endif
