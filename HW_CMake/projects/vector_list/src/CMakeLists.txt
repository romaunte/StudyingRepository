target_sources(vector_list_lib
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/doubly_linked_list.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vector.cpp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/doubly_linked_list.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vector.hpp
)

target_include_directories(vector_list_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
