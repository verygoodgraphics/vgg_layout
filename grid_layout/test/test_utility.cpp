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
