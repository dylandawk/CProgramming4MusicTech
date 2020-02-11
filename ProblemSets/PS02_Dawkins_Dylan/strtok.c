#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main (void)
{
	char line[512] = {0,};

	while (true) 
	{
		//prompt user to enter text
		printf("Enter a line of text: \n");
		fgets(line, sizeof(line), stdin);

		//print "bye" & exit program when nothing is entered
		if(line[0] == '\n')
		{
			puts("bye");
			break;
		}

		//find the word before " " and print it
		char *tok = strtok(line, " \t\n");
		while (tok != NULL)
		{
			printf("%s\n", tok);
			tok  = strtok(NULL, " \t\n");
		}
	}
	return 0;
}