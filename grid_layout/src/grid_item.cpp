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

#include "./grid_item.h"
#include "./utility.h"

void grid_item::set_width(t_length value)
{
    width_ = std::make_tuple(std::get<0>(value), insure_nonnegative(std::get<1>(value)));
}

void grid_item::set_min_width(optional<t_length> value)
{
    min_width_ = std::make_tuple(std::get<0>(*value), insure_nonnegative(std::get<1>(*value)));
}

void grid_item::set_max_width(optional<t_length> value)
{
    max_width_ = std::make_tuple(std::get<0>(*value), insure_nonnegative(std::get<1>(*value)));
}

void grid_item::set_height(t_length value)
{
    height_ = std::make_tuple(std::get<0>(value), insure_nonnegative(std::get<1>(value)));
}

void grid_item::set_min_height(optional<t_length> value)
{
    min_height_ = std::make_tuple(std::get<0>(*value), insure_nonnegative(std::get<1>(*value)));
}

void grid_item::set_max_height(optional<t_length> value)
{
    max_height_ = std::make_tuple(std::get<0>(*value), insure_nonnegative(std::get<1>(*value)));
}

double grid_item::calc_length(const t_length &length, const optional<t_length> &min_length,
    const optional<t_length> &max_length, double parent_length)
{
    auto get = [](const tuple<length_unit, double> &value, double parent_value)
    {
        auto unit_value = std::get<0>(value);
        auto real_value = std::get<1>(value);

        if (unit_value == length_unit::length_unit_percent)
        {
            return parent_value * real_value / 100.0;
        }
        
        assert(unit_value == length_unit::length_unit_point);
        return real_value;
    };

    auto value = get(length, parent_length);
    
    if (max_length)
    {
        value = (std::min)(value, get(*max_length, parent_length));
    }

    if (min_length)
    {
        value = (std::max)(value, get(*min_length, parent_length));
    }
    
    return value;
}

void grid_item::calc_layout(double cell_width, double cell_height)
{
    this->layout_width_ = grid_item::calc_length(this->width_, this->min_width_, this->max_width_, cell_width);
    this->layout_height_ = grid_item::calc_length(this->height_, this->min_height_, this->max_height_, cell_height);

    // 水平对齐
    if (this->horizontal_align_ == align_start)
    {
        this->layout_left_ = 0;
    }
    else if (this->horizontal_align_ == align_center)
    {
        this->layout_left_ = deal_align_center(0, 0, this->layout_width_, cell_width);
    }
    else 
    {
        assert(this->horizontal_align_ == align_end);
        this->layout_left_ = cell_width - this->layout_width_;
    }

    // 垂直对齐
    if (this->vertical_align_ == align_start)
    {
        this->layout_top_ = 0;
    }
    else if (this->vertical_align_ == align_center)
    {
        this->layout_top_ = deal_align_center(0, 0, this->layout_height_, cell_height);
    }
    else 
    {
        assert(this->vertical_align_ == align_end);
        this->layout_top_ = cell_height - this->layout_height_;
    }
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

    double center_row = std::abs((row_start_1 + row_end_1) / 2.0 - (row_start_2 + row_end_2) / 2.0);
    double half_sum_row = (item1.get_row_span() + item2.get_row_span()) / 2.0;

    double center_column = std::abs((column_start_1 + column_end_1) / 2.0 - (column_start_2 + column_end_2) / 2.0);
    double half_sum_column = (item1.get_column_span() + item2.get_column_span()) / 2.0;

    if (center_row < half_sum_row && center_column < half_sum_column)
    {
        return true;
    }
    
    return false;
}
