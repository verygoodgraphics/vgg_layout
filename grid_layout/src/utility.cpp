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