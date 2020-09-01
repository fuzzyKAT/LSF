cmake_minimum_required(VERSION 3.0.2)
set (PROJECT LSF)

project (${PROJECT})

set (HEADERS
	MainFrame.h
	TurtleGraph.h
	WordIterator.h)

set (SOURCES
	main.cpp
	MainFrame.cpp
	TurtleGraph.cpp)

add_executable (${PROJECT} ${HEADERS} ${SOURCES})

# find wxWidgets
find_package(wxWidgets COMPONENTS std gl REQUIRED)
include(${wxWidgets_USE_FILE})
# find OpenGL
find_package(OpenGL REQUIRED)
# link libraries
target_link_libraries(${PROJECT} ${wxWidgets_LIBRARIES} ${OPENGL_LIBRARIES})
