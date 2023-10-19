#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../lib/domain_ip.h"
#include "../lib/sll.h"



elementtype signinUser;

int retry = 0;
enum
{
    ACTIVATION_CODE = 20200650
};

int isFirstCharacterDigitOrLetter(const char *str) {
    if (str == NULL || str[0] == '\0') {
        // Handle empty or NULL input
        return 0; // You can choose an appropriate error code or value
    }

    if (isdigit(str[0])) {
        return 1; // First character is a digit
    } else if (isalpha(str[0])) {
        return 0; // First character is a letter
    } else {
        return 0; // Handle other characters (non-alphanumeric)
    }
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
        printf("\nEnter your homepage: ");
        scanf("%s", reguser.homepage);

        reguser.status = 2;
        node *newNode = makeNewNode(reguser);
        printf("\nSuccessful registration. Activation required.\n");
        fclose(fp);

        // Append to EOF
        fp = fopen("sample.txt", "a");
        insertEnd(&list, reguser);
        fprintf(fp, "\n%s %s %d %s", reguser.username, reguser.password, reguser.status, reguser.homepage);
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
                return;
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

void homepageDomain() {
    if(strlen(signinUser.username) == 0) {
        printf("\nSign in first!");
        return;
    }
    if(!isFirstCharacterDigitOrLetter(signinUser.homepage)) {
        get_ip(signinUser.homepage);
    }
    else 
        printf("\n%s not found information\n", signinUser.homepage);
}

void homepageIP() {
    if(strlen(signinUser.username) == 0) {
        printf("\nSign in first!");
        return;
    }
    if(isFirstCharacterDigitOrLetter(signinUser.homepage) && ip_valid(signinUser.homepage)) {
        get_info_ip_address(signinUser.homepage);
    }
    else
        printf("\nNot found information\n");
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
        printf("7. Homepage with domain name\n");
        printf("8. Homepage with IP address\n");
        printf("Your choice (1-8, other to quit):\n");
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
        case '7':
            homepageDomain();
            retry = 1;
            break;
        case '8':
            homepageIP();
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