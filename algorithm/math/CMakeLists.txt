project(math)

# executablename must start whih test_
add_executable(test_leetcode_263_ugly_number leetcode_263_ugly_number.cpp)
target_link_libraries(test_leetcode_263_ugly_number PRIVATE doctest::doctest)
