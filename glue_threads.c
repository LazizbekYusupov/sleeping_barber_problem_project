
#include <stdio.h>
#include <assert.h>
#include "glue_threads.h"

function_report_status_t init_glued_thread(glued_thread_t* glued_thread, size_t thread_offset)
{
    glued_thread->head = NULL;
    glued_thread->thread_offset = thread_offset;
    assert(glued_thread->thread_offset);
    return ALL_GOOD_MAN;
}

static function_report_status_t _add_near_after(glue_thread_node_t* existing_one, glue_thread_node_t* being_added)
{
    if(!existing_one)
    {
        return SOMETHING_IS_NOT_OK;
    }

    if(!existing_one->right_node)
    {
        existing_one->right_node = being_added;

        being_added->left_node = existing_one;
        being_added->right_node = NULL;

        return ALL_GOOD_MAN;
    }

    if(!existing_one->left_node)
    {
        existing_one->left_node = being_added;

        being_added->right_node = existing_one;
        being_added->left_node = NULL;

        return ALL_GOOD_MAN;
    }

    glue_thread_node_t* temp = existing_one->right_node;
    assert(existing_one->right_node);
    assert(temp->left_node);

    existing_one->right_node = being_added;
    temp->left_node = being_added;

    return ALL_GOOD_MAN;
}

function_report_status_t glued_thread_add_front(glued_thread_t* glued_thread, glue_thread_node_t* node_being_added)
{
    function_report_status_t report_code;

    if(!glued_thread->head)
    {
        glued_thread->head = node_being_added;

        return ALL_GOOD_MAN;
    }

    if(glued_thread->head->left_node)
    {
        return SOMETHING_IS_NOT_OK;
    }

    report_code = _add_near_after(glued_thread->head, node_being_added);

    if(report_code)
    {
        return SOMETHING_IS_NOT_OK;
    }

    glued_thread->head = node_being_added;
    glued_thread->head->left_node = NULL;

    return ALL_GOOD_MAN;
}

function_report_status_t glued_thread_add_rear(glued_thread_t* glued_thread, glue_thread_node_t* node_being_added)
{
    function_report_status_t report_code = 0;
    glue_thread_node_t* tail_node = NULL;

    if(!glued_thread->head)
    {
        glued_thread->head = node_being_added;

        return ALL_GOOD_MAN;
    }

    glue_thread_node_t* current_glue_node = NULL;
    glue_thread_node_t* saved_glue_node = NULL;

    for (current_glue_node = glued_thread->head; current_glue_node; current_glue_node = current_glue_node->right_node)
    {
        saved_glue_node = current_glue_node;
    }

    if(saved_glue_node->right_node)
    {
        return SOMETHING_IS_NOT_OK;
    }

    report_code = _add_near_after(saved_glue_node, node_being_added);

    if(report_code)
    {
        return SOMETHING_IS_NOT_OK;
    }

    return ALL_GOOD_MAN;
}

static function_report_status_t _remove_given_node(glue_thread_node_t* given_node, glue_thread_node_t** returning_variable)
{
    if(given_node == NULL)
    {
        return SOMETHING_IS_NOT_OK;
    }

    glue_thread_node_t* left_neighbour = given_node->left_node;
    glue_thread_node_t* right_neighbour = given_node->right_node;

    if(!left_neighbour)
    {
        if(right_neighbour)
        {
            given_node->right_node = NULL;
            right_neighbour->left_node = NULL;

            *returning_variable = given_node;

            return ALL_GOOD_MAN;
        }
        return ALL_GOOD_MAN;
    }

    if(!right_neighbour)
    {
        left_neighbour->right_node = NULL;
        given_node->left_node = NULL;

        *returning_variable = given_node;

        return ALL_GOOD_MAN;
    }

    left_neighbour->right_node = right_neighbour;
    right_neighbour->left_node = left_neighbour;

    given_node->right_node = NULL;
    given_node->left_node = NULL;

    *returning_variable = given_node;

    return ALL_GOOD_MAN;
}

function_report_status_t glued_thread_remove_front(glued_thread_t* glued_thread, glue_thread_node_t** returning_variable)
{
    glue_thread_node_t* temp = NULL;
    glue_thread_node_t* ret_str_ptr = NULL;
    size_t error_code = 0;

    temp = glued_thread->head;

    error_code = _remove_given_node(temp, &ret_str_ptr);
    *returning_variable = ret_str_ptr;

    if(temp == ret_str_ptr)
    {
        glued_thread->head = ret_str_ptr;
    }

    if(temp != ret_str_ptr)
    {
        return SOMETHING_IS_NOT_OK;
    }

    if(error_code)
    {
        return SOMETHING_IS_NOT_OK;
    }

    return ALL_GOOD_MAN;
}

function_report_status_t glued_thread_remove_rear(glued_thread_t* glued_thread, glue_thread_node_t** returning_variable)
{
    glue_thread_node_t* ret_str_ptr = NULL;
    size_t error_code = 0;

    glue_thread_node_t* current_glue_node = NULL;
    glue_thread_node_t* saved_glue_node = NULL;

    for (current_glue_node = glued_thread->head; current_glue_node; current_glue_node = current_glue_node->right_node)
    {
        saved_glue_node = current_glue_node;
    }

    error_code = _remove_given_node(current_glue_node, &ret_str_ptr);
    *returning_variable = ret_str_ptr;

    if(!error_code)
    {
        return SOMETHING_IS_NOT_OK;
    }

    return ALL_GOOD_MAN;
}

































