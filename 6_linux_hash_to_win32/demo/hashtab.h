/* hashtab.h - Simple, Reliable C Hashtable
 */

/* Only copies of keys and values are stored in the table, not the
 * orignals.
 *
 * The hashtable does not grow automatically, but when the hashtable
 * grow function is called. Growing the hashtable is a safe operation:
 * if growing the hashtable fails, the existing hashtable is not
 * destroyed or modified.
 *
 * This hashtable is not thread-safe.
 */

#ifndef HASHTAB_H
#define HASHTAB_H

#include <stdlib.h>
#include <string.h>


typedef struct hashtab_node_t {
    void *key;			/* key for the node */
    size_t keylen;		/* length of the key */
    void *value;		/* value for this node */
    size_t vallen;		/* length of the value */

    struct hashtab_node_t *next;	/* next node (open hashtable) */
} hashtab_node_t;

typedef struct hashtab_head {
    hashtab_node_t **parr;
    size_t slotnum;		/* number of slots in hash table */
    int cellcnt;			/* number of elements in hash table */
    int (*hash_func) (void *, size_t, size_t);	/* hash function */
} hashtab_head;

/* Iterator type for iterating through the hashtable. */
typedef struct hashtab_iter_t {
    /* key and value of current item */
    void *key;
    void *value;
    size_t keylen;
    size_t vallen;

    /* bookkeeping data */
    struct hashtab_internal_t {
	hashtab_head *hashtable;
	hashtab_node_t *node;
	int index;
    } internal;

} hashtab_iter_t;

/* Initialize a new hashtable (set bookingkeeping data) and return a
 * pointer to the hashtable. A hash function may be provided. If no
 * function pointer is given (a NULL pointer), then the built in hash
 * function is used. A NULL pointer returned if the creation of the
 * hashtable failed due to a failed malloc(). */
hashtab_head *ht_init(size_t size, int (*hash_func)
		    (void *key, size_t keylen, size_t ht_size));

/* Fetch a value from table matching the key. Returns a pointer to
 * the value matching the given key. */
void *ht_search(hashtab_head * hashtable, void *key, size_t keylen);

/* Put a value into the table with the given key. Returns NULL if
 * malloc() fails to allocate memory for the new node. */
void *ht_insert(hashtab_head * hashtable,
		void *key, size_t keylen, void *value, size_t vallen);

/* Delete the given key and value pair from the hashtable. If the key
 * does not exist, no error is given. */
void ht_remove(hashtab_head * hashtable, void *key, size_t keylen);


/* Free all resources used by the hashtable. */
void ht_destroy(hashtab_head * hashtable);


/* Default hashtable hash function. */
int ht_hash(void *key, size_t key_size, size_t hashtab_size);

#endif
