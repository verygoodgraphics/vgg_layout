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

#include <fstream>
#include "gtest/gtest.h"
#include "../src/flexbox_node.h"

using std::ofstream;

void write_node_info(flexbox_node *root, ofstream &ofs)
{
    float value[4] = {};
    value[0] = root->get_layout_left();
    value[1] = root->get_layout_top();
    value[2] = root->get_layout_width();
    value[3] = root->get_layout_height();

    char buf[1024] = {};
    for (int i = 0; i < 4; ++i)
    {
        sprintf(buf, "%.2f", value[i]);
        ofs.write(buf, strlen(buf));
        ofs.write(" ", 1);
    }
    ofs.write("\n", 1);
}

void print_node(flexbox_node *root)
{
    ofstream ofs("out.txt");
    if (!ofs.is_open())
    {
        assert(false);
        return;
    }

    write_node_info(root, ofs);

    for (uint32_t i = 0; i < root->child_count(); ++i)
    {
        write_node_info(root->get_child(i), ofs);
    }
}

TEST(Layout, Direction)
{
    flexbox_node root;
    root.set_direction(direction_column);

    root.add_child();
    root.add_child();
    root.add_child();

    for (uint32_t i = 0; i < root.child_count(); ++i)
    {
        auto child = root.get_child(i);
        child->set_position(position_relative);
    }

    root.get_child(0)->set_grow(0.1f);
    root.get_child(1)->set_grow(0.2f);
    root.get_child(2)->set_grow(0.4f);

    root.set_width(unit_point, 500.0f);
    root.set_height(unit_point, 500.0f);

    root.calc_layout();
    print_node(&root);

    char buf[1024] = {};
    std::ifstream ifs("out.txt");
    EXPECT_TRUE(ifs.is_open());
    ifs.read(buf, sizeof buf);
    buf[ifs.gcount()] = 0;
    ifs.close();
    EXPECT_STREQ(buf, "0.00 0.00 500.00 500.00 \n0.00 0.00 500.00 50.00 \n0.00 50.00 500.00 100.00 \n0.00 150.00 500.00 200.00 \n");

    root.set_direction(direction_row);
    root.calc_layout();
    print_node(&root);
    ifs.open("out.txt");
    EXPECT_TRUE(ifs.is_open());
    ifs.read(buf, sizeof buf);
    buf[ifs.gcount()] = 0;
    EXPECT_STREQ(buf, "0.00 0.00 500.00 500.00 \n0.00 0.00 50.00 500.00 \n50.00 0.00 100.00 500.00 \n150.00 0.00 200.00 500.00 \n");
}

TEST(Layout, DynamicChild)
{
    flexbox_node root;
    root.set_direction(direction_column);

    root.add_child();
    root.add_child();
    root.add_child();

    for (uint32_t i = 0; i < root.child_count(); ++i)
    {
        auto child = root.get_child(i);
        child->set_position(position_relative);
    }

    root.get_child(0)->set_width(unit_point, 50.0f);
    root.get_child(0)->set_height(unit_point, 100.0f);
    root.get_child(1)->set_width(unit_percent, 20.0f);
    root.get_child(1)->set_height(unit_percent, 40.0f);
    root.get_child(2)->set_grow(0.5);

    root.set_width(unit_point, 500.0f);
    root.set_height(unit_point, 500.0f);

    root.calc_layout();
    print_node(&root);

    char buf[1024] = {};
    std::ifstream ifs("out.txt");
    EXPECT_TRUE(ifs.is_open());
    ifs.read(buf, sizeof buf);
    buf[ifs.gcount()] = 0;
    ifs.close();
    EXPECT_STREQ(buf, "0.00 0.00 500.00 500.00 \n0.00 0.00 50.00 100.00 \n0.00 100.00 100.00 200.00 \n0.00 300.00 500.00 100.00 \n");

    root.remove_child(1);
    root.calc_layout();
    print_node(&root);
    ifs.open("out.txt");
    EXPECT_TRUE(ifs.is_open());
    ifs.read(buf, sizeof buf);
    buf[ifs.gcount()] = 0;
    ifs.close();
    EXPECT_STREQ(buf, "0.00 0.00 500.00 500.00 \n0.00 0.00 50.00 100.00 \n0.00 100.00 500.00 200.00 \n");
}

