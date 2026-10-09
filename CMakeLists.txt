cmake_minimum_required(VERSION 3.16)

project(BaudotApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(BaudotApp
    main.cpp
    mainwindow.cpp
    baudotencoder.cpp
    baudotdisplay.cpp
    baudotencoder.h
    baudotdisplay.h
    mainwindow.h
)

target_link_libraries(BaudotApp PRIVATE Qt6::Widgets)
