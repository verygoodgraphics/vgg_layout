#include <deque>
#include <algorithm>
#include <numeric>
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
    if (id == -1)
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

const grid_layout::p_node grid_layout::get_child(size_t id) const
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

void grid_layout::set_base_height(double value)
{
    assert(value >= 0);
    this->base_height_ = (std::max)(value, 0.0);
}

void grid_layout::set_row_gap(double value)
{
    assert(value >= 0);
    this->row_gap_ = (std::max)(value, 0.0);
}

// void grid_layout::set_column_gap(double value)
// {
//     assert(value >= 0);
//     this->column_gap_ = (std::max)(value, 0.0);
// }

void grid_layout::set_padding(double top, double right, double bottom, double left)
{
    assert(top >= 0.0);
    assert(right >= 0.0);
    assert(bottom >= 0.0);
    assert(left >= 0.0);
    
    this->padding_[0] = (std::max)(top, 0.0);
    this->padding_[1] = (std::max)(right, 0.0);
    this->padding_[2] = (std::max)(bottom, 0.0);
    this->padding_[3] = (std::max)(left, 0.0);
}

double grid_layout::get_height()
{
    if (!this->height_.has_value())
    {
        return -1;
    }

    return *this->height_;
}

bool grid_layout::calc_layout(optional<double> height)
{
    if (this->row_height_strategy_ == row_height_strategy_fill && !height.has_value())
    {
        // 行高度策略为 fill 时, 容器自身高度必须是一个固定值
        return false;
    }

    this->reset();
    height_ = height;
    
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

    return true;
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

















// bool grid_layout::set_expand_strategy(expand_strategy type, int min_row, int column_count)
// {


//     if (type == expand_strategy_auto)
//     {
//         if (!this->width_.has_value())
//         {
//             // 备注: 高度未知的情况下, 无法计算
//             return false;
//         }
        
        
//     }


//     // expand_strategy_ = type;
//     // row_count_ = min_row;
//     // column_count = column_count;
//     return false;
// }






















// 调试
#include <iostream>
#include <string>
// 调试


/*





*/

// void grid_layout::calc()
// {
    /*
    1.计算行列数, 同时放置所有子项
        A.auto: 列数 = width / column_width
        B.fix-column: 列数固定, 自动扩展行数
            a.放置所有指定行列的 node
            b. grid_auto_flow
                row: 在空的 cell 中, 按顺序排列 node
                row dense: 放置 node 前, 先查询已排列的空间中是否有足够的地方放置 node, 若无则继续按顺序排列
    
    2.根据行高的策略, 计算每个行的高度
    3.结合 gap padding cell_align row_align 行高总和 行宽总和, 放置 cells
    4.可以得到每个 cell 的位置
    5.根据每个子项的 top right bottom left width height 对齐方式, 获取子项在 cell 中的位置信息
    6.得到每个子项相对于父项的绝对位置信息
    */
    
    /*
    this->column_width_ = 100;
    this->column_count_ = 5;
    this->row_count_ = 3;


    auto print = [this](int row_count, int column_count)
    {
        for (int i = 0; i < row_count; ++i)
        {
            for (int j = 0; j < column_count; ++j)
            {
                try 
                {
                    if (this->cells_.at(i).at(j))
                    {
                        std::cout << "\t" << std::to_string(this->cells_.at(i).at(j)->n_id_) << "\t";
                    }
                }
                catch(...)
                {
                    std::cout << "\t0\t";
                }
            }
            std::cout << std::endl;
        }
    };

    p_node node(new grid_item);
    node->item_pos_strategy_ = item_pos_strategy_auto;
    node->column_span_ = 2;
    node->row_span_ = 2;
    node->n_id_ = 1;
    this->nodes_.emplace_back(node);

    node.reset(new grid_item);
    node->item_pos_strategy_ = item_pos_strategy_fix;
    node->row_id_ = 2;
    node->column_id_ = 0;
    node->n_id_ = 2;
    this->nodes_.emplace_back(node);

    node.reset(new grid_item);
    node->item_pos_strategy_ = item_pos_strategy_auto;
    node->column_span_ = 1;
    node->row_span_ = 1;
    node->n_id_ = 3;
    this->nodes_.emplace_back(node);

    node.reset(new grid_item);
    node->item_pos_strategy_ = item_pos_strategy_auto;
    node->column_span_ = 3;
    node->row_span_ = 1;
    node->n_id_ = 4;
    this->nodes_.emplace_back(node);    

    node.reset(new grid_item);
    node->item_pos_strategy_ = item_pos_strategy_auto;
    node->column_span_ = 1;
    node->row_span_ = 1;
    node->n_id_ = 5;
    this->nodes_.emplace_back(node);

    node.reset(new grid_item);
    node->item_pos_strategy_ = item_pos_strategy_auto;
    node->column_span_ = 3;
    node->row_span_ = 1;
    node->n_id_ = 6;
    this->nodes_.emplace_back(node);    

    node.reset(new grid_item);
    node->item_pos_strategy_ = item_pos_strategy_auto;
    node->column_span_ = 1;
    node->row_span_ = 1;
    node->n_id_ = 7;
    this->nodes_.emplace_back(node);    

    for (auto &item : this->nodes_)
    {
        if (item->item_pos_strategy_ == item_pos_strategy_fix)
        {
            take(item->row_id_, item->column_id_, item->row_span_, item->column_span_, item);
        }
    }
    this->nodes_.remove_if([](p_node &item){ return item->item_pos_strategy_ == item_pos_strategy_fix; });
    
    // row dense
    if (true)
    {
        for (auto &item : this->nodes_)
        {
            auto tmp = item->n_id_;
            assert(item->item_pos_strategy_ == item_pos_strategy_auto);

            while (true)
            {
                auto re = get_free_pos(0, 0, item->row_span_, item->column_span_);
                if (!re)
                {
                    ++row_count_;
                }
                else 
                {
                    this->take(std::get<0>(*re), std::get<1>(*re), item->row_span_, item->column_span_, item);
                    break;
                }
            }
        }
        print(row_count_, column_count_);        
    }
    else 
    {
        // row
        int now_row = 0;
        for (auto &item : this->nodes_)
        {
            auto tmp = item->n_id_;
            assert(item->item_pos_strategy_ == item_pos_strategy_auto);

            while (true)
            {
                auto re = get_free_pos(now_row, 0, item->row_span_, item->column_span_);
                if (!re)
                {
                    ++row_count_;
                }
                else 
                {
                    this->take(std::get<0>(*re), std::get<1>(*re), item->row_span_, item->column_span_, item);

                    now_row = std::get<0>(*re);
                    break;
                }
            }
        }
        print(row_count_, column_count_);     
    }

    return;
*/    
//}