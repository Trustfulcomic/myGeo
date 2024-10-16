find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})
include_directories(.)

add_executable(myGeo ${SRCES})
target_sources(myGeo PRIVATE    main.cpp
                                myFrame.cpp
                                drawingCanvas.cpp)

add_subdirectory(geoObjects)
add_subdirectory(tools)

target_link_libraries(myGeo PRIVATE ${wxWidgets_LIBRARIES})