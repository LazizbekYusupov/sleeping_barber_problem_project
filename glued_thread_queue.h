#pragma once

typedef struct glthread_node_ {

    struct glthread_node_ *left;
    struct glthread_node_ *right;
} glthread_node_t;

typedef struct glthread_ {
    glthread_node_t *head;
    unsigned int offset;
    pthread_mutex_t glthread_mutex;
    pthread_cond_t glthread_cv;
} glthread_t;

void
init_glthread(glthread_t *glthread, size_t offset);

void
glthread_add (glthread_t *lst, glthread_node_t *glnode);

void
glthread_add_last(glthread_t* glthread_list, glthread_node_t* new_glthread);

glthread_node_t*
glthread_remove(glthread_t *lst, glthread_node_t *glnode);

glthread_node_t*
glthread_remove_front(glthread_t *lst);

size_t
glthread_count(glthread_t* glue_thread_list);

#define glthread_node_init(glnode)  \
    glnode->left = NULL;            \
    glnode->right = NULL;

/*Iterative macro to Iterate Over GLTHhreads*/
#define ITERATE_GL_THREADS_BEGIN(lstptr, struct_type, ptr)      \
{                                                               \
    glthread_node_t *_glnode = NULL, *_next = NULL;             \
    for(_glnode = lstptr->head; _glnode; _glnode = _next){      \
        _next = _glnode->right;                                 \
        ptr = (struct_type *)((char *)_glnode - lstptr->offset);

#define ITERATE_GL_THREADS_ENDS }}