#include <fstream>
#include "gtest/gtest.h"
#include "grid_layout.h"

using std::ofstream;

void write_node_info(const double value[4], string &str)
{
    double tmp[4] = {};
    memcpy(tmp, value, 32);
    std::swap(tmp[0], tmp[1]);

	char buf[1024] = {};
	for (int i = 0; i < 4; ++i)
	{
		sprintf(buf, "%.2lf", tmp[i]);
        str += buf;
        str += " ";
	}
    str += "\n";
}

// 备注: 生成的文件, 可用 vgg_layout/flexbox/test/show_rect.py 进行可视化
string print_node(double self_width, double self_height, const vector<array<double, 4>> &child)
{
	ofstream ofs("out.txt");
    if (!ofs.is_open())
    {
        assert(false);
        return "";
    }
    
    string str;

    double value[4] = { 0, 0, self_width, self_height };
    write_node_info(value, str);

    for (auto &item : child)
    {
        write_node_info(item.data(), str);
    }

    ofs.write(str.c_str(), str.size());
    return str;
}

TEST(ExpandStrategy, Ins)
{
    grid_layout grid(3, 3);
    grid.set_base_height(100);
    grid.set_row_height(row_height_strategy::row_height_strategy_fit);
    grid.set_column_width(column_width_strategy::column_width_strategy_fix, 50);
    grid.set_padding(10, 20, 30, 40);
    grid.set_row_gap(5);
    grid.set_column_gap(10);

    auto result = grid.calc_layout();
    EXPECT_TRUE(result);
    print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    EXPECT_DOUBLE_EQ(grid.get_layout_width(), 230);
    EXPECT_DOUBLE_EQ(grid.get_layout_height(), 350);
}

TEST(ColumnWidth, FixSpaceEnough)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 50);
    grid.set_row_height(row_height_strategy_fix, 50);
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }

    auto result = grid.calc_layout(optional<double>(), 200);
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp = "0.00 0.00 200.00 100.00 \n"
                 "0.00 0.00 50.00 50.00 \n"
                 "50.00 0.00 50.00 50.00 \n"
                 "0.00 50.00 50.00 50.00 \n"
                 "50.00 50.00 50.00 50.00 \n";
    EXPECT_TRUE(str == cmp);
}

TEST(ColumnWidth, FixSpaceNotEnough)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 50);
    grid.set_row_height(row_height_strategy_fix, 50);
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }

    auto result = grid.calc_layout(optional<double>(), 80);
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp = "0.00 0.00 80.00 100.00 \n"
                 "0.00 0.00 50.00 50.00 \n"
                 "50.00 0.00 50.00 50.00 \n"
                 "0.00 50.00 50.00 50.00 \n"
                 "50.00 50.00 50.00 50.00 \n";
    EXPECT_TRUE(str == cmp);
}

TEST(ColumnWidth, MinSpaceEnough)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_min, 50);
    grid.set_row_height(row_height_strategy_fix, 50);
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }

    auto result = grid.calc_layout(optional<double>(), 200);
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp = "0.00 0.00 200.00 100.00 \n"
                 "0.00 0.00 100.00 50.00 \n"
                 "100.00 0.00 100.00 50.00 \n"
                 "0.00 50.00 100.00 50.00 \n"
                 "100.00 50.00 100.00 50.00 \n";
    EXPECT_TRUE(str == cmp);
}

TEST(ColumnWidth, MinSpaceNotEnough)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_min, 50);
    grid.set_row_height(row_height_strategy_fix, 50);
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }

    auto result = grid.calc_layout(optional<double>(), 80);
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp = "0.00 0.00 80.00 100.00 \n"
                 "0.00 0.00 50.00 50.00 \n"
                 "50.00 0.00 50.00 50.00 \n"
                 "0.00 50.00 50.00 50.00 \n"
                 "50.00 50.00 50.00 50.00 \n";
    EXPECT_TRUE(str == cmp);
}

