/*
 * getmaxlengthofline.c
 *
 *  Created on: Jul 31, 2022
 *      Author: ubuntu
 */
#include <stdio.h>
#include <string.h>
#define MAXLENGTH 100
#define TABSPACE 4
#define TABSPACEREDUCE(x) (TABSPACE -(x%TABSPACE))

#define debugstring(x) printf("debug %s",x)
#define debugno(x) printf("debug %d\n",x)

#define NOBLANK -1
#define STOP 1
#define SIZEINDEX 2
#define STARTINDEX 0
#define ENDINDEX 1

int get_length(char temp[],int length);
void copy_line(char to[],char from[]);
void remov_space(char *temp_arr);
void reversee(char *temp_arr,int len);
void detab(char *temp_arr,int len);
void entab(char *temp_arr,int len);
void tabatpos(char *temp_arr,int len,char *temp_tab);
int fold(char *temp_arr,int len,int start, int end,char *temp_arrfold);
void printfoldword(char *temp_space,int blankspacesize,int *t_space);
//void foldonbl(char *temp_arr,char *temp_tab,int tpossptbnl);
//int findposnltbsp(char *temp_arr,int len);
int main()
{
	char arr[MAXLENGTH]={'\0'};
	char linearr[MAXLENGTH]={'\0'},tabposline[MAXLENGTH]={'\0'};
	int noofcharacter,maxcharacter;
	int start=0,end=0;
	int space[SIZEINDEX]={0};
	int blankspacesize=0;
	noofcharacter=maxcharacter=0;

	while((noofcharacter=get_length(arr, MAXLENGTH))>0)
	{
		if(noofcharacter>maxcharacter && noofcharacter>1)
		{
			maxcharacter=noofcharacter;
			copy_line(linearr,arr);
		}
	}
	//tabatpos(linearr,maxcharacter,tabposline);
	end=maxcharacter;
	blankspacesize=fold(arr,maxcharacter,start,end,linearr);
	printfoldword(linearr,blankspacesize,space);
#ifndef STOP
	while(start<maxcharacter)
	{
	start=space[ENDINDEX];
	//debugno(space[ENDINDEX]);
	//debugno(end);
	blankspacesize=fold(arr,maxcharacter,start,end,linearr);
	printfoldword(linearr,blankspacesize,space);
	printf("\n");
	}
#endif
	//remov_space(linearr);
	//reversee(linearr,maxcharacter);
	//detab(linearr,maxcharacter);
	//entab(linearr,maxcharacter);
	printf("\norignal %s \n changed %s \n",arr,linearr);
	printf("No of character %d\n",maxcharacter);

	return 0;
}
/*arr->linearr*/

int fold(char *temp_arr,int len,int start,int end,char *temp_space)
{
#ifndef STOP
	int i,c;

	i=c=0;
	for(i=0;i<len;i++)
	{
		//calculate length reduce space in tab according to position + no of character
		c+=((temp_arr[i]=='\t')?TABSPACEREDUCE(i):1);
		//debugno(c);
	}
	//foldonbl(temp_arr,temp_tab,len);
#endif

	int i=start,spaceindex=0,blankspacesize=0;

	while(i<end && temp_arr[i]!='\0')
	{

		if(temp_arr[i]==' ')
		{
			temp_space[spaceindex++]=i+1;
		}
		i++;
	}
	debugno(temp_space[STARTINDEX]);
	debugno(temp_space[ENDINDEX]);
	blankspacesize=temp_space[ENDINDEX]-temp_space[STARTINDEX];
	return blankspacesize;
}

void printfoldword(char *temp_space,int blankspacesize,int *t_space)
{
	int i=0,start=temp_space[STARTINDEX],end=temp_space[ENDINDEX];

	while(i<blankspacesize && start<=end)
	{
		temp_space[i++]=temp_space[start++];
	}
	temp_space[end]='\0';
	i=0;
	while(i<blankspacesize)
	{
		printf("%c",temp_space[i]);
		i++;
	}

}



