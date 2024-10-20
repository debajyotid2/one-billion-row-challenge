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
    key_comparer keycmp;
    size_t capacity;
    size_t size;
    KeyValuePair* pairs;
} hash_table_t;

typedef size_t (*hash_function)(void*, hash_table_t*);

void ht_init(hash_table_t **table, size_t capacity, hash_function a_hashfunc, key_comparer a_keycmp) {
    assert(capacity>0);
    assert(a_hashfunc!=NULL);

    *table = (hash_table_t *)malloc(sizeof(hash_table_t));

    (*table)->hashfunc = a_hashfunc;
    (*table)->keycmp = a_keycmp;
    (*table)->capacity = capacity;
    (*table)->size = 0;
    (*table)->pairs = (KeyValuePair*)calloc(capacity, sizeof(KeyValuePair));
    memset((*table)->pairs, 0x0, capacity*sizeof(KeyValuePair));
}

void** ht_values(hash_table_t *table) {
    if (table==NULL) return NULL;
    void** values = (void**)calloc(table->size, sizeof(void*));
    for (size_t i=0; i<table->size; ++i) {
        if (table->pairs[i].value==NULL) continue;
        values[i] = table->pairs[i].value;
    }
    return values;
}

KeyValuePair ht_at(hash_table_t* table, void* key) {
    if (table==NULL) {
        fprintf(stderr, "table pointer is null.\n");
        exit(1);
    }
    if (key==NULL) {
        fprintf(stderr, "key pointer is null.\n");
        exit(1);
    }
    int start_index = table->hashfunc(key, table);
    int index = start_index;
    int n = 1;
    while (table->pairs[index].key!=NULL) {
        if (table->keycmp(table->pairs[index].key, key) == 0)
            return table->pairs[index];
        index = (index + n*n)%table->capacity;
        if (index==start_index)
            break;
        n++;
    }

    KeyValuePair ret = {.key = NULL, .value=NULL};
    return ret;
}

KeyValuePair ht_twos_pow_at(hash_table_t* table, void* key) {
    if (table==NULL) {
        fprintf(stderr, "table pointer is null.\n");
        exit(1);
    }
    if (key==NULL) {
        fprintf(stderr, "key pointer is null.\n");
        exit(1);
    }
    int start_index = table->hashfunc(key, table);
    int index = start_index;
    int n = 1;
    while (table->pairs[index].key!=NULL) {
        if (table->keycmp(table->pairs[index].key, key) == 0)
            return table->pairs[index];
        index = (index + n*n) & (table->capacity - 1);
        if (index==start_index)
            break;
        n++;
    }

    KeyValuePair ret = {.key = NULL, .value=NULL};
    return ret;
}

KeyValuePair ht_at_index(hash_table_t* table, size_t i) {
    if (table==NULL) {
        fprintf(stderr, "table pointer is null.\n");
        exit(1);
    }
    return table->pairs[i];
}

void ht_print(hash_table_t *table) {
    assert(table!=NULL);

    for (size_t i=0; i<table->capacity; i++) {
        if (table->pairs[i].value==NULL)
            continue;
        printf("%zu: x\n", i);
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

void** ht_keys(hash_table_t *table) {
    if (table==NULL) return NULL;
    void** keys = (void**)calloc(table->size, sizeof(void*));
    for (size_t i=0, j=0; i<table->capacity; ++i) {
        if (table->pairs[i].key==NULL) continue;
        keys[j] = table->pairs[i].key;
        j++;
    }
    return keys;
}

bool ht_contains(hash_table_t* table, void* key) {
    if (table==NULL||key==NULL) return false;
    KeyValuePair res = ht_at(table, key);
    return res.key!=NULL;
}

bool ht_twos_pow_contains(hash_table_t* table, void* key) {
    if (table==NULL||key==NULL) return false;
    KeyValuePair res = ht_twos_pow_at(table, key);
    return res.key!=NULL;
}

bool ht_insert(hash_table_t *table, void* key, void* value) {
    if (table==NULL||key==NULL) return false;
    size_t start_index = table->hashfunc(key, table);
    size_t index = start_index;
    
    if (index >= table->capacity || table->size >= table->capacity)
        return false;
    int n = 1;
    while (table->pairs[index].key!=NULL) {
        if (table->keycmp(table->pairs[index].key, key) == 0) {
            if (table->pairs[index].value != NULL)
                return false;
            table->pairs[index].value = value;
            return true;
        }
        index = (index + n*n)%table->capacity;
        if (index==start_index)
            return false;
        n++;
    }
    table->pairs[index].key = key;
    table->pairs[index].value = value;
    table->size++;
    
    return true;
}

bool ht_twos_pow_insert(hash_table_t *table, void* key, void* value) {
    if (table==NULL||key==NULL) return false;
    size_t start_index = table->hashfunc(key, table);
    size_t index = start_index;
    
    if (index >= table->capacity || table->size >= table->capacity)
        return false;
    int n = 1;
    while (table->pairs[index].key!=NULL) {
        if (table->keycmp(table->pairs[index].key, key) == 0) {
            if (table->pairs[index].value != NULL)
                return false;
            table->pairs[index].value = value;
            return true;
        }
        index = (index + n*n) & (table->capacity-1);
        if (index==start_index)
            return false;
        n++;
    }
    table->pairs[index].key = key;
    table->pairs[index].value = value;
    table->size++;
    
    return true;
}

bool ht_insert_by_index(hash_table_t *table, size_t index, void* key, void* value) {
    if (table==NULL||key==NULL) return false;
    
    if (index >= table->capacity || table->size >= table->capacity)
        return false;
    if (table->pairs[index].key!=NULL)
        return false;
    if (table->pairs[index].key==NULL)
        table->pairs[index].key = key;
    table->pairs[index].value = value;
    table->size++;
    
    return true;
}

void* ht_remove(hash_table_t *table, void* key) {
    if (table==NULL||key==NULL) return NULL;

    size_t start_index = table->hashfunc(key, table);
    size_t index = start_index;
    
    if (index >= table->capacity)
        return NULL;
    int n = 1;
    while (table->pairs[index].key!=NULL) {
        if (table->keycmp(table->pairs[index].key, key) == 0)
            break;
        index = (index + n*n)%table->capacity;
        if (index==start_index)
            return NULL;
        n++;
    }
    
    void* removed = table->pairs[index].value;
    table->pairs[index].key = NULL;
    table->pairs[index].value = NULL;
    table->size--;
    return removed;
}

void* ht_twos_pow_remove(hash_table_t *table, void* key) {
    if (table==NULL||key==NULL) return NULL;

    size_t start_index = table->hashfunc(key, table);
    size_t index = start_index;
    
    if (index >= table->capacity)
        return NULL;
    int n = 1;
    while (table->pairs[index].key!=NULL) {
        if (table->keycmp(table->pairs[index].key, key) == 0)
            break;
        index = (index + n*n) & (table->capacity - 1);
        if (index==start_index)
            return NULL;
        n++;
    }
    
    void* removed = table->pairs[index].value;
    table->pairs[index].key = NULL;
    table->pairs[index].value = NULL;
    table->size--;
    return removed;
}

void ht_destroy(hash_table_t *table) {
    assert(table!=NULL);

    free(table->pairs);
}
