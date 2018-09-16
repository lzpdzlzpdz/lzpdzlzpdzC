#include"my_array.h"


void main()
{
	//add book 
	fun_add_book_to_cabinet("sanguoyanyi");
	fun_add_book_to_cabinet("xiyouji");
	fun_add_book_to_cabinet("shuifuzhuang");

	//del book 
	fun_del_book_from_cabinet("sanguoyanyi");

	//list cabinet
	fun_print_all_books();

}