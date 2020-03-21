#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (q != NULL) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}
/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (q != NULL) {
        if (q->head != NULL) {
            list_ele_t *tmp;
            while (q->head != NULL) {
                tmp = q->head;
                free(tmp->value);  // free the string
                q->head = tmp->next;
                free(tmp);
            }
        }
        free(q);
    }
}
/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    /* TODO: What should you do if the q is NULL? */
    if (q == NULL)
        return false;
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;
    newh->value = malloc(sizeof(char) * strlen(s) + 1);
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if (q->head == NULL)
        q->tail = newh;
    strncpy(newh->value, s, strlen(s) + 1);  // copy the string to the node
    newh->next = q->head;
    q->head = newh;
    (q->size)++;
    return true;
}
/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (q == NULL)
        return false;
    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (newt == NULL)  // fail to allocate
        return false;
    newt->value = malloc(sizeof(char) * strlen(s) + 1);
    if (newt->value == NULL) {
        free(newt);
        return false;
    }
    strncpy(newt->value, s, strlen(s) + 1);
    if (q->tail == NULL) {
        newt->next = q->head;
        q->head = newt;
    } else {
        newt->next = q->tail->next;
        q->tail->next = newt;
        q->tail = newt;
    }
    (q->size)++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (q == NULL)  // queue is null or empty
        return false;
    if (q->head == NULL)
        return false;
    list_ele_t *ptr = q->head;
    snprintf(sp, bufsize, "%s", q->head->value);
    free(q->head->value);
    q->head = q->head->next;
    free(ptr);
    if (q->head == NULL)
        q->tail = NULL;
    (q->size)--;
    return true;
}
/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q == NULL)
        return 0;
    return q->size;
}
/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q != NULL) {
        if (q->head != NULL) {
            list_ele_t *prev = NULL, *cur = q->head, *next = NULL;
            q->tail = q->head;
            while (cur != NULL) {
                next = cur->next;
                cur->next = prev;
                prev = cur;
                cur = next;
            }
            q->head = prev;
        }
    } else
        return;
}
/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty.In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (q != NULL) {
        if (q->head != NULL && q->size > 1) {
            list_ele_t *start = q->head;
            q->head = sort(start);
            while (q->tail->next != NULL) {
                q->tail = q->tail->next;
            }
        }
    } else
        return;
}
list_ele_t *sort(list_ele_t *start)
{
    if ((start == NULL) || (start->next == NULL))
        return start;
    list_ele_t *head = start;
    list_ele_t *fast;
    list_ele_t *slow;
    list_ele_t *front;
    list_ele_t *back;
    slow = head;
    fast = head->next;
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    front = head;  // partition the linked list into two sublists
    back = slow->next;
    slow->next = NULL;

    front = sort(front);
    back = sort(back);

    head = SortedMerge(front, back);
    return head;
}
list_ele_t *SortedMerge(list_ele_t *a, list_ele_t *b)
{
    list_ele_t *result = NULL;

    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;
    if (strcmp(a->value, b->value) <= 0) {
        result = a;
        result->next = SortedMerge(a->next, b);
    } else {
        result = b;
        result->next = SortedMerge(a, b->next);
    }
    return result;
}