TEST(RowHeight, Fill)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 50);
    grid.set_row_height(row_height_strategy_fill);
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }

    EXPECT_TRUE(!grid.calc_layout());

    auto result = grid.calc_layout(500);
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 100.00 500.00 \n"
                "0.00 0.00 50.00 250.00 \n"
                "50.00 0.00 50.00 250.00 \n"
                "0.00 250.00 50.00 250.00 \n"
                "50.00 250.00 50.00 250.00 \n";
    EXPECT_TRUE(str == cmp);
}

TEST(RowHeight, Fit)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 100);
    grid.set_row_height(row_height_strategy_fit);
    grid.set_base_height(50);
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }
    grid.get_child(1)->set_height(std::make_tuple(length_unit::length_unit_percent, 50));
    grid.get_child(3)->set_height(std::make_tuple(length_unit::length_unit_point, 100));

    auto result = grid.calc_layout();
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 200.00 150.00 \n"
                "0.00 0.00 100.00 50.00 \n"
                "100.00 0.00 100.00 25.00 \n"
                "0.00 50.00 100.00 100.00 \n"
                "100.00 50.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);
}

TEST(RowHeight, Fix)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 100);
    EXPECT_TRUE(!grid.set_row_height(row_height_strategy_fix));
    EXPECT_TRUE(grid.set_row_height(row_height_strategy_fix, 100));
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }
    grid.get_child(1)->set_height(std::make_tuple(length_unit::length_unit_percent, 50));
    grid.get_child(3)->set_height(std::make_tuple(length_unit::length_unit_point, 60));

    auto result = grid.calc_layout();
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 200.00 200.00 \n"
                "0.00 0.00 100.00 100.00 \n"
                "100.00 0.00 100.00 50.00 \n"
                "0.00 100.00 100.00 100.00 \n"
                "100.00 100.00 100.00 60.00 \n";

    EXPECT_TRUE(str == cmp);
}

TEST(RowHeight, ColumnGap)
{
    grid_layout grid(3);

    grid.set_column_width(column_width_strategy_fix, 100);
    grid.set_row_height(row_height_strategy_fix, 100);
    grid.set_column_gap(10);
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }

    auto result = grid.calc_layout();
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 320.00 200.00 \n"
                "0.00 0.00 100.00 100.00 \n"
                "110.00 0.00 100.00 100.00 \n"
                "220.00 0.00 100.00 100.00 \n"
                "0.00 100.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);
}

TEST(RowHeight, RowGap)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 100);
    grid.set_row_height(row_height_strategy_fix, 100);
    grid.set_row_gap(10);
    
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }

    auto result = grid.calc_layout();
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 200.00 320.00 \n"
                "0.00 0.00 100.00 100.00 \n"
                "100.00 0.00 100.00 100.00 \n"
                "0.00 110.00 100.00 100.00 \n"
                "100.00 110.00 100.00 100.00 \n"
                "0.00 220.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);
}

TEST(RowHeight, GridAutoFlow)
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
    
    grid.set_column_width(column_width_strategy_fix, 100);
    grid.set_row_height(row_height_strategy_fix, 100);

    auto result = grid.calc_layout();
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 500.00 500.00 \n"
                "0.00 0.00 200.00 200.00 \n"
                "0.00 200.00 100.00 100.00 \n"
                "200.00 0.00 100.00 300.00 \n"
                "0.00 300.00 400.00 100.00 \n"
                "400.00 300.00 100.00 100.00 \n"
                "0.00 400.00 100.00 100.00 \n"
                "100.00 400.00 100.00 100.00 \n"
                "200.00 400.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);


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

    result = grid.calc_layout();
    EXPECT_TRUE(result);
    str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    cmp =
                "0.00 0.00 500.00 400.00 \n"
                "0.00 0.00 200.00 200.00 \n"
                "0.00 200.00 100.00 100.00 \n"
                "200.00 0.00 100.00 300.00 \n"
                "0.00 300.00 400.00 100.00 \n"
                "300.00 0.00 100.00 100.00 \n"
                "400.00 0.00 100.00 100.00 \n"
                "300.00 100.00 100.00 100.00 \n"
                "400.00 100.00 100.00 100.00 \n"
                "100.00 200.00 100.00 100.00 \n"
                "300.00 200.00 100.00 100.00 \n"
                "400.00 200.00 100.00 100.00 \n"
                "400.00 300.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);
}

