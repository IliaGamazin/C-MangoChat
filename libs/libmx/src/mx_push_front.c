#include "../inc/libmx.h"

void mx_push_front(t_list **list, void *data) {
    if (!list) {
        return;
    }
    t_list *temp = *list;
    *list = mx_create_node(data);
    if (*list){
        (*list)->next = temp;
    }
}


