#include"my_list.h"

void main()
{
	//init book list
	fun_book_list_head_init();
	printf("-------------------------------\n");

	//add book 
	fun_list_add_book_to_cabinet("sanguoyanyi");
	fun_list_add_book_to_cabinet("xiyouji");
	fun_list_add_book_to_cabinet("shuifuzhuang");
	printf("-------------------------------\n");

	//del book 
	fun_list_del_book_from_cabinet("sanguoyanyi");
	printf("-------------------------------\n");

	//list cabinet
	fun_list_print_all_books();
	printf("-------------------------------\n");
}