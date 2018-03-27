#include "fujia.h"


char* mygets(char *a)//需要返回这个数组 
{
	char c;
	int i=0;
	while((c=getchar())!='\n')
	{
		a[i]=c;
		i++;
	}
	a[i]='\0';
	return a;
}

void myputs(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
		putchar(a[i]);
	putchar('\n');
}

void change(int n,char str[])//将n变为字符串，并存在str中 
{
	int i=0,j,num;
	char ch;
	while((n/10 != 0)||(n>0 && n<10))
	{
		num=n%10;
		str[i]=num+'0';
		n/=10;
		i++; 
	}
	str[i]='\0';
	i--;
	for(j=0;j<=i;j++,i--)//颠倒，数组顺序和整数的顺序不一样 
	{
		ch=str[j];
		str[j]=str[i];
		str[i]=ch;
	}
}

void myprintf(const char *a,...)//a是引号里面的 
{
	va_list ap;
	va_start(ap,a);//获取第一个参数的地址，va-list的指针形式 
	char s,*str,b,sum[50];
	int num,i=0,j;
	float f;
	double dou;
	while(a[i]!='\0')
	{
		if(a[i]=='%')
		{
			i++;
			b=a[i];
			switch(b)
			{
				case 'c':
					s=va_arg(ap,int);//后续值依次添加 
					putchar(s);
					break;
				case 's':
					for(str=va_arg(ap,char *);*str;str++)
						putchar(*str);
					break;
				case 'd':
					num=va_arg(ap,int);
					if(num<0)
					{
						putchar('-');
						num=0-num;
					}
					change(num,sum);
					for(j=0;sum[j]!='\0';j++)
						putchar(sum[j]);
					break;
				case 'f':
					{
						f=va_arg(ap,double);
						if(f<0)
						{
							putchar('-');
							f=0-f;
						}
						j=(int)f;
						change(j,sum);
						myprintf("%s",sum);//整数部分 
						f -=(float)j;
						putchar('.');
						f*=1000000;
						j=(int)f;
						change(j,sum);//小数部分 
						myprintf("%s",sum);
						break;
					}
				case 'l':
					{
						i++;
						if(a[i]!='f')
							putchar(a[i--]);//看是不是lf 
						else
						{
							dou=va_arg(ap,double);
							if(dou<0)
							{
								putchar('-');
								dou=0-dou;
							}
						    b=(int)dou;
							change(b,sum);
							myprintf("%s",sum);
							dou=dou-(double)b;
							putchar('.');
							dou*=10000000;
							b=(int)dou;
							change(b,sum);
							myprintf("%s",sum);//同上 
							break;
						}
						break;
					}
				defult:
					putchar(s);			
			}
		}
		else
			putchar(a[i]);
		i++;
	}
	va_end(ap);//清空 
}

void myscanf(const char *b,...)
{
	char ch;
	char *addr;
	int *d,i=0,fu=1,e=1;//判断负数
	float *f;
	double *lf;	
	float f1=0,lf1=0;
	va_list ap;
	va_start (ap,b);
	while(b[i]!='\0')
	{
		if(b[i]=='%')
		{
			i++;
			ch=b[i];
			switch(ch)
			{
				case 'c':
					addr=va_arg(ap,char *);
					*addr=getchar();
					break;
				case 's':
					addr=va_arg(ap,char *);
					ch=getchar();
					while((ch==' ')||(ch=='\n'))//除去前面的空格 
						ch=getchar();
					for(;(ch!=' ')&&(ch!='\n');ch=getchar()) 
					{
						*addr=ch;
						addr++;
					}
					*addr='\0';
					break;
				case 'd':
					d=va_arg(ap,int *);
					ch=getchar();
					*d=0;
					while((ch==' ')||(ch=='\n'))//除去前面的空格 
						ch=getchar();
					for(;((ch!=' ')&&(ch!='\n'));ch=getchar())
					{
						if(ch=='-')
							fu=0;
						if(ch>='0'&&ch<='9')
							*d=*d*10+ch-'0';
					}
					if(fu==0)
						*d=0-*d;
					break;
				case 'f':
					f=va_arg(ap,float *);
					*f=0;
					ch=getchar();
					while((ch==' ')||(ch=='\n'))//除去前面的空格 
						ch=getchar();
					for(;(ch!='.')&&(ch!=' ')&&(ch!='\n');ch=getchar())
					{
						if(ch=='-')
							fu=0;
						if((ch>='0')&&(ch<='9'))
							*f=*f*10+ch-'0';
					}
					if(ch=='.')//处理小数部分 
					{
						e=1;
						for(ch=getchar();ch>='0'&&ch<='9';ch=getchar())
						{
							if((ch==' ')||(ch=='\n'))
								break;
							f1=f1*10+ch-'0';
							e*=10;
						}
						*f+=f1/e;
					}
					if(fu==0)
						*f=0-*f;
					break;
				case 'l'://其实程序同上 
					i++;
					if(b[i]!='f')
						break;
					else
					{
						lf=va_arg(ap,double *);
						*lf=0;
						ch=getchar();
						while((ch==' ')||(ch=='\n'))//除去前面的空格 
							ch=getchar();
						for(;(ch!='.')&&(ch!=' ')&&(ch!='\n');ch=getchar()) 
						{
							if(ch=='-')
								fu=0;
							if((ch>='0')&&(ch<='9'))
								*lf=*lf*10+ch-'0';	
						}
						if(ch=='.')
						{
							e=1;
							for(ch=getchar();ch>='0'&&ch<='9';ch=getchar())
							{
								lf1=lf1*10+ch-'0';
								e*=10;
							}
							*lf+=lf1/e;
						}
						if(fu==0)
							*lf=0-*lf;
						break;
					}
				default:
					break;
			 } 
		}
		i++;
	}
	va_end(ap);
 } 
 
