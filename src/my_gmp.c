#include "my_gmp.h"

const unsigned long long high_half_mask = 0xFFFFFFFF00000000;
const unsigned long long low_half_mask = 0x00000000FFFFFFFF;
const unsigned int element_bit_size = 32;

huge_number* new_huge_number(unsigned long long x, bool sign)
{
    huge_number* hn = malloc(sizeof(huge_number));
    
    hn->da = new_dynamic_array();
    hn->sign = sign;

    da_push(hn->da, x & low_half_mask);
    
    if(x & high_half_mask)
        da_push(hn->da, (x & high_half_mask) >> 32);
    
    return hn;
}


void destroy_huge_number(huge_number* hn)
{
    destroy_dynamic_array(hn->da);
    free(hn);
}


inline huge_number* new_huge_number_one()
{
    return new_huge_number(1, positive_sign);
}


inline huge_number* new_huge_number_zero()
{
    return new_huge_number(0, positive_sign);
}


void format_huge_number(huge_number* hn)
{
    size_t i;
    unsigned long long carry;
    unsigned long long* array = hn->da->array;

    for(i = 0; i < hn->da->length - 1; i++)
    {
        array[i + 1] += (array[i] & high_half_mask) >> element_bit_size;
        array[i] &= low_half_mask;
    }

    carry = da_top(hn->da);
    if(carry & high_half_mask)
    {
        array[i] &= low_half_mask;
        da_push(hn->da, (carry & high_half_mask) >> element_bit_size);
    }
}


void add_inplace(huge_number* hn_l, const huge_number* hn_r);


void sub_inplace(huge_number* hn_l, const huge_number* hn_r);


void mul_inplace(huge_number* hn_l, const huge_number* hn_r);


void div_inplace(huge_number* hn_l, const huge_number* hn_r);


void lshift_inplace(huge_number* hn, unsigned long long n)
{
    size_t i;
    unsigned long long rest_n = n % 32;
    unsigned long long count_to_padding = n / 32;

    for(i = 0; i < hn->da->length; i++)
        hn->da->array[i] <<= rest_n;

    format_huge_number(hn);

    da_padding(hn->da, 0ULL, count_to_padding);
}