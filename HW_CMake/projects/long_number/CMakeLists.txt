add_library(long_number_lib
    src/long_number.cpp
    src/long_number.hpp
)

target_include_directories(long_number_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(long_number_test
    test/test_long_number.cpp
)

target_link_libraries(long_number_test
    PRIVATE
    long_number_lib
    GTest::gtest_main
)

target_include_directories(long_number_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

enable_testing()

add_test(NAME long_number_test COMMAND long_number_test)
