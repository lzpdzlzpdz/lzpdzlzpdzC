#include"my_queue.h"

void main()
{
	printf("my_queue demo\n");

	//init book queue
	fun_book_queue_head_init();
	printf("-------------------------------\n");

	//add book 
	fun_queue_push_book_to_cabinet("sanguoyanyi");
	fun_queue_push_book_to_cabinet("xiyouji");
	fun_queue_push_book_to_cabinet("shuifuzhuang");
	printf("-------------------------------\n");

	//del book 
	fun_queue_pop_book_from_cabinet();
	fun_queue_pop_book_from_cabinet();
	fun_queue_pop_book_from_cabinet();
	fun_queue_pop_book_from_cabinet();

	printf("-------------------------------\n");

	//queue cabinet
	fun_queue_print_all_books();
	printf("-------------------------------\n");
}


