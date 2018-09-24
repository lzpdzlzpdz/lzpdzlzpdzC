/********************************************************
1. how to del a entry of list
list_del(&(pbook->book_list_node));

2. how to add a entry
list_add_tail(&(pbook->book_list_node), &books_list_head);

2018.9.17 create by lzpdz
********************************************************/

#include<stdio.h>
#include <string.h>
#include"my_list.h"

// init list book cabinet
#define books_max_num 100
struct list_head books_list_head = {0};

void fun_book_list_head_init(void)
{
	INIT_LIST_HEAD(&books_list_head);
}

void *fun_list_find_book_in_cabinet(char *bookname)
{
    list_book_struct *pbook = NULL;
    list_book_struct *ptempbook = NULL;
    int ret;

    list_for_each_entry_safe_ext(pbook, list_book_struct, ptempbook, &books_list_head, book_list_node)
    {
        ret = strcmp(pbook->bookname, bookname);
        if(FALSE == ret)
        {
            return pbook;
        }
    }

    return NULL;
}

int  fun_list_add_book(char *bookname)
{
    list_book_struct *pbook = NULL;

    pbook = (list_book_struct *)malloc(sizeof(list_book_struct));
    memset(pbook,0, sizeof(list_book_struct));


	pbook->useflag = TRUE;
	pbook->borrowflag = TRUE;
	strcpy(pbook->bookname,bookname);


    list_add_tail(&(pbook->book_list_node), &books_list_head);

	printf("add book success, bookname = %s\n", bookname);
	return TRUE;
}

void fun_list_add_book_to_cabinet(char *bookname)
{
	list_book_struct *pbook = NULL;
	int ret = 0;
	
	pbook = (list_book_struct*)fun_list_find_book_in_cabinet(bookname);

	if (pbook == NULL)
	{
		printf("add book to book cabinet\n");
		ret = fun_list_add_book(bookname);
		if (FALSE == ret)
		{
			printf("the book cabinet is full\n");
		}
		return;
	}

	printf("book is already exist\n");
	return;
}

void fun_list_del_book_from_cabinet(char *bookname)
{
	list_book_struct *pbook = NULL;

	pbook = (list_book_struct *)fun_list_find_book_in_cabinet(bookname);

	if (pbook == NULL)
	{
		printf("cannot find the book in book cabinet\n");
		return;
	}

	printf("del book = %s\n", pbook->bookname);
	list_del(&(pbook->book_list_node));
	free(pbook);

}

void fun_list_print_all_books(void)
{
    list_book_struct *pbook = NULL;
    list_book_struct *ptempbook = NULL;

    list_for_each_entry_safe_ext(pbook, list_book_struct, ptempbook, &books_list_head, book_list_node)
    {
        if(pbook->useflag == TRUE)
        {
            printf("fun_print_all_books, bookname = %s\n", pbook->bookname);
        }
    }
	return;
}
