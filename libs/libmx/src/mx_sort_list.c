#include "../inc/libmx.h"

t_list *mx_sort_list(t_list *lst, bool (*cmp)(void *, void *)) {
    if (!lst || !cmp) {
        return NULL;
    }
    t_list *res = NULL;
    t_list *next = NULL;
    t_list *current = lst;
    t_list *current_sorted;
    while (current) {
        next = current->next;
        if (!res || !cmp(current->data, res->data)) {
            current->next = res;
            res = current;
        }
        else {
            current_sorted = res;
            while (current_sorted->next && cmp(current->data, current_sorted->next->data)) {
                current_sorted = current_sorted->next;
            }
            current->next = current_sorted->next;
            current_sorted->next = current;
        }
        current = next;
    }
    return res;
}



