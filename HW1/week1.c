#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int retry = 0;
enum
{
    ACTIVATION_CODE = 20200650
};
typedef struct
{
    char username[255];
    char password[255];
    int status;
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
        printf("\n%s %s %d", list.cur->element.username, list.cur->element.password, list.cur->element.status);
        list.cur = list.cur->next;
    }
}

singlyLinkedList list;
elementtype signinUser;
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
        int result = fscanf(fp, "%s %s %d", tmp.username, tmp.password, &tmp.status);

        // Check if fscanf successfully read all three values
        if (result == 3)
        {
            insertEnd(&list, tmp);
        }
        else
            break;
    }
    fclose(fp);
    // displaySinglyLinkedList(list);
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

void reg()
{
    elementtype reguser;
    FILE *fp;
    printf("\nEnter your username: ");
    scanf("%s", reguser.username);

    elementtype tmp;

    if ((fp = fopen("sample.txt", "r+")) != NULL)
    {
        fseek(fp, 0, SEEK_SET);

        while (feof(fp) == 0)
        {

            fscanf(fp, "%s %s %d", tmp.username, tmp.password, &tmp.status);
            if (strcmp(tmp.username, reguser.username) == 0)
            {
                printf("\nUsername already taken!\nReenter your username: ");
                scanf("%s", reguser.username);
                fseek(fp, 0, SEEK_SET);
            }
        }

        printf("\nEnter your password: ");
        scanf("%s", reguser.password);
        reguser.status = 2;
        node *newNode = makeNewNode(reguser);
        printf("\nSuccessful registration. Activation required.\n");
        fclose(fp);

        // Append to EOF
        fp = fopen("sample.txt", "a");
        insertEnd(&list, reguser);
        fprintf(fp, "\n%s %s %d", reguser.username, reguser.password, reguser.status);
        fclose(fp);
    }
    else
    {
        // File not found, create a new file and add the user to the linked list
        fp = fopen("sample.txt", "a");
        printf("\nEnter your password: ");
        scanf("%s", reguser.password);
        reguser.status = 2;
        fprintf(fp, "\n%s %s %d", reguser.username, reguser.password, reguser.status);
        fclose(fp);

        // Add the new user to the linked list
        insertEnd(&list, reguser);
    }
}

void act()
{
    // Activate account (by changing integer active from 0 to 1) in elementtype's status in linked list
    readFile();
    elementtype tmp;
    int tmp_retry = 0;
    int tmp_code;
    do
    {
        printf("\nUsername: ");
        scanf(" %s", tmp.username);
        printf("\nPassword: ");
        scanf(" %s", tmp.password);
        printf("\nCode: ");
        scanf(" %d", &tmp_code);

        // Use the searchUser function to find the user
        node *foundUser = searchUser(&list, tmp.username, tmp.password);

        if (foundUser != NULL && tmp_code == ACTIVATION_CODE)
        {
            // Check if the user is found and the code is correct
            printf("Account found. Activating...\n");
            // Activate the account by changing status from 0 to 1
            foundUser->element.status = 1;
            writeFile(&list);
            printf("Account activated successfully.\n");
            break;
        }
        else
        {
            printf("Account not found or activation code is incorrect.\n");
            tmp_retry++;
        }
    } while (tmp_retry < 4);
    if (tmp_retry == 4)
    {
        printf("Activation code is incorrect. The account has been blocked.\n");
        node *blockUser = searchUser(&list, tmp.username, tmp.password);
        if (blockUser != NULL)
        {
            blockUser->element.status = 0;
        }
        else
        {
            printf("Account not found.\n");
        }
        writeFile(&list);
    }
}