TEST(RowHeight, Padding)
{
    grid_layout grid(3);

    grid.set_column_width(column_width_strategy_fix, 100);
    grid.set_row_height(row_height_strategy_fix, 100);
    grid.set_padding(10, 20, 40, 80);
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }

    auto result = grid.calc_layout();
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 400.00 250.00 \n"
                "80.00 10.00 100.00 100.00 \n"
                "180.00 10.00 100.00 100.00 \n"
                "280.00 10.00 100.00 100.00 \n"
                "80.00 110.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);
}

TEST(RowHeight, CellAlignStartEnoughSpace)
{
    grid_layout grid(3);

    grid.set_column_width(column_width_strategy_fix, 100);
    grid.set_row_height(row_height_strategy_fix, 100);
    grid.set_padding(10, 20, 30, 40);
    grid.set_column_gap(5);
    grid.set_horizontal_align(align_start);
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }

    auto result = grid.calc_layout(300, 500);
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 500.00 300.00 \n" 
                "40.00 10.00 100.00 100.00 \n" 
                "145.00 10.00 100.00 100.00 \n" 
                "250.00 10.00 100.00 100.00 \n" 
                "40.00 110.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);
}

TEST(RowHeight, CellAlignStartNotEnoughSpace)
{
    grid_layout grid(3);

    grid.set_column_width(column_width_strategy_fix, 100);
    grid.set_row_height(row_height_strategy_fix, 100);
    grid.set_padding(10, 20, 30, 40);
    grid.set_column_gap(5);
    grid.set_horizontal_align(align_start);
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }

    auto result = grid.calc_layout(300, 250);
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 250.00 300.00 \n" 
                "40.00 10.00 100.00 100.00 \n" 
                "145.00 10.00 100.00 100.00 \n" 
                "250.00 10.00 100.00 100.00 \n" 
                "40.00 110.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);
}

TEST(RowHeight, CellAlignEndEnoughSpace)
{
    grid_layout grid(3);

    grid.set_column_width(column_width_strategy_fix, 100);
    grid.set_row_height(row_height_strategy_fix, 100);
    grid.set_padding(10, 20, 30, 40);
    grid.set_column_gap(5);
    grid.set_horizontal_align(align_end);
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }

    auto result = grid.calc_layout(300, 500);
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 500.00 300.00 \n"
                "170.00 10.00 100.00 100.00 \n"
                "275.00 10.00 100.00 100.00 \n"
                "380.00 10.00 100.00 100.00 \n"
                "170.00 110.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);
}

TEST(RowHeight, CellAlignEndNotEnoughSpace)
{
    grid_layout grid(3);

    grid.set_column_width(column_width_strategy_fix, 100);
    grid.set_row_height(row_height_strategy_fix, 100);
    grid.set_padding(10, 20, 30, 40);
    grid.set_column_gap(5);
    grid.set_horizontal_align(align_end);
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }

    auto result = grid.calc_layout(300, 250);
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 250.00 300.00 \n"
                "-80.00 10.00 100.00 100.00 \n"
                "25.00 10.00 100.00 100.00 \n"
                "130.00 10.00 100.00 100.00 \n"
                "-80.00 110.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);
}

