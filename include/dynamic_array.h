#ifndef DYNAMIC_ARRAY_HDIFHD
#define DYNAMIC_ARRAY_HDIFHD

#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long element_t;   // 以后要改就改这里，这里是元素类型
#define elem_size sizeof(element_t)

typedef struct dy_array
{
    element_t* array;
    size_t length;
    size_t size;
}dynamic_array;

/* 生成一个空的动态扩容数组 */
dynamic_array* new_dynamic_array();

/* 删除动态扩容数组 */
void destroy_dynamic_array(dynamic_array* d_array);

void da_push(dynamic_array* d_array, element_t value);

/* 
    打印动态扩容数组
    以元素类型为 unsigned long long 来打印
    2022-2-8 18:16:43
 */
void print_dynamic_array(dynamic_array* d_array);

/* 
    输出到文件
    以
        length (8 Bytes)
        elem_size (8 Bytes)
        data..
    保存
 */
void to_file(dynamic_array* d_array, FILE* fp);

/* 
    从文件加载
    以
        length (8 Bytes)
        elem_size (8 Bytes)
        data..
    加载
 */
dynamic_array* from_file(FILE* fp);

#endif