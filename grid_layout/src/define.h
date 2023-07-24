#ifndef VGG_GRID_DEFINE
#define VGG_GRID_DEFINE

#ifndef NDEBUG
#define SZN_DEBUG
#endif

// namespace vgg_grid_layout
// {
//     enum unit
//     {
//         unit_point,
//         unit_percent,   // 50 for 50%
//         unit_auto
//     };
// }

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

// enum row_height_strategy
// {
//     row_height_strategy_fill = 1,
//     row_height_strategy_fit = 2,
//     row_height_strategy_fix = 3
// };

enum item_pos_strategy
{
    item_pos_strategy_auto = 1,
    item_pos_strategy_fix = 2
};

#endif