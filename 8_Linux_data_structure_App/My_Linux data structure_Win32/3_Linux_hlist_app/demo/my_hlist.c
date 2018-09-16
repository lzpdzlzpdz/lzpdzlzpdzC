/********************************************************
1. how to define a hlist head
struct hlist_head books_hlist_head[books_hash_max_slot] = {0};

2. how to cal a hash value
int fun_book_cal_hash(char *bookname)

********************************************************/
#include<stdio.h>
#include <string.h>
#include"my_hlist.h"

// init hlist book cabinet
#define books_max_num 120
#define books_hash_max_slot 6
struct hlist_head books_hlist_head[books_hash_max_slot] = {0};


void fun_book_hlist_head_init(void)
{
    int loop;
    for(loop=0; loop < books_hash_max_slot; loop++)
    {
	    INIT_HLIST_HEAD(&books_hlist_head[loop]);
	}
}

int fun_book_cal_hash(char *bookname)
{
    char *pos = NULL;
    unsigned int str_val = 0;

    if(NULL == bookname)
    {
        return 0;
    }

    for(pos = bookname; *pos; pos++)
    {
        str_val = str_val*31 + *pos;
    }

    printf("str_val = %lu, hash_value = %lu\n", str_val, str_val%books_hash_max_slot);
    return str_val%books_hash_max_slot;
}

void *fun_hlist_find_book_in_cabinet(char *bookname)
{
    hlist_book_struct *pbook = NULL;
    hlist_node_t *pos = NULL;
    hlist_node_t *tpos = NULL;
    int ret;

	hlist_for_each_entry_safe_ext(pbook, hlist_book_struct, pos, tpos, &books_hlist_head[fun_book_cal_hash(bookname)], book_hlist_node)
    {
        ret = strcmp(pbook->bookname, bookname);
        if(FALSE == ret)
        {
            return pbook;
        }
    }

    return NULL;
}

int  fun_hlist_add_book(char *bookname)
{
    hlist_book_struct *pbook = NULL;

    pbook = (hlist_book_struct *)malloc(sizeof(hlist_book_struct));
    memset(pbook,0, sizeof(hlist_book_struct));


	pbook->useflag = TRUE;
	pbook->borrowflag = TRUE;
	strcpy(pbook->bookname,bookname);


    hlist_add_head(&(pbook->book_hlist_node), &books_hlist_head[fun_book_cal_hash(bookname)]);
    
	printf("add book success, bookname = %s\n", bookname);
	return TRUE;
}

void fun_hlist_add_book_to_cabinet(char *bookname)
{
	hlist_book_struct *pbook = NULL;
	int ret = 0;
	
	pbook = (hlist_book_struct*)fun_hlist_find_book_in_cabinet(bookname);

	if (pbook == NULL)
	{
		printf("add book to book cabinet\n");
		ret = fun_hlist_add_book(bookname);
		if (FALSE == ret)
		{
			printf("the book cabinet is full\n");
		}
		return;
	}

	printf("book is already exist\n");
	return;
}

void fun_hlist_del_book_from_cabinet(char *bookname)
{
	hlist_book_struct *pbook = NULL;

	pbook = (hlist_book_struct *)fun_hlist_find_book_in_cabinet(bookname);

	if (pbook == NULL)
	{
		printf("cannot find the book in book cabinet\n");
		return;
	}

	printf("del book = %s\n", pbook->bookname);
	hlist_del(&(pbook->book_hlist_node));
	free(pbook);

}

void fun_hlist_print_all_books(void)
{
    hlist_book_struct *pbook = NULL;
    hlist_node_t *pos = NULL;
    hlist_node_t *tpos = NULL;

    int loop;
    for(loop=0; loop < books_hash_max_slot; loop++)
    {
		hlist_for_each_entry_safe_ext(pbook, hlist_book_struct, pos, tpos, &books_hlist_head[loop], book_hlist_node)
		{
			if(pbook->useflag == TRUE)
			{
				printf("fun_print_all_books, bookname = %s\n", pbook->bookname);
			}
		}
	}
	return;
}
