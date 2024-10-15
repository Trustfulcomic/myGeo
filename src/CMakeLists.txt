find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})
include_directories(. geoObjects)

add_executable(myGeo ${SRCES})
target_sources(myGeo PRIVATE    main.cpp
                                myFrame.cpp
                                drawingCanvas.cpp
                                geoObjects/geoObject.cpp
                                geoObjects/geoPoint.cpp)

target_link_libraries(myGeo PRIVATE ${wxWidgets_LIBRARIES})