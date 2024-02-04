#ifndef HASHTABLE
#define HASHTABLE
#define HT_PRIME_1 151
#define HT_PRIME_2 153

typedef struct
{
    char *key;
    char *value;
} h_item;

typedef struct
{
    int size;
    int count;
    h_item **item;
} h_table;

// HT_DELETED_ITEM is pointer to global sentinel item which represents that a bucket item is empty
static h_item HT_DELETED_ITEM = {NULL, NULL};

void ht_insert(h_table *ht, const char *k, const char *v);
char *ht_search(h_table *ht, const char *k);
void ht_delete(h_table *ht, const char *k);

#endif