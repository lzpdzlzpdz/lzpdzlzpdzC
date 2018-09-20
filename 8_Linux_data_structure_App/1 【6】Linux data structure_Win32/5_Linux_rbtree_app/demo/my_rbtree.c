/********************************************************
1. how to del a entry of rbtree
rb_erase(&pbook->book_rbtree_node,&BOOK_RB_ROOT);


2. how to add a entry
rbtree_insert(&BOOK_RB_ROOT,pbook);

3. how to compare
ret = rbtree_compare(pbookname, pbook->bookname);

2018.9.17 create by lzpdz
********************************************************/
#include<stdio.h>
#include <string.h>
#include"my_rbtree.h"

// init rbtree book cabinet
static struct rb_root BOOK_RB_ROOT =  { NULL, };


void fun_book_rbtree_head_init(void)
{
    return ;
}

rbtree_book_struct *fun_rbtree_find_book_in_cabinet(char *pbookname)
{
	struct rb_node *node = BOOK_RB_ROOT.rb_node;
	rbtree_book_struct *pbook = NULL;
	int ret = 0;

	while(node)
	{
	    
		pbook = rb_entry(node, rbtree_book_struct, book_rbtree_node);

        ret = rbtree_compare(pbookname, pbook->bookname);
		if(ret > 0)
		{
			node = node->rb_left;
		}
		else if(ret < 0)
		{
			node = node->rb_right;
		}
		else
		{
			return pbook;
		}
	}
	return NULL;
}

int rbtree_compare(rbtree_book_struct *bookname1,rbtree_book_struct *bookname2)
{
    int ret = 0;
    ret = strcmp(bookname1, bookname2);
    return ret;
}

int rbtree_insert(struct rb_root *root,rbtree_book_struct *p_rbbook_entry)
{
	struct rb_node **temp_rb_node = &(root->rb_node),*parent=NULL;
	rbtree_book_struct *temp_book_entry =NULL;
	int ret = 0;
	

	while(*temp_rb_node)
	{
		temp_book_entry = rb_entry(*temp_rb_node,rbtree_book_struct,book_rbtree_node);
		parent = *temp_rb_node;

        ret = rbtree_compare(p_rbbook_entry->bookname, temp_book_entry->bookname);
		if(ret > 0)
		{
			temp_rb_node = &((*temp_rb_node)->rb_left);
		}
		else if(ret < 0)
		{
			temp_rb_node = &((*temp_rb_node)->rb_right);
		}
		else
	    {
			return 0;
		}
	}

	rb_link_node(&p_rbbook_entry->book_rbtree_node,parent,temp_rb_node);
	rb_insert_color(&p_rbbook_entry->book_rbtree_node,root);

	return 1;
}


int  fun_rbtree_add_book(char *bookname)
{
    rbtree_book_struct *pbook = NULL;

    pbook = (rbtree_book_struct *)malloc(sizeof(rbtree_book_struct));
    memset(pbook,0, sizeof(rbtree_book_struct));


	pbook->useflag = TRUE;
	pbook->borrowflag = TRUE;
	strcpy(pbook->bookname,bookname);


    rbtree_insert(&BOOK_RB_ROOT,pbook);

	printf("add book success, bookname = %s\n", bookname);
	return TRUE;
}

void fun_rbtree_add_book_to_cabinet(char *bookname)
{
	rbtree_book_struct *pbook = NULL;
	int ret = 0;
	
	pbook = fun_rbtree_find_book_in_cabinet(bookname);

	if (pbook == NULL)
	{
		printf("add book to book cabinet\n");
		ret = fun_rbtree_add_book(bookname);
		if (FALSE == ret)
		{
			printf("the book cabinet is full\n");
		}
		return;
	}

	printf("book is already exist\n");
	return;
}

void fun_rbtree_del_book_from_cabinet(char *bookname)
{
	rbtree_book_struct *pbook = NULL;

	pbook = fun_rbtree_find_book_in_cabinet(bookname);

	if (pbook == NULL)
	{
		printf("cannot find the book in book cabinet\n");
		return;
	}

	printf("del book = %s\n", pbook->bookname);

	rb_erase(&pbook->book_rbtree_node,&BOOK_RB_ROOT);
	free(pbook);

}

void fun_rbtree_print_all_books(void)
{
    struct rb_node *node = NULL;
    rbtree_book_struct *pbook = NULL;

    for (node = rb_first(&BOOK_RB_ROOT); node; node = rb_next(node))
	{
		pbook = rb_entry(node, rbtree_book_struct, book_rbtree_node);
		printf("key = %s\n", pbook->bookname);
	}
	return;
}




