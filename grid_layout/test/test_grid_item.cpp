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

#include "gtest/gtest.h"
#include "grid_layout.h"

TEST(GridItem, CalcLayout)
{
    // 默认情况
    {
        grid_item item;
        item.calc_layout(10, 20);
        EXPECT_DOUBLE_EQ(item.get_layout_top(), 0);
        EXPECT_DOUBLE_EQ(item.get_layout_left(), 0);
        EXPECT_DOUBLE_EQ(item.get_layout_width(), 10);
        EXPECT_DOUBLE_EQ(item.get_layout_height(), 20);    
    }

    // 水平方向的测试
    {
        grid_item item;

        // 子项宽度足够, 测试左对齐
        item.set_width(std::make_tuple(length_unit::length_unit_point, 10));
        item.set_horizontal_align(align::align_start);
        item.calc_layout(20, 30);
        EXPECT_DOUBLE_EQ(item.get_layout_width(), 10);
        EXPECT_DOUBLE_EQ(item.get_layout_left(), 0);

        // 子项宽度足够, 测试居中对齐
        item.set_width(std::make_tuple(length_unit::length_unit_percent, 60));
        item.set_horizontal_align(align::align_center);
        item.calc_layout(20, 30);
        EXPECT_DOUBLE_EQ(item.get_layout_width(), 12);
        EXPECT_DOUBLE_EQ(item.get_layout_left(), 4);

        // 子项宽度足够, 测试右对齐
        item.set_width(std::make_tuple(length_unit::length_unit_percent, 60));
        item.set_horizontal_align(align::align_end);
        item.calc_layout(20, 30);
        EXPECT_DOUBLE_EQ(item.get_layout_width(), 12);
        EXPECT_DOUBLE_EQ(item.get_layout_left(), 8);   


        // 子项宽度不足, 测试左对齐
        item.set_width(std::make_tuple(length_unit::length_unit_point, 10));
        item.set_horizontal_align(align::align_start);
        item.calc_layout(5, 30);
        EXPECT_DOUBLE_EQ(item.get_layout_width(), 10);
        EXPECT_DOUBLE_EQ(item.get_layout_left(), 0);

        // 子项宽度不足, 测试居中对齐
        item.set_width(std::make_tuple(length_unit::length_unit_point, 10));
        item.set_horizontal_align(align::align_center);
        item.calc_layout(5, 30);
        EXPECT_DOUBLE_EQ(item.get_layout_width(), 10);
        EXPECT_DOUBLE_EQ(item.get_layout_left(), -2.5);

        // 子项宽度不足, 测试右对齐
        item.set_width(std::make_tuple(length_unit::length_unit_point, 10));
        item.set_horizontal_align(align::align_end);
        item.calc_layout(5, 30);
        EXPECT_DOUBLE_EQ(item.get_layout_width(), 10);
        EXPECT_DOUBLE_EQ(item.get_layout_left(), -5);        
    }

    // 垂直方向的测试
    {
        grid_item item;

        // 子项高度足够, 测试上对齐
        item.set_height(std::make_tuple(length_unit::length_unit_point, 10));
        item.set_vertical_align(align::align_start);
        item.calc_layout(30, 20);
        EXPECT_DOUBLE_EQ(item.get_layout_height(), 10);
        EXPECT_DOUBLE_EQ(item.get_layout_top(), 0);

        // 子项高度足够, 测试居中对齐
        item.set_height(std::make_tuple(length_unit::length_unit_percent, 60));
        item.set_vertical_align(align::align_center);
        item.calc_layout(30, 20);
        EXPECT_DOUBLE_EQ(item.get_layout_height(), 12);
        EXPECT_DOUBLE_EQ(item.get_layout_top(), 4);

        // 子项高度足够, 测试下对齐
        item.set_height(std::make_tuple(length_unit::length_unit_percent, 60));
        item.set_vertical_align(align::align_end);
        item.calc_layout(30, 20);
        EXPECT_DOUBLE_EQ(item.get_layout_height(), 12);
        EXPECT_DOUBLE_EQ(item.get_layout_top(), 8);   


        // 子项高度不足, 测试上对齐
        item.set_height(std::make_tuple(length_unit::length_unit_point, 10));
        item.set_vertical_align(align::align_start);
        item.calc_layout(30, 5);
        EXPECT_DOUBLE_EQ(item.get_layout_height(), 10);
        EXPECT_DOUBLE_EQ(item.get_layout_top(), 0);

        // 子项高度不足, 测试居中对齐
        item.set_height(std::make_tuple(length_unit::length_unit_point, 10));
        item.set_vertical_align(align::align_center);
        item.calc_layout(30, 5);
        EXPECT_DOUBLE_EQ(item.get_layout_height(), 10);
        EXPECT_DOUBLE_EQ(item.get_layout_top(), -2.5);

        // 子项高度不足, 测试下对齐
        item.set_height(std::make_tuple(length_unit::length_unit_point, 10));
        item.set_vertical_align(align::align_end);
        item.calc_layout(30, 5);
        EXPECT_DOUBLE_EQ(item.get_layout_height(), 10);
        EXPECT_DOUBLE_EQ(item.get_layout_top(), -5);        
    }
}

TEST(GridItem, AreaConflict)
{
    grid_item item_0;
    grid_item item_1;

    item_0.set_item_pos_strategy(item_pos_strategy_auto);
    item_1.set_item_pos_strategy(item_pos_strategy_auto);

    item_0.set_row_id(0);
    item_0.set_column_id(0);
    item_0.set_row_span(2);
    item_0.set_column_span(2);

    item_1.set_row_id(0);
    item_1.set_column_id(1);
    item_1.set_row_span(1);
    item_1.set_column_span(1);

    EXPECT_TRUE(!area_conflict(item_0, item_1));

    item_0.set_item_pos_strategy(item_pos_strategy_fix);
    item_1.set_item_pos_strategy(item_pos_strategy_fix);    
    EXPECT_TRUE(area_conflict(item_0, item_1));

    item_1.set_row_id(0);
    item_1.set_column_id(2);
    item_1.set_row_span(1);
    item_1.set_column_span(1);    
    EXPECT_TRUE(!area_conflict(item_0, item_1));

    item_1.set_row_id(2);
    item_1.set_column_id(0);
    item_1.set_row_span(1);
    item_1.set_column_span(1);    
    EXPECT_TRUE(!area_conflict(item_0, item_1));    
}