// auto-不换行-单个子项
TEST(Layout, smartGap0)
{
    flexbox_node root;
    root.set_direction(direction_row);
    root.set_wrap(wrap::wrap_no_wrap);
    root.set_justify_content(justify_content::justify_content_space_between);
    root.set_align_items(align_items::align_items_flex_start);
    root.set_align_content(align_content::align_content_space_between);

    root.set_gap(gap::gap_column, 0.0f);
    root.set_gap(gap::gap_row, 0.0f);

    root.add_child();
    auto child_0 = root.get_child(0);
    child_0->set_width(unit::unit_point, 150.0f);
    child_0->set_height(unit::unit_point, 100.0f);
    child_0->set_margin(margin::padding_left, unit::unit_auto);
    child_0->set_margin(margin::padding_right, unit::unit_auto);

    root.set_width(unit_point, 200.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 25.0);

    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), -25.0);
}

// auto-不换行-多个子项
TEST(Layout, smartGap1)
{
    flexbox_node root;
    root.set_direction(direction_row);
    root.set_wrap(wrap::wrap_no_wrap);
    root.set_justify_content(justify_content::justify_content_space_between);
    root.set_align_items(align_items::align_items_flex_start);
    root.set_align_content(align_content::align_content_space_between);

    root.set_gap(gap::gap_column, 0.0f);
    root.set_gap(gap::gap_row, 0.0f);

    root.add_child();
    auto child_0 = root.get_child(0);
    child_0->set_width(unit::unit_point, 100.0f);
    child_0->set_height(unit::unit_point, 100.0f);
    child_0->set_margin(margin::padding_left, unit::unit_point, 0.0f);
    child_0->set_margin(margin::padding_right, unit::unit_auto);

    root.add_child();
    auto child_1 = root.get_child(1);
    child_1->set_width(unit::unit_point, 150.0f);
    child_1->set_height(unit::unit_point, 150.0f);
    child_1->set_margin(margin::padding_left, unit::unit_auto);
    child_1->set_margin(margin::padding_right, unit::unit_auto);

    root.add_child();
    auto child_2 = root.get_child(2);
    child_2->set_width(unit::unit_point, 50.0f);
    child_2->set_height(unit::unit_point, 50.0f);
    child_2->set_margin(margin::padding_left, unit::unit_auto);
    child_2->set_margin(margin::padding_right, unit::unit_point, 0.0f);

    root.set_width(unit_point, 150.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 25.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 100.0);

    root.set_width(unit_point, 400.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 150.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 350.0);
}

// fix(负数)-不换行-单个子项
TEST(Layout, smartGap2)
{
    flexbox_node root;
    root.set_direction(direction_row);
    root.set_wrap(wrap::wrap_no_wrap);
    root.set_align_items(align_items::align_items_flex_start);
    root.set_align_content(align_content::align_content_space_between);

    root.set_gap(gap::gap_column, -20.0f);
    root.set_gap(gap::gap_row, 0.0f);

    root.add_child();
    auto child_0 = root.get_child(0);
    child_0->set_width(unit::unit_point, 150.0f);
    child_0->set_height(unit::unit_point, 100.0f);

    // 左, 空间不足
    root.set_justify_content(justify_content::justify_content_flex_start);
    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);

    // 左, 空间足够
    root.set_justify_content(justify_content::justify_content_flex_start);
    root.set_width(unit_point, 200.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);

    // 中, 空间不足
    root.set_justify_content(justify_content::justify_content_center);
    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), -25.0);

    // 左, 空间足够
    root.set_justify_content(justify_content::justify_content_center);
    root.set_width(unit_point, 200.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 25.0);

    // 右, 空间不足
    root.set_justify_content(justify_content::justify_content_flex_end);
    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), -50.0);

    // 右, 空间足够
    root.set_justify_content(justify_content::justify_content_flex_end);
    root.set_width(unit_point, 200.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 50.0);
}

