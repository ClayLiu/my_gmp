#include <stdio.h>
#include "my_gmp.h"

#define print_sign(hn) ((((hn)->sign) == negative_sign) && putchar('-'))

void print_huge_number_bin(const huge_number* hn)
{
    char j;
    size_t i;
    unsigned long long number;
    const unsigned long long kernel = (unsigned long long)1 << 31;

    const size_t length = hn->da->length;
    const unsigned long long* array = hn->da->array;

    print_sign(hn);
    
    for(i = 0; i < length; i++)
    {
        printf("," + (!i));
        number = array[length - 1 - i];
        // printf("%llu\n", number);

        for(j = 0; j < 32; j++)
        {
            if(number & kernel)
                putchar('1');
            else
                putchar('0');
            
            number <<= 1;
        }
    }
}


static void format_in_radix_10(dynamic_array* da)
{
    size_t k;
    unsigned long long carry;

    for(k = 0; k < da->length - 1; k++)
    {
        da->array[k + 1] += da->array[k] / 10;
        da->array[k] %= 10;
    }

    carry = da_top(da);
    while(carry > 10)
    {
        da->array[da->length - 1] %= 10;
        da_push(da, carry / 10);
        carry = da_top(da);
    }
}


static char* dec_huge_number_to_str(const dynamic_array* da)
{
    size_t length = da->length;
    size_t t = (length + 7) / 8;
    
    char* buffer = malloc(sizeof(char) * (length + 1));
    char* p = buffer;
    unsigned long long* array = da->array + length - 1;

    switch(length % 8)
    {
        case 0: do {    *p = *array + '0'; p++; array--;
        case 7:         *p = *array + '0'; p++; array--;
        case 6:         *p = *array + '0'; p++; array--;
        case 5:         *p = *array + '0'; p++; array--;
        case 4:         *p = *array + '0'; p++; array--;
        case 3:         *p = *array + '0'; p++; array--;
        case 2:         *p = *array + '0'; p++; array--;
        case 1:         *p = *array + '0'; p++; array--;
        } while(--t);
    }

    *p = 0;
    return buffer;
}


void print_huge_number_dec(const huge_number* hn)
{
    print_sign(hn);

    size_t i, j, k;
    size_t length = hn->da->length;
    size_t min_length;

    char flag;
    unsigned long long curr_element;

    dynamic_array* curr = new_dynamic_array();
    dynamic_array* sum = new_dynamic_array();

    char* output_buffer;

    for(i = 0; i < length; i++)
    {
        curr->length = 0;                               // curr = curr_element
        curr_element = hn->da->array[i];                //
        flag = curr_element == 0;                       //
                                                        //
        while(curr_element)                             //
        {                                               //
            da_push(curr, curr_element % 10);           //
            curr_element /= 10;                         //
        }                                               //
                                                        //
        if(flag)                                        //
            da_push(curr, 0ULL);                        //

        for(j = 0; j < i; j++)                          // calculate curr *= (2 ^ (32 * i))
        {
            for(k = 0; k < curr->length; k++)
                curr->array[k] <<= 32;
            
            format_in_radix_10(curr);
        }

        min_length = __min(curr->length, sum->length);  // sum += curr
        for(j = 0; j < min_length; j++)                 // 
            sum->array[j] += curr->array[j];            //
                                                        //
        for(; j < curr->length; j++)                    //
            da_push(sum, curr->array[j]);               // 

        format_in_radix_10(sum);
    }

    // for(i = 0; i < sum->length; i++)
    //     putchar(sum->array[sum->length - i - 1] + '0');

    output_buffer = dec_huge_number_to_str(sum);    // much faster than the way above.
    puts(output_buffer);
    free(output_buffer);

    destroy_dynamic_array(curr);
    destroy_dynamic_array(sum);
}


void print_huge_number_hex(const huge_number* hn)
{
    char j;
    size_t i;
    unsigned long long number;
    char value, flag = 0;
    const unsigned long long kernel = 0xF0000000ULL;

    const size_t length = hn->da->length;
    const unsigned long long* array = hn->da->array;

    print_sign(hn);
    printf("0x");

    for(i = 0; i < length; i++)
    {
        number = array[length - 1 - i];
        // printf("%llu\n", number);
        
        for(j = 0; j < 8; j++)
        {
            value = (number & kernel) >> 28;
            if(0 <= value && value <= 9)
                (value != 0 || flag) && (putchar(value + '0'), flag = 1);
            else
                putchar(value + 'A' - 10);
            
            number <<= 4;
        }
    }
}
