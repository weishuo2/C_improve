#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

char   *Menu[]   = {"File",
		    "Edit",
		    "Run",
		    "Compile",
		    "Project",
		    "Options",
		    "Debug",
		    "Break/watch"};
char   *Prompt[] = {"F1-Help",
		    "F5-Zoom",
		    "F6-Switch",
		    "F7-Trace",
		    "F8-Step",
		    "F9-Make",
		    "F10-Menu",
		    "NUM"};
char   *Menus[]  = {"Load      F3",
		    "Pick  Alt-F3",
		    "New",
		    "Save      F2",
		    "Write to",
		    "Driectory",
		    "Change dir",
		    "Os shell",
		    "Quit   Alt-X",

		    "Run            Ctrl-F9",
		    "Program reset  Ctrl-F2",
		    "Go to Cursor        F4",
		    "Trace into          F7",
		    "Step over           F8",
		    "User screen     Alt-F5",

		    "Compile to OBJ",
		    "Make EXE File",
		    "Link EXE File",
		    "Build all",
		    "Primary C file:",
		    "Get info",

		    "Project name",
		    "Break make on       Errors",
		    "Auto dependencies   Off",
		    "Clear project",
		    "Remove messages",

		    "Compiler",
		    "Linker",
		    "Environment",
		    "Directories",
		    "Arguments",
		    "Save options",
		    "Retrieve options",

		    "Evaluate    Ctrl-F4",
		    "Call stack  Ctrl-F3",
		    "Find function",
		    "Refresh display",
		    "Display swapping     Smart",
		    "Source debugging     On",

		    "Add watch          Ctrl-F7",
		    "Delete watch",
		    "Edit watch",
		    "Remove all watch",
		    "",
		    "Toggle breakpoint  Ctrl-F8",
		    "Clear all breakpoints",
		    "View next breakpoint"};
int   MnN[]      = { 9, 0, 6, 6, 5, 7, 6, 8};
int   MnLoc[][5] = {{4, 3, 2, 18, 12},
		    {11, 0, 0, 0, 0},
		    {18, 17, 2, 42, 9},
		    {24, 23, 2, 57, 9},
		    {34, 33, 2, 69, 8},
		    {44, 43, 2, 62, 10},
		    {54, 45, 2, 79, 9},
		    {62, 50, 2, 79, 11}};
int   Mnm, Mns;
char  buf[30*25*2], buf1[40*3*2];

void  ShowMenuM (void);
void  SetMenuM  (int );
void  ClrMenuM  (int );
void  ShowPrompt(void);
void  ShowMenuS (int );
void  HideMenuS (int );
void  SetMenuS  (int );
void  ClrMenuS  (int );
void  SelLR     (int );
void  SelUD     (int );
void  ExeItem   (int, int);
void  ScanKb    (int *, int *);

void  main(void)
{
    int i, idx, key0, key1, flag = 1;

    textbackground(BLUE);
    clrscr( );
    ShowMenuM();
    Mnm = 1;
    Mns = 0;
    SetMenuM(Mnm);
    ShowPrompt();

    while (flag) {
	key0 = key1 = 0;
	ScanKb(&key0, &key1);
	if (key0 != 0) {                    /*if an ordinary key is pressed*/
	    if (Mns == 0) {                 /*operate on main menu*/
		if (key0 == 13)
		    ShowMenuS(Mnm);
		else {
		    for (i=0;i<8;i++)
			if (key0 - *Menu[i] == 0 || key0 - *Menu[i] == 32)
			    break;
		    if (i < 8)
			ShowMenuS(i+1);
		}
	    }
	    else {                         /*operate on sub menu*/
		if (key0 == 13)
		    ExeItem(Mnm,Mns);
		else if (key0 == 27){
		    HideMenuS(Mnm);
		    Mns = 0;
		}
		else {
		    for (i=1,idx=0;i<Mnm;i++)
			idx += MnN[i-1];
		    for (i=idx;i<idx+MnN[Mnm-1];i++)
			if (key0 - *Menus[i] == 0 || key0 - *Menus[i] == 32)
			    break;
		    if (i < idx + MnN[Mnm-1])
			ExeItem(Mnm,i - idx + 1);
		}
	    }
	}
	else {                             /*if a special key is pressed*/
	    switch (key1){
		case 33: ShowMenuS(1); break;                 /*Alt+F*/
		case 18: ShowMenuS(2); break;                 /*Alt+E*/
		case 19: ShowMenuS(3); break;                 /*Alt+R*/
		case 46: ShowMenuS(4); break;                 /*Alt+C*/
		case 25: ShowMenuS(5); break;                 /*Alt+P*/
		case 24: ShowMenuS(6); break;                 /*Alt+O*/
		case 32: ShowMenuS(7); break;                 /*Alt+D*/
		case 48: ShowMenuS(8); break;                 /*Alt+B*/

		case 61: ExeItem(1,1); break;                 /*F3*/
		case 106:ExeItem(1,2); break;                 /*Alt+F3*/
		case 60: ExeItem(1,4); break;                 /*F2*/
		case 45: flag = 0; break;                     /*Alt+X*/

		case 102:ExeItem(3,1); break;                 /*Ctrl+F9*/
		case 95: ExeItem(3,2); break;                 /*Ctrl+F2*/
		case 62: ExeItem(3,3); break;                 /*F4*/
		case 65: ExeItem(3,4); break;                 /*F7*/
		case 66: ExeItem(3,5); break;                 /*F8*/
		case 108:ExeItem(3,6); break;                 /*Alt+F5*/

		case 97: ExeItem(7,1); break;                 /*Ctrl+F4*/
		case 96: ExeItem(7,2); break;                 /*Ctrl+F3*/

		case 100:ExeItem(8,1); break;                 /*Ctrl+F7*/
		case 101:ExeItem(8,6); break;                 /*Ctrl+F8*/

		case 72: SelUD(-1); break;                    /*UP*/
		case 80: SelUD( 1); break;                    /*DOWN*/
		case 75: SelLR(-1); break;                    /*LEFT*/
		case 77: SelLR( 1); break;                    /*Right*/
	    }
	}
    }
}

