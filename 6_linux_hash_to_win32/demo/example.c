#include<stdio.h>
#include"list.h"

static inline void INIT_LIST_NODE(struct list_head *list)
{
	list->next = NULL;
	list->prev = NULL;
}

typedef struct _fsnode {
    int val;
    struct hlist_node hlist;
} fsnode;

struct hlist_head rm_hlist_head;


int main(){
    fsnode node1;
    fsnode node2;
    fsnode node3;
    fsnode *pos = NULL;
    fsnode *node = NULL;

	printf("-----init Hlist hlist_head-----\n");
    INIT_HLIST_HEAD(&rm_hlist_head);

	printf("-----init node-----\n");
    INIT_HLIST_NODE(&node1.hlist);
    node1.val = 111;
    INIT_HLIST_NODE(&node2.hlist);
    node2.val = 222;
    INIT_HLIST_NODE(&node3.hlist);
    node3.val = 333;

	printf("-----add node-----\n");
    hlist_add_head(&node1.hlist, &rm_hlist_head);
    hlist_add_head(&node2.hlist, &rm_hlist_head);
    hlist_add_head(&node3.hlist, &rm_hlist_head);

	printf("-----list node-----\n");
    hlist_for_each_entry_safe_ext(pos, fsnode, node, &rm_hlist_head, hlist) 
	{
        printf("list find %d,\n", pos->val);    
    }

	printf("------del all node in no safety------\n");
    hlist_for_each_entry_safe_ext(pos, fsnode, node,&rm_hlist_head, hlist) 
	{
        printf("del node,list find %d,\n", pos->val);    
	    //list_del(&pos->list);
    }

	printf("-----del all node in safety------\n");
    hlist_for_each_entry_safe_ext(pos, fsnode, node,&rm_hlist_head, hlist) 
	{
        printf("del node,list find %d,\n", pos->val);    
	    hlist_del(&pos->hlist);
    }

    if (hlist_empty(&rm_hlist_head)) {
        printf("list is null\n");
    }
    return 0;
}

