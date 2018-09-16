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
	hlist_node_t book_hlist_node;
}hlist_book_struct;


void fun_book_hlist_head_init(void);
void fun_hlist_add_book_to_cabinet(char *bookname);
void fun_hlist_del_book_from_cabinet(char *bookname);
void fun_hlist_print_all_books(void);
