#include "nshHeader.h"

//Handles all of the actions on the main user input.
void shell_actions(char *usrInput)
{
	//For storing arguments entered into the shell.
	char *temp = NULL;
	char *arg1 = NULL;
	char *arg2 = NULL;
	char *arg3 = NULL;
	char temp2[256];

	//Removes everything in the string after the "~" is located.
	if (strpbrk(usrInput, "~"))
		strcpy(usrInput, strtok(usrInput, "~"));
//issues
	temp = (char*)calloc(strlen(usrInput), sizeof(char));
	strcpy(temp, usrInput);

	//Checks if line includes command separators, tokenates string, runs command strings.
	if (strpbrk(usrInput, "?"))
	{
		strtok(usrInput, "?");
		temp = strtok(NULL, "\0");
		temp[strlen(temp)] = '\0';
		shell_actions(usrInput);
		shell_actions(temp);
		return;
	}

	//Tokenate user input into arguments for passing to functions.
	arg1 = strtok(usrInput, " \n\0");
	arg2 = strtok(NULL, " \n\0");
	arg3 = strtok(NULL, "\0");

//	printf("\targ1: %s\n\targ2: %s\n\targ3: %s\n", arg1, arg2, arg3);

	if (arg1 == NULL)
		return;

	//Loop through alias list to look for values.
	while(1)
	{
		if (!nshFind(arg1, aliasHead))
			break;
		arg1 = nshFind(arg1, aliasHead);
	}

	//If arg1 is a complex command or alias value, recreate new usrInput and pass to shell.
	if (strpbrk(arg1, " "))
	{
		if (arg2 != NULL)
		{
			strcat(arg1, " ");
			strcat(arg1, arg2);
			
			if (arg3 != NULL)
			{
				strcat(arg1, " ");
				strcat(arg1, arg3);
			}
		}

		strcpy(temp2, arg1);
		temp2[strlen(temp2)] = '\0';

		free (temp);
		temp = NULL;

		shell_actions(temp2);
		return;
	}
		  
	//Check first argument string with set of internal commands.
	if (strcmp(arg1,"set") == 0)
		set_alias(arg2, arg3, &varHead);
	else if (strcmp(arg1,"tes") == 0)
		tes_saila(arg2, &varHead);
	else if (strcmp(arg1,"alias") == 0)
		set_alias(arg2, arg3, &aliasHead);
	else if (strcmp(arg1,"saila") == 0)
		tes_saila(arg2, &aliasHead);
	else if (strcmp(arg1,"echo") == 0)
		echo(temp, &aliasHead, &varHead);
	else if (strcmp(arg1,"where") == 0)
	{
		temp = where(arg2, arg3, &aliasHead, nshFind((char*)"PATH", varHead));
		if (temp != NULL)
			printf("%s\n", temp);
	}
	else if (strcmp(arg1,"exit") == 0)
		exit (0);
	else
		ext_cmd(temp, nshFind((char*)"PATH", varHead));
}