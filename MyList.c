#include "MyList.h"
#include "AllConsts.h"

#define ALL_DUMP

#define ASSERTION(code)                                                  \
    fprintf(stderr, "-----------------!WARNING!----------------\n");     \
    fprintf(stderr, "IN FILE %s\nIN LINE %d\n", __FILE__, __LINE__);     \
    assertion(code);                                                     \


#define VERIFYING(that_list, mode);                                      \
    list_code check = list_verifier(that_list);                          \
    if (check != LIST_OK)                                                \
    {                                                                    \
        ASSERTION(check);                                                \
        list_dump((*that_list), check, mode);                            \
    }                                                                    \
    if (!strcmp(mode, "DESTRUCTION"))                                    \
        list_dump((*that_list), check, mode);

#define QUESTION_VERIFYING(that_list, mode);                             \
    if (!strcmp(mode, "DELETION"))                                       \
    {                                                                    \
        if ((long long int)((*that_list)->length) <= 0)                  \
        {                                                                \
            ASSERTION(LIST_UNDERFLOW);                                   \
            list_dump((*that_list), LIST_UNDERFLOW, mode);               \
            return LIST_UNDERFLOW;                                       \
        }                                                                \
    }                                                                    \
    if (where > (*that_list)->length || where < 0)                       \
    {                                                                    \
        ASSERTION(LIST_OVERFLOW);                                        \
        list_dump((*that_list), LIST_OVERFLOW, mode);                    \
        return LIST_OVERFLOW;                                            \
    }


List* list_new(size_t list_size)
{
    List* new_list = NULL;

    if (list_construct(&new_list, list_size) != LIST_OK)
    {
        list_destruct(&new_list);
        return NULL;
    }

    return new_list;
}

static list_code list_construct(List** that_list, const size_t list_size)
{
    (*that_list) = (List*)calloc(1, sizeof(List));
    if (!(*that_list))
    {
        list_dump(NULL, LIST_NO_CONSTRUCT, LIST_CONSTRUCT);
        return LIST_NO_CONSTRUCT;
    }

    (*that_list)->capacity = list_size;
    (*that_list)->sorted = 1;
    (*that_list)->first_free = 0;
    (*that_list)->head = 0;
    (*that_list)->tail = 0;
    (*that_list)->data = (list_elem*)calloc(list_size + 1, sizeof(list_elem));
    if (!(*that_list)->data)
    {
        ASSERTION(LIST_NO_CONSTRUCT);
        list_dump(NULL, LIST_NO_CONSTRUCT, LIST_CONSTRUCT);
        return LIST_NO_CONSTRUCT;
    }

    (*that_list)->next = (long long*)calloc(list_size + 1, sizeof(long long));
    if (!(*that_list)->next)
    {
        ASSERTION(LIST_NO_CONSTRUCT);
        list_dump(NULL, LIST_NO_CONSTRUCT, LIST_CONSTRUCT);
        return LIST_NO_CONSTRUCT;
    }

    (*that_list)->prev = (long long*)calloc(list_size + 1, sizeof(long long));
    if (!(*that_list)->prev)
    {
        ASSERTION(LIST_NO_CONSTRUCT);
        list_dump(NULL, LIST_NO_CONSTRUCT, LIST_CONSTRUCT);
        return LIST_NO_CONSTRUCT;
    }

    for (long long i = 0; i <= list_size; i++)
    {
        (*that_list)->next[i] = i + 1;
        (*that_list)->prev[i] = i - 1;
        (*that_list)->data[i] = NAN;
    }
    (*that_list)->next[list_size] = -1;
    (*that_list)->prev[0] = list_size;

    (*that_list)->length = 0;

    return LIST_OK;
}

list_code list_destruct(List** that_list)
{
    VERIFYING(that_list, LIST_DESTRUCT);

    if (*that_list)
    {
        if ((*that_list)->data)
            free((*that_list)->data);
        if ((*that_list)->next)
            free((*that_list)->next);

        free(*that_list);
    }

    return LIST_OK;
}