// fix(负数)-不换行-多个子项
TEST(Layout, smartGap3)
{
    flexbox_node root;
    root.set_direction(direction_row);
    root.set_wrap(wrap::wrap_no_wrap);
    root.set_align_items(align_items::align_items_flex_start);
    root.set_align_content(align_content::align_content_space_between);

    root.set_gap(gap::gap_column, -20.0f);
    // root.set_gap(gap::gap_row, -20.0f);

    root.add_child();
    auto child_0 = root.get_child(0);
    child_0->set_width(unit::unit_point, 100.0f);
    child_0->set_height(unit::unit_point, 100.0f);

    root.add_child();
    auto child_1 = root.get_child(1);
    child_1->set_width(unit::unit_point, 150.0f);
    child_1->set_height(unit::unit_point, 150.0f);

    root.add_child();
    auto child_2 = root.get_child(2);
    child_2->set_width(unit::unit_point, 50.0f);
    child_2->set_height(unit::unit_point, 50.0f);

    // 左
    for (auto x : {240.0f, 400.0f})
    {
        root.set_justify_content(justify_content::justify_content_flex_start);
        root.set_width(unit_point, x);
        root.set_height(unit_point, 500.0f);
        root.calc_layout();
        print_node(&root);
        EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
        EXPECT_FLOAT_EQ(child_1->get_layout_left(), 80.0);
        EXPECT_FLOAT_EQ(child_2->get_layout_left(), 210.0);
    }

    // 中, 空间不足
    root.set_justify_content(justify_content::justify_content_center);
    root.set_width(unit_point, 240);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), -10.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 70.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 200.0);

    // 中, 空间足够
    root.set_justify_content(justify_content::justify_content_center);
    root.set_width(unit_point, 400);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 70.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 150.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 280.0);

    // 右, 空间不足
    root.set_justify_content(justify_content::justify_content_flex_end);
    root.set_width(unit_point, 240);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), -20.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 60.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 190.0);

    // 右, 空间足够
    root.set_justify_content(justify_content::justify_content_flex_end);
    root.set_width(unit_point, 400);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 140.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 220.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 350.0);
}

// fix(正数)-不换行-单个子项
TEST(Layout, smartGap4)
{
    flexbox_node root;
    root.set_direction(direction_row);
    root.set_wrap(wrap::wrap_no_wrap);
    root.set_align_items(align_items::align_items_flex_start);
    root.set_align_content(align_content::align_content_space_between);

    root.set_gap(gap::gap_column, 20.0f);
    // root.set_gap(gap::gap_row, 20.0f);

    root.add_child();
    auto child_0 = root.get_child(0);
    child_0->set_width(unit::unit_point, 150.0f);
    child_0->set_height(unit::unit_point, 100.0f);

    // 左, 空间不足
    root.set_justify_content(justify_content::justify_content_flex_start);
    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);

    // 左, 空间足够
    root.set_justify_content(justify_content::justify_content_flex_start);
    root.set_width(unit_point, 200.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);

    // 中, 空间不足
    root.set_justify_content(justify_content::justify_content_center);
    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), -25.0);

    // 左, 空间足够
    root.set_justify_content(justify_content::justify_content_center);
    root.set_width(unit_point, 200.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 25.0);

    // 右, 空间不足
    root.set_justify_content(justify_content::justify_content_flex_end);
    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), -50.0);

    // 右, 空间足够
    root.set_justify_content(justify_content::justify_content_flex_end);
    root.set_width(unit_point, 200.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 50.0);
}

