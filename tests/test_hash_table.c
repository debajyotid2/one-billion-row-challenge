#include "../src/hash_table.h"
#include <criterion/criterion.h>
#include <stdbool.h>
#include <stddef.h>

const size_t TABLE_SIZE = 20;
hash_table_t *table = NULL;
size_t* a = NULL;
size_t* b = NULL;

size_t a_hashfunc(void *key, hash_table_t* table) {
    return (*(size_t*)key)%TABLE_SIZE;
}

bool key_equal(void* key1, void* key2) {
    if (key1==NULL || key2==NULL) {
        return false;
    }
    return *(size_t*)key1 == *(size_t*)key2;
}

void testsetup(void) {
    ht_init(&table, TABLE_SIZE, a_hashfunc, key_equal);
    size_t* a = (size_t*)calloc(5, sizeof(size_t));
    size_t* b = (size_t*)calloc(5, sizeof(size_t));
    a[0] = b[0] =20;
    a[1] = b[1] =24;
    a[2] = b[2] =23;
    a[3] = b[3] =25;
    a[4] = b[4] =26;
    ht_insert(table, &a[0], &b[3]);
    ht_insert(table, &a[1], &b[2]);
    ht_insert(table, &a[2], &b[1]);
    ht_insert(table, &a[3], &b[4]);
    ht_insert(table, &a[4], &b[0]);
}

void testteardown(void) {
    free(a);
    free(b);
    ht_destroy(table);
    free(table);
}

TestSuite(hash_table_tests, .init=testsetup, .fini=testteardown);

Test(hash_table_tests, ht_insert) {
    size_t* a = (size_t*)malloc(sizeof(size_t));
    size_t* b = (size_t*)malloc(sizeof(size_t));
    *a = 19;
    *b = 99;
    cr_expect(ht_insert(table, a, b),
            "ht_insert should successfully insert the node into the table.");
    cr_expect(ht_insert(table, a, NULL)==false,
            "ht_insert must fail to enter a pre-existing key to the hash table.");
}

Test(hash_table_tests, ht_remove) {
    size_t* b = (size_t*)malloc(sizeof(size_t));
    *b = 24;
    cr_expect(ht_remove(table, b)!=NULL,
            "ht_remove must successfully remove an existing key from the hash table.");
    cr_expect(ht_remove(table, b)==NULL,
            "ht_remove must fail to delete a non-existing key from the hash table.");
}

Test(hash_table_tests, ht_keys) {
    size_t keys[] = {20, 24, 23, 25, 26};
    void** ret_keys = ht_keys(table);

    for (size_t i=0; i<5; i++) {
        size_t j = 0;
        for (; j<5; ++j) {
            if (*((size_t*)ret_keys[j])!=keys[i])
                continue;
            break;
        }
        cr_expect(j<5, "ht_keys should return the correct keys.");
    }
}

Test(hash_table_tests, ht_at) {
    size_t key = 24;
    KeyValuePair kv = ht_at(table, &key);
    cr_expect(*(size_t*)kv.value==23,
            "ht_at should return the correct value.");
    
    key = 16;
    kv = ht_at(table, &key);
    cr_expect(kv.value==NULL,
            "ht_value should return NULL when a key does not exist in the table.");
}

Test(hash_table_tests, ht_size) {
    cr_expect(ht_size(table)==5,
            "ht_size should return the correct table size.");
}
