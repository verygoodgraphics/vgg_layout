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
#include "../src/flexbox_node.h"

TEST(Child, AddChild)
{
    flexbox_node root;
    
    EXPECT_TRUE(root.add_child());
    EXPECT_TRUE(root.child_count() == 1);

    flexbox_node::p_node child(new flexbox_node);
    EXPECT_TRUE(root.add_child(child, 0));
    EXPECT_TRUE(root.child_count() == 2);
    EXPECT_TRUE(root.get_child(0) == child);

    EXPECT_TRUE(!root.add_child(1024));
}

TEST(Child, RemoveChild)
{
    flexbox_node root;
    EXPECT_TRUE(root.add_child());

    flexbox_node::p_node child(new flexbox_node);
    EXPECT_TRUE(root.add_child(child));

    EXPECT_TRUE(root.remove_child(0));
    EXPECT_TRUE(root.child_count() == 1);
    EXPECT_TRUE(root.get_child(0) == child);
    EXPECT_TRUE(!root.remove_child(1024));
}

TEST(Child, NestChild)
{
    flexbox_node root;
    EXPECT_TRUE(root.add_child());

    flexbox_node::p_node child(new flexbox_node);
    EXPECT_TRUE(child->add_child());
    EXPECT_TRUE(child->add_child());
    EXPECT_TRUE(child->child_count() == 2);

    EXPECT_TRUE(root.add_child(child));
    EXPECT_TRUE(root.child_count() == 2);
}

TEST(Child, ReuseChild)
{
    flexbox_node root;
    EXPECT_TRUE(root.add_child());

    flexbox_node::p_node child(new flexbox_node);
    EXPECT_TRUE(root.add_child(child));
    
    auto node = root.remove_child(1);
    EXPECT_TRUE(node);
    EXPECT_TRUE(node == child);

    EXPECT_TRUE(root.add_child(node, 0));
    EXPECT_TRUE(root.get_child(0) == child);
}