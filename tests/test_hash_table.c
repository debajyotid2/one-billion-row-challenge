#include "../src/linked_list.h"
#include "../src/hash_table.h"
#include <criterion/criterion.h>
#include <stdbool.h>
#include <stddef.h>

const size_t TABLE_SIZE = 20;
hash_table_t *table = NULL;

size_t a_hashfunc(ll_node_t *a_node) {
    return ll_get_node_value(a_node)%TABLE_SIZE;
}

void testsetup(void) {
    ht_init(&table, TABLE_SIZE, a_hashfunc);
    ll_node_t *a = ll_create_node(24, NULL);
    ll_node_t *b = ll_create_node(26, NULL);
    ll_node_t *c = ll_create_node(27, NULL);
    ll_node_t *d = ll_create_node(28, NULL);
    ll_node_t *e = ll_create_node(44, NULL);
    ht_insert_node(table, a);
    ht_insert_node(table, b);
    ht_insert_node(table, c);
    ht_insert_node(table, d);
    ht_insert_node(table, e);
}

void testteardown(void) {
    ht_destroy(table);
}

TestSuite(hash_table_tests, .init=testsetup, .fini=testteardown);

Test(hash_table_tests, ht_insert_node) {
    ll_node_t *e = ll_create_node(44, NULL);
    cr_expect(ht_insert_node(table, e),
            "ht_insert_node should successfully insert the node into the table.");
}

Test(hash_table_tests, ht_insert_key) {
    cr_expect(ht_insert_key(table, 12),
            "ht_insert_key must successfully enter a new key to the hash table.");
    cr_expect(ht_insert_key(table, 12)==false,
            "ht_insert_key must fail to enter a pre-existing key to the hash table.");
}

Test(hash_table_tests, ht_delete_key) {
    cr_expect(ht_delete_key(table, 4),
            "ht_delete_key must successfully delete an existing key from the hash table.");
    cr_expect(ht_delete_key(table, 4)==false,
            "ht_delete_key must fail to delete a non-existing key from the hash table.");
}

Test(hash_table_tests, ht_get_keys) {
    int keys[] = {4, 6, 7, 8};
    int *ret_keys = ht_get_keys(table);

    for (size_t i=0; i<4; i++) {
        size_t j;
        for (j=0; j<4; j++) {
            if (ret_keys[j]!=keys[i])
                continue;
            break;
        }
        cr_expect(j<4,
                "ht_get_keys should return the correct keys.");
    }
    free(ret_keys);
}

Test(hash_table_tests, ht_get_values) {
    linked_list_t *list = ht_get_values(table, 4);
    cr_expect(ll_length(*list)==2,
            "ht_get_values should return the values in a list of correct length.");
    ll_delete_linked_list(list);

    list = ht_get_values(table, 16);
    cr_expect((*list)==NULL,
            "ht_get_values should return NULL when a key does not exist in the table.");
}

Test(hash_table_tests, ht_remove_node) {
    ll_node_t *a_node = ht_remove_node(table, 44, NULL);
    cr_expect(ll_get_node_value(a_node)==44,
        "ht_remove_node should remove a node with the correct value.");
    ll_delete_node(&a_node);
    cr_expect(ht_remove_node(table, 44, NULL)==NULL,
        "ht_remove_node should return NULL when trying to remove a non-existent node.");
}

Test(hash_table_tests, ht_size) {
    cr_expect(ht_size(table)==4,
            "ht_size should return the correct table size.");
}
