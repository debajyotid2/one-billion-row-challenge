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

#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

typedef struct {
    void* key;
    void* value;
} KeyValuePair;

typedef struct hash_table hash_table_t;
typedef size_t (*hash_function)(void*, hash_table_t*);
typedef bool (*key_comparer)(void*, void*);

void ht_init(hash_table_t **table, size_t capacity, hash_function a_hashfunc, key_comparer a_keycmp);
void** ht_values(hash_table_t *table);
KeyValuePair ht_at(hash_table_t* table, void* key);
KeyValuePair ht_twos_pow_at(hash_table_t* table, void* key);
KeyValuePair ht_at_index(hash_table_t* table, size_t i);
bool ht_contains(hash_table_t* table, void* key);
bool ht_twos_pow_contains(hash_table_t* table, void* key);
void ht_print(hash_table_t *table);
size_t ht_size(hash_table_t *table);
size_t ht_capacity(hash_table_t *table);
void** ht_keys(hash_table_t *table);
bool ht_insert(hash_table_t *table, void* key, void* value);
bool ht_twos_pow_insert(hash_table_t *table, void* key, void* value);
bool ht_insert_by_index(hash_table_t *table, size_t index, void* key, void* value);
void* ht_remove(hash_table_t *table, void* key);
void* ht_twos_pow_remove(hash_table_t *table, void* key);
void ht_destroy(hash_table_t *table);

#endif
