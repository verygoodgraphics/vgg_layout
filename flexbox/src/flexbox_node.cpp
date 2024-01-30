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

#include <functional>
#include "flexbox_node.h"

tuple<unit, optional<float>> get_length(YGNodeRef node, std::function<decltype(YGNodeStyleGetWidth)> fun)
{
    auto tem = fun(node);

    switch (tem.unit)
    {
        case YGUnitPoint:
        {
            return std::make_tuple(unit_point, tem.value);
        }
        case YGUnitPercent:
        {
            return std::make_tuple(unit_percent, tem.value);
        }
        case YGUnitAuto:
        {
            return std::make_tuple(unit_auto, optional<float>());
        }
        case YGUnitUndefined:
        {
            return std::make_tuple(unit_undefine, optional<float>());
        }
        default:
        {
            assert(false);
            break;
        }
    }

    return std::make_tuple(unit_undefine, optional<float>());
}

void set_length(YGNodeRef node, unit unit_value, optional<float> value, 
    std::function<decltype(YGNodeStyleSetWidth)> fun_point,
    std::function<decltype(YGNodeStyleSetWidthPercent)> fun_percent,
    std::function<decltype(YGNodeStyleSetWidthAuto)> fun_auto)
{
    switch (unit_value)
    {
        case unit_point:
        {
            assert(value);
            fun_point(node, *value);
            return;
        }
        case unit_percent:
        {
            assert(value);
            fun_percent(node, *value);
            return;
        }
        case unit_auto:
        {
            assert(fun_auto);
            fun_auto(node);
            return;
        }
        case unit_undefine:
        {
            fun_point(node, YGUndefined);
            return;
        }
        default:
        {
            assert(false);
            return;
        }
    }    
}

/*flexbox_node::flexbox_node(YGConfigRef config)
{
    change::init();

    if (config)
    {
        node_ = YGNodeNewWithConfig(config);
    }
    else 
    {
        node_ = YGNodeNew();
    }
}*/
flexbox_node::flexbox_node()
{
    change::init();
    node_ = YGNodeNew();
}

flexbox_node::~flexbox_node()
{
    // 备注: 先将子项全部移除, 此时子项的 owner 还是正确指向当前对象, 才能避免子项被多次 delete
    this->childs_.clear();
    if (!YGNodeGetOwner(this->node_))
    {
        YGNodeFreeRecursive(this->node_);
    }
}

bool flexbox_node::add_child(uint32_t id)
{
    p_node child(new flexbox_node);
    return this->add_child(child, id);
}

bool flexbox_node::add_child(p_node &child, uint32_t id)
{
    auto count = YGNodeGetChildCount(this->node_);
    if (id == static_cast<uint32_t>(-1))
    {
        id = count;
    }
    else if (count < id)
    {
        return false;
    }

    YGNodeInsertChild(this->node_, child->node_, id);
    childs_.insert(childs_.begin() + id, std::move(child));
    return true;
}

flexbox_node::p_node flexbox_node::remove_child(uint32_t id)
{
    auto child = this->get_child(id);
    if (!child)
    {
        return p_node(nullptr);
    }
    
    YGNodeRemoveChild(this->node_, child->node_);

    auto node = std::move(childs_.at(id));
    childs_.erase(childs_.begin() + id);
    return node;
}

flexbox_node *flexbox_node::get_child(uint32_t id) const
{
    if (id >= static_cast<uint32_t>(this->childs_.size()))
    {
        return nullptr;
    }
    
    return this->childs_.at(id).get();
}

uint32_t flexbox_node::child_count() const
{
    assert(YGNodeGetChildCount(this->node_) == static_cast<uint32_t>(this->childs_.size()));
    return static_cast<uint32_t>(this->childs_.size());
}

template<typename t_yoga_fun, typename t_change>
auto get_property(YGNodeRef node, t_yoga_fun fun_yoga, const t_change &change)
{
    return change.right().find(fun_yoga(node))->second;
}

template<typename t_yoga_fun, typename t_change, typename t_value>
auto get_property(YGNodeRef node, t_yoga_fun fun_yoga, const t_change &change, t_value value)
{
    return fun_yoga(node, change.left().find(value)->second);
}

