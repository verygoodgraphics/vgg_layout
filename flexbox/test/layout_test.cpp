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
        write_node_info(root->get_child(i).get(), ofs);
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