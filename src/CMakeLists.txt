set(SOURCES mainWindow.cpp v4l2controls.cpp v4l2ucp.cpp)
set(HEADERS mainWindow.h v4l2controls.h)
qt4_wrap_cpp(MOC_SOURCES ${HEADERS})
include_directories({CMAKE_BINARY_DIR})

add_executable(v4l2ucp ${SOURCES} ${MOC_SOURCES})
target_link_libraries(v4l2ucp ${QT_LIBRARIES})

add_executable(v4l2ctrl v4l2ctrl.c)

install(TARGETS v4l2ucp v4l2ctrl DESTINATION bin)
