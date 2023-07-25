#ifndef VGG_GRID_ITEM
#define VGG_GRID_ITEM

#include <stdint.h>
#include <cassert>
#include <tuple>
#include "./define.h"

using std::tuple;

class grid_item
{
public:
    void set_width(length_unit unit_value, double value);
    void set_min_width(length_unit unit_value, double value);
    void set_max_width(length_unit unit_value, double value);
    tuple<length_unit, double> get_width() const { return this->width_; }
    tuple<length_unit, double> get_min_width() const { return this->min_width_; }
    tuple<length_unit, double> get_max_width() const { return this->max_width_; }

    void set_height(length_unit unit_value, double value);
    void set_min_height(length_unit unit_value, double value);
    void set_max_height(length_unit unit_value, double value);
    tuple<length_unit, double> get_height() { return this->height_; }
    tuple<length_unit, double> get_min_height() { return this->min_height_; }
    tuple<length_unit, double> get_max_height() { return this->max_height_; }

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

private:
    tuple<length_unit, double> width_;
    tuple<length_unit, double> min_width_;
    tuple<length_unit, double> max_width_;

    tuple<length_unit, double> height_;
    tuple<length_unit, double> min_height_;
    tuple<length_unit, double> max_height_;

private:
    item_pos_strategy item_pos_strategy_ = item_pos_strategy_auto;
    uint32_t row_id_ = -1;
    uint32_t column_id_ = -1;

    uint32_t row_span_ = 1;
    uint32_t column_span_ = 1;

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
