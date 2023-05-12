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

#include <random>
#include "gtest/gtest.h"
#include "../src/flexbox_node.h"

#define gen_test(test_name, enum_type, set_name, get_name, last_enum) \
TEST(Property, test_name) \
{ \
    flexbox_node root; \
    for (size_t i = 0; i <= last_enum; ++i) \
    { \
        root.set_name(static_cast<enum_type>(i)); \
        EXPECT_TRUE(root.get_name() == static_cast<enum_type>(i)); \
    } \
}

#define gen_test1(test_name, enum_type, set_name, get_name, last_enum) \
TEST(Property, test_name) \
{ \
    flexbox_node root; \
    std::default_random_engine e; \
    std::uniform_real_distribution<float> u(0.0f, 100.0f); \
    for (size_t i = 0; i <= last_enum; ++i) \
    { \
        auto value = u(e); \
        root.set_name(static_cast<enum_type>(i), value); \
        EXPECT_FLOAT_EQ(root.get_name(static_cast<enum_type>(i)), value); \
    } \
}

gen_test(Direction, direction, set_direction, get_direction, direction_column)
gen_test(JustifyContent, justify_content, set_justify_content, get_justify_content, justify_content_space_evenly)
gen_test(AlignItems, align_items, set_align_items, get_align_items, align_items_flex_end)
gen_test(AlignContent, align_content, set_align_content, get_align_content, align_content_flex_end)
gen_test(Wrap, wrap, set_wrap, get_wrap, wrap_no_wrap)
gen_test1(Gap, gap, set_gap, get_gap, gap_row);
gen_test1(Padding, padding, set_padding, get_padding, padding_bottom);
gen_test(Position, position, set_position, get_position, position_relative)
gen_test1(LTRB, ltrb, set_ltrb, get_ltrb, ltrb_bottom);
gen_test(Overflow, overflow, set_overflow, get_overflow, overflow_scroll)

TEST(Property, length)
{
    flexbox_node root;
    vector<decltype(std::make_tuple(
        std::bind(&flexbox_node::set_width, &root, std::placeholders::_1, std::placeholders::_2),
        std::bind(&flexbox_node::get_width, &root), 
        true/*true for have unit_auto, false for not have unit_auto*/))> fun;

    fun.emplace_back(std::make_tuple(
        std::bind(&flexbox_node::set_width, &root, std::placeholders::_1, std::placeholders::_2),
        std::bind(&flexbox_node::get_width, &root), 
        true));
    fun.emplace_back(std::make_tuple(
        std::bind(&flexbox_node::set_min_width, &root, std::placeholders::_1, std::placeholders::_2),
        std::bind(&flexbox_node::get_min_width, &root), 
        false));
    fun.emplace_back(std::make_tuple(
        std::bind(&flexbox_node::set_max_width, &root, std::placeholders::_1, std::placeholders::_2),
        std::bind(&flexbox_node::get_max_width, &root), 
        false));  
    fun.emplace_back(std::make_tuple(
        std::bind(&flexbox_node::set_height, &root, std::placeholders::_1, std::placeholders::_2),
        std::bind(&flexbox_node::get_height, &root), 
        true));
    fun.emplace_back(std::make_tuple(
        std::bind(&flexbox_node::set_min_height, &root, std::placeholders::_1, std::placeholders::_2),
        std::bind(&flexbox_node::get_min_height, &root), 
        false));
    fun.emplace_back(std::make_tuple(
        std::bind(&flexbox_node::set_max_height, &root, std::placeholders::_1, std::placeholders::_2),
        std::bind(&flexbox_node::get_max_height, &root), 
        false));          
              

#define fun_set std::get<0>(item)
    #define fun_get std::get<1>(item)

    for (auto item : fun)
    {
        fun_set(unit_point, optional<float>(3.14f));
        auto re = fun_get();
        EXPECT_TRUE(std::get<0>(re) == unit_point);
        EXPECT_FLOAT_EQ(*std::get<1>(re), 3.14f);

        fun_set(unit_percent, optional<float>(50.0f));
        re = fun_get();
        EXPECT_TRUE(std::get<0>(re) == unit_percent);
        EXPECT_FLOAT_EQ(*std::get<1>(re), 50.0f);

        if (std::get<2>(item))
        {
            fun_set(unit_auto, optional<float>());
            re = fun_get();
            EXPECT_TRUE(std::get<0>(re) == unit_auto && !std::get<1>(re));
        }

        fun_set(unit_undefine, optional<float>());
        re = fun_get();
        EXPECT_TRUE(std::get<0>(re) == unit_undefine && !std::get<1>(re));
    }

    #undef fun_set
    #undef fun_get
}