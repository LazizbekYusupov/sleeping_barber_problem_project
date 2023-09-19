#pragma once

#define GET_OFFSET_OF(structure_type, field_member)    ((size_t)&(((structure_type*)0)->field_member))

#define GL_THREAD_TO_STRUCT(func_name, structure_type, member) \
    static inline structure_type* func_name(glue_thread_node_t* glue_node) \
    { \
        return (structure_type*)((char*)glue_node - (char*)GET_OFFSET_OF(structure_type, member)); \
    }

#define glthread_node_init(glnode)  \
    glnode->left_node = NULL;            \
    glnode->right_node = NULL;

typedef enum function_report_status_
{
    ALL_GOOD_MAN,
    SOMETHING_IS_NOT_OK
}function_report_status_t;

typedef struct glue_thread_node_
{
    struct glue_thread_node_* left_node;
    struct glue_thread_node_* right_node;
}glue_thread_node_t;

typedef struct glued_thread_
{
    glue_thread_node_t* head;
    size_t thread_offset;
}glued_thread_t;

function_report_status_t init_glued_thread(glued_thread_t* glued_thread, size_t thread_offset);

static function_report_status_t add_near_after(glue_thread_node_t* existing_one, glue_thread_node_t* being_added);
function_report_status_t glued_thread_add_front(glued_thread_t* glued_thread, glue_thread_node_t* node_being_added);
function_report_status_t glued_thread_add_rear(glued_thread_t* glued_thread, glue_thread_node_t* node_being_added);

static function_report_status_t _remove_given_node(glue_thread_node_t* given_node, glue_thread_node_t** returning_variable);
function_report_status_t glued_thread_remove_front(glued_thread_t* glued_thread, glue_thread_node_t** returning_variable);
function_report_status_t glued_thread_remove_rear(glued_thread_t* glued_thread, glue_thread_node_t** returning_variable);

size_t glued_thread_count(glued_thread_t* glued_thread);

