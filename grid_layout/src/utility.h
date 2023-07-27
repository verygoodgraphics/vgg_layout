#ifndef VGG_GRID_UTILITY
#define VGG_GRID_UTILITY

#include <cassert>
#include <algorithm>

inline double insure_nonnegative(double value)
{
    assert(value >= 0);
    return (std::max)(value, 0.0);
}

/**
 * 处理居中对齐
 * 
 * @param padding_0 padding_0 和 padding_1 是一对, 要么是左右, 要么是上下
 * @param padding_1 padding_0 和 padding_1 是一对, 要么是左右, 要么是上下
 * @param content_len 被对齐的内容长度, 含 gap
 * @param widget_len 容器的长度
 * 
 * @return 布局的起始位置(可能为负数)
 * 
 * @note 当前的规则, 参考了 framer
*/
double deal_align_center(double padding_0, double padding_1, double content_len, double widget_len);

#endif