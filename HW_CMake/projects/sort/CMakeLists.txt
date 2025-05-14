add_library(sort_lib
    src/merge_sort.cpp
    src/merge.cpp
)

target_include_directories(sort_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(sort_test
    test/merge_sort_test.cpp
)

target_link_libraries(sort_test
    PRIVATE
    sort_lib
    gtest_main
)

target_include_directories(sort_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_test(NAME sort_test COMMAND sort_test)

