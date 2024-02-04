#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

// instance of item that fills in bucket
static h_item *ht_new_item(const char *k, const char *v)
{
    h_item *new = (h_item *)(sizeof(h_item));
    new->key = strdup(k);
    new->value = strdup(v);
    return new;
}

// initialization of new hash table
h_table *ht_new()
{
    h_table *new_table = (h_table *)(malloc(sizeof(h_table)));
    new_table->size = 53;
    new_table->count = 0;
    new_table->item = calloc((size_t)new_table->size, sizeof(h_item));
    return new_table;
}

// deletes an item from the hash table avoiding memory leaks
void ht_del_item(h_item *i)
{
    free(i->key);
    free(i->value);
    free(i);
}

void del_ht(h_table *ht)
{
    for (int i = 0; i < ht->size; i++)
    {
        h_item *hi = ht->item[i];
        if (hi != NULL)
            ht_del_item(hi);
    }
    free(ht->item);
    free(ht);
}

// building a hash function that returns a hash value
int h_value(const char *s, const int a, const int m)
{
    long hash = 0;
    int slen = strlen(s);
    for (int i = 0; i < slen; i++)
    {
        hash += ((long)(pow(a, slen - (i + 1)) * s[i]) % m);
    }
    return (int)hash;
}

// to avoid collisions we'll use double hashing method
int ht_get_hash(const char *s, const int num_bucket, int attempts)
{
    const int hash_a = h_value(s, HT_PRIME_1, num_bucket);
    const int hash_b = h_value(s, HT_PRIME_2, num_bucket);
    return (hash_a + (attempts * (hash_b + 1)) % num_bucket);
}

// for inserting a new key-value pair (iterates until it finds an empty bucket)
void ht_insert(h_table *ht, const char *k, const char *v)
{
    h_item *item = ht_new_item(k, v);
    int index = ht_get_hash(item->key, ht->size, 0);
    // cur_item serves no other purpose than checking for an empty bucket
    h_item *cur_item = ht->item[index];
    int i = 1;
    while (cur_item != NULL)
    {
        index = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->item[index];
        i++;
    }
    ht->item[index] = item;
    ht->count++;
}

// searches and returns the value of key passed
char *ht_search(h_table *ht, const char *key)
{
    int index = ht_get_hash(key, ht->size, 0);
    h_item *item = ht->item[index];
    int i = 1;
    while (item != NULL && item != &HT_DELETED_ITEM)
    {
        if (strcmp(item->value, key)==0 )
        {
            return item->value;
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->item[index];
        i++;
    }
    return item->value;
}

void ht_delete(h_table *ht, const char *key)
{
    int index = ht_get_hash(key, ht->count, 0);
    h_item *item = ht->item[index];
    int i = 1;
    while (item != NULL)
    {
        if (item != &HT_DELETED_ITEM)
        {
            if (strcmp(item->key, key) == 0)
            {
                ht_del_item(item);
                ht->item[index] = &HT_DELETED_ITEM;
            }
        }
        index = ht_get_hash(key,ht->size,i);
        item = ht->item[index];
        i++;
    }
    ht->count--;
}

int main()
{
    h_table *newtable = ht_new();
    del_ht(newtable);
}
