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

#include <deque>
#include <algorithm>
#include <numeric>
#include "./utility.h"
#include "./grid_layout.h"

using std::deque;

grid_layout::grid_layout(uint32_t column_count, optional<uint32_t> min_row)
 : column_count_(column_count), min_row_(min_row)
{
    if (!column_count_)
    {
        assert(false);
        column_count_ = 1;
    }
}

bool grid_layout::add_child(p_node &child, size_t id)
{
    if (id == static_cast<size_t>(-1))
    {
        id = this->nodes_.size();
    }

    if (id > this->nodes_.size())
    {
        return false;
    }

    if (child->get_item_pos_strategy() == item_pos_strategy_fix)
    {
        if (child->get_column_id() + child->get_column_span() > this->column_count_)
        {
            return false;
        }

        for (auto &item : this->nodes_)
        {
            if (area_conflict(*child, *item))
            {
                return false;
            }
        }
    }
    
    this->nodes_.insert(this->nodes_.begin() + id, child);
    return true;
}

bool grid_layout::add_child(size_t id)
{
    p_node child(new grid_item);
    return add_child(child, id);
}

grid_layout::p_node grid_layout::get_child(size_t id)
{
    if (id < this->nodes_.size())
    {
        return this->nodes_[id];
    }
    
    return p_node();
}

grid_layout::p_node grid_layout::remove_child(size_t id)
{
    if (id >= this->nodes_.size())
    {
        return p_node();
    }
    
    auto item = this->nodes_[id];
    this->nodes_.erase(this->nodes_.begin() + id);
    return item;
}

bool grid_layout::is_free(uint32_t row, uint32_t column, uint32_t row_span, uint32_t column_span)
{
    if (row + row_span > row_count_ || column + column_span > column_count_)
    {
        return false;
    }

    for (uint32_t i = row; i < row + row_span; ++i)
    {
        auto it = this->cells_.find(i);
        if (it == this->cells_.end())
        {
            continue;
        }

        for (uint32_t j = column; j < column + column_span; ++j)
        {
            if (it->second.find(j) != it->second.end())
            {
                return false;
            }
        }
    }

    return true;
}

optional<tuple<uint32_t, uint32_t>> grid_layout::get_free_pos(uint32_t begin_row, uint32_t begin_column, uint32_t row_span, uint32_t column_span)
{
    for (uint32_t i = begin_row; i < row_count_; ++i)
    {
        for (uint32_t j = begin_column; j < column_count_; ++j)
        {
            if (this->is_free(i, j, row_span, column_span))
            {
                return std::make_tuple(i, j);
            }
        }
    }

    return optional<tuple<uint32_t, uint32_t>>();
}

void grid_layout::take_area(uint32_t row, uint32_t column, uint32_t row_span, uint32_t column_span, p_node &node)
{
    for (uint32_t i = row; i < row + row_span; ++i)
    {
        for (uint32_t j = column; j < column + column_span; ++j)
        {
            this->cells_[i][j] = node;
        }
    }
}

bool grid_layout::set_row_height(row_height_strategy type, optional<double> fixed_value)
{
    if (type == row_height_strategy_fix && !fixed_value.has_value())
    {
        return false;
    }

    row_height_strategy_ = type;
    row_height_fixed_value_ = fixed_value;

    return true;
}

void grid_layout::set_column_width(column_width_strategy type, double value)
{
    this->column_width_strategy_ = type;
    this->column_width_strategy_width_value_ = insure_nonnegative(value);
}

tuple<column_width_strategy, double> grid_layout::get_column_width() const
{
    return std::make_tuple(this->column_width_strategy_, this->column_width_strategy_width_value_);
}

void grid_layout::set_base_height(double value)
{
    this->base_height_ = insure_nonnegative(value);
}

void grid_layout::set_row_gap(double value)
{
    this->row_gap_ = insure_nonnegative(value);
}

void grid_layout::set_column_gap(double value)
{
    this->column_gap_ = insure_nonnegative(value);
}

void grid_layout::set_padding(double top, double right, double bottom, double left)
{
    this->padding_[0] = insure_nonnegative(top);
    this->padding_[1] = insure_nonnegative(right);
    this->padding_[2] = insure_nonnegative(bottom);
    this->padding_[3] = insure_nonnegative(left);
}

