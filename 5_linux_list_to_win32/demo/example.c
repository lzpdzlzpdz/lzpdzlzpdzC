#include<stdio.h>
#include"list.h"

static inline void INIT_LIST_NODE(struct list_head *list)
{
	list->next = NULL;
	list->prev = NULL;
}

typedef struct _fsnode {
    int val;
    struct list_head list;
} fsnode;

struct list_head head;


int main(){
    fsnode node1;
    fsnode node2;
    fsnode node3;
    fsnode *pos = NULL;
    fsnode *node = NULL;

	printf("-----init List head-----\n");
    INIT_LIST_HEAD(&head);

	printf("-----init node-----\n");
    INIT_LIST_NODE(&node1.list);
    node1.val = 111;
    INIT_LIST_NODE(&node2.list);
    node2.val = 222;
    INIT_LIST_NODE(&node3.list);
    node3.val = 333;

	printf("-----add node-----\n");
    list_add_tail(&node1.list, &head);
    list_add_tail(&node2.list, &head);
    list_add_tail(&node3.list, &head);

	printf("-----list node-----\n");
    list_for_each_entry_ext(pos, fsnode, &head, list) 
	{
        printf("list find %d,\n", pos->val);    
    }

	printf("------del all node in no safety------\n");
    list_for_each_entry_ext(pos, fsnode,&head, list) 
	{
        printf("del node,list find %d,\n", pos->val);    
	    //list_del(&pos->list);
    }

	printf("-----del all node in safety------\n");
    list_for_each_entry_safe_ext(pos, fsnode, node,&head, list) 
	{
        printf("del node,list find %d,\n", pos->val);    
	    list_del(&pos->list);
    }

    if (list_empty(&head)) {
        printf("list is null\n");
    }
    return 0;
}

