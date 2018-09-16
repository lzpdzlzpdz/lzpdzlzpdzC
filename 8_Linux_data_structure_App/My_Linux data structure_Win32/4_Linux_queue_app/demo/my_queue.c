/********************************************************
1. how to del a entry of queue
queue_del(&(pbook->book_queue_node));

2. how to add a entry
queue_add_tail(&(pbook->book_queue_node), &books_queue_head);

2018.9.16 create by lzpdpz
********************************************************/
#include<stdio.h>
#include <string.h>
#include"my_queue.h"

// init queue book cabinet
#define books_max_num 100
struct list_head books_queue_head = {0};

void fun_book_queue_head_init(void)
{
	INIT_LIST_HEAD(&books_queue_head);
}

void *fun_queue_find_book_in_cabinet(char *bookname)
{
    queue_book_struct *pbook = NULL;
    queue_book_struct *ptempbook = NULL;
    int ret;

    list_for_each_entry_safe_ext(pbook, queue_book_struct, ptempbook, &books_queue_head, book_queue_node)
    {
        ret = strcmp(pbook->bookname, bookname);
        if(FALSE == ret)
        {
            return pbook;
        }
    }

    return NULL;
}

int  fun_queue_add_book(char *bookname)
{
    queue_book_struct *pbook = NULL;

    pbook = (queue_book_struct *)malloc(sizeof(queue_book_struct));
    memset(pbook,0, sizeof(queue_book_struct));


	pbook->useflag = TRUE;
	pbook->borrowflag = TRUE;
	strcpy(pbook->bookname,bookname);


    list_add_tail(&(pbook->book_queue_node), &books_queue_head);

	printf("add book success, bookname = %s\n", bookname);
	return TRUE;
}

void fun_queue_push_book_to_cabinet(char *bookname)
{
	queue_book_struct *pbook = NULL;
	int ret = 0;
	
	pbook = (queue_book_struct*)fun_queue_find_book_in_cabinet(bookname);

	if (pbook == NULL)
	{
		printf("add book to book cabinet\n");
		ret = fun_queue_add_book(bookname);
		if (FALSE == ret)
		{
			printf("the book cabinet is full\n");
		}
		return;
	}

	printf("book is already exist\n");
	return;
}

void fun_queue_pop_book_from_cabinet()
{
	queue_book_struct *pbook = NULL;

	pbook = list_first_entry_or_null(&books_queue_head, queue_book_struct, book_queue_node);
	
	if (pbook == NULL)
	{
		printf("the queue is empty now!\n");
		return;
	}

	printf("del book = %s\n", pbook->bookname);
	list_del(&(pbook->book_queue_node));
	free(pbook);

}

void fun_queue_print_all_books(void)
{
    queue_book_struct *pbook = NULL;
    queue_book_struct *ptempbook = NULL;

    list_for_each_entry_safe_ext(pbook, queue_book_struct, ptempbook, &books_queue_head, book_queue_node)
    {
        if(pbook->useflag == TRUE)
        {
            printf("fun_print_all_books, bookname = %s\n", pbook->bookname);
        }
    }
	return;
}
