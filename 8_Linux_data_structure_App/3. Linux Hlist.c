#define hash_slot_max_num  6
#define cell_max_num  20
hlist_head hlist_book_cabinet_head[hash_slot_max_num] = {{0}};

typdef struct
{
	int useflag:1;
	int borrowflag:1;
	
	char bookname[60];
	hlist_node hlist;
}hlist_book_cabinet_struct;