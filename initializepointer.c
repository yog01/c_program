#include<stdio.h>
#include<string.h>
#include<math.h>
int main()
{

	int t=0;
	const char *p="hello";

	printf(p);
	printf("\n");
	for(t=strlen(p)-1;t>-1;t--)
	{
		printf("%c",p[t]);
	}


	return 0;
}