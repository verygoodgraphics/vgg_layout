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

#include "./change.h"

bimap<direction, YGFlexDirection> change::change_direction_;
bimap<justify_content, YGJustify> change::change_justify_content_;
bimap<align_items, YGAlign> change::change_align_items_;
bimap<align_content, YGAlign> change::change_align_content_;
bimap<wrap, YGWrap> change::change_wrap_;
bimap<gap, YGGutter> change::change_gap_;
bimap<padding, YGEdge> change::change_padding_;
bimap<position, YGPositionType> change::change_position_;
bimap<ltrb, YGEdge> change::change_ltrb_;
bimap<overflow, YGOverflow> change::change_overflow_;

void change::init()
{
    if (!change_direction_.empty())
    {
        return;
    }

    change_direction_.insert({direction_column, YGFlexDirectionColumn});
    change_direction_.insert({direction_row, YGFlexDirectionRow});

    change_justify_content_.insert({justify_content_flex_start, YGJustifyFlexStart});
    change_justify_content_.insert({justify_content_center, YGJustifyCenter});
    change_justify_content_.insert({justify_content_flex_end, YGJustifyFlexEnd});
    change_justify_content_.insert({justify_content_space_between, YGJustifySpaceBetween});
    change_justify_content_.insert({justify_content_space_around, YGJustifySpaceAround});
    change_justify_content_.insert({justify_content_space_evenly, YGJustifySpaceEvenly});

    change_align_items_.insert({align_items_flex_start, YGAlignFlexStart});
    change_align_items_.insert({align_items_center, YGAlignCenter});
    change_align_items_.insert({align_items_flex_end, YGAlignFlexEnd});
    change_align_items_.insert({align_items_stretch, YGAlignStretch});

    change_align_content_.insert({align_content_flex_start, YGAlignFlexStart});
    change_align_content_.insert({align_content_center, YGAlignCenter});
    change_align_content_.insert({align_content_flex_end, YGAlignFlexEnd});

    change_wrap_.insert({wrap_wrap, YGWrapWrap});
    change_wrap_.insert({wrap_no_wrap, YGWrapNoWrap});

    change_gap_.insert({gap_column, YGGutterColumn});
    change_gap_.insert({gap_row, YGGutterRow});
    //change_gap_.insert({gap_all, YGGutterAll});
    
    change_padding_.insert({padding_left, YGEdgeLeft});
    change_padding_.insert({padding_top, YGEdgeTop});
    change_padding_.insert({padding_right, YGEdgeRight});
    change_padding_.insert({padding_bottom, YGEdgeBottom});

    //szn_todo position_sticky position_fixed
    change_position_.insert({position_absolute, YGPositionTypeAbsolute});
    change_position_.insert({position_relative, YGPositionTypeRelative});

    change_ltrb_.insert({ltrb_left, YGEdgeLeft});
    change_ltrb_.insert({ltrb_top, YGEdgeTop});
    change_ltrb_.insert({ltrb_right, YGEdgeRight});
    change_ltrb_.insert({ltrb_bottom, YGEdgeBottom});

    change_overflow_.insert({overflow_visible, YGOverflowVisible});
    change_overflow_.insert({overflow_hidden, YGOverflowHidden});
    change_overflow_.insert({overflow_scroll, YGOverflowScroll});
}