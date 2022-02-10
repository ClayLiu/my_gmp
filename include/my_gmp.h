#ifndef MY_GMP_H_HDIFHD
#define MY_GMP_H_HDIFHD

#include <stdlib.h>
#include "dynamic_array.h"

extern const unsigned long long high_half_mask;
extern const unsigned long long low_half_mask;
extern const unsigned int element_bit_size;

typedef char bool;

enum sign_value {positive_sign = 0, negative_sign = 1};

typedef struct hn
{
    dynamic_array* da;
    bool sign;  // negative -> 1; positive -> 0;
}huge_number;

/* 生成一个超大数 */
huge_number* new_huge_number(unsigned long long x, bool sign);

/* 删除超大数 */
void destroy_huge_number(huge_number* hn);

/* 超大数 1 */
huge_number* new_huge_number_one();

/* 超大数 0 */
huge_number* new_huge_number_zero();

void format_huge_number(huge_number* hn);

/* 输出 */

    /* 以二进制打印超大数 */
    void print_huge_number_bin(const huge_number* hn);

    /* 以十进制打印超大数 */
    void print_huge_number_dec(const huge_number* hn);

    /* 以十六进制打印超大数 */
    void print_huge_number_hex(const huge_number* hn);

/* 原地运算 */

    /* l += r */
    void add_inplace(huge_number* hn_l, const huge_number* hn_r);

    /* l -= r */
    void sub_inplace(huge_number* hn_l, const huge_number* hn_r);

    /* l *= r */
    void mul_inplace(huge_number* hn_l, const huge_number* hn_r);

    /* l /= r */
    void div_inplace(huge_number* hn_l, const huge_number* hn_r);

    /* hn <<= n */
    void lshift_inplace(huge_number* hn, unsigned long long n);

#endif