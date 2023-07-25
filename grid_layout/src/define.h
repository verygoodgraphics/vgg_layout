#ifndef VGG_GRID_DEFINE
#define VGG_GRID_DEFINE

#ifndef NDEBUG
#define SZN_DEBUG
#endif

enum length_unit
{
    length_unit_point,
    length_unit_percent,   // 50 for 50%

    // 当项作为容器时, grid_layout 自身提供了计算自适应的功能
    // 当项作为元素时, 不支持自适应
    //length_unit_auto
};

enum grid_auto_flow
{
    grid_auto_flow_row = 1,
    grid_auto_flow_row_dense = 2
};

// enum expand_strategy
// {
//     expand_strategy_auto = 1,
//     expand_strategy_fix_column = 2
// };

// enum column_width_strategy
// {
//     column_width_strategy_min = 1,
//     column_width_strategy_fix = 2
// };

enum row_height_strategy
{
    row_height_strategy_fill = 1,
    row_height_strategy_fit = 2,
    row_height_strategy_fix = 3
};

// enum align
// {
//     align_start = 1,
//     align_center = 2,
//     align_end = 3
// };

enum item_pos_strategy
{
    item_pos_strategy_auto = 1,
    item_pos_strategy_fix = 2
};

#endif