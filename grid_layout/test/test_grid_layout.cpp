#include "gtest/gtest.h"
#include "grid_layout.h"

void judge_child_area(const grid_item &node, uint32_t row, uint32_t column, uint32_t row_span, uint32_t column_span)
{
    EXPECT_TRUE(node.get_row_id() == row);
    EXPECT_TRUE(node.get_column_id() == column);
    EXPECT_TRUE(node.get_row_span() == row_span);
    EXPECT_TRUE(node.get_column_span() == column_span);
}

TEST(GridLayout, AddChild)
{
    grid_layout grid(10);

    EXPECT_TRUE(!grid.add_child(1));
    EXPECT_TRUE(grid.add_child(-1));
    EXPECT_TRUE(grid.add_child(-1));
    EXPECT_TRUE(grid.add_child(1));
    EXPECT_TRUE(grid.get_child_count() == 3);

    grid_layout::p_node child(new grid_item);
    child->set_item_pos_strategy(item_pos_strategy_fix);
    child->set_row_id(0);
    child->set_column_id(0);
    child->set_row_span(2);
    child->set_column_span(2);
    EXPECT_TRUE(grid.add_child(child, 1));
    EXPECT_TRUE(grid.get_child(1)->get_row_span() == 2);

    child.reset(new grid_item);
    child->set_item_pos_strategy(item_pos_strategy_fix);
    child->set_row_id(0);
    child->set_column_id(1);
    child->set_row_span(1);
    child->set_column_span(1);    
    EXPECT_TRUE(!grid.add_child(child));

    child->set_item_pos_strategy(item_pos_strategy_auto);
    EXPECT_TRUE(grid.add_child(child));

    child.reset(new grid_item);
    child->set_item_pos_strategy(item_pos_strategy_fix);
    child->set_column_id(grid.get_column_count() - 1);
    child->set_column_span(2);    
    EXPECT_TRUE(!grid.add_child(child));
}

TEST(GridLayout, ChangeChildAttr)
{
    grid_layout grid(10);

    grid_layout::p_node child(new grid_item);
    child->set_item_pos_strategy(item_pos_strategy_fix);
    child->set_column_span(2);
    EXPECT_TRUE(grid.add_child(child));
    EXPECT_TRUE(grid.get_child(0)->get_column_span() == 2);

    child = grid.remove_child(0);
    EXPECT_TRUE(!grid.get_child_count());
    EXPECT_TRUE(child);
    child->set_column_span(1);
    EXPECT_TRUE(grid.add_child(child));
    EXPECT_TRUE(grid.get_child(0)->get_column_span() == 1);
}

// 比较简单的布局
TEST(GridLayout, ChildAreaSimple)
{
    grid_layout grid(5);

    grid_layout::p_node node(new grid_item);
    node->id_ = 1;
    node->set_item_pos_strategy(item_pos_strategy_auto);
    node->set_row_span(2);
    node->set_column_span(2);
    EXPECT_TRUE(grid.add_child(node));

    node.reset(new grid_item);
    node->id_ = 2;
    node->set_item_pos_strategy(item_pos_strategy_fix);
    node->set_row_id(2);
    node->set_column_id(0);
    node->set_row_span(1);
    node->set_column_span(1);
    EXPECT_TRUE(grid.add_child(node));    

    node.reset(new grid_item);
    node->id_ = 3;
    node->set_item_pos_strategy(item_pos_strategy_auto);
    node->set_row_span(1);
    node->set_column_span(1);
    EXPECT_TRUE(grid.add_child(node));        

    node.reset(new grid_item);
    node->id_ = 4;
    node->set_item_pos_strategy(item_pos_strategy_auto);
    node->set_row_span(1);
    node->set_column_span(3);
    EXPECT_TRUE(grid.add_child(node));        

    node.reset(new grid_item);
    node->id_ = 5;
    node->set_item_pos_strategy(item_pos_strategy_auto);
    node->set_row_span(1);
    node->set_column_span(1);
    EXPECT_TRUE(grid.add_child(node));   

    node.reset(new grid_item);
    node->id_ = 6;
    node->set_item_pos_strategy(item_pos_strategy_auto);
    node->set_row_span(1);
    node->set_column_span(3);
    EXPECT_TRUE(grid.add_child(node));   

    node.reset(new grid_item);
    node->id_ = 7;
    node->set_item_pos_strategy(item_pos_strategy_auto);
    node->set_row_span(1);
    node->set_column_span(1);
    EXPECT_TRUE(grid.add_child(node));   

    grid.set_grid_auto_flow(grid_auto_flow_row);
    grid.calc_layout();
    grid.print();
    /*
	1		1		3		0		0	
	1		1		4		4		4	
	2		5		6		6		6	
	7		0		0		0		0	
    */
    judge_child_area(*grid.get_child(0), 0, 0, 2, 2);
    judge_child_area(*grid.get_child(1), 2, 0, 1, 1);
    judge_child_area(*grid.get_child(2), 0, 2, 1, 1);
    judge_child_area(*grid.get_child(3), 1, 2, 1, 3);
    judge_child_area(*grid.get_child(4), 2, 1, 1, 1);
    judge_child_area(*grid.get_child(5), 2, 2, 1, 3);
    judge_child_area(*grid.get_child(6), 3, 0, 1, 1);

    grid.set_grid_auto_flow(grid_auto_flow_row_dense);
    grid.calc_layout();    
    grid.print();
    /*
	1		1		3		5		7	
	1		1		4		4		4	
	2		6		6		6		0	
    */
    judge_child_area(*grid.get_child(0), 0, 0, 2, 2);
    judge_child_area(*grid.get_child(1), 2, 0, 1, 1);
    judge_child_area(*grid.get_child(2), 0, 2, 1, 1);
    judge_child_area(*grid.get_child(3), 1, 2, 1, 3);
    judge_child_area(*grid.get_child(4), 0, 3, 1, 1);
    judge_child_area(*grid.get_child(5), 2, 1, 1, 3);
    judge_child_area(*grid.get_child(6), 0, 4, 1, 1);
}