// fix(正数)-不换行-多个子项
TEST(Layout, smartGap5)
{
    flexbox_node root;
    root.set_direction(direction_row);
    root.set_wrap(wrap::wrap_no_wrap);
    root.set_align_items(align_items::align_items_flex_start);
    root.set_align_content(align_content::align_content_space_between);

    root.set_gap(gap::gap_column, 20.0f);
    // root.set_gap(gap::gap_row, 20.0f);

    root.add_child();
    auto child_0 = root.get_child(0);
    child_0->set_width(unit::unit_point, 100.0f);
    child_0->set_height(unit::unit_point, 100.0f);

    root.add_child();
    auto child_1 = root.get_child(1);
    child_1->set_width(unit::unit_point, 150.0f);
    child_1->set_height(unit::unit_point, 150.0f);

    root.add_child();
    auto child_2 = root.get_child(2);
    child_2->set_width(unit::unit_point, 50.0f);
    child_2->set_height(unit::unit_point, 50.0f);

    // 左
    for (auto x : {240.0f, 400.0f})
    {
        root.set_justify_content(justify_content::justify_content_flex_start);
        root.set_width(unit_point, x);
        root.set_height(unit_point, 500.0f);
        root.calc_layout();
        print_node(&root);
        EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
        EXPECT_FLOAT_EQ(child_1->get_layout_left(), 120.0);
        EXPECT_FLOAT_EQ(child_2->get_layout_left(), 290.0);
    }

    // 中, 空间不足
    root.set_justify_content(justify_content::justify_content_center);
    root.set_width(unit_point, 240);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), -50.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 70.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 240.0);

    // 中, 空间足够
    root.set_justify_content(justify_content::justify_content_center);
    root.set_width(unit_point, 400);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 30.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 150.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 320.0);

    // 右, 空间不足
    root.set_justify_content(justify_content::justify_content_flex_end);
    root.set_width(unit_point, 240);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), -100.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 20.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 190.0);

    // 右, 空间足够
    root.set_justify_content(justify_content::justify_content_flex_end);
    root.set_width(unit_point, 400);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 60.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 180.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 350.0);
}

// 主轴auto-换行-单个子项
TEST(Layout, smartGap6)
{
    flexbox_node root;
    root.set_direction(direction_row);
    root.set_wrap(wrap::wrap_wrap);
    root.set_justify_content(justify_content::justify_content_space_between);
    root.set_align_items(align_items::align_items_flex_start);
    root.set_align_content(align_content::align_content_space_between);

    root.set_gap(gap::gap_column, 0.0f);
    root.set_gap(gap::gap_row, 0.0f);

    root.add_child();
    auto child_0 = root.get_child(0);
    child_0->set_width(unit::unit_point, 150.0f);
    child_0->set_height(unit::unit_point, 100.0f);

    root.set_width(unit_point, 200.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);

    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
}

// 主轴auto-换行-多个子项
TEST(Layout, smartGap7)
{
    flexbox_node root;
    root.set_direction(direction_row);
    root.set_wrap(wrap::wrap_wrap);
    root.set_justify_content(justify_content::justify_content_space_between);
    root.set_align_items(align_items::align_items_flex_start);
    root.set_align_content(align_content::align_content_space_between);

    root.set_gap(gap::gap_column, 0.0f);
    root.set_gap(gap::gap_row, 0.0f);

    root.add_child();
    auto child_0 = root.get_child(0);
    child_0->set_width(unit::unit_point, 100.0f);
    child_0->set_height(unit::unit_point, 100.0f);

    root.add_child();
    auto child_1 = root.get_child(1);
    child_1->set_width(unit::unit_point, 150.0f);
    child_1->set_height(unit::unit_point, 150.0f);

    root.add_child();
    auto child_2 = root.get_child(2);
    child_2->set_width(unit::unit_point, 50.0f);
    child_2->set_height(unit::unit_point, 50.0f);

    root.set_width(unit_point, 250.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 100.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 0.0);

    root.set_width(unit_point, 400.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 150.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 350.0);
}

