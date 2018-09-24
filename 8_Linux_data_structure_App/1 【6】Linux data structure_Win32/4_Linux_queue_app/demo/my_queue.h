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
	list_node book_queue_node;
}queue_book_struct;


void fun_book_queue_head_init(void);
void fun_queue_push_book_to_cabinet(char *bookname);
void fun_queue_pop_book_from_cabinet();
void fun_queue_print_all_books(void);
