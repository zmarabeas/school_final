#include<stdio.h>

int main()
{
	char sum[20];
	int a = 20, b = 40, c;
	c = a + b;
	
	sprintf(sum, "Sum of %d and %d is %d", a, b, c);

	printf("%s \n\n", sum);

	return 0;

}
