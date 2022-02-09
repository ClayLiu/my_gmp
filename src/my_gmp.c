#include "my_gmp.h"
#include "string_tool.h"

huge_number* new_huge_number(unsigned long long x, bool sign)
{
    huge_number* hn = malloc(sizeof(huge_number));
    
    hn->da = new_dynamic_array();
    hn->sign = sign;

    da_push(hn->da, x);
    return hn;
}


void destroy_huge_number(huge_number* hn)
{
    destroy_dynamic_array(hn->da);
    free(hn);
}


huge_number* new_huge_number_one()
{
    return new_huge_number(1, positive_sign);
}


huge_number* new_huge_number_zero()
{
    return new_huge_number(0, positive_sign);
}


void print_huge_number_bin(const huge_number* hn)
{
    char j;
    size_t i;
    unsigned long long number;
    const unsigned long long kernel = (unsigned long long)1 << 31;

    const size_t length = hn->da->length;
    const unsigned long long* array = hn->da->array;

    if(hn->sign == negative_sign)
        putchar('-');
    
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

static char* mul_2_raise_32(char* x, size_t length, size_t* new_length)
{

}

/* return x * (2 ^ (32 * power)) in reversed str */
static char* muled_ull_to_str(unsigned long long x, unsigned long long power)
{
    size_t i;
    size_t length;
    char* str = malloc(sizeof(char) * 16);
    ull_to_str(x, str, &length);
    
    while(power--)
        str = mul_2_raise_32(str, length, &length);
    
    for(i = 0; i < length; i++)
        str[i] += '0';
    
    str[i] = 0;

    return str;
}

void print_huge_number_dec(const huge_number* hn)
{
    size_t i;
    size_t length = hn->da->length;
    char** numbers_dec = malloc(sizeof(char*) * length);
    
    for(i = 0; i < length; i++)
        numbers_dec[i] = muled_ull_to_str(hn->da->array[i], i);

    for(i = 0; i < length; i++)
        free(numbers_dec[i]);

    free(numbers_dec);

}


/* l += r */
void add_inplace(huge_number* hn_l, const huge_number* hn_r);

/* l -= r */
void sub_inplace(huge_number* hn_l, const huge_number* hn_r);

/* l *= r */
void mul_inplace(huge_number* hn_l, const huge_number* hn_r);

/* l /= r */
void div_inplace(huge_number* hn_l, const huge_number* hn_r);