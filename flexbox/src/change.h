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

#ifndef VGG_FLEXBOX_CHANGE
#define VGG_FLEXBOX_CHANGE

#include "yoga/Yoga.h"
#include "./define.h"
#include <map>

template<typename t_left, typename t_right>
class bimap
{
private:
    std::map<t_left, t_right> left_;
    std::map<t_right, t_left> right_;

public:
    auto left() const { return this->left_; }
    auto right() const { return this->right_; }

    void insert(const std::pair<t_left, t_right> &value)
    {
        left_.insert(value);
        right_.insert(std::make_pair(value.second, value.first));
    }

    bool empty() const
    {
        return left_.empty();
    }
};

class change
{
private:
    static bimap<direction, YGFlexDirection> change_direction_;
    static bimap<justify_content, YGJustify> change_justify_content_;
    static bimap<align_items, YGAlign> change_align_items_;
    static bimap<align_content, YGAlign> change_align_content_;
    static bimap<wrap, YGWrap> change_wrap_;
    static bimap<gap, YGGutter> change_gap_;
    static bimap<padding, YGEdge> change_padding_;
    static bimap<position, YGPositionType> change_position_;
    static bimap<ltrb, YGEdge> change_ltrb_;
    static bimap<overflow, YGOverflow> change_overflow_;
    
public:
    static void init();

public:
    #define interface(name, value) static const decltype(value) &name() { return change::value; } 

    interface(change_direction, change_direction_);
    interface(change_justify_content, change_justify_content_);
    interface(change_align_items, change_align_items_);
    interface(change_align_content, change_align_content_);
    interface(change_wrap, change_wrap_);
    interface(change_gap, change_gap_);
    interface(change_padding, change_padding_);
    interface(change_position, change_position_);
    interface(change_ltrb, change_ltrb_);
    interface(change_overflow, change_overflow_);
};

#endif