// 比较复杂的布局
TEST(GridLayout, ChildAreaComplex)
{
    grid_layout grid(5);

    grid_layout::p_node node(new grid_item);
    node.reset(new grid_item);
    node->id_ = 1;
    node->set_item_pos_strategy(item_pos_strategy_auto);
    node->set_row_span(2);
    node->set_column_span(2);
    EXPECT_TRUE(grid.add_child(node));

    node.reset(new grid_item);
    node->id_ = 2;
    node->set_item_pos_strategy(item_pos_strategy_fix);
    node->set_row_id(2);
    node->set_column_id(0);
    node->set_row_span(1);
    node->set_column_span(1);
    EXPECT_TRUE(grid.add_child(node));    

    node.reset(new grid_item);
    node->id_ = 3;
    node->set_item_pos_strategy(item_pos_strategy_auto);
    node->set_row_span(3);
    node->set_column_span(1);
    EXPECT_TRUE(grid.add_child(node));        

    node.reset(new grid_item);
    node->id_ = 4;
    node->set_item_pos_strategy(item_pos_strategy_auto);
    node->set_row_span(1);
    node->set_column_span(4);
    EXPECT_TRUE(grid.add_child(node));

    for (int i = 5; i <= 8; ++i)
    {
        node.reset(new grid_item);
        node->id_ = i;
        node->set_item_pos_strategy(item_pos_strategy_auto);
        node->set_row_span(1);
        node->set_column_span(1);
        EXPECT_TRUE(grid.add_child(node));        
    }

    grid.set_grid_auto_flow(grid_auto_flow_row);
    grid.calc_layout();
    grid.print();
    /*
	1		1		3		0		0	
	1		1		3		0		0	
	2		0		3		0		0	
	4		4		4		4		5	
	6		7		8		0		0	
    */
    judge_child_area(*grid.get_child(0), 0, 0, 2, 2);
    judge_child_area(*grid.get_child(1), 2, 0, 1, 1);
    judge_child_area(*grid.get_child(2), 0, 2, 3, 1);
    judge_child_area(*grid.get_child(3), 3, 0, 1, 4);
    judge_child_area(*grid.get_child(4), 3, 4, 1, 1);
    judge_child_area(*grid.get_child(5), 4, 0, 1, 1);
    judge_child_area(*grid.get_child(6), 4, 1, 1, 1);
    judge_child_area(*grid.get_child(7), 4, 2, 1, 1);

    for (int i = 9; i <= 12; ++i)
    {
        node.reset(new grid_item);
        node->id_ = i;
        node->set_item_pos_strategy(item_pos_strategy_auto);
        node->set_row_span(1);
        node->set_column_span(1);
        EXPECT_TRUE(grid.add_child(node));        
    }
    grid.set_grid_auto_flow(grid_auto_flow_row_dense);
    grid.calc_layout();
    grid.print();
    /*
	1		1		3		5		6	
	1		1		3		7		8	
	2		9		3		10		11	
	4		4		4		4		12	
    */
    judge_child_area(*grid.get_child(0), 0, 0, 2, 2);
    judge_child_area(*grid.get_child(1), 2, 0, 1, 1);
    judge_child_area(*grid.get_child(2), 0, 2, 3, 1);
    judge_child_area(*grid.get_child(3), 3, 0, 1, 4);
    judge_child_area(*grid.get_child(4), 0, 3, 1, 1);
    judge_child_area(*grid.get_child(5), 0, 4, 1, 1);
    judge_child_area(*grid.get_child(6), 1, 3, 1, 1);
    judge_child_area(*grid.get_child(7), 1, 4, 1, 1);   
    judge_child_area(*grid.get_child(8), 2, 1, 1, 1);   
    judge_child_area(*grid.get_child(9), 2, 3, 1, 1);
    judge_child_area(*grid.get_child(10), 2, 4, 1, 1);       
    judge_child_area(*grid.get_child(11), 3, 4, 1, 1);       
}

