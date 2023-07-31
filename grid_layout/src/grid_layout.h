#ifndef VGG_GRID_LAYOUT
#define VGG_GRID_LAYOUT

#include <memory>
#include <vector>
#include <list>
#include <map>
#include <string>
#include "./grid_item.h"

using std::shared_ptr;
using std::vector;
using std::map;
using std::string;

class grid_layout
{
public:
    typedef shared_ptr<grid_item> p_node;

public:
    // 用于计算 expand_strategy.strategy 是 auto 时, grid 具备的列数
    // 备注:  
    //      1.expand_strategy.strategy 是 auto 时, column_width.strategy 值应为 fix (否则有逻辑依赖问题)
    //      2.因为本类不支持显示设置 expand_strategy.strategy, 所以第一点约束由使用者自行保证
    static uint32_t calc_column_count(double width, double column_width)
    {
        assert(width >= column_width && width > 0);
        return static_cast<uint32_t>(width / column_width);
    }

public:
    grid_layout(uint32_t column_count, optional<uint32_t> min_row = optional<uint32_t>());

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
    // grid_layout_yaml.grid_auto_flow
    grid_auto_flow get_grid_auto_flow() const { return this->grid_auto_flow_; }
    void set_grid_auto_flow(grid_auto_flow type) { this->grid_auto_flow_ = type; }

    // grid_layout_yaml.row_height
    // 备注: 仅在 row_height_strategy 为 row_height_strategy_fix 时, 才需要设置 fixed_value
    bool set_row_height(row_height_strategy type, optional<double> fixed_value = optional<double>());
    tuple<row_height_strategy, optional<double>> get_row_height() { return std::make_tuple(row_height_strategy_, row_height_fixed_value_); }

    // grid_layout_yaml.column_width
    void set_column_width(column_width_strategy type, double value);
    tuple<column_width_strategy, double> get_column_width() const;

    // grid_layout_yaml.base_height
    void set_base_height(double value);
    double get_base_height() const { return this->base_height_; }

    // grid_layout_yaml.row_gap
    void set_row_gap(double value);
    double get_row_gap() const { return this->row_gap_; }

    // grid_layout_yaml.column_gap
    void set_column_gap(double value);
    double get_column_gap() const { return this->column_gap_; }

    // grid_layout_yaml.cell_align
    void set_horizontal_align(align type) { this->horizontal_align_ = type; }
    align get_horizontal_align() const { return this->horizontal_align_; }

    // grid_layout_yaml.padding
    // [top right bottom left]
    std::array<double, 4> get_padding() { return this->padding_; }
    void set_padding(double top, double right, double bottom, double left);

public:
    uint32_t get_layout_row_count() const { return this->row_count_; }
    uint32_t get_layout_column_count() const { return this->column_count_; }

    const vector<double>& get_layout_row_height() const { return this->row_height_; }
    const vector<double>& get_layout_row_start() const { return this->row_start_; }

    double get_layout_column_width() const { return this->column_width_; }
    const vector<double>& get_layout_column_start() const { return this->column_start_; }

    double get_layout_height();
    double get_layout_width();

public:
    /**
     * 核心布局函数
     * 
     * @param height 非空则指明固定高度, 为空则标识高度自适应
     * @param width 非空则指明固定宽度, 为空则标识宽度自适应
     * 
     * @return 非空标识成功, 按序返回所有子项的布局信息: [top, left, width, height]
     * 
     * @note
     *  1.row_height_strategy 为 row_height_strategy_fill 和自身高度自适应互斥
     *  2.column_width_strategy 为 column_width_strategy_min 和自身宽度自适应互斥
     *  3.row_height_strategy 为 row_height_strategy_fit 时, 禁止有子项 row_span 大于 1
    */
    optional<vector<array<double, 4>>> calc_layout(optional<double> height = optional<double>(),
        optional<double> width = optional<double>());

#ifdef SZN_DEBUG
public:
    void print();
#endif

private:
    // 布局前, 对部分参数进行重置
    void reset();

private:
    // 基于 grid 当前的行列数, 判断指定的区域是否可用
    bool is_free(uint32_t row, uint32_t column, uint32_t row_span, uint32_t column_span);

    // 基于 grid 当前的行列数, 获取从指定的行列开始, 首次具备可用 area(row_span * column_span) 的行列位置
    optional<tuple<uint32_t, uint32_t>> get_free_pos(
        uint32_t begin_row, uint32_t begin_column, uint32_t row_span, uint32_t column_span);

    // 占用指定区域
    void take_area(uint32_t row, uint32_t column, uint32_t row_span, uint32_t column_span, p_node &node);

private:
    // 计算行高、行起始位置
    void calc_row_info();

    // 计算列宽、列起始位置
    void calc_column_info();

private:
    uint32_t column_count_ = -1;
    uint32_t row_count_ = 1;
    optional<uint32_t> min_row_;

    grid_auto_flow grid_auto_flow_ = grid_auto_flow_row_dense;

    row_height_strategy row_height_strategy_ = row_height_strategy_fix;
    optional<double> row_height_fixed_value_ = 1.0;

    column_width_strategy column_width_strategy_ = column_width_strategy_fix;
    double column_width_strategy_width_value_ = 1.0;

    // 非空则指明固定宽度, 为空则标识宽度自适应
    optional<double> width_;

    // 非空则指明固定高度, 为空则标识高度自适应
    optional<double> height_;

    align horizontal_align_ = align_start;

    double row_gap_ = 0;
    double column_gap_ = 0;

    // padding [top right bottom left]
    std::array<double, 4> padding_ = {};

    // 每行高度
    vector<double> row_height_;

    // 每行的起始位置
    vector<double> row_start_;

    // 每列宽度
    double column_width_ = -1;

    // 每列的起始位置
    vector<double> column_start_;

    // 用于行高计算循环依赖或该行是空行时, 使用该值作为基准值
    double base_height_ = 1.0;

    // grid 的有序子项集合
    vector<p_node> nodes_;

    // grid 中单元格对象放置情况, 
    // 由于子项可占用多行多列, 所以会存在多个单元格指向同一个对象的情况
    std::map<uint32_t/*行 id*/, std::map<uint32_t/*列 id*/, p_node>> cells_;
};

#endif
