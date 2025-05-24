if (MINGW)
    # Обычно не нужно добавлять -mconsole вручную
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mconsole")
endif()

add_executable(vector_tests vector_test.cpp templates_vector.cpp)
target_link_libraries(vector_tests
    PRIVATE
    vector_list_lib
    GTest::gtest_main
)
set_target_properties(vector_tests PROPERTIES WIN32_EXECUTABLE OFF)
add_test(NAME test_vector COMMAND vector_tests)

add_executable(doubly_linked_list_tests doubly_linked_list_test.cpp templates_doubly_linked_list.cpp)
target_link_libraries(doubly_linked_list_tests
    PRIVATE
    vector_list_lib
    GTest::gtest_main
)
set_target_properties(doubly_linked_list_tests PROPERTIES WIN32_EXECUTABLE OFF)
add_test(NAME test_doubly_linked_list COMMAND doubly_linked_list_tests)