list_code list_resize(List** that_list, const double amount)
{
    VERIFYING(that_list, LIST_RESIZE);

    if ((*that_list)->capacity > ((size_t)-1) / 2)
    {
        ASSERTION(LIST_TOO_BIG);
        list_dump((*that_list), LIST_TOO_BIG, LIST_RESIZE);
    }

    size_t new_capacity = (size_t)((*that_list)->capacity * amount);

    list_elem* new_data = (list_elem*)realloc((*that_list)->data, sizeof(list_elem) * (new_capacity + 1));
    if (new_data) (*that_list)->data = new_data;

    long long* new_next = (long long*)realloc((*that_list)->next, sizeof(long long) * (new_capacity + 1));
    if (new_next) (*that_list)->next = new_next;

    long long* new_prev = (long long*)realloc((*that_list)->prev, sizeof(long long) * (new_capacity + 1));
    if (new_prev) (*that_list)->prev = new_prev;

    if (!new_data || !new_next || !new_prev)
    {
        ASSERTION(LIST_NO_MEMORY);
        list_dump((*that_list), LIST_NO_MEMORY, LIST_RESIZE);
    }

    if (amount > 1)
    {
        for (long long i = (*that_list)->length; i <= new_capacity; i++)
        {
            (*that_list)->next[i] = i + 1;
            (*that_list)->prev[i] = i - 1;
            (*that_list)->data[i] = NAN;
        }
        (*that_list)->next[new_capacity] = -1;
    }

    (*that_list)->capacity = new_capacity;

    #ifdef ALL_DUMP
    list_dump(*that_list, LIST_OK, LIST_RESIZE);
    #endif

    return LIST_OK;
}

list_code list_insert(List** that_list, const long long where, const list_elem value)
{
    VERIFYING(that_list, LIST_INSERT);
    QUESTION_VERIFYING(that_list, LIST_INSERT);

    if ((*that_list)->length + 1 >= (*that_list)->capacity)
    {
        list_resize(that_list, 2);
    }
    if (where != (*that_list)->length)
        (*that_list)->sorted = 0;

    long long tmp_free = (*that_list)->first_free;
    (*that_list)->first_free = (*that_list)->next[(*that_list)->first_free];

    (*that_list)->data[tmp_free] = value;

    (*that_list)->next[(*that_list)->prev[where]] = tmp_free;
    (*that_list)->next[tmp_free] = where;

    (*that_list)->prev[tmp_free] = (*that_list)->prev[where];

    if (tmp_free != where)
        (*that_list)->prev[where] = tmp_free;


    if (!where)
    {
        (*that_list)->head = tmp_free;
        (*that_list)->prev[tmp_free] = (*that_list)->tail;
        (*that_list)->next[(*that_list)->tail] = tmp_free;
    }

    if (where == (*that_list)->length)
    {
        (*that_list)->tail = tmp_free;
        (*that_list)->next[tmp_free] = (*that_list)->head;
        (*that_list)->prev[(*that_list)->head] = (*that_list)->tail;
    }

    (*that_list)->length++;

    #ifdef ALL_DUMP
    list_dump(*that_list, LIST_OK, LIST_INSERT);
    #endif

    return LIST_OK;
}

list_code list_delete(List** that_list, const long long where, list_elem* value)
{
    VERIFYING(that_list, LIST_DELETE);
    QUESTION_VERIFYING(that_list, LIST_DELETE);

    if ((*that_list)->length <= (*that_list)->capacity / 2)
    {
        list_resize(that_list, 0.5);
    }

    if (where != (*that_list)->length)
        (*that_list)->sorted = 0;

    *value = (*that_list)->data[where];
    (*that_list)->data[where] = NAN;

    if (where == (*that_list)->tail)
        (*that_list)->tail = (*that_list)->prev[where];

    if (where == (*that_list)->head)
        (*that_list)->head = (*that_list)->next[where];

    (*that_list)->next[(*that_list)->prev[where]] = (*that_list)->next[where];
    (*that_list)->prev[(*that_list)->next[where]] = (*that_list)->prev[where];

    (*that_list)->next[where] = (*that_list)->first_free;
    (*that_list)->first_free = where;

    (*that_list)->length--;

    #ifdef ALL_DUMP
    list_dump(*that_list, LIST_OK, LIST_DELETE);
    #endif

    return LIST_OK;
}

