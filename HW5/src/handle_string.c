#include<ctype.h>
#include<string.h>
#include"../lib/handle_string.h"

// Break the password down to alphabet and num, return null if special character / invalid password, return 1 if valid 0 if invalid
int breakPassword(char *buffer, char *only_string, char *only_number)
{
    // Only number in buffer converts to string only_number
	strcpy(only_string, buffer);
	int k = 0;
	strcpy(only_number, buffer);
	int j = 0;

	// if number, copy to only_number
	// if character, copy to only_string (CAPITAL INCLUDED)
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
		else if ((ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z'))
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