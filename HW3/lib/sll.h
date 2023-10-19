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

extern singlyLinkedList list;
void createSinglyLinkedList(singlyLinkedList *list);

void deleteSinglyList(singlyLinkedList *list);

node *makeNewNode(elementtype e);

void insertEnd(singlyLinkedList *list, elementtype e);

void displaySinglyLinkedList(singlyLinkedList list);

void writeFile(singlyLinkedList *list);

void readFile();

node *searchUser(singlyLinkedList *list, const char *username, const char *password);

node *searchUserByUsername(singlyLinkedList *list, const char *username);


