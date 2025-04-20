project(linked_list)

# 每个文件之间都不需要有依赖，这样是最简单的，每个文件都可以是一个单独的可执行文件
# 也就是一个单独的doctest
add_executable(test_singly_linked_list singly_linked_list.cpp)
target_link_libraries(test_singly_linked_list PRIVATE doctest::doctest)

# no
# # Register with CTest for better test management
# include(CTest)
# add_test(NAME test_singly_linked_list COMMAND test_singly_linked_list)

add_executable(test_leetcode_21_merge_two_lists leetcode_21_merge_two_lists.cpp)
target_link_libraries(test_leetcode_21_merge_two_lists PRIVATE doctest::doctest)
