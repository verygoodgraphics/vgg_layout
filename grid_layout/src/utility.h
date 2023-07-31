/*
MIT License

Copyright (c) 2023 Very Good Graphics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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