cmake_minimum_required(VERSION 2.6)

project(v4l2ucp)
set(V4L2UCP_VERSION 2.0.0)

add_definitions(-Wall -DV4L2UCP_VERSION="${V4L2UCP_VERSION}")
find_package(Qt4 REQUIRED)
include( ${QT_USE_FILE} )

add_subdirectory(src)
