#ifndef SLL_H
#define SLL_H

typedef struct
{
    char username[255];
    char password[255];
    int status;
} elementtype;

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

/**
 *    Hàm khởi tạo sll
 */
void createSinglyLinkedList(singlyLinkedList *list);

/**
 *    Hàm xóa sll
 */
void deleteSinglyList(singlyLinkedList *list);

/**
 *    Tạo node mới
 */
node *makeNewNode(elementtype e);

/**
 *    Thêm vào cuối sll
 */
void insertEnd(singlyLinkedList *list, elementtype e);

/**
 *    Hiển thị ds
 */
void displaySinglyLinkedList(singlyLinkedList list);

/**
 *  Viết vào file nguoidung.txt
 */
void writeFile(singlyLinkedList *list);

/**
 *  Đọc file nguoidung.txt
 */
void readFile();

/**
 *  Tìm kiếm người dùng có tài khoản và mật khẩu
 */
node *searchUser(singlyLinkedList *list, const char *username, const char *password);

/**
 *  Tìm kiếm người dùng chỉ cần tên tài khoản
 */
node *searchUserByUsername(singlyLinkedList *list, const char *username);

/**
 * Split string to number and string
 */
int split(char *buffer, char *only_number, char *only_string);

#endif