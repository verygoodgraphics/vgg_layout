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

#ifndef VGG_FLEXBOX_NODE
#define VGG_FLEXBOX_NODE

#include <vector>
#include <memory>
#include <optional>
#include <tuple>
#include "./change.h"

using std::vector;
using std::shared_ptr;
using std::optional;
using std::tuple;

class flexbox_node
{
public:
    //flexbox_node(YGConfigRef config = nullptr);
    flexbox_node();
    ~flexbox_node();
    
public:    
    typedef shared_ptr<flexbox_node> p_node;

public:
    /**
     * 添加一个子项
     * 
     * @param child 被添加的子项
     * @param id 子项被添加的位置, -1 标识插入到末尾
     * 
     * @return 操作是否成功
    */
    bool add_child(p_node child, uint32_t id = -1);
    bool add_child(uint32_t id = -1);

    /**
     * 删除指定位置的子项
     * 
     * @param id 指定的索引
     * 
     * @return 被删除的子项, 可能是 p_node(nullptr), 此情况仅发生再 id 越界时
    */
    p_node remove_child(uint32_t id);

    /**
     * 获取指定索引处的子项
     * 
     * @param id 指定的索引
     * 
     * @return 返回 nullptr 标识失败, 否则返回指定子项
    */
    p_node get_child(uint32_t id) const;

    // 获取子项个数
    uint32_t child_count() const;

public:
    direction get_direction() const;
    void set_direction(direction value);

    justify_content get_justify_content() const;
    void set_justify_content(justify_content value);

    align_items get_align_items() const;
    void set_align_items(align_items value);

    align_content get_align_content() const;
    void set_align_content(align_content value);

    wrap get_wrap() const;
    void set_wrap(wrap value);

    float get_gap(gap value) const;
    void set_gap(gap gap_value, float value);

    float get_padding(padding value) const;
    void set_padding(padding padding_value, float value);

    position get_position() const;
    void set_position(position value);

    float get_ltrb(ltrb value) const;
    void set_ltrb(ltrb ltrb_value, float value);

    float get_grow() const;
    void set_grow(float value);

    overflow get_overflow() const;
    void set_overflow(overflow value);

    void set_width(unit unit_value, optional<float> value = optional<float>());
    void set_min_width(unit unit_value, optional<float> value = optional<float>());
    void set_max_width(unit unit_value, optional<float> value = optional<float>());
    tuple<unit, optional<float>> get_width();
    tuple<unit, optional<float>> get_min_width();
    tuple<unit, optional<float>> get_max_width();
    void set_height(unit unit_value, optional<float> value = optional<float>());
    void set_min_height(unit unit_value, optional<float> value = optional<float>());
    void set_max_height(unit unit_value, optional<float> value = optional<float>());
    tuple<unit, optional<float>> get_height();
    tuple<unit, optional<float>> get_min_height();
    tuple<unit, optional<float>> get_max_height();

public:
    /**
     * 1.根据设置的属性, 计算当前项及其子项的位置信息
     * 2.子项的坐标系是相对于父项的
     * 3.当调整了 node 及其子项的属性后, 必须重新调用该函数, 以获取最新的布局信息
    */
    void calc_layout();

public:
    // 获取当前项的布局信息
    float get_layout_left();
    float get_layout_top();
    float get_layout_width();
    float get_layout_height();

private:
    // node 自身
    YGNodeRef node_;
    
    // node 的子项
    vector<p_node> childs_;
};

#endif