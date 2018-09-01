/* hashtab.c - Simple, Reliable C Hashtable
https://blog.csdn.net/stayneckwind2/article/details/53574685
 */

#include <stdlib.h>
#include <string.h>
#include "hashtab.h"
#include <stdio.h>

hashtab_head *ht_init(size_t init_slot_num, int (*hash_func) (void *, size_t, size_t))
{
    hashtab_head *new_ht = (hashtab_head *) malloc(sizeof(hashtab_head));
    int loop = 0;

    new_ht->parr =
	(hashtab_node_t **) malloc(sizeof(hashtab_node_t *) * init_slot_num);

	printf("sizeof hashtab_node_t *=%d\n",sizeof(hashtab_node_t *));

    new_ht->slotnum = init_slot_num;
    new_ht->cellcnt = 0;
    /* all entries are empty */
    for (loop = 0; loop < (int) init_slot_num; loop++) 
	{
		new_ht->parr[loop] = NULL;
    }

    if (hash_func == NULL)
	{
		new_ht->hash_func = &ht_hash;
	}
    else
	{
		new_ht->hash_func = hash_func;
	}

    return new_ht;
}

void *ht_search(hashtab_head * hashtable, void *key, size_t keylen)
{
    int index = ht_hash(key, keylen, hashtable->slotnum);
	hashtab_node_t *pos_node = NULL;

    if (hashtable->parr[index] == NULL)
	{
		return NULL;
	}

	for(pos_node = hashtable->parr[index]; pos_node != NULL; pos_node = pos_node->next)
	{
		/* only compare matching keylens */
		if (pos_node->keylen == keylen) 
		{
			/* compare keys */
			if (memcmp(key, pos_node->key, keylen) == 0) 
			{
				return pos_node->value;
			}
		}
    }

    return NULL;
}

void *ht_insert(hashtab_head * hashtable,
		void *key, size_t keylen, void *value, size_t vallen)
{
    int index = ht_hash(key, keylen, hashtable->slotnum);
	hashtab_node_t *new_node;
    hashtab_node_t *pos_node, *last_node;
    last_node = NULL;

	printf("index = %d\n", index);

    /*如果hash表的槽位号不为空，查询元素是否已经存�*/
	for(pos_node = hashtable->parr[index]; pos_node != NULL; pos_node = pos_node->next)
	{
		if (pos_node->keylen == keylen) {
			/* compare keys */
			if (memcmp(key, pos_node->key, keylen) == 0) 
			{
				/* this key already exists, replace it */
				if (pos_node->vallen != vallen) {
					/* new value is a different size */
					free(pos_node->value);
					pos_node->value = malloc(vallen);
					if (pos_node->value == NULL)
					return NULL;
				}
				memcpy(pos_node->value, value, vallen);
				pos_node->vallen = vallen;
				return pos_node->value;
			}
		}

		last_node = pos_node;
    }

    /*如果hash表的槽位号为空，则新建一个槽位号 */
    new_node = (hashtab_node_t *) malloc(sizeof(hashtab_node_t));
    if (new_node == NULL)
    {
	    return NULL;
    }

    /* get some memory for the new node data */
    new_node->key = malloc(keylen);
    new_node->value = malloc(vallen);
    if (new_node->key == NULL || new_node->key == NULL) 
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return NULL;
    }

    /* copy over the value and key */
    memcpy(new_node->key, key, keylen);
    memcpy(new_node->value, value, vallen);
    new_node->keylen = keylen;
    new_node->vallen = vallen;

    /* no next node */
    new_node->next = NULL;

    /* Tack the new node on the end or right on the table. */
	/*如果槽位号为空，则查询槽位号右边，如果槽位号不为空，则插在最�*/
    if (last_node != NULL)
	{
		last_node->next = new_node;
	}
    else
	{
		hashtable->parr[index] = new_node;
	}
    hashtable->cellcnt++;
    return new_node->value;
}

/* delete the given key from the hashtable */
void ht_remove(hashtab_head * hashtable, void *key, size_t keylen)
{
    hashtab_node_t *last_node, *pos_node;
    int index = ht_hash(key, keylen, hashtable->slotnum);
    last_node = NULL;

	for(pos_node = hashtable->parr[index]; pos_node != NULL; pos_node = pos_node->next)
	{
		if (pos_node->keylen == keylen) 
		{
		    /* compare keys */
		    if (memcmp(key, pos_node->key, keylen) == 0)
			{
				/* free node memory */
				free(pos_node->value);
				free(pos_node->key);

				/* adjust the list pointers */
				if (last_node != NULL)
				    last_node->next = pos_node->next;
				else
				    hashtable->parr[index] = pos_node->next;

				/* free the node */
				free(pos_node);
				break;
		    }
		}

		last_node = pos_node;
    }
}

/* free all resources used by the hashtable */
void ht_destroy(hashtab_head * hashtable)
{
    hashtab_node_t *pos_node, *last_node;

    /* Free each linked list in hashtable. */
    int loop;
    for (loop = 0; loop < (int) hashtable->slotnum; loop++)
	{
		pos_node = hashtable->parr[loop];
		while(pos_node != NULL)
		{
		    /* destroy node */
		    free(pos_node->key);
		    free(pos_node->value);
		    last_node = pos_node;
			pos_node = pos_node->next;
		    free(last_node);
		}
    }

    free(hashtable->parr);
    free(hashtable);
}

int ht_hash(void *key, size_t keylen, size_t hashtab_size)
{
    int sum = 0;

    /* very simple hash function for now */
    int loop;
    for (loop = 0; loop < (int) keylen; loop++) {
	sum += ((unsigned char *) key)[loop] * (loop + 1);
    }

    return (sum % (int) hashtab_size);
}
