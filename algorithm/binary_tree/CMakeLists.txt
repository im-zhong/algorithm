project(binary_tree)

# 每个文件之间都不需要有依赖，这样是最简单的，每个文件都可以是一个单独的可执行文件
# 也就是一个单独的doctest
add_executable(test_binary_heap binary_heap.cpp)
target_link_libraries(test_binary_heap PRIVATE doctest::doctest)

# no
# # Register with CTest for better test management
# include(CTest)
# add_test(NAME test_binary_heap COMMAND test_binary_heap)
