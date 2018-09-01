/* main.c - Hashtable test
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtab.h"
#include "strht.h"

#define strht_search(h, k)    ht_search(h, k, strlen(k) + 1)
#define strht_insert(h, k, v) ht_insert(h, k, strlen(k) + 1, v, strlen(v) + 1)
#define strht_remove(h, k)    ht_remove(h, k, strlen(k) + 1);


void ht_print_hash_tab(hashtab_head * hashtable)
{
    hashtab_node_t *pos_node;
    int loop;
    /* Free each linked list in hashtable. */

    for (loop = 0; loop < (int) hashtable->slotnum; loop++)
	{
		for(pos_node = hashtable->parr[loop]; pos_node != NULL; pos_node = pos_node->next)
		{
		    /* print node */
			printf("pos_node->key = %s, pos_node_value = %s\n",(char *)(pos_node->key),(char *)(pos_node->value));
		}
    }
}


int main()
{
    hashtab_head *test_ht_head = ht_init(2, NULL);

    /* stick some data into the table (using the string front-end)
	void *ht_insert(hashtab_head * hashtable,
		void *key, size_t keylen, void *value, size_t vallen)*/
    strht_insert(test_ht_head, "1 Perl", "Language");
    strht_insert(test_ht_head, "2 GNU", "System");
    strht_insert(test_ht_head, "3 Java", "Verbose");
    strht_insert(test_ht_head, "4 Pidgin", "Instant Messenger");
    strht_insert(test_ht_head, "5 Firefox", "Web Browser");
    strht_insert(test_ht_head, "6 ofo", "bike");
    strht_insert(test_ht_head, "6 ofo", "bike1");
	

    /* display table data */
	ht_print_hash_tab(test_ht_head);

    /* remove table data */
	printf("remove 1 Perl\n");
	strht_remove(test_ht_head, "1 Perl");

	

    /* display table data */
	ht_print_hash_tab(test_ht_head);


    /* free the hashtable */
    ht_destroy(test_ht_head);

    return EXIT_SUCCESS;
}
