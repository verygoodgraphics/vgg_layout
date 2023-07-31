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
#include "utility.h"

TEST(Utility, DealAlign)
{
    // 有充足的空间
    EXPECT_DOUBLE_EQ(deal_align_center(100, 50, 610, 800), 120.0);
    EXPECT_DOUBLE_EQ(deal_align_center(50, 50, 610, 800), 95.0);
    EXPECT_DOUBLE_EQ(deal_align_center(0, 0, 610, 800), 95.0);

    // 空间不足, 且 widget_len >= content_len
    EXPECT_DOUBLE_EQ(deal_align_center(50, 50, 610, 710), 50.0);
    EXPECT_DOUBLE_EQ(deal_align_center(50, 50, 610, 650), 20.0);
    EXPECT_DOUBLE_EQ(deal_align_center(50, 100, 610, 650), -5.0);
    EXPECT_DOUBLE_EQ(deal_align_center(100, 50, 610, 650), 45.0);
    EXPECT_DOUBLE_EQ(deal_align_center(0, 0, 610, 610), 0.0);

    // 空间不足, 且 widget_len < content_len
    EXPECT_DOUBLE_EQ(deal_align_center(50, 50, 610, 600), -5.0);
    EXPECT_DOUBLE_EQ(deal_align_center(100, 50, 610, 600), 20);
    EXPECT_DOUBLE_EQ(deal_align_center(50, 100, 610, 600), -30);
    EXPECT_DOUBLE_EQ(deal_align_center(0, 0, 610, 600), -5.0);
}