double grid_layout::get_layout_height()
{
    if (!this->height_.has_value())
    {
        return -1;
    }

    return *this->height_;
}

double grid_layout::get_layout_width()
{
    if (!this->width_.has_value())
    {
        return -1;
    }

    return *this->width_;
}

optional<vector<array<double, 4>>> grid_layout::calc_layout(optional<double> height, optional<double> width)
{
    optional<vector<array<double, 4>>> result;

    if (this->row_height_strategy_ == row_height_strategy_fill && !height.has_value())
    {
        return result;
    }

    if (this->column_width_strategy_ == column_width_strategy_min && !width.has_value())
    {
        return result;
    }

    if (this->row_height_strategy_ == row_height_strategy_fit)
    {
        for (auto &item : this->nodes_)
        {
            if (item->get_row_span() > 1)
            {
                return result;
            }
        }
    }

    this->reset();
    height_ = height;
    width_ = width;
    
    std::deque<p_node> node_fixed;
    for (auto &item : this->nodes_)
    {
        if (item->get_item_pos_strategy() == item_pos_strategy_fix)
        {
            this->take_area(item->get_row_id(), item->get_column_id(), item->get_row_span(), item->get_column_span(), item);
            node_fixed.emplace_back(item);
        }
    }

    uint32_t now_row = 0;
    for (auto &item : this->nodes_)
    {
        if (!node_fixed.empty() && item == node_fixed.front())
        {
            node_fixed.pop_front();
            continue;
        }

        assert(item->get_item_pos_strategy() == item_pos_strategy_auto);

        while (true)
        {
            auto re = this->get_free_pos(this->grid_auto_flow_ == grid_auto_flow_row ? now_row : 0, 0,
                item->get_row_span(), item->get_column_span());

            if (!re)
            {
                ++this->row_count_;
                continue;
            }
            else 
            {
                auto row_id = std::get<0>(*re);
                auto column_id = std::get<1>(*re);
                this->take_area(row_id, column_id, item->get_row_span(), item->get_column_span(), item);
                item->set_row_id(row_id);
                item->set_column_id(column_id);
                now_row = row_id;
                break;
            }
        }
    }

    // 修正行数, 将 item_pos_strategy = item_pos_strategy_fix 的 node 纳入到最终布局中
    if (!this->cells_.empty())
    {
        auto last_row = this->cells_.rbegin()->first + 1;
        assert(this->row_count_ <= last_row);
        this->row_count_ = last_row;        
    }
    if (this->min_row_.has_value())
    {
        this->row_count_ = (std::max)(this->row_count_, *this->min_row_);
    }

    this->calc_row_info();
    this->calc_column_info();

    result = vector<array<double, 4>>();

    for (auto &item : this->nodes_)
    {
        auto row_id = item->get_row_id();
        auto column_id = item->get_column_id();

        item->calc_layout(this->column_width_ * item->get_column_span(), 
            std::accumulate(this->row_height_.begin() + row_id, this->row_height_.begin() + row_id + item->get_row_span(), 0.0));

        auto top = item->get_layout_top() + this->row_start_.at(row_id);
        auto left = item->get_layout_left() + this->column_start_.at(column_id);
        auto width = item->get_layout_width();
        auto height = item->get_layout_height();

        result->emplace_back(array<double, 4>{top, left, width, height});
    }

    return result;
}

void grid_layout::reset()
{
    this->row_count_ = 1;
    this->cells_.clear();
}

