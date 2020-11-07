/**
    \file
    File with functions for using the list
    It has construction, destruction, insertion, deletion, resizing, looking for head and tail, next and previous elements.\n
    It can also sort elements int he order and get the index of needed element.\n
    Thank you for using this program!
    \warning The sortion takes the time of O(n), get_element - O(n). Please, try to put elements at the beginning and at the end of the list.\n
    \authors Anna Savchuk
    \note    To change the type, change the typedef about list_elem in MyList.c\n
             If you want functions not to print state of the list at the end, comment the define about ALL_DUMP in AllConsts.h\n
             At the end the last information about the list will be added to log_file\n
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "AllConsts.h"

typedef double list_elem;

typedef struct The_list
{
    list_elem  *data;
    long long  *next;
    long long  *prev;

    size_t      length;
    size_t      capacity;

    long long   head;
    long long   tail;
    long long   first_free;

    char        sorted;
} List;

typedef enum list_code_errors
{
    LIST_OK,
    LIST_NULL,
    LIST_SEG_FAULT,
    LIST_DELETED,
    LIST_NO_CONSTRUCT,
    LIST_UNDERFLOW,
    LIST_OVERFLOW,
    LIST_NO_MEMORY,
    LIST_TOO_BIG,
    LIST_CONNECT_ERROR
} list_code;

const char LIST_CONSTRUCT[]     = "CONSTRUCTION";
const char LIST_DESTRUCT[]      = "DESTRUCTION";
const char LIST_INSERT[]        = "INSERTION";
const char LIST_RESIZE[]        = "RESIZING";
const char LIST_DELETE[]        = "DELETION";
const char LIST_FIND[]          = "FINDING";
const char LIST_SORT[]          = "SORTING";
const char LIST_GET[]           = "GETTING";

/*!
Creates a new list and deletes it if the construction failed

@param[in]       list_size                 The size of the list

@param[out]      new_list                  The pointer on the list
*/
List            *list_new                  (size_t list_size);
/*!
Constructs a new list

@param[in]       that_list                 The construction of the list
@param[in]       list_size                 The size of the list

Returns          LIST_OK                   If everything is ok
                 LIST_NO_CONSTRUCT         If it was impossible to create the list
*/
static list_code list_construct            (List **that_list, const size_t list_size);
/*!
Destructs the list

@param[in]       that_list                 The construction of the list

Returns          LIST_OK                   If everything is ok\n
                 LIST_NULL                 If there were not pointers on units of list\n
                 LIST_SEG_FAULT            If memory access denied\n
                 LIST_DELETED              If some of the units were deleted\n
                 LIST_CONNECT_ERROR        If the connection between next array and previous array was broken\n
*/
list_code        list_destruct             (List **that_list);

/*!
Changes the capacity of the list

@param[in]       that_list                 The construction of the list
@param[in]       amount                    2, if you want to make list twice bigger, 0.5, if you if you want to make list twice smaller

Returns          LIST_OK                   If everything is ok\n
                 LIST_NULL                 If there were not pointers on units of list\n
                 LIST_SEG_FAULT            If memory access denied\n
                 LIST_DELETED              If some of the units were deleted\n
                 LIST_CONNECT_ERROR        If the connection between next array and previous array was broken\n
                 LIST_NO_MEMORY            If there is no memory to resize the list\n
                 LIST_TOO_BIG              If the new capacity is bigger than the maximum size of the type\n
*/
list_code        list_resize               (List **that_list, const double amount);
/*!
Inserts the value into the list

@param[in]       that_list                 The construction of the list
@param[in]       where                     The index to put value there
@param[in]       value                     The value to put into the list

Returns          LIST_OK                   If everything is ok\n
                 LIST_NULL                 If there were not pointers on units of list\n
                 LIST_SEG_FAULT            If memory access denied\n
                 LIST_DELETED              If some of the units were deleted\n
                 LIST_OVERFLOW             If the index does not exist or can not be added\n
*/
list_code        list_insert               (List **that_list, const long long where, const list_elem value);
/*!
Deletes the value of the list

@param[in]       that_list                 The construction of the list
@param[in]       where                     The index to delete value off
@param[in]       value                     The value deleting

Returns          LIST_OK                   If everything is ok\n
                 LIST_NULL                 If there were not pointers on units of list\n
                 LIST_SEG_FAULT            If memory access denied\n
                 LIST_DELETED              If some of the units were deleted\n
                 LIST_UNDERFLOW            If the list is empty\n
                 LIST_OVERFLOW             If the index does not exist or can not be added\n
*/
list_code        list_delete               (List **that_list, const long long where, list_elem *value);
/*!
Finds the index that matces to the sought one

@param[in]       that_list                 The construction of the list
@param[in]       where                     The sought index
@param[in]       index                     The index that matches the sought one

Returns          LIST_OK                   If everything is ok\n
                 LIST_NULL                 If there were not pointers on units of list\n
                 LIST_SEG_FAULT            If memory access denied\n
                 LIST_DELETED              If some of the units were deleted\n
                 LIST_OVERFLOW             If the index does not exist or can not be added\n
*/
list_code        find_index                (List *that_list, const long long where, long long *index);
/*!
Finds the element that is located at the given index

@param[in]       that_list                 The construction of the list
@param[in]       where                     The index element is located at

Returns          LIST_OK                   If everything is ok\n
                 LIST_NULL                 If there were not pointers on units of list\n
                 LIST_SEG_FAULT            If memory access denied\n
                 LIST_DELETED              If some of the units were deleted\n
                 LIST_OVERFLOW             If the index does not exist or can not be added\n
*/
list_elem        get_element               (List *that_list, const long long where);

