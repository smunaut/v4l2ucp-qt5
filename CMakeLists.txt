cmake_minimum_required(VERSION 2.6)

project(v4l2ucp)
set(V4L2UCP_VERSION 2.0.2)

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

install(FILES ${CMAKE_SOURCE_DIR}/${APP_ICON} DESTINATION share/icons)
install(FILES ${CMAKE_SOURCE_DIR}/${DESKTOP_ENTRY} DESTINATION share/applications)
