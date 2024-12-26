find_package(wxWidgets QUIET)
if (wxWidgets_FOUND)
    message(STATUS "Found preinstalled wxWidgets libraries at ${wxWidgets_LIBRARIES}")
    add_library(wxWidgets_external INTERFACE)
else()
    message(STATUS "Preinstalled wxWidgets not found.")
    message(STATUS "Will download and install wxWidgets in ${STAGED_INSTALL_PREFIX}")
    include(FetchContent)
    FetchContent_Declare(
        wx
        GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets
        GIT_TAG 7ad1bffa875f7a38db58b6069ea3ff0c49c211d2 # 3.1.7
    )
    FetchContent_MakeAvailable(wx)
endif()