direction flexbox_node::get_direction() const
{
    return get_property(this->node_, YGNodeStyleGetFlexDirection, change::change_direction());
}

justify_content flexbox_node::get_justify_content() const
{
    return get_property(this->node_, YGNodeStyleGetJustifyContent, change::change_justify_content());
}

align_items flexbox_node::get_align_items() const
{
    return get_property(this->node_, YGNodeStyleGetAlignItems, change::change_align_items());
}

align_self flexbox_node::get_align_self() const
{
    return get_property(this->node_, YGNodeStyleGetAlignSelf, change::change_align_items());
}

align_content flexbox_node::get_align_content() const
{
    return get_property(this->node_, YGNodeStyleGetAlignContent, change::change_align_content());
}

wrap flexbox_node::get_wrap() const
{
    return get_property(this->node_, YGNodeStyleGetFlexWrap, change::change_wrap());
}

float flexbox_node::get_gap(gap value) const
{
    return get_property(this->node_, YGNodeStyleGetGap, change::change_gap(), value);
}

float flexbox_node::get_padding(padding value) const
{
    auto re = get_property(this->node_, YGNodeStyleGetPadding, change::change_padding(), value);
    return re.value;
}

tuple<unit, optional<float>> flexbox_node::get_margin(margin value)
{
    auto edge = change::change_padding().left().find(value)->second;
    return get_length(this->node_, std::bind(YGNodeStyleGetMargin, std::placeholders::_1, edge));
}

position flexbox_node::get_position() const
{
    return get_property(this->node_, YGNodeStyleGetPositionType, change::change_position());
}

float flexbox_node::get_ltrb(ltrb value) const
{
    auto re = get_property(this->node_, YGNodeStyleGetPosition, change::change_ltrb(), value);
    return re.value;
}

float flexbox_node::get_grow() const
{
    return YGNodeStyleGetFlexGrow(this->node_);
}

float flexbox_node::get_shrink() const
{
    return YGNodeStyleGetFlexShrink(this->node_);
}

overflow flexbox_node::get_overflow() const
{
    return get_property(this->node_, YGNodeStyleGetOverflow, change::change_overflow());
}

template<typename t_yoga_fun, typename t_change, typename t_value>
void set_property(YGNodeRef node, t_yoga_fun fun_yoga, const t_change &change, t_value value)
{
    fun_yoga(node, change.left().find(value)->second);
}

template<typename t_yoga_fun, typename t_change, typename t_value, typename t_set_value>
void set_property(YGNodeRef node, t_yoga_fun fun_yoga, const t_change &change, t_value value, t_set_value set_value)
{
    fun_yoga(node, change.left().find(value)->second, set_value);
}

void flexbox_node::set_direction(direction value)
{
    set_property(this->node_, YGNodeStyleSetFlexDirection, change::change_direction(), value);
}

void flexbox_node::set_justify_content(justify_content value)
{
    set_property(this->node_, YGNodeStyleSetJustifyContent, change::change_justify_content(), value);
}

void flexbox_node::set_align_items(align_items value)
{
    set_property(this->node_, YGNodeStyleSetAlignItems, change::change_align_items(), value);
}

void flexbox_node::set_align_self(align_self value)
{
    set_property(this->node_, YGNodeStyleSetAlignSelf, change::change_align_items(), value);
}

void flexbox_node::set_align_content(align_content value)
{
    set_property(this->node_, YGNodeStyleSetAlignContent, change::change_align_content(), value);
}

void flexbox_node::set_wrap(wrap value)
{
    set_property(this->node_, YGNodeStyleSetFlexWrap, change::change_wrap(), value);
}

void flexbox_node::set_gap(gap gap_value, float value)
{
    set_property(this->node_, YGNodeStyleSetGap, change::change_gap(), gap_value, value);
}

void flexbox_node::set_padding(padding padding_value, float value)
{
    set_property(this->node_, YGNodeStyleSetPadding, change::change_padding(), padding_value, value);
}