// 主轴fix(负数)-换行-单个子项
TEST(Layout, smartGap8)
{
    flexbox_node root;
    root.set_direction(direction_row);
    root.set_wrap(wrap::wrap_wrap);
    root.set_align_items(align_items::align_items_flex_start);
    root.set_align_content(align_content::align_content_space_between);

    root.set_gap(gap::gap_column, -20.0f);
    root.set_gap(gap::gap_row, -20.0f);

    root.add_child();
    auto child_0 = root.get_child(0);
    child_0->set_width(unit::unit_point, 150.0f);
    child_0->set_height(unit::unit_point, 100.0f);

    // 左, 空间不足
    root.set_justify_content(justify_content::justify_content_flex_start);
    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);

    // 左, 空间足够
    root.set_justify_content(justify_content::justify_content_flex_start);
    root.set_width(unit_point, 200.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);

    // 中, 空间不足
    root.set_justify_content(justify_content::justify_content_center);
    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), -25.0);

    // 左, 空间足够
    root.set_justify_content(justify_content::justify_content_center);
    root.set_width(unit_point, 200.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 25.0);

    // 右, 空间不足
    root.set_justify_content(justify_content::justify_content_flex_end);
    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), -50.0);

    // 右, 空间足够
    root.set_justify_content(justify_content::justify_content_flex_end);
    root.set_width(unit_point, 200.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 50.0);
}

// 主轴fix(负数)-换行-多个子项
TEST(Layout, smartGap9)
{
    flexbox_node root;
    root.set_direction(direction_row);
    root.set_wrap(wrap::wrap_wrap);
    root.set_align_items(align_items::align_items_flex_start);
    root.set_align_content(align_content::align_content_space_between);

    root.set_gap(gap::gap_column, -20.0f);
    root.set_gap(gap::gap_row, -20.0f);

    root.add_child();
    auto child_0 = root.get_child(0);
    child_0->set_width(unit::unit_point, 100.0f);
    child_0->set_height(unit::unit_point, 100.0f);

    root.add_child();
    auto child_1 = root.get_child(1);
    child_1->set_width(unit::unit_point, 150.0f);
    child_1->set_height(unit::unit_point, 150.0f);

    root.add_child();
    auto child_2 = root.get_child(2);
    child_2->set_width(unit::unit_point, 50.0f);
    child_2->set_height(unit::unit_point, 50.0f);

    // 左, 空间不足
    root.set_justify_content(justify_content::justify_content_flex_start);
    root.set_width(unit_point, 240);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 80.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 0.0);

    // 左, 空间足够
    root.set_justify_content(justify_content::justify_content_flex_start);
    root.set_width(unit_point, 400);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 80.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 210.0);

    // 中, 空间不足
    root.set_justify_content(justify_content::justify_content_center);
    root.set_width(unit_point, 240);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 5.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 85.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 95.0);

    // 中, 空间足够
    root.set_justify_content(justify_content::justify_content_center);
    root.set_width(unit_point, 400);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 70.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 150.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 280.0);

    // 右, 空间不足
    root.set_justify_content(justify_content::justify_content_flex_end);
    root.set_width(unit_point, 240);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 10.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 90.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 190.0);

    // 右, 空间足够
    root.set_justify_content(justify_content::justify_content_flex_end);
    root.set_width(unit_point, 400);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 140.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 220.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 350.0);
}

// 主轴fix(正数)-换行-单个子项
TEST(Layout, smartGap10)
{
    flexbox_node root;
    root.set_direction(direction_row);
    root.set_wrap(wrap::wrap_wrap);
    root.set_align_items(align_items::align_items_flex_start);
    root.set_align_content(align_content::align_content_space_between);

    root.set_gap(gap::gap_column, 20.0f);
    root.set_gap(gap::gap_row, 20.0f);

    root.add_child();
    auto child_0 = root.get_child(0);
    child_0->set_width(unit::unit_point, 150.0f);
    child_0->set_height(unit::unit_point, 100.0f);

    // 左, 空间不足
    root.set_justify_content(justify_content::justify_content_flex_start);
    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);

    // 左, 空间足够
    root.set_justify_content(justify_content::justify_content_flex_start);
    root.set_width(unit_point, 200.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);

    // 中, 空间不足
    root.set_justify_content(justify_content::justify_content_center);
    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), -25.0);

    // 左, 空间足够
    root.set_justify_content(justify_content::justify_content_center);
    root.set_width(unit_point, 200.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 25.0);

    // 右, 空间不足
    root.set_justify_content(justify_content::justify_content_flex_end);
    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), -50.0);

    // 右, 空间足够
    root.set_justify_content(justify_content::justify_content_flex_end);
    root.set_width(unit_point, 200.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 50.0);
}

