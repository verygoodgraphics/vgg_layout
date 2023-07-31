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

#ifndef VGG_GRID_DEFINE
#define VGG_GRID_DEFINE

#ifndef NDEBUG
#define SZN_DEBUG
#endif

enum length_unit
{
    length_unit_point,
    length_unit_percent,   // 50 for 50%

    // 当项作为容器时, grid_layout 自身提供了计算自适应的功能
    // 当项作为元素时, 不支持自适应
    //length_unit_auto
};

enum grid_auto_flow
{
    grid_auto_flow_row = 1,
    grid_auto_flow_row_dense = 2
};

enum column_width_strategy
{
    column_width_strategy_min = 1,
    column_width_strategy_fix = 2
};

enum row_height_strategy
{
    row_height_strategy_fill = 1,
    row_height_strategy_fit = 2,
    row_height_strategy_fix = 3
};

enum align
{
    align_start = 1,
    align_center = 2,
    align_end = 3
};

enum item_pos_strategy
{
    item_pos_strategy_auto = 1,
    item_pos_strategy_fix = 2
};

#endif