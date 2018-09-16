#include<stdio.h>
#include <string.h>
#include"my_array.h"

// init array book cabinet
#define books_max_num 6
array_book_cabinet_struct g_book_arr[books_max_num] = {0};


int fun_find_book_in_cabinet(char *bookname)
{
	int loop;
	int ret;
	for (loop = 0; loop < books_max_num; loop++)
	{
		ret = strcmp(bookname,g_book_arr[loop].bookname);
		if(FALSE == ret)
		{
			return loop;
		}
	}

	return books_max_num;
}

int fun_create_index_in_cabinet(void)
{
	int loop;
	for (loop = 0; loop < books_max_num; loop++)
	{
		if(g_book_arr[loop].useflag == FALSE)
		{
			return loop;
		}
	}

	return books_max_num;
}


int  fun_add_book(char *bookname)
{
	int book_index = 0;
	
	book_index = fun_create_index_in_cabinet();

	if (book_index >=  books_max_num)
	{
		return FALSE;
	}

	g_book_arr[book_index].useflag = TRUE;
	g_book_arr[book_index].borrowflag = TRUE;
	strcpy(g_book_arr[book_index].bookname,bookname);
	

	printf("add book success, bookname = %s\n", bookname);
	return TRUE;
}

void fun_add_book_to_cabinet(char *bookname)
{
	int find_index = 0;
	int ret = 0;
	
	find_index = fun_find_book_in_cabinet(bookname);

	if (find_index >= books_max_num)
	{
		printf("add book to book cabinet\n");
		ret = fun_add_book(bookname);
		if (FALSE == ret)
		{
			printf("the book cabinet is full\n");
		}
		return;
	}

	printf("book is already exist\n");
	return;
}

void fun_del_book_from_cabinet(char *bookname)
{
	int find_index = 0;
	
	find_index = fun_find_book_in_cabinet(bookname);

	if (find_index >= books_max_num)
	{
		printf("cannot find the book in book cabinet\n");
		return;
	}

	g_book_arr[find_index].useflag = FALSE;
	g_book_arr[find_index].borrowflag = FALSE;
}

void fun_print_all_books(void)
{
	int loop;
	for (loop = 0; loop < books_max_num; loop++)
	{
		if(g_book_arr[loop].useflag == TRUE)
		{
			printf("fun_print_all_books, bookname = %s\n", g_book_arr[loop].bookname);
		}
	}
	return;
}
