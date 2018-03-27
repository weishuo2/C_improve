#include<stdio.h>
#include<stdarg.h>//可变参数 
#include<string.h>
#include<math.h>
char* mygets(char *a);
void myputs(char *a);
void change(int n,char str[]);
void myprintf(const char *a,...);
void myscanf(const char *b,...);//由于参数个数不定，所以只能用这个 
