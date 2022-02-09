#include "string_tool.h"
#include <stdio.h>

void reverse_str(char* str)
{
    if(str == NULL)
        return;

    char t;
    char* tail = str;
        
    while(*(tail + 1))
        tail++;
    
    while(str != tail)
    {
        t = *str;
        *str = *tail;
        *tail = t;

        str++;
        tail--;
    }
}

void ull_to_str(unsigned long long x, char* dst, size_t* length_p)
{
    char flag = x != 0;
    size_t length = 0;

    while(x)
    {
        dst[length++] = x % 10;
        x /= 10;
    }

    if(!flag)
        dst[length++] = 0;
    
    *length_p = length;
}