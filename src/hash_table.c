/* Hash table

                    GNU AFFERO GENERAL PUBLIC LICENSE
                       Version 3, 19 November 2007

    Copyright (C) 2024  Debajyoti Debnath

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include "hash_table.h"

typedef struct hash_table {
    hash_function hashfunc;
    size_t capacity;
    size_t size;
    linked_list_t *keys;
} hash_table_t;

typedef size_t (*hash_function)(ll_node_t *, hash_table_t* table);

void ht_init(hash_table_t **table, size_t capacity, hash_function a_hashfunc) {
    assert(capacity>0);
    assert(a_hashfunc!=NULL);

    *table = (hash_table_t *)malloc(sizeof(hash_table_t));

    (*table)->hashfunc = a_hashfunc;
    (*table)->capacity = capacity;
    (*table)->size = 0;
    (*table)->keys = (linked_list_t *)calloc(capacity, sizeof(linked_list_t));

    for (size_t i=0; i<capacity; i++)
        (*table)->keys[i] = NULL;
}

linked_list_t ht_get_values(hash_table_t *table, size_t index) {
    return table->keys[index];
}

const void* ht_at(size_t i, hash_table_t* table) {
    linked_list_t values = ht_get_values(table, i);
    if (values==NULL) return NULL;
    ll_node_t* head_node = ll_find(values, 0);
    return ll_get_node_data(head_node);
}

void ht_print_table(hash_table_t *table) {
    assert(table!=NULL);

    for (size_t i=0; i<table->capacity; i++) {
        if (table->keys[i]==NULL)
            continue;
        ll_print(table->keys[i]);
    }
}

size_t ht_size(hash_table_t *table) {
    assert(table!=NULL);
    return table->size;
}

size_t ht_capacity(hash_table_t *table) {
    assert(table!=NULL);
    return table->capacity;
}

int *ht_get_keys(hash_table_t *table) {
    assert(table!=NULL);
    size_t size = ht_size(table);
    if (size==0)
        return NULL;
    int *keys = (int *)calloc(size, sizeof(int));
    size_t j=0;
    for (size_t i=0; i<table->capacity; i++) {
        if (table->keys[i]==NULL)
            continue;
        keys[j] = ll_get_node_value(*(table->keys[i]));
        j++;
    }
    return keys;
}

bool ht_insert_node(hash_table_t *table, ll_node_t *a_node) {
    size_t index = table->hashfunc(a_node, table);
    
    if (index>table->capacity || table->size >= table->capacity)
        return false;
    if (table->keys[index]==NULL)
        ll_init(&(table->keys[index]), index, NULL);
    // Limit the size of linked lists to be 1
    if (ll_length(table->keys[index])==1)
        return false;
    ll_add_at_head(table->keys[index], a_node);
    table->size++;
    
    return true;
}

bool ht_insert_key(hash_table_t *table, int value) {
    assert(table!=NULL);

    ll_node_t *dummy = ll_create_node(value, NULL);
    size_t index = table->hashfunc(dummy, table);
    ll_delete_node(&dummy);
    
    if (index>=table->capacity)
        return false;
    if (table->keys[index]!=NULL)
        return false;
    ll_init(&(table->keys[index]), value, NULL);
    return true;
}

bool ht_delete_key(hash_table_t *table, int value) {
    assert(table!=NULL);

    ll_node_t *dummy = ll_create_node(value, NULL);
    size_t index = table->hashfunc(dummy, table);
    ll_delete_node(&dummy);
    
    if (index>=table->capacity)
       return false;

    if (table->keys[index]==NULL)
       return false;

    ll_delete_linked_list(&(table->keys[index]));
    table->keys[index] = NULL;
    table->size--;
 
    return true;
}

ll_node_t *ht_remove_node(hash_table_t *table, int value, void *data) {
    assert(table!=NULL);

    ll_node_t *removed = NULL;
    ll_node_t *dummy = ll_create_node(value, data);
    size_t index = table->hashfunc(dummy, table);
    ll_delete_node(&dummy);

    if (index>table->capacity)
        return NULL;
    if (table->keys[index]==NULL)
        return NULL;
    
    removed = ll_find(table->keys[index], value);
    if (removed!=NULL)
        removed = ll_dequeue_node(table->keys[index], removed);
    if (ll_length(table->keys[index])==0) {
        ll_delete_linked_list(&(table->keys[index]));
        table->keys[index] = NULL;
        table->size--;
    }
    return removed;
}

void ht_destroy(hash_table_t *table) {
    assert(table!=NULL);

    for (size_t i=0; i<table->capacity; i++) {
        if (table->keys[i]==NULL)
            continue;
        ll_delete_linked_list(&(table->keys[i]));
    }
    free(table->keys);
    free(table);
}