// 固定布局的行是最后一行
TEST(GridLayout, ChildAreaFixRow)
{
    grid_layout grid(5);

    grid_layout::p_node node(new grid_item);
    node.reset(new grid_item);
    node->id_ = 1;
    node->set_item_pos_strategy(item_pos_strategy_auto);
    node->set_row_span(2);
    node->set_column_span(2);
    EXPECT_TRUE(grid.add_child(node));

    node.reset(new grid_item);
    node->id_ = 2;
    node->set_item_pos_strategy(item_pos_strategy_fix);
    node->set_row_id(3);
    node->set_column_id(0);
    node->set_row_span(1);
    node->set_column_span(1);
    EXPECT_TRUE(grid.add_child(node));    

    node.reset(new grid_item);
    node->id_ = 3;
    node->set_item_pos_strategy(item_pos_strategy_auto);
    node->set_row_span(1);
    node->set_column_span(1);
    EXPECT_TRUE(grid.add_child(node));     

    grid.set_grid_auto_flow(grid_auto_flow_row);
    grid.calc_layout();    
    grid.print();
    /*
	1		1		3		0		0	
	1		1		0		0		0	
	0		0		0		0		0	
	2		0		0		0		0	
    */
    judge_child_area(*grid.get_child(0), 0, 0, 2, 2);
    judge_child_area(*grid.get_child(1), 3, 0, 1, 1);
    judge_child_area(*grid.get_child(2), 0, 2, 1, 1);
}

// min_row
TEST(GridLayout, MinRow)
{
    grid_layout grid_0(5);
    grid_0.calc_layout();
    EXPECT_TRUE(grid_0.get_row_count() == 1);

    grid_layout grid_1(5, 2);
    grid_1.calc_layout();
    EXPECT_TRUE(grid_1.get_row_count() == 2);    
    grid_layout::p_node node(new grid_item);
    node->id_ = 2;
    node->set_item_pos_strategy(item_pos_strategy_fix);
    node->set_row_id(3);
    node->set_column_id(0);
    node->set_row_span(1);
    node->set_column_span(1);
    EXPECT_TRUE(grid_1.add_child(node));  
    grid_1.calc_layout();
    EXPECT_TRUE(grid_1.get_row_count() == 4);    
}

