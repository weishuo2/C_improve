#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_LEN 150
#define INSTRS_COUNT (sizeof(g_instrs_name)/sizeof(g_instrs_name[0]))
#define INSTR_SYM {"HLT","JMP","CJMP","OJMP","CALL","RET","PUSH","POP","LOADB",\
                   "LOADW","STOREB","STOREW","LOADI","NOP","IN","OUT","ADD",\
                   "ADDI","SUB","SUBI","MUL","DIV","AND","OR","NOR","NOTB",\
                   "SAL","SAR","EQU","LT","LTE","NOTC","BYTE","WORD"\
                   }

const char *g_instrs_name[] = INSTR_SYM;
const char instr_format[33] = "12222133444451667575777778778881";

int GetInstrCode(const char *op_sym);//指令对应的号码 
unsigned long TransToCode(char *instr_line, int instr_num);//指令转码 
int GetRegNum(char *instr_line, char *reg_name);//寄存器名字 

typedef struct biaohao
{
    int addr;
    char str[20];
    struct biaohao *next;
}BH;
BH *head1 = NULL;

typedef struct byte
{
    int type;
    char name[8];
    int num;
    long dat[65];
    struct byte *next;
}BYTE;
BYTE *head2 = NULL;

int main(int argc, char **argv)
{
    char a_line[MAX_LEN];
    char op_sym[8];
    int op_num;
    int s = 0, k = 0, m = 0, j = 0, p = 0, q = 0; 
    char num[5];
    char dat[50][50];
    unsigned long str[4];
    char *pcPos;
    FILE *pfIn, *pfOut;

    head1 = (struct biaohao *)malloc(sizeof(struct biaohao));
    BH *p1 = head1;
    head2 = (struct byte *)malloc(sizeof(struct byte));
    BYTE *p2 = head2, *temp, *tail = head2;
    int n;
    if (argc < 3)
    {
        printf("ERROR:no enough command line arguments!\n");
        return 0;
    }
    if ((pfIn = fopen(argv[1], "r")) == NULL)
    {
        printf("ERROR:cannot open file %s for reading!\n", argv[1]);
        return 0;
    }
    if ((pfOut = fopen(argv[2], "w")) == NULL)
    {
        printf("ERROR:cannot open file %s for writing!\n", argv[2]);
        return 0;
    }

    while (!feof(pfIn))
    {
        fgets(a_line, MAX_LEN, pfIn);
        if ((pcPos = strchr(a_line, '#')) != NULL)
            *pcPos = '\0';
        n = sscanf(a_line, "%s", op_sym);
        if(n < 1)
            continue;
        if(strchr(a_line, ':') != NULL)
        {
            p1->addr = s;                               /*记录biaohao的行数*/
            sscanf(a_line, "%[^:]", p1->str);           /*将冒号为止的字符串存入str中*/
            p1->next = (struct biaohao *)malloc(sizeof(struct biaohao));
            p1 = p1->next;
        }
        if ((strcmp("BYTE", op_sym) == 0) || (strcmp("WORD", op_sym) == 0))       /*处理byte*/
        {
            if(strcmp("BYTE", op_sym) == 0)
                p2->type = 1;
            else
                p2->type = 2;
            sscanf(a_line, "%*s%s", p2->name);               /*存byte名*/
            while(p2->name[k] != '\0')               /*去掉字符串中方括号的部分*/
            {
                if(p2->name[k] == '[')
                    p2->name[k] = '\0';
                k++;
            }
            k = 0;
            if(strchr(a_line, '[') != NULL)
            {
                sscanf(a_line, "%*[^[][%[0-9]", num);
                p2->num = atoi(num);          /*将num转化为整数*/
                for(m = 0; m < p2->num; m++)
                    p2->dat[m] = 0;
                m = 0;
                if(strchr(a_line, '=') != NULL)
                {
                    while ((a_line[j] != '{') && (a_line[j] != '"'))
                        j++;
                    if(a_line[j] == '"')
                    {
                        while(a_line[++j] != '"')
                            p2->dat[m++] = a_line[j];
                    }
                    else
                    {
                        while (a_line[++j] != '}')
                        {
                            if (isdigit(a_line[j]))     /*是否数字*/
                                dat[p][m++] = a_line[j];
                            else if (a_line[j] == ',')
                            {
                                p++;
                                m = 0;
                            }
                        }
                        for (j = 0; j <= p; j++)
                            p2->dat[j] = atoi(dat[j]);
                   }
                }
                else
                {
                    for(m = 0; m < p2->num; m++)
                        p2->dat[m] = 0;
                }
            }
            else if(strchr(a_line, '=') != NULL)
            {
                sscanf(a_line, "%*[^=]=%s", dat[0]);
                p2->dat[0] = atoi(dat[0]);
                p2->num = 1;
            }
            else
                p2->num = 1;
            k = 0; m = 0; p = 0; q = 0;
            p2->next = (struct byte *)malloc(sizeof(struct byte));
            p2 = p2->next;
            continue;
        }
        s++;                /*行数加1*/
    }
    free(p1);
    free(p2);
    p1 = NULL;
    p2 = NULL;
    s = 0;
    fclose(pfIn);
    fclose(pfOut);
    pfIn = fopen(argv[1], "r");
    pfOut = fopen(argv[2], "w");

    while (!feof(pfIn))         {
        fgets(a_line, MAX_LEN, pfIn);
        if ((pcPos = strchr(a_line, '#')) != NULL)
            *pcPos = '\0';
        n = sscanf(a_line, "%s", op_sym);
        if ((n < 1) || (strcmp("BYTE", op_sym) == 0) || (strcmp("WORD", op_sym) == 0))
            continue;
        if (strchr(a_line, ':'))
        {
            sscanf(a_line, "%*s%s", op_sym);
            strcpy(a_line, strchr(a_line, ':') + 1);
        }
        op_num = GetInstrCode(op_sym);
        if(op_num > 33)
        {
            printf("ERROR: %s is a invalid instruction! \n", a_line);
            exit(-1);
        }
        fprintf(pfOut,"0x%08lx\n",TransToCode(a_line, op_num));
        s++;
    }

    for (p2 = head2, k = 0, m = 0; (p2 != NULL) && (p2->num > 0) && (p2->num < 66); p2 = p2->next)
    {
    if(p2->type == 1)
    {
        for(s = 0, j = 0; s < p2->num; s++)
        {
            str[j] = p2->dat[s];
            j++;
            if(j == 4)
            {
                fprintf(pfOut, "0x%02lx%02lx%02lx%02lx\n", str[3], str[2], str[1], str[0]);
                j = 0;
            }
            k++;
        }
        switch(j)
        {
            case 1:
                fprintf(pfOut, "0x%08lx\n", str[0]);
                break;
            case 2:
                fprintf(pfOut, "0x%04lx%04lx\n", str[1], str[0]);
                break;
            case 3:
                fprintf(pfOut, "0x00%02lx%02lx%02lx\n", str[2], str[1], str[0]);
                break;
            case 4:
                fprintf(pfOut, "0x%02lx%02lx%02lx%02lx\n", str[3], str[2], str[1], str[0]);
                break;
        }
    }
    if(p2->type == 2)
    {
        for(s = 0, j = 0; s < p2->num; s++)
        {
            str[j] = p2->dat[s];
            j++;
            if(j == 2)
                fprintf(pfOut, "0x%04lx%04lx4\n", str[1], str[0]);
            k += 2;
        }
        switch(j)
        {
            case 1:
                fprintf(pfOut, "0x%08lx\n", str[0]);
                break;
            case 2:
                fprintf(pfOut, "0x%04lx%04lx\n", str[1], str[0]);
                break;
        }
    }
}
    fprintf(pfOut, "0x%08lx\n", k);
    fclose(pfIn);
    fclose(pfOut);
    return 1;
}

