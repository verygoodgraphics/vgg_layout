#ifndef VGG_GRID_ITEM
#define VGG_GRID_ITEM

#include <stdint.h>
#include <cassert>
#include <tuple>
#include <array>
#include <optional>
#include "./define.h"

using std::tuple;
using std::array;
using std::optional;

// 备注: 该类仅用于 relative 类型的子项, 且 relative 不用支持 top left right bottom
class grid_item
{
public:
    typedef tuple<length_unit, double> t_length;

public:    
    void set_width(t_length value);
    void set_min_width(optional<t_length> value = optional<t_length>());
    void set_max_width(optional<t_length> value = optional<t_length>());
    t_length get_width() const { return this->width_; }
    optional<t_length> get_min_width() const { return this->min_width_; }
    optional<t_length> get_max_width() const { return this->max_width_; }

    void set_height(t_length value);
    void set_min_height(optional<t_length> value = optional<t_length>());
    void set_max_height(optional<t_length> value = optional<t_length>());
    t_length get_height() { return this->height_; }
    optional<t_length> get_min_height() { return this->min_height_; }
    optional<t_length> get_max_height() { return this->max_height_; }

public:
    void set_item_pos_strategy(item_pos_strategy type) { this->item_pos_strategy_ = type; }
    item_pos_strategy get_item_pos_strategy() const { return this->item_pos_strategy_; }

    void set_row_id(uint32_t id) { this->row_id_ = id; }
    uint32_t get_row_id() const { return this->row_id_; }

    void set_column_id(uint32_t id) { this->column_id_ = id; }
    uint32_t get_column_id() const { return this->column_id_; }

    void set_row_span(uint32_t span) { this->row_span_ = span; }
    uint32_t get_row_span() const { return this->row_span_; }

    void set_column_span(uint32_t span) { this->column_span_ = span; }
    uint32_t get_column_span() const { return this->column_span_; }

    void set_horizontal_align(align value) { this->horizontal_align_ = value; }
    align get_horizontal_align() const { return this->horizontal_align_; }

    void set_vertical_align(align value) { this->vertical_align_ = value; }
    align get_vertical_align() const { return this->vertical_align_; }

public:
    /**
     * 计算子项的布局
     * 
     * @param cell_width 子项占据的单元格的总宽度(需考虑 column span)
     * @param cell_height 子项占据的单元格的总高度(需考虑 row span)
    */
    void calc_layout(double cell_width, double cell_height);

    // 获取当前项的布局信息 (返回的坐标基于当前子项所占据的单元格的左上角位置)
    double get_layout_left() const { return this->layout_left_; }
    double get_layout_top() const { return this->layout_top_; }
    double get_layout_width() const { return this->layout_width_; }
    double get_layout_height() const { return this->layout_height_; }

private:
    // 优先级(以宽度举例): min_width > max_width > width
    static double calc_length(const t_length &length,
        const optional<t_length> &min_length,
        const optional<t_length> &max_length,
        double parent_length);

private:
    t_length width_ = { length_unit::length_unit_percent, 100.0 };
    optional<t_length> min_width_;
    optional<t_length> max_width_;

    t_length height_ = { length_unit::length_unit_percent, 100.0 };
    optional<t_length> min_height_;
    optional<t_length> max_height_;

    double layout_left_ = -1;
    double layout_top_ = -1;
    double layout_width_ = -1;
    double layout_height_ = -1;

private:
    item_pos_strategy item_pos_strategy_ = item_pos_strategy_auto;
    uint32_t row_id_ = -1;
    uint32_t column_id_ = -1;

    uint32_t row_span_ = 1;
    uint32_t column_span_ = 1;

    align horizontal_align_ = align_start;
    align vertical_align_ = align_start;

#ifdef SZN_DEBUG
public:
    int id_ = 1;
#endif
};

/**
 * 判断两个子项占用的区域是否重叠
 * 
 * @return true: 重叠, false: 不重叠
 * @note 两个子项的 item_pos_strategy 都是 item_pos_strategy_fix 是判断的前提
*/
bool area_conflict(const grid_item &item1, const grid_item &item2);

#endif
