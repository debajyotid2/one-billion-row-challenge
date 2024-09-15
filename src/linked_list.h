/* Linked list

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

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

typedef struct node ll_node_t;
typedef ll_node_t** linked_list_t;

ll_node_t *ll_create_node(int, void *);
int ll_get_node_value(ll_node_t *);
const void *ll_get_node_data(ll_node_t *);
void ll_init(linked_list_t *, int, void *);
ll_node_t *ll_dequeue_node(linked_list_t, ll_node_t *);
void ll_add_at_head(linked_list_t, ll_node_t *);
void ll_add_at_tail(linked_list_t, ll_node_t *);
void ll_add_after_node(ll_node_t *, ll_node_t *);
void ll_delete_linked_list(linked_list_t *);
bool ll_is_enqueued(ll_node_t *);
size_t ll_length(linked_list_t);
ll_node_t *ll_find(linked_list_t, int);
void ll_print(linked_list_t);
bool ll_delete_node(ll_node_t **);

#endif
