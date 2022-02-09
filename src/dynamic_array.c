#include <string.h>
#include "dynamic_array.h"

const size_t initial_size = 16 * elem_size;  // inital 16 element
const size_t size_of_element = elem_size;

dynamic_array* new_dynamic_array()
{
    dynamic_array* array = malloc(sizeof(dynamic_array));
    array->size = initial_size;
    array->array = malloc(initial_size);
    array->length = 0;
    
    return array;
}


void destroy_dynamic_array(dynamic_array* d_array)
{
    free(d_array->array);
    d_array->array = NULL;
    free(d_array);
}


void da_push(dynamic_array* d_array, element_t value)
{
    if(d_array->length * sizeof(element_t) == d_array->size)
    {
        d_array->size <<= 1;

        element_t* old_array = d_array->array;
        element_t* new_array = malloc(d_array->size);

        memcpy
        (
            new_array, 
            old_array, 
            d_array->length * sizeof(element_t)
        );

        free(old_array);
        d_array->array = new_array;
    }
    
    d_array->array[d_array->length] = value;
    d_array->length++;
}


void print_dynamic_array(dynamic_array* d_array)
{
    size_t i;
    for(i = 0; i < d_array->length; i++)
        printf(", %llu" + ((!i) * 2), d_array->array[i]);
    
    putchar('\n');
}


void to_file(dynamic_array* d_array, FILE* fp)
{
    fwrite(&d_array->length, sizeof(size_t), 1, fp);   // 第一个 8 字节保存数组长度
    fwrite(&size_of_element, sizeof(size_t), 1, fp);   // 第一个 8 字节保存元素大小

    fwrite(d_array->array, sizeof(element_t), d_array->length, fp);
}


unsigned long long highbit(unsigned long long x)
{
    unsigned long long prev, curr;
    prev = curr = x;
    while(curr)
    {
        prev = curr;
        curr &= prev - 1;
    }
    return prev;
}


dynamic_array* from_file(FILE* fp)
{
    size_t element_size;
    dynamic_array* array = malloc(sizeof(dynamic_array));

    fread(&array->length, sizeof(size_t), 1, fp);
    fread(&element_size, sizeof(size_t), 1, fp);

    array->size = (highbit(array->length) << 1) * element_size;
    array->array = malloc(array->size);

    fread(array->array, element_size, array->length, fp);
    
    return array;
}

