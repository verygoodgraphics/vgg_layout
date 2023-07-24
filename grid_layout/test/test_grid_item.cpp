#include "gtest/gtest.h"
#include "grid_layout.h"

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