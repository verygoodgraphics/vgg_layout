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