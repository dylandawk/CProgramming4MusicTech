#define _FORTIFY_SOURCE 0
#include <stdio.h>
#include <string.h>

int main (void)
{
	int pascal[33][33] = {0};
	pascal[0][0] = 1;
	for(int i = 1; i <= 32; i++)
	{
		for (int j = 1; j <= 32; j++)
		{
			pascal[i][j] = pascal[i-1][j-1] + pascal[i-1][j];
			if(pascal[i][j] != 0)//skip 0 values
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