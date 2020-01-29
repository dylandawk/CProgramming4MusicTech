#include<stdio.h>
#include<stdbool.h>

int main(void)
{
	//prints the size (in bytes) of each of the following types and values
	printf("sizeof char: %zu\n", sizeof(char));
	printf("sizeof int: %zu\n", sizeof(int));
	printf("sizeof short: %zu\n", sizeof(short));
	printf("sizeof long: %zu\n", sizeof(long));
	printf("sizeof long long: %zu\n", sizeof(long long));
	printf("sizeof bool: %zu\n", sizeof(bool));
	printf("sizeof unsigned int: %zu\n", sizeof(unsigned int));
	printf("sizeof float: %zu\n", sizeof(float));
	printf("sizeof double: %zu\n", sizeof(double));
	printf("sizeof long double: %zu\n", sizeof(long double));
	printf("sizeof 12345: %zu\n", sizeof(12345));
	printf("sizeof 123456789012: %zu\n", sizeof(123456789012));
	printf("sizeof 3.14: %zu\n", sizeof(3.14));
	printf("sizeof 3.14f: %zu\n", sizeof(3.14f));
	printf("sizeof 'hello': %zu\n", sizeof("hello"));
}