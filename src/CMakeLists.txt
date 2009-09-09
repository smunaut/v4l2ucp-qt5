set(SOURCES mainWindow.cpp previewSettings.cpp v4l2controls.cpp v4l2ucp.cpp)
set(HEADERS mainWindow.h previewSettings.h v4l2controls.h)
set(UI_FILES previewSettings.ui)
qt4_wrap_cpp(MOC_SOURCES ${HEADERS})
qt4_wrap_ui(UI_HEADERS ${UI_FILES})
include_directories(${CMAKE_BINARY_DIR}/src)

add_executable(v4l2ucp ${SOURCES} ${MOC_SOURCES} ${UI_HEADERS})
target_link_libraries(v4l2ucp ${QT_LIBRARIES})

add_executable(v4l2ctrl v4l2ctrl.c)

install(TARGETS v4l2ucp v4l2ctrl DESTINATION bin)
