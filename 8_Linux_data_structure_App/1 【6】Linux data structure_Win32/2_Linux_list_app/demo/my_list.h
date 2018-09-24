#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define TRUE 1
#define FALSE 0

typedef struct
{
	int useflag;
	int borrowflag;
	
	char bookname[60];
	list_node book_list_node;
}list_book_struct;


void fun_book_list_head_init(void);
void fun_list_add_book_to_cabinet(char *bookname);
void fun_list_del_book_from_cabinet(char *bookname);
void fun_list_print_all_books(void);