TEST(GridLayoutRowInfo, RowHeightStrategyFix)
{
    // 默认情况
    {
        grid_layout grid(5, 2);
        EXPECT_TRUE(grid.calc_layout());
        EXPECT_TRUE(grid.get_height() == 2.0);

        auto row_height = grid.get_row_height();
        EXPECT_TRUE(row_height.size() == 2);
        EXPECT_DOUBLE_EQ(row_height[0], 1.0);
        EXPECT_DOUBLE_EQ(row_height[1], 1.0);

        auto row_start = grid.get_row_start();
        EXPECT_TRUE(row_start.size() == 2);
        EXPECT_DOUBLE_EQ(row_start[0], 0);
        EXPECT_DOUBLE_EQ(row_start[1], 1.0);
    }

    // 通过 grid_layout::set_row_height 设置
    {
        grid_layout grid(5, 2);

        EXPECT_TRUE(!grid.set_row_height(row_height_strategy_fix));
        EXPECT_TRUE(grid.set_row_height(row_height_strategy_fix, 10.0));
        grid.calc_layout();
        EXPECT_TRUE(grid.get_height() == 20.0);

        auto row_height = grid.get_row_height();
        EXPECT_TRUE(row_height.size() == 2);
        EXPECT_DOUBLE_EQ(row_height[0], 10.0);
        EXPECT_DOUBLE_EQ(row_height[1], 10.0);

        auto row_start = grid.get_row_start();
        EXPECT_TRUE(row_start.size() == 2);
        EXPECT_DOUBLE_EQ(row_start[0], 0.0);
        EXPECT_DOUBLE_EQ(row_start[1], 10.0);
    }

    // 测试 padding gap
    {
        grid_layout grid(5, 3);
        EXPECT_TRUE(grid.set_row_height(row_height_strategy_fix, 10.0));
        grid.set_padding(10, 20, 30, 40);
        grid.set_row_gap(5);
        grid.calc_layout();
        EXPECT_TRUE(grid.get_height() == 80.0);

        auto row_height = grid.get_row_height();
        EXPECT_TRUE(row_height.size() == 3);
        EXPECT_DOUBLE_EQ(row_height[0], 10.0);
        EXPECT_DOUBLE_EQ(row_height[1], 10.0);
        EXPECT_DOUBLE_EQ(row_height[2], 10.0);

        auto row_start = grid.get_row_start();
        EXPECT_TRUE(row_start.size() == 3);
        EXPECT_DOUBLE_EQ(row_start[0], 10.0);
        EXPECT_DOUBLE_EQ(row_start[1], 25.0);
        EXPECT_DOUBLE_EQ(row_start[2], 40.0);
    }

    // 固定高度
    {
        grid_layout grid(5, 3);
        EXPECT_TRUE(grid.set_row_height(row_height_strategy_fix, 10.0));
        grid.set_padding(10, 20, 30, 40);
        grid.set_row_gap(5);
        grid.calc_layout(50);
        EXPECT_TRUE(grid.get_height() == 50.0);        
    }
}

TEST(GridLayoutRowInfo, RowHeightStrategyFill)
{
    // 将高度设置为自适应, 导致布局失败
    {
        grid_layout grid(5, 2);
        EXPECT_TRUE(grid.set_row_height(row_height_strategy_fill));
        EXPECT_TRUE(!grid.calc_layout());
    }

    // 不设置 padding 和 gap
    {
        grid_layout grid(5, 2);
        EXPECT_TRUE(grid.set_row_height(row_height_strategy_fill));
        EXPECT_TRUE(grid.calc_layout(100));        
        EXPECT_DOUBLE_EQ(grid.get_height(), 100.0);

        auto row_height = grid.get_row_height();
        EXPECT_TRUE(row_height.size() == 2);
        EXPECT_DOUBLE_EQ(row_height[0], 50.0);
        EXPECT_DOUBLE_EQ(row_height[1], 50.0);     

        auto row_start = grid.get_row_start();
        EXPECT_TRUE(row_start.size() == 2);
        EXPECT_DOUBLE_EQ(row_start[0], 0.0);
        EXPECT_DOUBLE_EQ(row_start[1], 50.0);           
    }

    // 设置 padding 和 gap
    {
        grid_layout grid(5, 2);
        EXPECT_TRUE(grid.set_row_height(row_height_strategy_fill));
        grid.set_padding(10, 20, 30, 40);
        grid.set_row_gap(5);
        EXPECT_TRUE(grid.calc_layout(100)); 
        EXPECT_DOUBLE_EQ(grid.get_height(), 100.0);

        auto row_height = grid.get_row_height();
        EXPECT_TRUE(row_height.size() == 2);
        EXPECT_DOUBLE_EQ(row_height[0], 27.5);
        EXPECT_DOUBLE_EQ(row_height[1], 27.5);     

        auto row_start = grid.get_row_start();
        EXPECT_TRUE(row_start.size() == 2);
        EXPECT_DOUBLE_EQ(row_start[0], 10);
        EXPECT_DOUBLE_EQ(row_start[1], 10 + 27.5 + 5);  
    }

    // 设置 padding 和 gap 导致高度不足
    {
        grid_layout grid(5, 2);
        EXPECT_TRUE(grid.set_row_height(row_height_strategy_fill));
        grid.set_padding(10, 20, 30, 40);
        grid.set_row_gap(5);
        EXPECT_TRUE(grid.calc_layout(5)); 
        EXPECT_DOUBLE_EQ(grid.get_height(), 5.0);

        auto row_height = grid.get_row_height();
        EXPECT_TRUE(row_height.size() == 2);
        EXPECT_DOUBLE_EQ(row_height[0], 0.0);
        EXPECT_DOUBLE_EQ(row_height[1], 0.0);     

        auto row_start = grid.get_row_start();
        EXPECT_TRUE(row_start.size() == 2);
        EXPECT_DOUBLE_EQ(row_start[0], 10.0);
        EXPECT_DOUBLE_EQ(row_start[1], 15.0);          
    }
}

