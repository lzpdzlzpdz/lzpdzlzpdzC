#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef struct
{
	int useflag;
	int borrowflag;
	
	char bookname[60];
}array_book_cabinet_struct;


void fun_add_book_to_cabinet(char *bookname);
void fun_del_book_from_cabinet(char *bookname);
void fun_print_all_books(void);
