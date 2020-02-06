#define _FORTIFY_SOURCE 0
#include <stdio.h>
#include <string.h>

int main (void)
{
	int pascal[16][16];
	pascal[0][0] = 1;
	for(int i = 1; i <= 16; i++)
	{
		for (int j = 1; j <= 16; j++)
		{
			pascal[i][j] = pascal[i-1][j-1] + pascal[i-1][j];
			if(pascal[i][j] != 0)
			{
				printf("%5d", pascal[i][j]);	
			}
		}
		printf("\n");
	}

	return 0;
}