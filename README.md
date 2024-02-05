# vgg_layout
## flexbox使用示例

* 编写`CMakeLists.txt`

  ```cmake
  cmake_minimum_required(VERSION 3.0.0)
  project(szn_test VERSION 0.1.0)
  
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
  
  add_subdirectory(vgg_layout)
  add_executable(szn_test main.cpp)
  
  # flexbox 是需要 link 的库
  target_link_libraries(szn_test PRIVATE flexbox)
  ```

* 运行测试用例（可选）：`flexbox_text`

* `main.cpp`

  ```c++
  #include <iostream>
  #include "flexbox_node.h"
  
  int main()
  {
      flexbox_node node;
      node.add_child();
      node.add_child();
  
      std::cout << node.child_count() << std::endl;
      // 2
  }
  ```



## 测试

* 测试用例：运行`flexbox_test`
* 图形化展示：`vgg_layout/flexbox/test/show_rect.py`

