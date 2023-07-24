#ifndef VGG_GRID_LAYOUT
#define VGG_GRID_LAYOUT

#include <memory>
#include <vector>
#include <list>
#include <map>
#include <optional>
#include <tuple>
#include <string>
#include <cassert>
#include "./grid_item.h"

using std::shared_ptr;
using std::vector;
using std::map;
using std::optional;
using std::tuple;
using std::string;

class grid_layout
{
public:
    typedef shared_ptr<grid_item> p_node;

public:
    // 用于计算 expand_strategy.strategy 是 auto 时, grid 具备的列数
    static uint32_t calc_column_count(double width, double column_width)
    {
        assert(width >= column_width && width > 0);
        return static_cast<uint32_t>(width / column_width);
    }

public:
    grid_layout(int column_count);

public:
    /**
     * 添加一个子项
     * 
     * @param child 被添加的子项
     * @param id 子项被添加的位置, -1 标识插入到末尾
     * 
     * @return 操作是否成功
     * 
     * @note 若子项指定了行列位置, 且该位置已经被其他子项占据, 则会失败
    */
    bool add_child(p_node &child, size_t id = -1);
    bool add_child(size_t id = -1);

    // 禁止直接修改已加入布局的子项, 防止其属性修改后产生冲突
    const p_node get_child(size_t id) const;

    // 删除指定的项, 并进行返回
    p_node remove_child(size_t id);

    void clear_child() { this->nodes_.clear(); }
    size_t get_child_count() const { return this->nodes_.size(); }

public:
    uint32_t get_row_count() const { return this->row_count_; }
    uint32_t get_column_count() const { return this->column_count_; }

    grid_auto_flow get_grid_auto_flow() const { return this->grid_auto_flow_; }
    void set_grid_auto_flow(grid_auto_flow type) { this->grid_auto_flow_ = type; }

    void calc_layout();

private:
    // 基于 grid 当前的行列数, 判断指定的区域是否可用
    bool is_free(uint32_t row, uint32_t column, uint32_t row_span, uint32_t column_span);

    // 基于 grid 当前的行列数, 获取从指定的行列开始, 首次具备可用 area(row_span * column_span) 的行列位置
    optional<tuple<uint32_t, uint32_t>> get_free_pos(
        uint32_t begin_row, uint32_t begin_column, uint32_t row_span, uint32_t column_span);

    // 占用指定区域
    void take_area(uint32_t row, uint32_t column, uint32_t row_span, uint32_t column_span, p_node &node);

    // 布局前, 对部分参数进行重置
    void reset();

#ifdef SZN_DEBUG
public:
    void print();
#endif

private:
    uint32_t column_count_ = -1;
    uint32_t row_count_ = 1;

    grid_auto_flow grid_auto_flow_ = grid_auto_flow_row_dense;

    // grid 的有序子项集合
    vector<p_node> nodes_;

    // grid 中单元格对象放置情况, 
    // 由于子项可占用多行多列, 所以会存在多个单元格指向同一个对象的情况
    std::map<uint32_t/*行 id*/, std::map<uint32_t/*列 id*/, p_node>> cells_;
};

#endif
