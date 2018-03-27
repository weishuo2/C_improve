#include"fujia.h"
 int main(void)
 {
 	char a[20];
 	char c;
 	char s[100];
 	int d;
 	float f;
 	double lf;
 	myprintf("请输入一个字符串\n");
 	mygets(a);
 	myputs(a);
 	myprintf("请输入字符，字符串,整数，浮点数，双精度浮点数。\n"); 
 	myscanf("%c %s %d %f %lf",&c,s,&d,&f,&lf);
 	myprintf("\nc:%c\ns:%s\nd:%d\nf:%f\nlf:%lf",c,s,d,f,lf);
 	return 0;
 }
