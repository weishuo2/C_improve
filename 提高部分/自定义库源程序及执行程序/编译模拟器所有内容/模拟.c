#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <string.h>
#include <math.h>
//����
#define REG0 ((IR >> 24) & 0x07)
#define REG1 ((IR >> 20) & 0x0F)
#define REG2 ((IR >> 16) & 0x0F)
#define IMMEDIATE (IR & 0xFFFF)
#define ADDRESS (IR & 0xFFFFFF)
#define PORT (IR & 0xFF)
#define OPCODE ((IR >> 27) & 0X1F)

typedef struct _PROG_STATE_WORD
{
    unsigned short overflow_flg: 1;
    unsigned short compare_flg: 1;
    unsigned short reserve: 14;
} PROG_STATE_WORD;

struct fujia
{
    PROG_STATE_WORD EPSW;  /*�ֶνṹģ�����״̬��*/
    unsigned short EGR[8];  /*unsigned short ģ��ͨ�üĴ���������������Ϊ�����±���д�ȡ*/
    unsigned long *EPC;    //�����������ִ��һ������ָ���unsigned long ���ֽڵıȽϺ�
} *ES;

PROG_STATE_WORD PSW;  /*�ֶνṹģ�����״̬��*/
unsigned char *MEM;    /*char�� �ö�̬�洢�� ģ���ڴ棬��С�������в���ȷ��*/
unsigned long *CS;     /*unsigned long ģ������*/
unsigned char *DS;               /*char ģ�����ݶ�*/
short *SS;              /*short ģ���ջ��*/
unsigned long *PC;    //���������
short GR[8];            /*unsigned short ģ��ͨ�üĴ���������������Ϊ�����±���д�ȡ*/
unsigned long IR;
/*unsigned long ģ��ָ��Ĵ�����ִ��ʱ���������PCָʾ���ڴ浥Ԫ��ָ�����뵽ָ��Ĵ���IR�У�
PC��ֵ+1��ʱ��ָ����һ��ָ�Ϊ��һ��ѭ����׼��*/

/*����ʮ��������ʵ����ʮ����ָ�����Ӧ����*/
int HLT(void);
int JMP(void);
int CJMP(void);
int OJMP(void);
int CALL(void);
int RET(void);
int PUSH(void);
int POP(void);
int LOADB(void);
int LOADW(void);
int STOREB(void);
int STOREW(void);
int LOADI(void);
int NOP(void);
int IN(void);
int OUT(void);
int ADD(void);
int ADDI(void);
int SUB(void);
int SUBI(void);
int MUL(void);
int DIV(void);
int AND(void);
int OR(void);
int NOR(void);
int NOTB(void);
int SAL(void);
int SAR(void);
int EQU(void);
int LT(void);
int LTE(void);
int NOTC(void);


int main(int argc,char **argv)
{
    unsigned long instruction;
    int length;
    int (* ops[])(void)={HLT, JMP, CJMP, OJMP, CALL,RET,PUSH,POP,LOADB,LOADW,
    STOREB,STOREW,LOADI,NOP,IN,OUT,ADD,ADDI,SUB,SUBI,MUL,DIV,AND,OR,NOR,
    NOTB,SAL,SAR,EQU,LT,LTE,NOTC};/*����ָ�����飬����ָ���Ӧ�ĺ����ĵ���*/

    FILE *pfIn;
    int ret = 1;
    GR[0]=0;

    //�ж��Ƿ������㹻�Ĳ���
    if (argc < 2)
    {
        printf("ERROR:no enough command line argument!\n");
        exit(-1);
    }
    /*��ϵͳ���붯̬�洢��ģ���ڴ�*/
    if ((MEM=(unsigned char*)malloc((unsigned int)(1<<24))) == NULL)
    {
        printf("ERROR:fail to allocate memery for code!\n");
        exit(-1);
    }
    PC = (unsigned long *)MEM;/*ʹָ�������ָ��ģ���ڴ�Ķ���*/
    CS = PC;//��������������׵�ַ����������׵�ַ�����������׵�ַ
    if ((pfIn = fopen(argv[1],"r")) == NULL)//��Ŀ���ļ�
    {
        printf("ERROR:cannot open file %s for reading\n",argv[1]);
        exit(-1);
    }

    while (fscanf(pfIn, "%li",&instruction) == 1)/*���ļ���ȡ��Ŀ����룬���ص�ģ���ڴ�*/
    {
        memcpy(PC, &instruction, sizeof(instruction));
        PC++;
    }
    SS = (short *)PC;
    length = *(PC-1);
    DS = (unsigned char *)(PC - (length + 3)/4 - 1);
    ES = (struct fujia *)(MEM + (1<<24)-sizeof(struct fujia));
    fclose(pfIn);     //��pfIn�е�ָ�����뵽�ڴ棬֮�����û���ˣ���ʼģ�⴦����ִ��ָ��
    PC = (unsigned long *)MEM;
    while (ret)   /*ģ�⴦����ִ��ָ��*/
    {
        IR = *PC; /*ȡָ����PCָʾ��ָ����ص�ָ��Ĵ���IR*/
        PC++;     /*CSָ����һ��ִ��ָ��*/
        ret = (*ops[OPCODE])();/*���벢ִ��ָ��*/
    }
    free(MEM);
    return 1;
}

int HLT(void)
{
    return 0;
}

int JMP(void)
{
    PC = CS + ADDRESS/4;//ADDRESS������Ǳ�ŵĵ�ֵַ�����кų�����
    return 1;
}

int CJMP(void)
{
    if(PSW.compare_flg)
    {
        PC = CS + ADDRESS/4;
    }
    return 1;
}

int OJMP(void)
{
    if(PSW.overflow_flg)
    {
       PC = CS + ADDRESS/4;
    }
    return 1;
}