void ShowMenuM(void)                      /*show main menu*/
{
    int i;

    window(1, 1, 80, 1);
    textbackground(WHITE);
    textcolor(BLACK);
    clrscr( );
    for (i=0; i<8; i++) {
	gotoxy(MnLoc[i][0], 1);
	textcolor(RED);
	cprintf("%c", *Menu[i]);
	textcolor(BLACK);
	cprintf("%s", Menu[i]+1);
    }
}

void SetMenuM(int mmn)                    /*tag main menu*/
{
    window(1, 1, 80, 1);
    textbackground(BLACK);
    textcolor(WHITE);
    gotoxy(MnLoc[mmn-1][0], 1);
    cprintf("%s", Menu[mmn-1]);
}

void ClrMenuM(int mmn)                    /*clear the tag of main menu*/
{
    window(1, 1, 80, 1);
    textbackground(WHITE);
    gotoxy(MnLoc[mmn-1][0], 1);
    textcolor(RED);
    cprintf("%c", *Menu[mmn-1]);
    textcolor(BLACK);
    cprintf("%s", Menu[mmn-1]+1);
}

void ShowPrompt(void)                     /*show prompt*/
{
    int i;

    window(1, 25, 80, 25);
    textbackground(WHITE);
    textcolor(BLACK);
    clrscr( );
    for(i=0; i<6; i++){
	textcolor(RED);
	cprintf(" %c%c", *Prompt[i], *(Prompt[i]+1));
	textcolor(BLACK);
	cprintf("%s ", Prompt[i]+2);
    }
    textcolor(RED);
    cprintf(" %c%c%c", *Prompt[i], *(Prompt[i]+1), *(Prompt[i]+2));
    textcolor(BLACK);
    cprintf("%s  ", Prompt[i]+3);
    textbackground(BLUE);
    textcolor(WHITE);
    cprintf(" %s ", Prompt[7]);
}

void ShowMenuS(int mmn)                   /*show sub menu of main menu mmn*/
{
    int i, left, top, right, bottom, ht, wd, idx;

    if (mmn == Mnm && Mns > 0)
	return;
    if (mmn != Mnm) {
	if (Mns>0 && Mnm != 2)
            HideMenuS(Mnm);
	ClrMenuM(Mnm);
	SetMenuM(Mnm=mmn);
    }
    Mns = 1;
    if (Mnm == 2)
	return;
    left   = MnLoc[Mnm-1][1];
    top    = MnLoc[Mnm-1][2];
    right  = MnLoc[Mnm-1][3];
    bottom = MnLoc[Mnm-1][4];
    ht     = bottom - top + 1;
    wd     = right - left + 1;
    for (i=1,idx=0;i<Mnm;i++)
	idx += MnN[i-1];
    gettext(left, top, right, bottom, buf);
    window (left, top, right, bottom);
    textbackground(WHITE);
    textcolor(BLACK);
    clrscr( );
    window (left, top, right, bottom + 1);
    putch(0xda);
    for (i=2; i<wd; i++)
	putch(0xc4);
    putch(0xbf);
    for (i=2; i<ht; i++) {
	gotoxy(1, i);
	textcolor(BLACK);
	cprintf("%c %-*s%c", 0xb3, wd - 3, Menus[idx+i-2], 0xb3);
	gotoxy(3, i);
	textcolor(RED);
	cprintf("%c", *Menus[idx+i-2]);
    }
    gotoxy(1, i);
    textcolor(BLACK);
    putch(0xc0);
    for (i=2; i<=right-left; i++)
	putch(0xc4);
    putch(0xd9);
    SetMenuS(Mns);
}

