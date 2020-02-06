#define _FORTIFY_SOURCE 0
#include <stdio.h>
#include <string.h>

int main (void)
{
	char src[24] = "C Programming is so fun";
	char dest[16];

	strncpy(dest, src, sizeof(dest)-1);	

	printf("dest: %s\n", dest);
	printf("src: %s\n", src);

	return 0;
}	