int CALL(void)
{
    int i;
    for(i = 0;i < 8;i++)
        ES->EGR[i] = GR[i];
    ES->EPC = PC;
    ES->EPSW = PSW;
    PC = CS + ADDRESS/4;
    ES--;
    return 1;
}

int RET(void)
{
    int i;
    ES++;
    for(i = 0;i < 8;i++)
       GR[i] = ES -> EGR[i];
    PC = ES -> EPC;
    PSW = ES -> EPSW;
    return 1;
}

int PUSH(void)
{
    *SS = GR[REG0];
    SS++;
    return 1;
}

int POP(void)
{
    SS--;
    GR[REG0] = *SS;
    return 1;
}

int LOADB(void)
{
    int displace = GR[7];
    int MOVE=(int)(ADDRESS+displace);
    GR[REG0] = (short)(*(DS+MOVE));
    return 1;
}

int LOADW(void)
{
    int displace = GR[7];
    int MOVE=(int)(ADDRESS+2*displace);
    GR[REG0] = *(short *)(DS+MOVE);
    return 1;
}

int STOREB(void)
{
    int displace = GR[7];
    int MOVE=(int)(ADDRESS+displace);
    *(DS+MOVE) = GR[REG0];
    return 1;
}

int STOREW(void)
{
    int displace = GR[7];
    int MOVE=(int)(ADDRESS+2*displace);
    *(short *)(DS+MOVE) = GR[REG0];
    return 1;
}

int LOADI(void)
{
    GR[REG0] = (short)(IMMEDIATE);
    return 1;
}

int NOP(void)
{
    return 1;
}

int IN(void)
{
    read(0,(int *)(GR+REG0),1);
    return 1;
}/*�Ӽ�������һ���ֽ�*/

int OUT(void)
{
    write(1,(int *)(GR+REG0),1);
    return 1;
}/*����Ļ���һ���ַ�*/

int ADD(void)
{
    GR[REG0] = GR[REG1] + GR[REG2];
    if(GR[REG2] > 0)
    {
        if(GR[REG0] < GR[REG1])
            PSW.overflow_flg = 1;
        else
            PSW.overflow_flg = 0;
    }
    else if(GR[REG2] < 0)
    {
        if(GR[REG0] > GR[REG1])
            PSW.overflow_flg = 1;
        else
            PSW.overflow_flg = 0;
    }
    else
        PSW.overflow_flg = 0;
    return 1;
}

int ADDI(void)
{
    short n;
    n= GR[REG0] + IMMEDIATE;
    if(IMMEDIATE > 0)
    {
        if(n < GR[REG0] )
           PSW.overflow_flg = 1;
        else
            PSW.overflow_flg = 0;
    }
    else if(IMMEDIATE < 0)
    {
        if(n > GR[REG0])
           PSW.overflow_flg = 1;
        else
           PSW.overflow_flg = 0;
    }
    else
      PSW.overflow_flg = 0;
    GR[REG0] = n;
    return 1;
}

int SUB(void)
{
    GR[REG0] = GR[REG1] - GR[REG2];
    if(GR[REG2] > 0)
    {
        if(GR[REG0] > GR[REG1])
            PSW.overflow_flg = 1;
        else
            PSW.overflow_flg = 0;
    }
    else if(GR[REG2] < 0)
    {
        if(GR[REG0] < GR[REG1])
            PSW.overflow_flg = 1;
        else
            PSW.overflow_flg = 0;
    }
    else
        PSW.overflow_flg = 0;
    return 1;
}

int SUBI(void)
{
    short n;
    n = GR[REG0] - IMMEDIATE;
    if(IMMEDIATE > 0)
    {
        if(n > GR[REG0])
           PSW.overflow_flg = 1;
        else
            PSW.overflow_flg = 0;
    }
    else if(IMMEDIATE < 0)
    {
        if(n < GR[REG0])
           PSW.overflow_flg = 1;
        else
          PSW.overflow_flg = 0;
    }
    else
      PSW.overflow_flg = 0;
    GR[REG0] = n;
    return 1;
}

int MUL(void)
{
    GR[REG0] = GR[REG1]*GR[REG2];
    if(abs(GR[REG2]) > 1)
    {
        if(abs(GR[REG0]) < abs(GR[REG1]))
            PSW.overflow_flg = 1;
        else
            PSW.overflow_flg = 0;
    }
    else
        PSW.overflow_flg = 0;
    return 1;
}

int DIV(void)
{
    if(GR[REG2]==0)
    printf("the divider is 0!");
    else
    GR[REG0] = GR[REG1]/GR[REG2];
    return 1;
}

int AND(void)
{
    GR[REG0] = GR[REG1]&GR[REG2];
    return 1;
}

int OR(void)
{
    GR[REG0] = GR[REG1]|GR[REG2];
    return 1;
}

int NOR(void)
{
    GR[REG0] = GR[REG1]^GR[REG2];
    return 1;
}

int NOTB(void)
{
    GR[REG0] = ~GR[REG1];
    return 1;
}

int SAL(void)
{
    GR[REG0] = GR[REG1] << GR[REG2];
    return 1;
}

int SAR(void)
{
    GR[REG0] = GR[REG1] >> GR[REG2];
    return 1;
}

int EQU(void)
{
    PSW.compare_flg = ( GR[REG0] == GR[REG1] );
    return 1;
}

int LT(void)
{
    PSW.compare_flg  = ( GR[REG0] < GR[REG1] );
    return 1;
}

int LTE(void)
{
    PSW.compare_flg = ( GR[REG0] <= GR[REG1] );
    return 1;
}

int NOTC(void)
{
    PSW.compare_flg = !PSW.compare_flg;
    return 1;
}
