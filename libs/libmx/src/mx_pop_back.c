#include "../inc/libmx.h"

void mx_pop_back(t_list **head) {
    if (!head || !(*head)) {
        return;
    }
    t_list *temp = *head;
    t_list *prev = NULL;
    while (temp->next) {
        prev = temp;
        temp = temp->next;
    }
    if (!prev) {
        free(*head);
        *head = NULL;
    }
    else {
        free(temp);
        prev->next = NULL;
    }
}