TEST(RowHeight, CellAlignCenterEnoughSpace)
{
    grid_layout grid(3);

    grid.set_column_width(column_width_strategy_fix, 100);
    grid.set_row_height(row_height_strategy_fix, 100);
    grid.set_padding(10, 20, 30, 40);
    grid.set_column_gap(5);
    grid.set_horizontal_align(align_center);
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }

    auto result = grid.calc_layout(300, 500);
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 500.00 300.00 \n"
                "105.00 10.00 100.00 100.00 \n"
                "210.00 10.00 100.00 100.00 \n"
                "315.00 10.00 100.00 100.00 \n"
                "105.00 110.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);
}

// 空间不足, 但是仍大于所需宽度之和(因为存在左右的 padding)
TEST(RowHeight, CellAlignCenterNotEnough)
{
    grid_layout grid(3);

    grid.set_column_width(column_width_strategy_fix, 100);
    grid.set_row_height(row_height_strategy_fix, 100);
    grid.set_padding(10, 20, 30, 40);
    grid.set_column_gap(5);
    grid.set_horizontal_align(align_center);
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }

    auto result = grid.calc_layout(300, 350);
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 350.00 300.00 \n"
                "30.00 10.00 100.00 100.00 \n"
                "135.00 10.00 100.00 100.00 \n"
                "240.00 10.00 100.00 100.00 \n"
                "30.00 110.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);
}

// 空间不足, 小于所需宽度之和
TEST(RowHeight, CellAlignCenterVeryNotEnough)
{
    grid_layout grid(3);

    grid.set_column_width(column_width_strategy_fix, 100);
    grid.set_row_height(row_height_strategy_fix, 100);
    grid.set_padding(10, 20, 30, 40);
    grid.set_column_gap(5);
    grid.set_horizontal_align(align_center);
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }

    auto result = grid.calc_layout(300, 250);
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 250.00 300.00 \n"
                "-20.00 10.00 100.00 100.00 \n"
                "85.00 10.00 100.00 100.00 \n"
                "190.00 10.00 100.00 100.00 \n"
                "-20.00 110.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);
}


TEST(ItemMinMaxHeight, OnlyMin)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 100);
    EXPECT_TRUE(grid.set_row_height(row_height_strategy_fix, 100));
    
    for (int i = 0; i < 3; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }
    grid.get_child(1)->set_height(std::make_tuple(length_unit::length_unit_point, 50));
    grid.get_child(1)->set_min_height(std::make_tuple(length_unit::length_unit_point, 120));

    auto result = grid.calc_layout();
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 200.00 200.00 \n"
                "0.00 0.00 100.00 100.00 \n"
                "100.00 0.00 100.00 120.00 \n"
                "0.00 100.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);    
}

TEST(ItemMinMaxHeight, OnlyMax)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 100);
    EXPECT_TRUE(grid.set_row_height(row_height_strategy_fix, 100));
    
    for (int i = 0; i < 3; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }
    grid.get_child(1)->set_height(std::make_tuple(length_unit::length_unit_point, 50));
    grid.get_child(1)->set_max_height(std::make_tuple(length_unit::length_unit_point, 25));

    auto result = grid.calc_layout();
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 200.00 200.00 \n"
                "0.00 0.00 100.00 100.00 \n"
                "100.00 0.00 100.00 25.00 \n"
                "0.00 100.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);    
}

TEST(ItemMinMaxHeight, MinMax)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 100);
    EXPECT_TRUE(grid.set_row_height(row_height_strategy_fix, 100));
    
    for (int i = 0; i < 3; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }
    grid.get_child(1)->set_height(std::make_tuple(length_unit::length_unit_point, 1024));
    grid.get_child(1)->set_max_height(std::make_tuple(length_unit::length_unit_point, 25));
    grid.get_child(1)->set_min_height(std::make_tuple(length_unit::length_unit_point, 75));

    auto result = grid.calc_layout();
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
        "0.00 0.00 200.00 200.00 \n"
        "0.00 0.00 100.00 100.00 \n"
        "100.00 0.00 100.00 75.00 \n"
        "0.00 100.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);    
}

