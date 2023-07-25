#include "grid_item.h"
#include <cmath>

void set_length(tuple<length_unit, double> &in, length_unit unit_value, double value)
{
    assert(value >= 0);
    in = std::make_tuple(unit_value, (std::max)(value, 0.0));
}

void grid_item::set_width(length_unit unit_value, double value)
{
    set_length(width_, unit_value, value);
}

void grid_item::set_min_width(length_unit unit_value, double value)
{
    set_length(min_width_, unit_value, value);
}

void grid_item::set_max_width(length_unit unit_value, double value)
{
    set_length(max_width_, unit_value, value);
}

void grid_item::set_height(length_unit unit_value, double value)
{
    set_length(height_, unit_value, value);
}

void grid_item::set_min_height(length_unit unit_value, double value)
{
    set_length(min_height_, unit_value, value);
}

void grid_item::set_max_height(length_unit unit_value, double value)
{
    set_length(max_height_, unit_value, value);
}

bool area_conflict(const grid_item &item1, const grid_item &item2)
{
    if (item1.get_item_pos_strategy() != item_pos_strategy_fix || item2.get_item_pos_strategy() != item_pos_strategy_fix)
    {
        return false;
    }

    long long row_start_1 = item1.get_row_id();
    long long row_end_1 = row_start_1 + item1.get_row_span();
    long long column_start_1 = item1.get_column_id();
    long long column_end_1 = column_start_1 + item1.get_column_span();

    long long row_start_2 = item2.get_row_id();
    long long row_end_2 = row_start_2 + item2.get_row_span();
    long long column_start_2 = item2.get_column_id();
    long long column_end_2 = column_start_2 + item2.get_column_span();    

    double center_row = abs((row_start_1 + row_end_1) / 2.0 - (row_start_2 + row_end_2) / 2.0);
    double half_sum_row = (item1.get_row_span() + item2.get_row_span()) / 2.0;

    double center_column = abs((column_start_1 + column_end_1) / 2.0 - (column_start_2 + column_end_2) / 2.0);
    double half_sum_column = (item1.get_column_span() + item2.get_column_span()) / 2.0;

    if (center_row < half_sum_row && center_column < half_sum_column)
    {
        return true;
    }
    
    return false;
}