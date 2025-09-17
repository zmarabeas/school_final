/* example of using fflush(stdin) */


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
	char str[20];
	int i;
	for (i = 0; i < 2; i++){
		scanf("%[^\n]s", str);
		printf("%s\n", str);
		fflush(stdin);
		sleep(1);
	}
	return 0;
}

