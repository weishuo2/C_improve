#include"fujia.h"
 int main(void)
 {
 	char a[20];
 	char c;
 	char s[100];
 	int d;
 	float f;
 	double lf;
 	myprintf("������һ���ַ���\n");
 	mygets(a);
 	myputs(a);
 	myprintf("�������ַ����ַ���,��������������˫���ȸ�������\n"); 
 	myscanf("%c %s %d %f %lf",&c,s,&d,&f,&lf);
 	myprintf("\nc:%c\ns:%s\nd:%d\nf:%f\nlf:%lf",c,s,d,f,lf);
 	return 0;
 }
