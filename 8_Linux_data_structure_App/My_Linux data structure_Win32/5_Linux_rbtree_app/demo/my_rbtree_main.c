#include"my_rbtree.h"

void main()
{
	//init book rbtree
	fun_book_rbtree_head_init();
	printf("-------------------------------\n");

	//add book 
	fun_rbtree_add_book_to_cabinet("sanguoyanyi");
	fun_rbtree_add_book_to_cabinet("xiyouji");
	fun_rbtree_add_book_to_cabinet("shuifuzhuang");
	printf("-------------------------------\n");

	//del book 
	fun_rbtree_del_book_from_cabinet("sanguoyanyi");
	printf("-------------------------------\n");

	//rbtree cabinet
	fun_rbtree_print_all_books();
	printf("-------------------------------\n");
}