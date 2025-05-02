project(hash)

add_executable(test_linked_hash_map linked_hash_map.cpp)
target_link_libraries(test_linked_hash_map PRIVATE doctest::doctest)
