#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "glued_thread_queue.h"

void
init_glthread(glthread_t* glthread, size_t offset){

    glthread->head = NULL;
    glthread->offset = offset;
    pthread_mutex_init(&glthread->glthread_mutex, NULL);
    pthread_cond_init(&glthread->glthread_cv, NULL);
}

/*Private function to add a new_node right after curr_node*/
static void
_glthread_add_next(glthread_node_t *curr_node,
                   glthread_node_t *new_node){

    if(!curr_node->right){
        curr_node->right = new_node;
        new_node->left = curr_node;
        return;
    }

    glthread_node_t *temp = curr_node->right;
    curr_node->right = new_node;
    new_node->left = curr_node;
    new_node->right = temp;
    temp->left = new_node;
}

/*Fn to insert a new GLnode into a glthread at the first position
 * in a glthread i.e. new node becomes a head
 * */
void
glthread_add (glthread_t *lst, glthread_node_t *glnode){

    glnode->left = NULL;
    glnode->right = NULL;

    if(!lst->head){
        lst->head = glnode;
        return;
    }
    glthread_node_t *head = lst->head;
    _glthread_add_next(glnode, head);
    lst->head = glnode;
}

void
glthread_add_last(glthread_t* glthread_list, glthread_node_t* new_glthread){

    glthread_node_t* tail_node = NULL;
    glthread_node_t* next_node = NULL;

    if(!glthread_list->head){
        glthread_list->head = new_glthread;
        return;
    }

    for (next_node = glthread_list->head; next_node; next_node = next_node->right)
    {
        tail_node = next_node;
    }

    _glthread_add_next(tail_node, new_glthread);
}

static void
_remove_glthread(glthread_node_t *glnode){

    if(!glnode->left){
        if(glnode->right){
            glnode->right->left = NULL;
            glnode->right = 0;
            return;
        }
        return;
    }
    if(!glnode->right){
        glnode->left->right = NULL;
        glnode->left = NULL;
        return;
    }

    glnode->left->right = glnode->right;
    glnode->right->left = glnode->left;
    glnode->left = 0;
    glnode->right = 0;
}

/*API to remove a glnode from glthread*/
glthread_node_t*
glthread_remove(glthread_t *lst, glthread_node_t *glnode){

    glthread_node_t *head = lst->head;
    /*If the node being removed is the head node itself, then update the 
     * head*/
    if(head == glnode){
        lst->head = head->right;
    }
    _remove_glthread(glnode);

    return glnode;
}

glthread_node_t*
glthread_remove_front(glthread_t *lst){

    if(!lst)
    {
        return NULL;
    }

    if(!lst->head)
    {
        return NULL;
    }

    glthread_node_t *temp = lst->head;

    lst->head = temp->right;

    _remove_glthread(temp);

    return temp;
}

size_t
glthread_count(glthread_t* glue_thread_list)
{
    glthread_node_t* next_node = NULL;
    size_t list_count = 0;

    if(!glue_thread_list->head){
        return 0;
    }

    for (next_node = glue_thread_list->head; next_node; next_node = next_node->right)
    {
        list_count++;
    }

    return list_count;
}




















