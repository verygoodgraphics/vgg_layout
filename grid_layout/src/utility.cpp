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

#include "./utility.h"

double deal_align_center(double padding_0, double padding_1, double content_len, double widget_len)
{
    padding_0 = insure_nonnegative(padding_0);
    padding_1 = insure_nonnegative(padding_1);
    content_len = insure_nonnegative(content_len);
    widget_len = insure_nonnegative(widget_len);

    auto dif = widget_len - content_len;

    if (dif >= padding_0 + padding_1)
    {
        return padding_0 + (dif - (padding_0 + padding_1)) / 2.0;
    }
    else if (dif >= 0 && dif < padding_0 + padding_1)
    {
        return (dif + padding_0 - padding_1) / 2.0;
    }
    else 
    {
        assert(dif < 0);
        return -(-dif + padding_0 - padding_1) / 2.0 + padding_0 - padding_1;
    }

    assert(false);
    return -1;
}