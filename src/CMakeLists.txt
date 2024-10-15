find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})
include_directories(.)

add_executable(myGeo ${SRCES})
target_sources(myGeo PRIVATE main.cpp)

target_link_libraries(myGeo PRIVATE ${wxWidgets_LIBRARIES})