TEST(ItemMinMaxWidth, OnlyMin)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 100);
    EXPECT_TRUE(grid.set_row_height(row_height_strategy_fix, 100));
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }
    grid.get_child(1)->set_width(std::make_tuple(length_unit::length_unit_point, 50));
    grid.get_child(1)->set_min_width(std::make_tuple(length_unit::length_unit_point, 120));

    auto result = grid.calc_layout(optional<double>(), 300);
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 300.00 200.00 \n"
                "0.00 0.00 100.00 100.00 \n"
                "100.00 0.00 120.00 100.00 \n"
                "0.00 100.00 100.00 100.00 \n"
                "100.00 100.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);    
}

TEST(ItemMinMaxWidth, OnlyMax)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 100);
    EXPECT_TRUE(grid.set_row_height(row_height_strategy_fix, 100));
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }
    grid.get_child(1)->set_width(std::make_tuple(length_unit::length_unit_point, 500));
    grid.get_child(1)->set_max_width(std::make_tuple(length_unit::length_unit_point, 70));

    auto result = grid.calc_layout(optional<double>(), 300);
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 300.00 200.00 \n"
                "0.00 0.00 100.00 100.00 \n"
                "100.00 0.00 70.00 100.00 \n"
                "0.00 100.00 100.00 100.00 \n"
                "100.00 100.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);    
}

TEST(ItemMinMaxWidth, MinMax)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 100);
    EXPECT_TRUE(grid.set_row_height(row_height_strategy_fix, 100));
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }
    grid.get_child(1)->set_width(std::make_tuple(length_unit::length_unit_point, 500));
    grid.get_child(1)->set_max_width(std::make_tuple(length_unit::length_unit_point, 70));
    grid.get_child(1)->set_min_width(std::make_tuple(length_unit::length_unit_point, 120));

    auto result = grid.calc_layout(optional<double>(), 300);
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 300.00 200.00 \n"
                "0.00 0.00 100.00 100.00 \n"
                "100.00 0.00 120.00 100.00 \n"
                "0.00 100.00 100.00 100.00 \n"
                "100.00 100.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);    
}

TEST(ItemRowAlign, Start)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 100);
    EXPECT_TRUE(grid.set_row_height(row_height_strategy_fix, 100));
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }
    grid.get_child(1)->set_width(std::make_tuple(length_unit::length_unit_point, 50));
    grid.get_child(1)->set_height(std::make_tuple(length_unit::length_unit_point, 50));
    grid.get_child(1)->set_horizontal_align(align_start);

    auto result = grid.calc_layout();
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 200.00 200.00 \n"
                "0.00 0.00 100.00 100.00 \n"
                "100.00 0.00 50.00 50.00 \n"
                "0.00 100.00 100.00 100.00 \n"
                "100.00 100.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);  
}

TEST(ItemRowAlign, Center)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 100);
    EXPECT_TRUE(grid.set_row_height(row_height_strategy_fix, 100));
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }
    grid.get_child(1)->set_width(std::make_tuple(length_unit::length_unit_point, 50));
    grid.get_child(1)->set_height(std::make_tuple(length_unit::length_unit_point, 50));
    grid.get_child(1)->set_horizontal_align(align_center);

    auto result = grid.calc_layout();
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 200.00 200.00 \n"
                "0.00 0.00 100.00 100.00 \n"
                "125.00 0.00 50.00 50.00 \n"
                "0.00 100.00 100.00 100.00 \n"
                "100.00 100.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);  
}