list_code find_index (List *that_list, const long long where, long long *index)
{
    VERIFYING(&that_list, LIST_FIND);
    QUESTION_VERIFYING(&that_list, LIST_FIND);

    long long tmp_index = 0;
    long long cur_index = that_list->head;

    while (tmp_index < where)
    {
        tmp_index++;
        cur_index = that_list->next[cur_index];
    }

    *index = that_list->prev[cur_index];

    return LIST_OK;
}

list_elem get_element(List* that_list, const long long where)
{
    VERIFYING(&that_list, LIST_GET);
    QUESTION_VERIFYING(&that_list, LIST_GET);

    if (that_list->sorted)
        return that_list->data[where];

    list_elem element = that_list->data[that_list->head];
    long long index = that_list->head;

    for (long long i = 0; i < where; i++)
    {
        index = that_list->next[index];
        element = that_list->data[index];
    }

    #ifdef ALL_DUMP
    list_dump(that_list, LIST_OK, LIST_GET);
    #endif

    return element;
}

long long get_head_index(List *that_list)
{
    return that_list->head;
}

long long get_tail_index(List *that_list)
{
    return that_list->tail;
}

long long get_next_index(List *that_list, long long index)
{
    return that_list->next[index];
}

long long get_prev_index(List *that_list, long long index)
{
    return that_list->prev[index];
}

list_code list_sort(List** that_list)
{
    VERIFYING(that_list, LIST_SORT);

    long long index = (*that_list)->head;
    if (!(*that_list)->sorted)
    {
        list_elem* new_data = (list_elem*)calloc((*that_list)->capacity, sizeof(list_elem));
        if (!new_data)
        {
            ASSERTION(LIST_NO_MEMORY);
            list_dump(*that_list, LIST_NO_MEMORY, LIST_SORT);
            list_destruct(that_list);
        }

        for (long long i = 0; i < (*that_list)->length; i++)
        {
            new_data[i] = (*that_list)->data[index];

            index = (*that_list)->next[index];
        }
        free((*that_list)->data);
        (*that_list)->data = new_data;

        for (long long i = 0; i < (*that_list)->length; i++)
        {
            (*that_list)->next[i] = (i + 1) % (*that_list)->length;
            (*that_list)->prev[i] = (i - 1 + (*that_list)->length) % (*that_list)->length;
        }
        (*that_list)->head = 0;
        (*that_list)->tail = (*that_list)->length - 1;
        (*that_list)->sorted = 1;

        for (long long i = (*that_list)->length; i < (*that_list)->capacity - 1; i++)
        {
            (*that_list)->next[i] = i + 1;
            (*that_list)->prev[i] = i - 1;
        }
        (*that_list)->next[(*that_list)->capacity - 1] = -1;
        (*that_list)->prev[(*that_list)->capacity - 1] = (*that_list)->capacity - 1;
    }

    #ifdef ALL_DUMP
    list_dump(*that_list, LIST_OK, LIST_SORT);
    #endif

    return LIST_OK;
}

void assertion(list_code code)
{
    switch (code)
    {
    case LIST_OK:

        break;

    case LIST_NULL:

        printf("Error: NO POINTER ON STACK FOUND\n\n");
        break;

    case LIST_SEG_FAULT:

        printf("Error: USAGE OF PROHIBITED PART OF MEMORY\n\n");
        break;

    case LIST_DELETED:

        printf("Error: DEALING WITH NON-EXISTENT UNIT OR THE UNIT WAS DAMAGED\n\n");
        break;

    case LIST_NO_CONSTRUCT:

        printf("Error: NO MEMORY FOR CONSTRUCTION\n\n");
        break;

    case LIST_UNDERFLOW:

        printf("Error: GOING OUT OF BONDS OF LIST\n\n");
        break;

    case LIST_OVERFLOW:

        printf("Error: WRONG PLACE OF INSERTION\n\n");
        break;

    case LIST_NO_MEMORY:

        printf("Error: NO FREE MEMORY\n\n");
        break;

    case LIST_TOO_BIG:

        printf("Error: TOO BIG CAPACITY REQUIRED\n\n");
        break;

    case LIST_CONNECT_ERROR:

        printf("Error: PROBLEM WITH ELEMENTS CONNECTION\n\n");
        break;

    default:
        break;
    }
}