void flexbox_node::set_margin(margin margin_value, unit unit_value, optional<float> value)
{
    auto edge = change::change_padding().left().find(margin_value)->second;
    set_length(this->node_, unit_value, value, 
        std::bind(YGNodeStyleSetMargin, std::placeholders::_1, edge, std::placeholders::_2),
        std::bind(YGNodeStyleSetMarginPercent, std::placeholders::_1, edge, std::placeholders::_2),
        std::bind(YGNodeStyleSetMarginAuto, std::placeholders::_1, edge));
}

void flexbox_node::set_position(position value)
{
    set_property(this->node_, YGNodeStyleSetPositionType, change::change_position(), value);
}

void flexbox_node::set_ltrb(ltrb ltrb_value, float value)
{
    set_property(this->node_, YGNodeStyleSetPosition, change::change_ltrb(), ltrb_value, value);
}

void flexbox_node::set_grow(float value)
{
    YGNodeStyleSetFlexGrow(this->node_, value);
}

void flexbox_node::set_shrink(float value)
{
    YGNodeStyleSetFlexShrink(this->node_, value);
}

void flexbox_node::set_overflow(overflow value)
{
    set_property(this->node_, YGNodeStyleSetOverflow, change::change_overflow(), value);
}

void flexbox_node::set_width(unit unit_value, optional<float> value)
{
    set_length(this->node_, unit_value, value, YGNodeStyleSetWidth, YGNodeStyleSetWidthPercent, YGNodeStyleSetWidthAuto);
}

void flexbox_node::set_min_width(unit unit_value, optional<float> value)
{
    set_length(this->node_, unit_value, value, YGNodeStyleSetMinWidth, YGNodeStyleSetMinWidthPercent, nullptr);
}

void flexbox_node::set_max_width(unit unit_value, optional<float> value)
{
    set_length(this->node_, unit_value, value, YGNodeStyleSetMaxWidth, YGNodeStyleSetMaxWidthPercent, nullptr);
}

void flexbox_node::set_height(unit unit_value, optional<float> value)
{
    set_length(this->node_, unit_value, value, YGNodeStyleSetHeight, YGNodeStyleSetHeightPercent, YGNodeStyleSetHeightAuto);
}

void flexbox_node::set_min_height(unit unit_value, optional<float> value)
{
    set_length(this->node_, unit_value, value, YGNodeStyleSetMinHeight, YGNodeStyleSetMinHeightPercent, nullptr);
}

void flexbox_node::set_max_height(unit unit_value, optional<float> value)
{
    set_length(this->node_, unit_value, value, YGNodeStyleSetMaxHeight, YGNodeStyleSetMaxHeightPercent, nullptr);
}

tuple<unit, optional<float>> flexbox_node::get_width()
{
    return get_length(this->node_, YGNodeStyleGetWidth);
}

tuple<unit, optional<float>> flexbox_node::get_min_width()
{
    return get_length(this->node_, YGNodeStyleGetMinWidth);
}

tuple<unit, optional<float>> flexbox_node::get_max_width()
{
    return get_length(this->node_, YGNodeStyleGetMaxWidth);
}

tuple<unit, optional<float>> flexbox_node::get_height()
{
    return get_length(this->node_, YGNodeStyleGetHeight);
}

tuple<unit, optional<float>> flexbox_node::get_min_height()
{
    return get_length(this->node_, YGNodeStyleGetMinHeight);
}

tuple<unit, optional<float>> flexbox_node::get_max_height()
{
    return get_length(this->node_, YGNodeStyleGetMaxHeight);
}

void flexbox_node::calc_layout()
{
    YGNodeCalculateLayout(this->node_, YGUndefined, YGUndefined, YGDirectionLTR);
}

float flexbox_node::get_layout_left()
{
    return YGNodeLayoutGetLeft(this->node_);
}

float flexbox_node::get_layout_top()
{
    return YGNodeLayoutGetTop(this->node_);
}

float flexbox_node::get_layout_width()
{
    return YGNodeLayoutGetWidth(this->node_);
}

float flexbox_node::get_layout_height()
{
    return YGNodeLayoutGetHeight(this->node_);
}