void signin()
{
    readFile();
    int tmp_retry = 0;
    elementtype tmp;
    do
    {
        printf("\nEnter your username: ");
        scanf("%s", tmp.username);
        node *foundUser = searchUserByUsername(&list, tmp.username);

        // If found user
        if (foundUser == NULL)
        {
            printf("User not found.\n");
            return; // Exit the function if the user is not found
        }

        printf("\nEnter your password: ");
        scanf("%s", tmp.password);
        if (strcmp(tmp.password, foundUser->element.password) == 0)
        {
            if (foundUser->element.status == 1)
            {
                printf("Hello %s", foundUser->element.username);
                signinUser = foundUser->element;
            }
            else if (foundUser->element.status == 0)
            {
                printf("Your account has been blocked.\n");
                return;
            }
            else if (foundUser->element.status == 2)
            {
                printf("Your account has not been activated yet.\n");
                return;
            }
        }
        else
        {
            printf("Sign in failed.\n");
            tmp_retry++;
        }
    } while (tmp_retry < 3);
    if (tmp_retry == 3)
    {
        printf("Sign in failed. Account is blocked.\n");
        node *blockUser = searchUserByUsername(&list, tmp.username);
        if (blockUser != NULL)
        {
            blockUser->element.status = 0;
        }
        else
        {
            printf("Account not found.\n");
        }
        writeFile(&list);
    }
}

void search()
{
    readFile();
    char tmp_username[255];
    printf("\nUsername: ");
    scanf("%s", tmp_username);

    node *foundUser = searchUserByUsername(&list, tmp_username);
    if (foundUser == NULL)
        printf("Cannot found account.\n");
    if (foundUser->element.status == 1)
        printf("Account is active.\n");
    else if (foundUser->element.status == 0)
        printf("Account is blocked.\n");
    else if (foundUser->element.status == 2)
        printf("Account is not activated yet.\n");
}

void changepass()
{
    readFile();
    elementtype tmp;
    char tmppass[255];
    printf("\nEnter your username: ");
    scanf("%s", tmp.username);
    node *foundUser = searchUserByUsername(&list, tmp.username);

    // If found user
    if (foundUser == NULL)
    {
        printf("User not found.\n");
        return; // Exit the function if the user is not found
    }

    printf("\nEnter your password: ");
    scanf("%s", tmp.password);

    printf("\nEnter your new password: ");
    scanf("%s", tmppass);
    if (strcmp(tmp.password, foundUser->element.password) == 0)
    {
        if (foundUser->element.status == 1)
        {
            strcpy(foundUser->element.password, tmppass);
            writeFile(&list);
            printf("\nPassword is changed");
        }
        else if (foundUser->element.status == 0)
        {
            printf("Your account has been blocked.\n");
            return;
        }
        else if (foundUser->element.status == 2)
        {
            printf("Your account has not been activated yet.\n");
            return;
        }
    }
    else
    {
        printf("\nCurrent password is incorrect. Please try again");
    }
}

void signout()
{
    char tmpusername[255];
    printf("%s", signinUser.username);
    if (strcmp(signinUser.username, "") == 0)
    {
        printf("You are not signed in.\n");
        return;
    }
    printf("\nUsername: ");
    scanf("%s", tmpusername);
    if (tmpusername == signinUser.username)
    {
        printf("\nGoodbye %s", signinUser.username);
    }
    else if (searchUserByUsername(&list, tmpusername) != 0)
    {
        printf("\nUser doesn't exist");
    }
    else
    {
        printf("\nAccount is not sign in");
    }
}

void printScreen()
{
    int retry = 0;
    do
    {
        createSinglyLinkedList(&list);
        char choice;
        printf("\n\n\n\n");
        printf("USER MANAGEMENT PROGRAM\n");
        printf("-----------------------------------\n");
        printf("1. Register\n");
        printf("2. Activate\n");
        printf("3. Sign in\n");
        printf("4. Search\n");
        printf("5. Change password\n");
        printf("6. Sign out\n");
        printf("Your choice (1-6, other to quit):\n");
        scanf(" %c", &choice);
        switch (choice)
        {
        case '1':
            reg();
            retry = 1;
            break;

        case '2':
            act();
            retry = 1;
            break;

        case '3':
            signin();
            retry = 1;
            break;

        case '4':
            search();
            retry = 1;
            break;

        case '5':
            changepass();
            retry = 1;
            break;

        case '6':
            signout();
            retry = 1;
            break;

        default:
            retry = 0;
            break;
        }
    } while (retry == 1);
}
int main()
{
    printScreen();
    deleteSinglyList(&list);
    return 0;
}