/*
void foldonbl(char *temp_arr,char *temp_tab,int tpossptbnl)
{
	int possptbnl,i,j;
	possptbnl=i=j=0;
	if((tpossptbnl=findposnltbsp(temp_arr, tpossptbnl))==NOBLANK)
	{
			printf("%s",temp_arr);
	}
	for(i=0,j=(tpossptbnl+1);temp_arr[j]!=' ';i++,++j)
	{
			temp_tab[i]=temp_arr[j];
	}
	tpossptbnl=j;
	debugno(tpossptbnl);
	debugstring(temp_tab);
}
*/
void tabatpos(char *temp_arr,int len, char *temp_tab)
{
	int pos,nt,ns;
	pos=nt=ns=0;
	memcpy(temp_tab, temp_arr, MAXLENGTH);
	for(pos=0;pos<len;++pos)
	{
		if(temp_tab[pos]==' ')
		{
			if((pos % TABSPACE)!=0)
			{
				ns++;
			}else {
				ns=0;
				nt++;
			}

		}else {

			if(temp_tab[pos]==' ')
			{
				ns=0;
			}else if((pos%TABSPACE)==0 && pos!=0)
			{
				for(;nt>0;--nt)
					temp_tab[pos]='\t';
			}
			else {
				for(;ns>0;--ns)
				{
					temp_tab[pos]=' ';
				}

			}

			if(temp_tab[pos]=='\t')
			{
				pos=pos+(TABSPACE-(pos - 1)%TABSPACE)-1;
			}
			if(temp_tab[pos]=='\n')
			{
				ns=nt=0;
			}

		}
	}
}
void entab(char *temp_arr,int len)
{
	int count=0;

	if(temp_arr[count]!=' ' && count<len)
	{
		temp_arr[count++]=temp_arr[count];
	}
	else
	{
		temp_arr[count++]='\t';
	}
}

void detab(char *temp_arr,int len)
{
	int count=0;

	if(temp_arr[count]!='\t' && count<len)
	{
		temp_arr[count++]=temp_arr[count];
	}
	else
	{
		temp_arr[count++]=' ';
	}
}
void reversee(char *temp_arr,int len)
{
	char temp_ch;
	int i,j;
	i=j=0;
	for(i=0,j=len-1;j>(len/2);++i,--j)
	{
		temp_ch=temp_arr[i];
		temp_arr[i]=temp_arr[j];
		temp_arr[j]=temp_ch;

	}
}
void remov_space(char *temp_arr)
{
	int count=0, i=0;
	for(i=0;temp_arr[i];++i)
	{
		if(temp_arr[i]!=' ' && temp_arr[i]!='\t')
		{
			temp_arr[count++]=temp_arr[i];
		}
	}
}
void copy_line(char to[],char from[])
{
	int i=0;

	while((to[i]=from[i])!='\0')
		i++;
}
//By default '\n' is included in string

int get_length(char temp[],int length)
{
	char c;
	int i=0;

	scanf("%c",&c);
	temp[0]=c;
	/*in for loop condition become false, Relation higher precedence than logical
	for(i=1;(i<length-1) == (c!='\n') == (c!=EOF);++i)
	Left to Right evaluate
	*/
	for(i=1;(i<length-1) && (c!='\n') && (c!=EOF);++i)
	{
		scanf("%c",&c);
		temp[i]=c;
	}

		if(temp[0]=='\n')
		{
		  return 0;
		}
		else
		{

		}

		temp[i]='\0';
		return i;
}

/*
int findposnltbsp(char *temp_arr,int tpossptbnl)
{
	int i=0,pos=-1;


	for(i=0;i<tpossptbnl;i++)
	{
		if(temp_arr[i]==' '||temp_arr[i]=='\t')
		{
			pos=i;
		}
	}

	return pos;
}
*/
