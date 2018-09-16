INIT_LIST_HEAD list_book_cabinet_head = {0};

typdef struct
{
	int useflag:1;
	int borrowflag:1;
	
	char bookname[60];
	INIT_LIST_NODE list;
}list_book_cabinet_struct;