TEST(GridLayoutRowInfo, RowHeightStrategyFit)
{
    // 默认情况
    {
        grid_layout grid(5, 2);
        EXPECT_TRUE(grid.set_row_height(row_height_strategy_fit));
        EXPECT_TRUE(grid.calc_layout());
        EXPECT_DOUBLE_EQ(grid.get_height(), 2.0);
    }

    // 定高
    {
        grid_layout grid(5, 2);
        EXPECT_TRUE(grid.set_row_height(row_height_strategy_fit));
        EXPECT_TRUE(grid.calc_layout(100));
        EXPECT_DOUBLE_EQ(grid.get_height(), 100.0);
    }    

    // 设置基准高度
    {
        grid_layout grid(5, 2);
        grid.set_base_height(10.0);
        EXPECT_TRUE(grid.set_row_height(row_height_strategy_fit));
        EXPECT_TRUE(grid.calc_layout());
        EXPECT_DOUBLE_EQ(grid.get_height(), 20.0);     

        auto row_height = grid.get_row_height();
        EXPECT_TRUE(row_height.size() == 2);
        EXPECT_DOUBLE_EQ(row_height[0], grid.get_base_height());
        EXPECT_DOUBLE_EQ(row_height[1], grid.get_base_height());

        auto row_start = grid.get_row_start();
        EXPECT_TRUE(row_start.size() == 2);
        EXPECT_DOUBLE_EQ(row_start[0], 0);
        EXPECT_DOUBLE_EQ(row_start[1], grid.get_base_height());
    }

    // 第一行: 两个能确定高度的项 + 一个百分比项
    // 第二行: 两个百分比项
    // 第三行: 空行
    // 同时设置 padding 和 gap
    // 分别测试 高度自适应 + 固定高度
    {
        grid_layout grid(3, 3);

        // (0, 0)
        grid_layout::p_node node(new grid_item);
        node->set_item_pos_strategy(item_pos_strategy_auto);
        node->set_row_span(1);
        node->set_column_span(1);
        node->set_height(length_unit::length_unit_point, 10);
        EXPECT_TRUE(grid.add_child(node));

        // (0, 1)
        node.reset(new grid_item);
        node->set_item_pos_strategy(item_pos_strategy_auto);
        node->set_row_span(1);
        node->set_column_span(1);
        node->set_height(length_unit::length_unit_point, 20);
        EXPECT_TRUE(grid.add_child(node));     

        // (0, 2)
        node.reset(new grid_item);
        node->set_item_pos_strategy(item_pos_strategy_auto);
        node->set_row_span(1);
        node->set_column_span(1);
        node->set_height(length_unit::length_unit_percent, 50);
        EXPECT_TRUE(grid.add_child(node));  

        // (1, 0)
        node.reset(new grid_item);
        node->set_item_pos_strategy(item_pos_strategy_auto);
        node->set_row_span(1);
        node->set_column_span(1);
        node->set_height(length_unit::length_unit_percent, 50);
        EXPECT_TRUE(grid.add_child(node));

        // (1, 1)
        node.reset(new grid_item);
        node->set_item_pos_strategy(item_pos_strategy_auto);
        node->set_row_span(1);
        node->set_column_span(1);
        node->set_height(length_unit::length_unit_percent, 60);
        EXPECT_TRUE(grid.add_child(node)); 

        grid.set_padding(10, 20, 30, 40);
        grid.set_row_gap(5);
        grid.set_base_height(50.0);
        grid.set_row_height(row_height_strategy_fit);
        EXPECT_TRUE(grid.calc_layout());
        EXPECT_DOUBLE_EQ(grid.get_height(), 10 + 20 + 5 + 30 + 5 + 50 + 30);     

        auto row_height = grid.get_row_height();
        EXPECT_TRUE(row_height.size() == 3);
        EXPECT_DOUBLE_EQ(row_height[0], 20.0);
        EXPECT_DOUBLE_EQ(row_height[1], 30.0);
        EXPECT_DOUBLE_EQ(row_height[2], 50.0);

        auto row_start = grid.get_row_start();
        EXPECT_TRUE(row_start.size() == 3);
        EXPECT_DOUBLE_EQ(row_start[0], 10);
        EXPECT_DOUBLE_EQ(row_start[1], 10 + 20 + 5);
        EXPECT_DOUBLE_EQ(row_start[2], 10 + 20 + 5 + 30 + 5);


        EXPECT_TRUE(grid.calc_layout(50));
        EXPECT_DOUBLE_EQ(grid.get_height(), 50);             
    }
}
