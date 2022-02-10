#include "my_gmp.h"

const unsigned long long high_half_mask = 0xFFFFFFFF00000000;
const unsigned long long low_half_mask = 0x00000000FFFFFFFF;


huge_number* new_huge_number(unsigned long long x, bool sign)
{
    huge_number* hn = malloc(sizeof(huge_number));
    
    hn->da = new_dynamic_array();
    hn->sign = sign;

    da_push(hn->da, x & low_half_mask);
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




/* l += r */
void add_inplace(huge_number* hn_l, const huge_number* hn_r);

/* l -= r */
void sub_inplace(huge_number* hn_l, const huge_number* hn_r);

/* l *= r */
void mul_inplace(huge_number* hn_l, const huge_number* hn_r);

/* l /= r */
void div_inplace(huge_number* hn_l, const huge_number* hn_r);