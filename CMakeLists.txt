cmake_minimum_required(VERSION 2.6)

project(v4l2ucp)
set(V4L2UCP_VERSION 2.0.1)

add_definitions(-Wall -DV4L2UCP_VERSION="${V4L2UCP_VERSION}")
find_package(Qt4 REQUIRED)
include( ${QT_USE_FILE} )

MESSAGE(STATUS "Looking for libv4l")
find_library(V4L2_LIBRARY v4l2)
if (V4L2_LIBRARY)
    MESSAGE(STATUS "Looking for libv4l - found")
else (V4L2_LIBRARY)
    MESSAGE(FATAL_ERROR "Looking for libv4l - not found.")
endif (V4L2_LIBRARY)

add_subdirectory(src)

set(DESKTOP_ENTRY v4l2ucp.desktop)
set(APP_ICON v4l2ucp.png)

add_custom_command(OUTPUT ${DESKTOP_ENTRY}
    COMMAND touch ${DESKTOP_ENTRY}
    COMMAND sh ${CMAKE_SOURCE_DIR}/v4l2ucp-desktop.sh ${CMAKE_INSTALL_PREFIX}
    >${DESKTOP_ENTRY}
    DEPENDS ${CMAKE_SOURCE_DIR}/v4l2ucp-desktop.sh
    COMMENT "Generating desktop entry file"
    )
add_custom_target(desktop-file ALL
    DEPENDS ${DESKTOP_ENTRY}
    )

install(FILES ${CMAKE_SOURCE_DIR}/${APP_ICON} DESTINATION share/icons)
install(FILES ${CMAKE_BINARY_DIR}/${DESKTOP_ENTRY} DESTINATION
    share/applications)
