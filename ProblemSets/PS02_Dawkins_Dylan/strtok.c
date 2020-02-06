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

		//find the word before a " " and print it
		char* word = strtok(line, " ");
		while (word != NULL)
		{
			printf("%s\n", word);
			word  = strtok(NULL, " ");
		}
	}
	return 0;
}