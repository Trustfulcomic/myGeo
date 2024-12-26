cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(myGeo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

if(DEFINED ENV_WX_CONFIG)
    set(ENV{WX_CONFIG} ${ENV_WX_CONFIG})
endif()

set(wxWidgets_USE_STATIC 1)

find_package(wxWidgets REQUIRED xml core base)

add_executable(main main.cpp)

target_sources(main PRIVATE     myFrame.cpp
                                drawingCanvas.cpp
                                nameHandler.cpp)
add_subdirectory(geoTransforms)
add_subdirectory(tools)
add_subdirectory(utils)
add_subdirectory(geoObjects)

include(${wxWidgets_USE_FILE})

target_link_libraries(main PRIVATE ${wxWidgets_LIBRARIES})