// 主轴fix(正数)-换行-多个子项
TEST(Layout, smartGap11)
{
    flexbox_node root;
    root.set_direction(direction_row);
    root.set_wrap(wrap::wrap_wrap);
    root.set_align_items(align_items::align_items_flex_start);
    root.set_align_content(align_content::align_content_space_between);

    root.set_gap(gap::gap_column, 20.0f);
    root.set_gap(gap::gap_row, 20.0f);

    root.add_child();
    auto child_0 = root.get_child(0);
    child_0->set_width(unit::unit_point, 100.0f);
    child_0->set_height(unit::unit_point, 100.0f);

    root.add_child();
    auto child_1 = root.get_child(1);
    child_1->set_width(unit::unit_point, 150.0f);
    child_1->set_height(unit::unit_point, 150.0f);

    root.add_child();
    auto child_2 = root.get_child(2);
    child_2->set_width(unit::unit_point, 50.0f);
    child_2->set_height(unit::unit_point, 50.0f);

    // 左, 空间不足
    root.set_justify_content(justify_content::justify_content_flex_start);
    root.set_width(unit_point, 240);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 170.0);

    // 左, 空间足够
    root.set_justify_content(justify_content::justify_content_flex_start);
    root.set_width(unit_point, 400);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 120.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 290.0);

    // 中, 空间不足
    root.set_justify_content(justify_content::justify_content_center);
    root.set_width(unit_point, 240);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 70.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 10.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 180.0);

    // 中, 空间足够
    root.set_justify_content(justify_content::justify_content_center);
    root.set_width(unit_point, 400);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 30.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 150.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 320.0);

    // 右, 空间不足
    root.set_justify_content(justify_content::justify_content_flex_end);
    root.set_width(unit_point, 240);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 140.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 20.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 190.0);

    // 右, 空间足够
    root.set_justify_content(justify_content::justify_content_flex_end);
    root.set_width(unit_point, 400);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 60.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 180.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 350.0);
}

// 侧轴auto-单元素
TEST(Layout, smartGap12)
{
    flexbox_node root;
    root.set_direction(direction_row);
    root.set_wrap(wrap::wrap_wrap);
    root.set_justify_content(justify_content::justify_content_flex_start);
    root.set_align_items(align_items::align_items_flex_start);
    root.set_align_content(align_content::align_content_space_between);

    root.set_gap(gap::gap_column, 0.0f);
    root.set_gap(gap::gap_row, 0.0f);

    root.add_child();
    auto child_0 = root.get_child(0);
    child_0->set_width(unit::unit_point, 150.0f);
    child_0->set_height(unit::unit_point, 100.0f);

    root.set_width(unit_point, 200.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_0->get_layout_top(), 0.0);

    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 100.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_0->get_layout_top(), 0.0);
}

// 侧轴auto-多元素
TEST(Layout, smartGap13)
{
    flexbox_node root;
    root.set_direction(direction_row);
    root.set_wrap(wrap::wrap_wrap);
    root.set_justify_content(justify_content::justify_content_space_between);
    root.set_align_items(align_items::align_items_flex_start);
    root.set_align_content(align_content::align_content_space_between);

    root.set_gap(gap::gap_column, 0.0f);
    root.set_gap(gap::gap_row, 0.0f);

    root.add_child();
    auto child_0 = root.get_child(0);
    child_0->set_width(unit::unit_point, 100.0f);
    child_0->set_height(unit::unit_point, 100.0f);

    root.add_child();
    auto child_1 = root.get_child(1);
    child_1->set_width(unit::unit_point, 150.0f);
    child_1->set_height(unit::unit_point, 150.0f);

    root.add_child();
    auto child_2 = root.get_child(2);
    child_2->set_width(unit::unit_point, 50.0f);
    child_2->set_height(unit::unit_point, 50.0f);

    root.set_width(unit_point, 160.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_0->get_layout_top(), 0.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_top(), 200.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_top(), 450.0);

    root.set_width(unit_point, 240.0f);
    root.set_height(unit_point, 300.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_0->get_layout_top(), 0.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_top(), 150.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_left(), 190.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_top(), 150.0);
}

