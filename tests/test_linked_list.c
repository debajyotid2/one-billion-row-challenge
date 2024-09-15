#include <stddef.h>
#include <criterion/criterion.h>
#include "../src/linked_list.h"

linked_list_t list = NULL;

void test_setup(void) {
    ll_init(&list, 0, NULL);

    ll_node_t *a = ll_create_node(5, NULL);
    ll_node_t *b = ll_create_node(4, NULL);
    ll_node_t *c = ll_create_node(63, NULL);
    ll_node_t *d = ll_create_node(-1, NULL);
    ll_add_at_tail(list, a);
    ll_add_at_tail(list, b);
    ll_add_at_tail(list, c);
    ll_add_at_tail(list, d);
}

void test_teardown(void) {
    ll_delete_linked_list(&list);
}

TestSuite(linked_list_tests, .init=test_setup, .fini=test_teardown);

Test(linked_list_tests, ll_create_node) {
    cr_expect((*list)!=NULL, "ll_create_node should return not NULL.");
}

Test(linked_list_tests, ll_add_at_head) {
    size_t original_length = ll_length(list);
    ll_node_t *new_node_2 = ll_create_node(-102, NULL);

    ll_add_at_head(list, new_node_2);
    cr_expect(ll_length(list)==(original_length+1),
            "ll_add_at_head should increase the length by 1.");
}

Test(linked_list_tests, ll_length) {
    cr_expect(ll_length(list)==4,
            "Length of the linked list should be 4.");
}

Test(linked_list_tests, ll_add_at_tail) {
    size_t original_length = ll_length(list);
    ll_node_t *new_node = ll_create_node(42, NULL);
    
    ll_add_at_tail(list, new_node);
    cr_expect(ll_length(list)==(original_length+1), 
            "ll_add_at_tail should increase the length by 1.");
}

Test(linked_list_tests, ll_add_after_node) {
    size_t original_length = ll_length(list);
    ll_node_t *new_node = ll_create_node(-40, NULL);
    ll_node_t *new_node_2 = ll_create_node(-487, NULL);

    ll_add_at_tail(list, new_node);
    ll_add_after_node(new_node, new_node_2);

    cr_expect(ll_length(list)==(original_length+2),
            "ll_add_after_node should increase the length by 1.");
}

Test(linked_list_tests, ll_dequeue_node_from_tail) {
    size_t original_length = ll_length(list);
    ll_node_t *new_node = ll_create_node(100, NULL);
    
    ll_add_at_tail(list, new_node);
    new_node = ll_dequeue_node(list, new_node);

    cr_expect(ll_length(list)==original_length,
            "ll_dequeue_node should reduce the length by 1.");
    ll_delete_node(&new_node);
}

Test(linked_list_tests, ll_dequeue_node_from_head) {
    size_t original_length = ll_length(list);
    ll_node_t *new_node = ll_create_node(100, NULL);
 
    ll_add_at_head(list, new_node);
    new_node = ll_dequeue_node(list, new_node);

    cr_expect(ll_length(list)==original_length,
            "ll_dequeue_node should reduce the length by 1.");

    ll_delete_node(&new_node);
}

Test(linked_list_tests, ll_dequeue_node_from_middle) {
    size_t original_length = ll_length(list);
    ll_node_t *new_node = ll_create_node(100, NULL);
    ll_node_t *new_node_2 = ll_create_node(242, NULL);
    
    ll_add_at_head(list, new_node);
    ll_add_after_node(new_node, new_node_2);
    new_node_2 = ll_dequeue_node(list, new_node_2);

    cr_expect(ll_length(list)==original_length+1,
            "ll_dequeue_node should reduce the length by 1.");
    ll_delete_node(&new_node_2);
}

Test(linked_list_tests, ll_find) {
    ll_node_t *temp_node = ll_find(list, 4);
    cr_expect(ll_get_node_value(temp_node)==4,
            "ll_find should return the node with the correct value.");
}
