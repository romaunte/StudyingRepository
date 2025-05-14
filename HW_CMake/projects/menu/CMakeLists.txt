cmake_minimum_required(VERSION 3.10)

project(MenuProject)

add_executable(menu_app
    main.cpp
    menu_functions.cpp
    menu_items.cpp
)

target_include_directories(menu_app PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
