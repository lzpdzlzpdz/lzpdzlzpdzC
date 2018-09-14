#include<stdio.h>
#include"list.h"

#define max_hash_slot_num 6
#define max_entry_num 18



typedef struct _fsnode {
    int val;
    struct hlist_node hlist;
} fsnode;

struct hlist_head rm_hlist_head[max_hash_slot_num];

int cal_hash_val(int cell_val)
{
    return cell_val%max_hash_slot_num;
}

int main(){
    fsnode node[max_entry_num];

    fsnode *entry = NULL;
    struct hlist_node *pos = NULL;
    struct hlist_node *tpos = NULL;
	int loop = 0;
    int slop_loop = 0;


	printf("-----init Hlist hlist_head-----\n");
	for(slop_loop = 0; slop_loop < max_hash_slot_num; slop_loop++)
	{
    	INIT_HLIST_HEAD(&rm_hlist_head[loop]);
	}

	printf("-----init and add node-----\n");
	for(loop = 0; loop < max_entry_num; loop++)
    {   
        INIT_HLIST_NODE(&node[loop].hlist);
        node[loop].val = loop+50;
        hlist_add_head(&node[loop].hlist, &rm_hlist_head[cal_hash_val(node[loop].val)]);
    }


    printf("-----list  pos-----\n");
	for (pos = rm_hlist_head[0].first; 
		 pos && (tpos = pos->next,pos) && 
		( entry = hlist_entry(pos, fsnode, hlist),pos); 
		 pos = tpos)
	{
        printf("slop_index = 0, list find %d,\n", entry->val);    
    }

	printf("-----list pos-----\n");
    loop = 0;
    for(slop_loop = 0; slop_loop < max_hash_slot_num; slop_loop++)
    {
        hlist_for_each_entry_safe_ext(entry, fsnode, pos, tpos, &rm_hlist_head[slop_loop], hlist) 
    	{
            printf("list slop_loop = %d, cell_loop = %d, list find %d,\n", slop_loop, loop ,entry->val);  
            loop++;
        }
    }

	printf("-----del all pos in safety------\n");
    loop = 0;
    for(slop_loop = 0; slop_loop < max_hash_slot_num; slop_loop++)
    {

        hlist_for_each_entry_safe_ext(entry, fsnode, pos, tpos, &rm_hlist_head[slop_loop], hlist) 
        {
            printf("del slop_loop = %d, cell_loop = %d, list find %d,\n", slop_loop, loop ,entry->val);    
            hlist_del(&entry->hlist);
            loop++;
        }
    }

    if (hlist_empty(&rm_hlist_head)) {
        printf("list is null\n");
    }

    
    return 0;
}