// 侧轴正数-单元素
TEST(Layout, smartGap14)
{
    flexbox_node root;
    root.set_direction(direction_row);
    root.set_wrap(wrap::wrap_wrap);
    root.set_justify_content(justify_content::justify_content_flex_start);
    root.set_align_items(align_items::align_items_flex_start);

    root.set_gap(gap::gap_column, 20.0f);
    root.set_gap(gap::gap_row, 20.0f);

    root.add_child();
    auto child_0 = root.get_child(0);
    child_0->set_width(unit::unit_point, 150.0f);
    child_0->set_height(unit::unit_point, 150.0f);

    // 上, 空间不足
    root.set_align_content(align_content::align_content_flex_start);
    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_0->get_layout_top(), 0.0);

    // 上, 空间足够
    root.set_align_content(align_content::align_content_flex_start);
    root.set_width(unit_point, 200.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_0->get_layout_top(), 0.0);

    // 中, 空间不足
    root.set_align_content(align_content::align_content_center);
    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 100.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_0->get_layout_top(), -25.0);

    // 中, 空间足够
    root.set_align_content(align_content::align_content_center);
    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_0->get_layout_top(), 175.0);

    // 下, 空间不足
    root.set_align_content(align_content::align_content_flex_end);
    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 100.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_0->get_layout_top(), -50.0);

    // 下, 空间足够
    root.set_align_content(align_content::align_content_flex_end);
    root.set_width(unit_point, 100.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_left(), 0.0);
    EXPECT_FLOAT_EQ(child_0->get_layout_top(), 350.0);
}

// 侧轴正数-多元素
TEST(Layout, smartGap15)
{
    flexbox_node root;
    root.set_direction(direction_row);
    root.set_wrap(wrap::wrap_wrap);
    root.set_justify_content(justify_content::justify_content_flex_start);
    root.set_align_items(align_items::align_items_flex_start);

    root.set_gap(gap::gap_column, 0.0f);
    root.set_gap(gap::gap_row, 20.0f);

    root.add_child();
    auto child_0 = root.get_child(0);
    child_0->set_width(unit::unit_point, 100.0f);
    child_0->set_height(unit::unit_point, 100.0f);

    root.add_child();
    auto child_1 = root.get_child(1);
    child_1->set_width(unit::unit_point, 150.0f);
    child_1->set_height(unit::unit_point, 150.0f);

    root.add_child();
    auto child_2 = root.get_child(2);
    child_2->set_width(unit::unit_point, 50.0f);
    child_2->set_height(unit::unit_point, 50.0f);

    // 上-空间不足
    root.set_align_content(align_content::align_content_flex_start);
    root.set_width(unit_point, 170.0f);
    root.set_height(unit_point, 200.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_top(), 0.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_top(), 120.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_top(), 290.0);

    // 上-空间足够
    root.set_align_content(align_content::align_content_flex_start);
    root.set_width(unit_point, 170.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_top(), 0.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_top(), 120.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_top(), 290.0);

    // 中-空间不足
    root.set_align_content(align_content::align_content_center);
    root.set_width(unit_point, 170.0f);
    root.set_height(unit_point, 200.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_top(), -70.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_top(), 50.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_top(), 220.0);

    // 中-空间足够
    root.set_align_content(align_content::align_content_center);
    root.set_width(unit_point, 170.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_top(), 80.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_top(), 200.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_top(), 370.0);

    // 下-空间不足
    root.set_align_content(align_content::align_content_flex_end);
    root.set_width(unit_point, 170.0f);
    root.set_height(unit_point, 200.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_top(), -140.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_top(), -20.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_top(), 150.0);

    // 下-空间足够
    root.set_align_content(align_content::align_content_flex_end);
    root.set_width(unit_point, 170.0f);
    root.set_height(unit_point, 500.0f);
    root.calc_layout();
    print_node(&root);
    EXPECT_FLOAT_EQ(child_0->get_layout_top(), 160.0);
    EXPECT_FLOAT_EQ(child_1->get_layout_top(), 280.0);
    EXPECT_FLOAT_EQ(child_2->get_layout_top(), 450.0);
}
