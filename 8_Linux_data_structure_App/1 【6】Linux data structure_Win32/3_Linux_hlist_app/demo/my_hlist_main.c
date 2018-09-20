#include"my_hlist.h"

void main()
{
	//init book list
	fun_book_hlist_head_init();
	printf("-------------------------------\n");

	//add book 
	fun_hlist_add_book_to_cabinet("sanguoyanyi");
	fun_hlist_add_book_to_cabinet("xiyouji");
	fun_hlist_add_book_to_cabinet("shuifuzhuang");
	printf("-------------------------------\n");

	//del book 
	fun_hlist_del_book_from_cabinet("sanguoyanyi");
	printf("-------------------------------\n");

	//list cabinet
	fun_hlist_print_all_books();
	printf("-------------------------------\n");
}