void list_call(const char* name_file, const char* new_name)
{
    char temp[512];
    sprintf(temp, "dot %s -T%s -O", name_file, new_name);
    system((char*)temp);
}

void print_sequence(FILE *picture, List *that_list, const char state, const char *color)
{
    long long index = that_list->head;

    fprintf(picture, "  ");
    for (long long i = 0; i < that_list->length; i++)
    {
        fprintf(picture, "nod%lld:<%c%lld>->", index, state, index);

        index = that_list->prev[index];
    }
    fprintf(picture, "nod%lld:<%c%lld>", index, state, index);
    fprintf(picture, "[color=\"%s\", arrowsize = \"1\"]", color);
    fprintf(picture, ";\n");
}

void print_main_sequence(FILE *picture, List *that_list, long long start, long long end, long long index)
{
    for (long long i = start; i < end; i++)
    {
        fprintf(picture, "nod%lld->", index);

        index = that_list->next[index];
    }
}

#define MAKE_BOX                                                                   \
    fprintf(picture, "label = \" <p%lld> %lld |", index, that_list->prev[index]);  \
    fprintf(picture, "{ %lld | ", index);                                          \
    fprintf(picture, "  %lf } |", that_list->data[index]);                         \
    fprintf(picture, "<n%lld> %lld \"];\n", index, that_list->next[index]);

void print_state_list(List* that_list, const char* picture_name)
{
    FILE* picture = fopen(picture_name, "wb");

    fprintf(picture, "digraph list\n{\n  rankdir = LR;\n");

    long long index = that_list->head;

    char next_bad = 0;
    char now_bad  = 0;

    for (long long i = 0; i < that_list->length; i++)
    {
        if (next_bad)
        {
            next_bad = 0;
            now_bad = 1;
        }
        else
            now_bad = 0;

        fprintf(picture, "  nod%ld[shape=\"record\", ", index);
        if (index != that_list->prev[that_list->next[index]] || (that_list->next[index] == that_list->head && i < that_list->length - 1))
        {
            next_bad = 1;
        }

        if (now_bad)
        {
            fprintf(picture, "color = \"%s\", ", COLOR_LINE_BAD);
            fprintf(picture, "fillcolor = \"%s\", ", COLOR_FILL_BAD);
        }
        else
        {
            fprintf(picture, "color = \"%s\", ", COLOR_LINE);
            fprintf(picture, "fillcolor = \"%s\", ", COLOR_FILL);
        }
        fprintf(picture, "style = filled, ");
        MAKE_BOX

        index = that_list->next[index];
    }

    fprintf(picture, "  ");

    print_main_sequence(picture, that_list, 0, that_list->length, index);
    fprintf(picture, "nod%lld", that_list->head);
    fprintf(picture, "[color = \"Black\"];\n");

    print_sequence(picture, that_list, 'n', "Crimson");
    print_sequence(picture, that_list, 'p', "DarkMagenta");

    index = that_list->first_free;
    for (unsigned long long i = that_list->length; i <= that_list->capacity; i++)
    {
        fprintf(picture, "  nod%lld[shape=\"record\", ", index);
        fprintf(picture, "color = \"Black\", fillcolor = \"%s\", style = filled, ", COLOR_FILL_ADD);
        MAKE_BOX

        index = that_list->next[index];
    }
    index = that_list->first_free;
    fprintf(picture, "  nod%lld->", that_list->tail);

    print_main_sequence(picture, that_list, that_list->length, that_list->capacity, index);
    fprintf(picture, "nod%u", that_list->capacity);
    fprintf(picture, "[color = \"Black\"];\n");

    fprintf(picture, "}");
    fclose(picture);

    list_call(picture_name, PICTURE_FORMAT);
}

