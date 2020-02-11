#define _FORTIFY_SOURCE 0
#include <stdio.h>
#include <string.h>

int main (void)
{
	const int rows = 16;
	int pascal[rows][rows];

	for(int i = 0; i < rows; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			if(i == 0) //first row condition 
			{
				if( j == 0) pascal[i][j] = 1;
				else pascal[i][j] = 0;
			} 
			else if (j == 0)//first index condition
			{
				pascal[i][j] = 1;
			}
			else pascal[i][j] = pascal[i-1][j-1] + pascal[i-1][j];
			if(pascal[i][j] != 0)
			{
				if(pascal[i][j] %2 == 1) // if number is odd
				{
					printf("*");
				} else printf(" ");	
			}
		}
		printf("\n");
	}

	return 0;
}