void HideMenuS(int mmn)                   /*hide sub menu of main menu mmn*/
{
    int left, top, right, bottom;

    left   = MnLoc[mmn-1][1];
    top    = MnLoc[mmn-1][2];
    right  = MnLoc[mmn-1][3];
    bottom = MnLoc[mmn-1][4];
    puttext(left, top, right, bottom, buf);
    Mns = 0;
}

void SetMenuS(int smn)                    /*tag sub menu*/
{
    int i, left, top, right, bottom, ht, wd, idx;

    if (Mnm == 2)
	return;
    ClrMenuS(Mns);
    Mns = smn;
    left   = MnLoc[Mnm-1][1];
    top    = MnLoc[Mnm-1][2];
    right  = MnLoc[Mnm-1][3];
    bottom = MnLoc[Mnm-1][4];
    wd     = right - left + 1;
    for (i=1,idx=0; i<Mnm; i++)
	idx += MnN[i-1];
    idx += Mns - 1;
    window (left, top, right, bottom);
    textbackground(BLACK);
    textcolor(WHITE);
    gotoxy(2,Mns+1);
    cprintf(" %-*s", wd - 3, Menus[idx]);
}

void ClrMenuS(int smn)                    /*clear the tag of sub menu*/
{
    int i, left, top, right, bottom, ht, wd, idx;

    if (Mnm == 2)
	return;
    left   = MnLoc[Mnm-1][1];
    top    = MnLoc[Mnm-1][2];
    right  = MnLoc[Mnm-1][3];
    bottom = MnLoc[Mnm-1][4];
    wd     = right - left + 1;
    for (i=1,idx=0; i<Mnm; i++)
	idx += MnN[i-1];
    idx += smn - 1;
    window (left, top, right, bottom);
    textbackground(WHITE);
    textcolor(BLACK);
    gotoxy(2, smn+1);
    cprintf(" %-*s", wd - 3, Menus[idx]);
    gotoxy(3, smn+1);
    textcolor(RED);
    cprintf("%c", *Menus[idx]);
}

void SelLR(int lr)                        /*select menu item left or right*/
{
    int mmn = Mnm + lr;

    if (mmn > 8)
	mmn = 1;
    else if (mmn < 1)
	mmn = 8;
    if (Mns > 0)
	ShowMenuS(mmn);
    else {
	ClrMenuM(Mnm);
	SetMenuM(Mnm=mmn);
    }
}

void SelUD(int ud)                        /*select submenu item up or down*/
{
    int smn = Mns + ud;

    if (Mnm == 2)
	return;
    if (smn > MnN[Mnm-1])
	smn = 1;
    else if (smn < 1)
	smn = MnN[Mnm-1];
    if (Mns == 0) {
	if (ud == 1)
            ShowMenuS(Mnm);
    }
    else {
	if (Mnm == 8 && smn == 5)
	    if(ud > 0)
                smn++;
            else
                smn--;
	ClrMenuS(Mns);
	SetMenuS(Mns=smn);
    }
}

void ExeItem(int mmn, int smn)            /*execute the corresponding command*/
{
    int i, key0, key1, ht, wd, idx;

    if (mmn == 1 && smn == 9)
        exit(0);
    if (mmn == 2)
        return;
    if (Mns > 0 && Mnm != 2)
        HideMenuS(Mnm);
    if (mmn != Mnm) {
	ClrMenuM(Mnm);
	SetMenuM(Mnm=mmn);
    }
    Mns = 0;
    for (i=1,idx=0; i<Mnm; i++)
	idx += MnN[i-1];
    idx += smn - 1;
    ht = 3;
    wd = 40;
    gettext(21, 12, 60, 14, buf1);
    window (21, 12, 60, 14);
    textbackground(WHITE);
    textcolor(BLACK);
    clrscr( );
    window (21, 12, 60, 14 + 1);
    putch(0xda);
    for (i=2; i<wd; i++)
	putch(0xc4);
    putch(0xbf);
    for (i=2; i<ht; i++){
	gotoxy(1,i);
	cprintf("%c %-*s%c", 0xb3, wd - 3, "", 0xb3);
    }
    gotoxy(1,i);
    putch(0xc0);
    for (i=2; i<wd; i++)
	putch(0xc4);
    putch(0xd9);
    textcolor(YELLOW);
    gotoxy(3,2);
    cprintf("Executing...(MMN=%d,SMN=%d), ESC return",mmn,smn);
    do ScanKb(&key0, &key1); while(key0 != 27);
    puttext(21, 12, 60, 14, buf1);
}

void ScanKb(int *pk0, int *pk1)           /*accept input from keyboard*/
{
    int key;

    ShowPrompt();
    while (bioskey(1) == 0)
        ;
    key = bioskey(0);
    *pk0 = key & 0xff;
    *pk1 = (key >> 8) & 0xff;
}