#define MAKE_PICTURE                                                \
    if (that_list->sorted)                                          \
        fprintf(log, "CURRENT STATE OF SORTING IS TRUE\n");         \
    else                                                            \
        fprintf(log, "CURRENT STATE OF SORTING IS FALSE\n");        \
    fprintf(log, "CURRENT CAPACITY IS %u\n", that_list->capacity);  \
    fprintf(log, "CURRENT SIZE IS     %lld\n", that_list->length);  \
    print_state_list(that_list, picture_name);                      \
    fprintf(log, "<img src=%s>", picture_png_name);


void list_dump(List* that_list, list_code code, const char* who)
{
    static long int doing = 0;

    const char* mode = "";
    if (!doing)
        mode = "wb";
    else
        mode = "ab";
    FILE* log = fopen("list_dump.html", mode);

    fprintf(log, "<pre><tt>\n");

    fprintf(log, "CURRENT STATE OF LIST\n");
    fprintf(log, "THE NEWS FROM %s\n", who);

    char picture_name[MAX_PICTURE_NAME];
    sprintf(picture_name, "%s%d.txt", PICTURE_FILE_CODE, doing);
    char picture_png_name[MAX_PICTURE_NAME];
    sprintf(picture_png_name, "%s%d.txt.%s", PICTURE_FILE_CODE, doing, PICTURE_FORMAT);

     switch (code)
    {
        case LIST_OK:

            fprintf(log, "EVERYTHING IS OKAY\n");
            MAKE_PICTURE
            break;

        case LIST_NULL:

            fprintf(log, "LIST DOES NOT EXIST\n");
            break;

        case LIST_SEG_FAULT:

            fprintf(log, "MEMORY ACCESS DENIED\n");
            break;

        case LIST_DELETED:

            fprintf(log, "DEALING WITH NON-EXISTENT UNIT OR THE UNIT WAS DAMAGED\n");
            break;

        case LIST_NO_CONSTRUCT:

            fprintf(log, "NO MEMORY FOR CONSTRUCTION\n");
            break;

        case LIST_UNDERFLOW:

            fprintf(log, "NOTHING TO DELETE\n");
            MAKE_PICTURE
            break;

        case LIST_OVERFLOW:

            fprintf(log, "WRONG ADDRESS\n");
            MAKE_PICTURE
            break;

        case LIST_NO_MEMORY:

            fprintf(log, "MEMORY ACCESS DENIED\n");
            MAKE_PICTURE
            break;

        case LIST_TOO_BIG:

            fprintf(log, "TOO BIG CAPACITY IS REQUIRED\n");
            MAKE_PICTURE
            break;

        case LIST_CONNECT_ERROR:

            fprintf(log, "ERROR WITH ACCESS TO ELEMENTS\n");
            MAKE_PICTURE
            break;

        default:
            break;
    }

    fprintf(log, "\n");

    fclose(log);

    doing++;
}

list_code is_pointer_valid(List* that_list)
{
    if (!that_list || !that_list->data || !that_list->next || !that_list->prev)
        return LIST_NULL;

    if ((long long)(that_list) <= 4096 || (long long)(that_list->data) <= 4096 || (long long)(that_list->next) <= 4096 || (long long)(that_list->prev) <= 4096)
        return LIST_SEG_FAULT;

    return LIST_OK;
}

list_code list_verifier(List** that_list)
{
    list_code indicator = is_pointer_valid(*that_list);
    if (indicator != LIST_OK)
    {
        return indicator;
    }

    long long index = (*that_list)->head;

    for (long long i = 0; i < (*that_list)->length; i++)
    {
        if (index != (*that_list)->prev[(*that_list)->next[index]])
            return LIST_CONNECT_ERROR;

        if ((*that_list)->next[index] == (*that_list)->head && i < (*that_list)->length - 1)
            return LIST_CONNECT_ERROR;

        index = (*that_list)->next[index];
    }

    return LIST_OK;
}
