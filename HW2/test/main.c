#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../lib/domain_ip.h"

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

int main(int argc, char *argv[]){
	if(argc!=3){
		printf("Incorrect format!\nCorrect format: ./resolver argv[1] argv[2]\nargv[1]: 1 or 2\nargv[2]: ip(1) or domain(2) depending on argv[1]\n");
		return 0;
	}
	else{
		char *parameter1 = argv[1];
		char *input =argv[2];

        if(isFirstCharacterDigitOrLetter(input) && strcmp(argv[1], "1") == 0) {
           if (ip_valid(input)==1) 
			get_info_ip_address(input);
			else {
				printf("Not found information\n");
				return 0;
			}
        }
		else if (ip_valid(input)==0 && strcmp(argv[1], "2") == 0)
		{
			get_ip(input);
		}
		else
		{
			printf("Wrong parameter\n");
		}
	}
	return 0;
}
