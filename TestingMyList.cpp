#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MyList.c"

//#define FIRST_TEST
//#define SECOND_TEST
//#define THIRD_TEST
//#define FORTH_TEST
//#define FIFTH_TEST
#define SIXTH_TEST

void unit_tests();

int main()
{
    unit_tests();

    return 0;
}

void unit_tests()
{
    #ifdef FIRST_TEST
    {
        printf("The first unit-test\n");
        List* pain = list_new(10);
        printf("cap %lld\n", pain->capacity);

        list_insert(&pain, 0, 10);
        list_insert(&pain, 1, 20);
        list_insert(&pain, 2, 30);
        list_insert(&pain, 1, 40);

        double a = 0;

        a = get_element(pain, 1);
        printf("%lg\n", a);

        list_destruct(&pain);
    }
    #endif

    #ifdef SECOND_TEST
    {
        printf("The second unit-test\n");
        List* pain = list_new(2);

        double a = 0;

        list_insert(&pain, 0, 10);
        a = get_element(pain, 0);
        printf("%lg\n", a);

        list_insert(&pain, 1, 11);
        a = get_element(pain, 1);
        printf("%lg\n", a);

        list_insert(&pain, 2, 12);
        a = get_element(pain, 2);
        printf("%lg\n", a);

        list_insert(&pain, 3, 13);

        list_destruct(&pain);
    }
    #endif

    #ifdef THIRD_TEST
    {
        printf("The third unit-test\n");
        List* pain = list_new(2);

        double a = 0;

        list_insert(&pain, 0, 10);
        list_insert(&pain, 1, 11);
        list_insert(&pain, 2, 12);
        list_insert(&pain, 3, 13);

        list_dump(pain, LIST_OK, LIST_INSERT);

        list_insert(&pain, 1, 14);
        list_insert(&pain, 1, 15);
        list_insert(&pain, 0, 16);

        pain->prev[2] = 0;

        list_dump(pain, LIST_OK, LIST_INSERT);

        list_sort(&pain);

        list_destruct(&pain);
    }
    #endif

    #ifdef FORTH_TEST
    {
        printf("The forth unit-test\n");
        List* pain = list_new(2);

        double a = 0;

        list_insert(&pain, 0, 1.0);
        list_insert(&pain, 1, 1.1);
        list_insert(&pain, 2, 1.2);
        list_insert(&pain, 3, 1.3);

        list_insert(&pain, 1, 1.4);
        list_insert(&pain, 1, 1.5);
        list_insert(&pain, 0, 1.6);

        list_delete(&pain, 0, &a);

        printf("my value %lg\n", a);

        list_destruct(&pain);
    }
    #endif

    #ifdef FIFTH_TEST
    {
        printf("The fifth unit-test\n");
        List *pain  = list_new(2);
        List *konan = list_new(10);

        list_insert(&pain, 0, 1.0);
        list_insert(&pain, 1, 1.1);
        list_insert(&pain, 2, 1.2);
        list_insert(&pain, 3, 1.3);

        list_insert(&konan, 0, 10.4);
        list_insert(&konan, 0, 10.5);
        list_insert(&konan, 5, 10.6);

        list_destruct(&pain);
        list_destruct(&konan);
    }
    #endif

    #ifdef SIXTH_TEST
    {
        printf("The sixth unit-test\n");
        List *pain  = list_new(2);

        list_insert(&pain, 0, 100);
        list_insert(&pain, 1, 101);
        list_insert(&pain, 2, 102);
        list_insert(&pain, 3, 103);

        list_insert(&pain, 4, 104);
        list_insert(&pain, 1, 105);
        list_insert(&pain, 5, 106);

        long long mid_index = 0;
        find_index(pain, 2, &mid_index);

        printf("index %lld, this element %lg\n", mid_index, get_element(pain, mid_index));

        double value = 0;

        list_delete(&pain, mid_index, &value);

        list_destruct(&pain);
    }
    #endif
}