/*!
Finds the index of the first element of the list

@param[in]       that_list                 The construction of the list

@param[out]                                The index of the first element of the list
*/
long long        get_head_index            (List *that_list);
/*!
Finds the index of the last element of the list

@param[in]       that_list                 The construction of the list

@param[out]                                The index of the last element of the list
*/
long long        get_tail_index            (List *that_list);
/*!
Finds the index of the element of the list after the given one

@param[in]       that_list                 The construction of the list
@param[in]       index                     The index the element is sought for after

@param[out]                                the index of the element of the list after the given one
*/
long long        get_next_index            (List *that_list, long long index);
/*!
Finds the index of the element of the list before the given one

@param[in]       that_list                 The construction of the list
@param[in]       index                     The index the element is sought for before

@param[out]                                the index of the element of the list before the given one
*/
long long        get_prev_index            (List *that_list, long long index);

/*!
Sorts the elements of the list at the order of usual array

@param[in]       that_list                 The construction of the list

Returns          LIST_OK                   If everything is ok\n
                 LIST_NULL                 If there were not pointers on units of list\n
                 LIST_SEG_FAULT            If memory access denied\n
                 LIST_DELETED              If some of the units were deleted\n
                 LIST_NO_MEMORY            If there is no memory to sort the list\n

\warning         Takes the time of O(n). Try to avoid using this function
*/
list_code        list_sort                 (List **that_list);

/*!
Prints the errors in the console

@param[in]       code                      The identifier of the error
*/
void             assertion                 (list_code code);

/*!
Calls the dot-program to make the picture by graphviz
@param[in]       name_file                 The name of file to make a picture from
@param[in]       new_name                  The format of the picture
*/
void             list_call                 (const char *name_file, const char *new_name);
/*!
Outputs the information about the adding nodes of the list
@param[in]       picture                   The file to print code for graphviz into
@param[in]       that_list                 The structure of the list
@param[in]       state                     The indicator if the next-array is printed or the previous one
@param[in]       color                     The color of edges to connect the nodes
*/
void             print_sequence            (FILE *picture, List *that_list, const char state, const char *color);
/*!
Outputs the information about the main nodes of the list
@param[in]       picture                   The file to print code for graphviz into
@param[in]       that_list                 The structure of the list
@param[in]       start                     The counter of the array to start with
@param[in]       end                       The length of the array
@param[in]       index                     The index of the first element
*/
void             print_main_sequence       (FILE *picture, List *that_list, long long start, long long end, long long index);
/*!
Outputs the information about the current state of the list into the file
@param[in]       that_list                 The structure of the list
@param[in]       picture_name              The name of the file to print the code for graphviz
*/
void             print_state_list          (List* that_list, const char* picture_name);
/*!
Outputs the information about the current state of the list into "log_file.html"

@param[in]       that_list                 The structure of the list
@param[in]       code                      The code of the mistake
@param[in]       who                       The code of the function requested for dump
*/
void             list_dump                 (List *that_list, list_code code, const char *who);

/*!
Checks if all pointers are valid
@param[in]       that_list                 The structure of the list

Returns          LIST_NULL                 If the pointer does not exists\n
                 LIST_SEG_FAULT            If the pointer points in prohibited for using block of memory\n
                 LIST_OK                   If the pointer is valid\n
*/
list_code        is_pointer_valid          (List *that_list);
/*!
Checks all of the states of the list

@param[in]       that_list                 The structure of the list

Returns          LIST_OK                   If everything is ok\n
                 LIST_NULL                 If there were not pointers on units of list\n
                 LIST_SEG_FAULT            If memory access denied\n
                 LIST_DELETED              If some of the units were deleted\n
                 LIST_CONNECT_ERROR        If the connection between next array and previous array was broken\n
*/
list_code        list_verifier             (List **that_list);
