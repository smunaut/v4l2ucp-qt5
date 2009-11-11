set(SOURCES mainWindow.cpp previewSettings.cpp v4l2controls.cpp v4l2ucp.cpp)
set(HEADERS mainWindow.h previewSettings.h v4l2controls.h)
set(UI_FILES previewSettings.ui)
set(RCS v4l2ucp.qrc)
qt4_wrap_cpp(MOC_SOURCES ${HEADERS})
qt4_wrap_ui(UI_HEADERS ${UI_FILES})
qt4_add_resources(RC_SOURCES ${RCS})
include_directories(${CMAKE_BINARY_DIR}/src)

add_executable(v4l2ucp ${SOURCES} ${MOC_SOURCES} ${UI_HEADERS} ${RC_SOURCES})
target_link_libraries(v4l2ucp ${QT_LIBRARIES} ${V4L2_LIBRARY})

add_executable(v4l2ctrl v4l2ctrl.c)
target_link_libraries(v4l2ctrl ${V4L2_LIBRARY})

install(TARGETS v4l2ucp v4l2ctrl DESTINATION bin)