int GetInstrCode(const char *op_sym)//指令对应的号码 
{
    int i;
    for (i = 0; i < INSTRS_COUNT; i++)
    {
        if(strcmp(g_instrs_name[i], op_sym) == 0)
        {
            break;
        }
    }
    return i;
}

unsigned long TransToCode(char *instr_line, int instr_num)//指令转码 
{
    unsigned long op_code;
    unsigned long arg1, arg2, arg3;
    unsigned long instr_code = 0ul;
    char op_sym[8], reg0[8], reg1[8], reg2[8];
    unsigned long addr;
    int immed, port;
    int n;
    char a[50];
    switch (instr_format[instr_num])
    {
        case '1':                       /*第一种指令格式（HLT、RET、NOP、NOTC）的译码*/
            op_code = instr_num;
            instr_code = op_code << 27;
            break;
        case '2':                       /*第二种指令格式（JMP、CJMP、OJMP、CALL）的译码*/

            n = sscanf(instr_line, "%s%s", op_sym, a);
            if (n < 2)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            addr = SearchBH(a) * 4;
            op_code = GetInstrCode(op_sym);
            instr_code = (op_code << 27) | (addr & 0x00ffffff);
            break;
        case '3':                              /*第三种格式指令（PUSH、POP）的译码*/
            n = sscanf(instr_line, "%s %s", op_sym, reg0);
            if (n < 2)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            op_code = GetInstrCode(op_sym);
            arg1 = GetRegNum(instr_line, reg0);
            instr_code = (op_code << 27) | (arg1 << 24);
            break;
        case '4':                              /*第四种格式指令（LOARB、LOADW、STOREB、STOREW）的译码*/
            n = sscanf(instr_line, "%s %s %s", op_sym, reg0, &a);
            if (n < 3)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            addr = Search_BYTE(a);
            op_code = GetInstrCode(op_sym);
            arg1 = GetRegNum(instr_line, reg0);
            instr_code = (op_code << 27) | (arg1 << 24) | (addr & 0x00ffffff);
            break;
        case '5':                              /*第五种格式指令（LOADI、ADDI、SUBI）的译码*/
            n = sscanf(instr_line, "%s %s %i", op_sym, reg0, &immed);
            if (n < 3)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            op_code = GetInstrCode(op_sym);
            arg1 = GetRegNum(instr_line, reg0);
            instr_code = (op_code << 27) | (arg1 << 24) | (immed & 0x0000ffff);
            break;
        case '6':                             /*第六种格式指令（IN、OUT）的译码*/
            n = sscanf(instr_line, "%s %s %i", op_sym, reg0, &port);
            if (n < 3)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            op_code = GetInstrCode(op_sym);
            arg1 = GetRegNum(instr_line, reg0);
            instr_code = (op_code << 27) | (arg1 << 24) | (port & 0x0000ffff);
            break;
        case '7':                       /*第七种格式指令（ADD、SUB、MUL、DIV、AND、OR、NOR、SAL、SAR）的译码*/
            n = sscanf(instr_line, "%s%s%s%s", op_sym, reg0, reg1, reg2);
            if (n < 4){
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            op_code = GetInstrCode(op_sym);
            arg1 = GetRegNum(instr_line, reg0);
            arg2 = GetRegNum(instr_line, reg1);
            arg3 = GetRegNum(instr_line, reg2);
            instr_code = (op_code << 27) | (arg1 << 24) | (arg2 << 20) | (arg3 << 16);
            break;
        case '8':                        /*第八种格式指令（NOTB、EQU、LT、LTE）的译码*/
            n = sscanf(instr_line, "%s%s%s", op_sym, reg0, reg1);
            if (n < 3)
            {
                printf("ERROR:bad instruction format!\n");
                exit(-1);
            }
            op_code = GetInstrCode(op_sym);
            arg1 = GetRegNum(instr_line, reg0);
            arg2 = GetRegNum(instr_line, reg1);
            instr_code = (op_code << 27) | (arg1 << 24) | (arg2 << 20);
            break;
        default:
            break;
    }
    return instr_code;
}
int SearchBH(char *s)              /*找标号的行数*/
{
    BH *p1 = head1;
    while(p1 != NULL)
    {
        if(strcmp(p1->str, s) == 0)
            return(p1->addr);
        else
            p1 = p1 -> next;
    }
    exit(-1);
}

int Search_BYTE(char *s)//字节 
{
    int numb = 0;
    BYTE *p2 = head2;
    while (p2)
    {
        if (strcmp(p2->name, s) == 0)
            return  numb;
        else
        {
            numb = numb + p2->num * p2->type;
            p2 = p2->next;
        }
    }
    printf("ERROR!");
    exit(-1);
}
int GetRegNum(char *instr_line, char *reg_name)//寄存器 
{
    int reg_num;
    if ((tolower(*reg_name) >= 'a') && (tolower(*reg_name) <= 'g'))
        reg_num = tolower(*reg_name) - 'a' + 1;
    else if(tolower(*reg_name) == 'z')
        reg_num = 0;
    else
    {
        printf("ERROR:bad register name in %s!\n", instr_line);
        exit(-1);
    }
    return reg_num;
}

