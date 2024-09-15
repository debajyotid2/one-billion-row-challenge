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
#include <assert.h>
#include "linked_list.h"

typedef struct hash_table hash_table_t;
typedef size_t (*hash_function)(ll_node_t *);

void ht_init(hash_table_t **, size_t, size_t (*)(ll_node_t *));
bool ht_insert_node(hash_table_t *, ll_node_t *);
size_t ht_size(hash_table_t *);
int *ht_get_keys(hash_table_t *);
ll_node_t *ht_remove_node(hash_table_t *, int, void *);
void ht_print_table(hash_table_t *);
linked_list_t *ht_get_values(hash_table_t *, size_t);
bool ht_insert_key(hash_table_t *, int);
bool ht_delete_key(hash_table_t *, int);
void ht_destroy(hash_table_t *);

#endif
