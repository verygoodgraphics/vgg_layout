# MIT License

# Copyright (c) 2023 Very Good Graphics

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# 本文件仅用于测试, 测试所需的文件由 layout_test.cpp 中的测试用例生成

import tkinter

file_name = r'D:\work\project\github_project\vgg_layout_with_test\build\vgg_layout\grid_layout\Debug\out.txt'

# 创建窗口
window = tkinter.Tk()

with open(file_name) as f:
    lines = f.readlines()

if not lines:
    exit(0)    

root = lines.pop(0)
re = root.split(' ')

# 创建画布
canvas = tkinter.Canvas(window, width = float(re[2]), height = float(re[3]), bg = 'white')

id = 1

for line in lines:
    re = line.split(' ')

    left = float(re[0])
    top = float(re[1])
    width = float(re[2])
    height = float(re[3])

    # 绘制矩形
    canvas.create_rectangle(left, top, left + width, top + height, fill = 'red')
    canvas.create_text(left + width / 2, top + height / 2, text = str(id), font=("courier", 20))
    id = id + 1

# 包装画布
canvas.pack()

# 运行并显示窗口
window.mainloop()