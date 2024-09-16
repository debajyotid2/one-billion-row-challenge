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

#include "linked_list.h"

typedef struct node {
    void *data;
    int value;
    struct node* next;
} ll_node_t;

typedef ll_node_t** linked_list_t;

ll_node_t *ll_create_node(int value, void *data) {
    ll_node_t *ptr = (ll_node_t *)malloc(sizeof(ll_node_t));
    
    ptr->data = data;
    ptr->value = value;
    ptr->next = NULL;

    return ptr;
}

void ll_init(linked_list_t *list, int value, void *data) {
    *list = (linked_list_t)malloc(sizeof(ll_node_t *));
    **list = ll_create_node(value, data);
}

bool ll_is_enqueued(ll_node_t *a_node) {
    return (a_node->next==NULL) ? false: true;
}

int ll_get_node_value(ll_node_t *a_node) {
    assert(a_node!=NULL);
    return a_node->value;
}

const void *ll_get_node_data(ll_node_t *a_node) {
    assert(a_node!=NULL);
    const void *data_ptr = a_node->data;
    return data_ptr;
}

ll_node_t *ll_dequeue_node(linked_list_t list, ll_node_t *a_node) {
    ll_node_t *ptr = *list;

    if (ptr!=a_node) {
        while (ptr!=NULL) {
            if (ptr->next==a_node)
                break;
            ptr = ptr->next;
        }
    }
    else {
        *list = a_node->next;
    }
    if (ptr!=NULL)
        ptr->next = a_node->next;
    else
        return NULL;
    a_node->next = NULL;

    return a_node;
}

void ll_add_at_head(linked_list_t list, ll_node_t *newnode) {
    newnode->next = (*list)->next;
    (*list)->next = newnode;
}

void ll_add_at_tail(linked_list_t list, ll_node_t *newnode) {
    ll_node_t *ptr = *list;

    while (ptr->next!=NULL)
        ptr = ptr->next;
    ptr->next = newnode;
}

void ll_add_after_node(ll_node_t *present, ll_node_t *newnode) {
    newnode->next = present->next;
    present->next = newnode;
}

bool ll_delete_node(ll_node_t **a_node) {
    if (ll_is_enqueued(*a_node))
        return false;
    if ((*a_node)->data!=NULL)
        free((*a_node)->data);
    free(*a_node);
    *a_node = NULL;

    return true;
}

void ll_delete_linked_list(linked_list_t *list) {
    ll_node_t *ptr = **list, *tmp = NULL;
    while (ptr!=NULL) {
        tmp = ptr;
        ptr = ptr->next;
        ll_dequeue_node(*list, tmp);
        ll_delete_node(&tmp);
    }
}

size_t ll_length(linked_list_t list) {
    size_t count = 0;
    ll_node_t *ptr = (*list)->next;

    while (ptr!=NULL) {
        ptr = ptr->next;
        count++;
    }
    return count;
}

ll_node_t *ll_find(linked_list_t list, int value) {
    ll_node_t *ptr = *list;
    while (ptr!=NULL) {
        if (ptr->value==value)
            break;
        ptr = ptr->next;
    }
    return ptr;
}

void ll_print(linked_list_t list) {
    printf("%d\t: ", (*list)->value);
    for (ll_node_t *ptr = (*list)->next; ptr != NULL; ptr = ptr->next)
        printf("%d, ", ptr->value);
    printf("\n");
}
