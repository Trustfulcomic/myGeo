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
        GIT_TAG 5ff25322553c1870cf20a2e1ba6f20ed50d9fe9a # 3.2.6
    )
    FetchContent_MakeAvailable(wx)
endif()