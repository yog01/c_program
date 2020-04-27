#include<stdio.h>


int main(){

int a[5]={1,2,4};
int *p=a;
a++; //lvaue us reuquired
p++;
printf("%d",*p);
return 0;
}
