#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

#define TRUE 1
#define FALSE 0

typedef struct
{
	int useflag;
	int borrowflag;
	
	char bookname[60];
	rbtree_node book_rbtree_node;
}rbtree_book_struct;


void fun_book_rbtree_head_init(void);
void fun_rbtree_add_book_to_cabinet(char *bookname);
void fun_rbtree_del_book_from_cabinet(char *bookname);
void fun_rbtree_print_all_books(void);
