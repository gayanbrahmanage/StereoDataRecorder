cmake_minimum_required(VERSION 2.8)
project(3DSLAM)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -pthread ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread ")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

set(CMAKE_CXX_FLAGS "${C_CXX_FLAGS_DEFAULT} ${CMAKE_CXX_FLAGS} --std=c++11")

# PCL #######################
find_package(PCL 1.8 REQUIRED)
include_directories(SYSTEM ${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

#GLAD setup
add_library("glad" "src/glad.c")
#include_directories("${PROJECT_SOURCE_DIR}/include/glad/glad")

 
# GLEW 
#sudo apt-get install libglew-dev
#find_package(GLEW REQUIRED)
#include_directories(${GLEW_INCLUDE_DIRS})
#link_libraries(${GLEW_LIBRARIES})


#find additional libraries GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS} ${GLFW_LIBRARIES} ${PROJECT_SOURCE_DIR}/include)
#sudo apt-get install freeglut3-dev
find_package(OpenGL REQUIRED)
#find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  
		     #${GLUT_INCLUDE_DIRS}
		     ${PROJECT_SOURCE_DIR}/eigen )

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

#imgui 
include_directories( ${PROJECT_SOURCE_DIR}/imgui )

#configuration for openCV
find_package( OpenCV REQUIRED )

#building main 

add_executable( main 
		src/main.cpp
		src/viz.cpp
		src/axes.cpp
		src/pose.cpp
		imgui/imgui.cpp
		imgui/imgui_draw.cpp 
		imgui/imgui_demo.cpp
		imgui/imgui_impl_glfw.cpp
		imgui/imgui_impl_opengl3.cpp
		imgui/imgui_widgets.cpp
		imgui/imgui_tables.cpp)

target_link_libraries(main 
			${OpenCV_LIBS}
			${GLFW_STATIC_LIBRARIES} 
			${OPENGL_LIBRARIES} 
			"glad")

add_executable( main_imgui
		src/main_imgui.cpp
		src/viz.cpp
		src/axes.cpp
		src/pose.cpp
		imgui/imgui.cpp
		imgui/imgui_draw.cpp 
		imgui/imgui_demo.cpp
		imgui/imgui_impl_glfw.cpp
		imgui/imgui_impl_opengl3.cpp
		imgui/imgui_widgets.cpp
		imgui/imgui_tables.cpp
		)

target_link_libraries(main_imgui 
			${OpenCV_LIBS}
			${GLFW_STATIC_LIBRARIES} 
			${OPENGL_LIBRARIES} 
			"glad")