void grid_layout::calc_row_info()
{
    row_height_.resize(this->row_count_);
    row_start_.resize(this->row_count_);

    assert(row_gap_ >= 0.0 && row_count_ >= 1);
    const double total_gap = (row_count_ - 1) * row_gap_;

    // 获取每行高度
    if (this->row_height_strategy_ == row_height_strategy_fill)
    {
        assert(this->height_.has_value());
        
        double available_height = *this->height_ - padding_[0] - padding_[2] - total_gap;
        available_height = (std::max)(available_height, 0.0);

        std::fill(row_height_.begin(), row_height_.end(), available_height / row_count_);
    }
    else if (this->row_height_strategy_ == row_height_strategy_fix)
    {
        assert(this->row_height_fixed_value_.has_value());
        std::fill(row_height_.begin(), row_height_.end(), *row_height_fixed_value_);
    }
    else 
    {
        assert(this->row_height_strategy_ == row_height_strategy_fit);

        for (uint32_t i = 0; i < this->row_count_; ++i)
        {
            auto it = this->cells_.find(i);

            if (it == this->cells_.end())
            {
                // 空行
                this->row_height_.at(i) = this->base_height_;
            }
            else
            {
                double max_height = -1;
                double max_percent = -1;

                for (auto &node : it->second)
                {
                    auto node_height = node.second->get_height();
                    if (std::get<0>(node_height) == length_unit::length_unit_point)
                    {
                        max_height = (std::max)(max_height, std::get<1>(node_height));
                    }
                    else 
                    {
                        max_percent = (std::max)(max_percent, std::get<1>(node_height));
                    }
                }

                if (max_height < 0)
                {
                    // 当前行的高度均为 length_unit_percent
                    this->row_height_.at(i) = max_percent * this->base_height_ / 100.0;
                }
                else 
                {
                    this->row_height_.at(i) = max_height;
                }
            }
        }
    }

    double total_row_height = std::accumulate(row_height_.begin(), row_height_.end(), 0.0);

    // 计算每行的起始位置
    double start = this->padding_[0];
    for (uint32_t i = 0; i < this->row_count_; ++i)
    {
        row_start_.at(i) = start;
        start += row_height_.at(i);
        start += this->row_gap_;
    }

    assert(this->row_height_.size() == this->row_count_);
    assert(this->row_start_.size() == this->row_count_);    

    // 设置自适应高度
    if (!this->height_.has_value())
    {
        this->height_ = total_row_height + total_gap + padding_[0] + padding_[2];
    }
}

void grid_layout::calc_column_info()
{    
    column_start_.resize(this->column_count_);

    assert(column_gap_ >= 0.0 && column_count_ >= 1);
    const double total_gap = (column_count_ - 1) * column_gap_;

    // 计算每列宽度
    if (this->column_width_strategy_ == column_width_strategy_fix)
    {
        this->column_width_ = this->column_width_strategy_width_value_;
    }
    else
    {
        assert(this->column_width_strategy_ == column_width_strategy_min);
        assert(this->width_.has_value());

        double available_width = *this->width_ - (padding_[1] + padding_[3] + total_gap);
        this->column_width_ = (std::max)(available_width / this->column_count_, this->column_width_strategy_width_value_);
    }

    // 列宽总和(含 gap)
    double sum_of_column = this->column_width_ * this->column_count_ + total_gap;
    
    // 宽度自适应
    if (!this->width_.has_value())
    {
        this->width_ = sum_of_column + padding_[3] + padding_[1];
    }

    // 对齐
    if (this->horizontal_align_ == align_start || this->horizontal_align_ == align_center)
    {
        double start = this->horizontal_align_ == align_start ? this->padding_[3] : 
            deal_align_center(padding_[3], padding_[1], sum_of_column, *this->width_);
        for (uint32_t i = 0; i < this->column_count_; ++i)
        {
            this->column_start_.at(i) = start;
            start += this->column_width_;
            start += this->column_gap_;
        }
    }
    else
    {
        assert(this->horizontal_align_ == align_end);

        double start = *this->width_ - this->padding_[1];
        for (long long i = this->column_count_ - 1; i >= 0; --i)
        {
            start -= this->column_width_;
            this->column_start_.at(i) = start;
            start -= this->column_gap_;
        }
    }

    assert(this->column_start_.size() == this->column_count_);
}

#ifdef SZN_DEBUG
#include <fstream>
void grid_layout::print()
{
    std::ofstream ofs("out.txt");

    for (uint32_t i = 0; i < this->row_count_; ++i)
    {
        for (uint32_t j = 0; j < this->column_count_; ++j)
        {
            try 
            {
                string str = std::to_string(this->cells_.at(i).at(j)->id_);
                ofs.write("\t", 1);
                ofs.write(str.c_str(), str.size());
                ofs.write("\t", 1);
            }
            catch(...)
            {
                ofs.write("\t0\t", 3);
            }
        }

        ofs.write("\n", 1);
    }
}
#endif
