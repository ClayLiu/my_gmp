#include <assert.h>
#include <string.h>
#include "dynamic_array.h"

// #define debug_mode
#include "debug.h"

const size_t initial_size = 16 * elem_size;  // inital 16 element
const size_t size_of_element = elem_size;
const size_t max_array_size = 1ULL << 63;

static unsigned long long highbit(unsigned long long x)
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


static void memset_ex(element_t* dst, element_t value, size_t count)
{
    // memset in string.h only can set int value
    // so dump. Since that, this function is written.

    size_t t = (count + 7) / 8;
    switch(count % 8)
    {
        case 0: do {    *dst = value; dst++;
        case 7:         *dst = value; dst++;
        case 6:         *dst = value; dst++;
        case 5:         *dst = value; dst++;
        case 4:         *dst = value; dst++;
        case 3:         *dst = value; dst++;
        case 2:         *dst = value; dst++;
        case 1:         *dst = value; dst++;
        } while(--t);
    }
}


static void move_to_high(element_t* dst, element_t* src, size_t count)
{
    // This function perticularly for
    // moving elements to higher address.
    // Hope it can be faster than memmove in string.h

    dst += count - 1;
    src += count - 1;
    size_t t = (count + 7) / 8;
    switch(count % 8)
    {
        case 0: do {    *dst = *src; dst--; src--;
        case 7:         *dst = *src; dst--; src--;
        case 6:         *dst = *src; dst--; src--;
        case 5:         *dst = *src; dst--; src--;
        case 4:         *dst = *src; dst--; src--;
        case 3:         *dst = *src; dst--; src--;
        case 2:         *dst = *src; dst--; src--;
        case 1:         *dst = *src; dst--; src--;
        } while(--t);
    }
}


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
        // assert(d_array->size < max_array_size);  // if size equals to 2 ^ 63
                                                    // that means have to allocate 2 ^ (33) GB
                                                    // memory, that's crazy.
        d_array->size <<= 1;
        
        element_t* old_array = d_array->array;
        element_t* new_array = malloc(d_array->size);
        assert(new_array != NULL);

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


inline element_t da_top(dynamic_array* d_array)
{
    assert(d_array->length > 0ULL);
    return d_array->array[d_array->length - 1];
}


void da_padding(dynamic_array* d_array, element_t value, size_t count)
{
    if(count == 0)
        return;

    size_t new_size;
    size_t new_length = d_array->length + count;
    size_t new_real_size = new_length * sizeof(element_t);
    size_t new_real_size_highbit = highbit(new_real_size);

    new_size = new_real_size_highbit << (new_real_size > new_real_size_highbit);

    llu_debug(count);
    llu_debug(d_array->length);
    llu_debug(new_length);
    llu_debug(new_real_size);
    llu_debug(new_real_size_highbit);
    llu_debug(new_size);
    llu_debug(d_array->size);

    if(new_size > d_array->size)
    {
        element_t* old_array = d_array->array;
        element_t* new_array = malloc(new_size);        
        assert(new_array != NULL);

        memcpy
        (
            new_array + count, 
            old_array, 
            d_array->length * sizeof(element_t)
        );

        d_array->size = new_size;
        d_array->array = new_array;
        free(old_array);
    }
    else
    {
        // memmove
        // (
        //     d_array->array + count,
        //     d_array->array, 
        //     d_array->length * sizeof(element_t)
        // );

        move_to_high
        (
            d_array->array + count,
            d_array->array,
            d_array->length
        );
    }
    
    memset_ex(d_array->array, value, count);
    d_array->length = new_length;
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

