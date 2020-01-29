#include<stdio.h>
#include<stdbool.h>

int main(void)
{
	printf("%s\n", 2 ? "true" : "false");
	printf("%s\n", 3 ? "true" : "false");
	printf("%s\n", 3.14 ? "true" : "false");
	printf("%s\n", 0.0 ? "true" : "false");
	printf("%s\n", 0.0f ? "true" : "false");
	printf("%s\n", "hello" ? "true" : "false");

}