TEST(ItemRowAlign, End)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 100);
    EXPECT_TRUE(grid.set_row_height(row_height_strategy_fix, 100));
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }
    grid.get_child(1)->set_width(std::make_tuple(length_unit::length_unit_point, 50));
    grid.get_child(1)->set_height(std::make_tuple(length_unit::length_unit_point, 50));
    grid.get_child(1)->set_horizontal_align(align_end);

    auto result = grid.calc_layout();
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 200.00 200.00 \n"
                "0.00 0.00 100.00 100.00 \n"
                "150.00 0.00 50.00 50.00 \n"
                "0.00 100.00 100.00 100.00 \n"
                "100.00 100.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);  
}

TEST(ItemColumnAlign, Start)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 100);
    EXPECT_TRUE(grid.set_row_height(row_height_strategy_fix, 100));
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }
    grid.get_child(1)->set_width(std::make_tuple(length_unit::length_unit_point, 50));
    grid.get_child(1)->set_height(std::make_tuple(length_unit::length_unit_point, 50));
    grid.get_child(1)->set_vertical_align(align_start);

    auto result = grid.calc_layout();
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 200.00 200.00 \n"
                "0.00 0.00 100.00 100.00 \n"
                "100.00 0.00 50.00 50.00 \n"
                "0.00 100.00 100.00 100.00 \n"
                "100.00 100.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);  
}

TEST(ItemColumnAlign, Center)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 100);
    EXPECT_TRUE(grid.set_row_height(row_height_strategy_fix, 100));
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }
    grid.get_child(1)->set_width(std::make_tuple(length_unit::length_unit_point, 50));
    grid.get_child(1)->set_height(std::make_tuple(length_unit::length_unit_point, 50));
    grid.get_child(1)->set_vertical_align(align_center);

    auto result = grid.calc_layout();
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 200.00 200.00 \n"
                "0.00 0.00 100.00 100.00 \n"
                "100.00 25.00 50.00 50.00 \n"
                "0.00 100.00 100.00 100.00 \n"
                "100.00 100.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);  
}

TEST(ItemColumnAlign, End)
{
    grid_layout grid(2);

    grid.set_column_width(column_width_strategy_fix, 100);
    EXPECT_TRUE(grid.set_row_height(row_height_strategy_fix, 100));
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }
    grid.get_child(1)->set_width(std::make_tuple(length_unit::length_unit_point, 50));
    grid.get_child(1)->set_height(std::make_tuple(length_unit::length_unit_point, 50));
    grid.get_child(1)->set_vertical_align(align_end);

    auto result = grid.calc_layout();
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 200.00 200.00 \n"
                "0.00 0.00 100.00 100.00 \n"
                "100.00 50.00 50.00 50.00 \n"
                "0.00 100.00 100.00 100.00 \n"
                "100.00 100.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);  
}

TEST(ItemPos, Conflict)
{
    grid_layout grid(3);

    grid.set_column_width(column_width_strategy_fix, 100);
    EXPECT_TRUE(grid.set_row_height(row_height_strategy_fix, 100));
    
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(grid.add_child(-1));
    }
    grid.get_child(1)->set_item_pos_strategy(item_pos_strategy_fix);
    grid.get_child(1)->set_row_id(0);
    grid.get_child(1)->set_column_id(1);
    grid.get_child(1)->set_row_span(2);
    grid.get_child(1)->set_column_span(2);

    grid.get_child(2)->set_item_pos_strategy(item_pos_strategy_fix);
    grid.get_child(2)->set_row_id(1);
    grid.get_child(2)->set_column_id(1);
    grid.get_child(2)->set_row_span(2);
    grid.get_child(2)->set_column_span(1);

    auto result = grid.calc_layout();
    EXPECT_TRUE(result);
    auto str = print_node(grid.get_layout_width(), grid.get_layout_height(), *result);

    string cmp =
                "0.00 0.00 300.00 300.00 \n"
                "0.00 0.00 100.00 100.00 \n"
                "100.00 0.00 200.00 200.00 \n"
                "100.00 100.00 100.00 200.00 \n"
                "0.00 100.00 100.00 100.00 \n";

    EXPECT_TRUE(str == cmp);  
}
