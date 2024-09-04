cmake_minimum_required(VERSION 2.8)
project(CSI3DEDGE)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)

if(COMPILER_SUPPORTS_CXX17)
   
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread ")
elseif(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -pthread ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread ")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

set(CMAKE_CXX_FLAGS "${C_CXX_FLAGS_DEFAULT} ${CMAKE_CXX_FLAGS} --std=c++11")


#GLAD setup
#add_library("glad" ${PROJECT_SOURCE_DIR}/3Dviz_image_studio/src/glad.c)
#include_directories(${PROJECT_SOURCE_DIR}/3Dviz_image_studio/include/glad)

include_directories(${PROJECT_SOURCE_DIR}/mavlink/c_library_v2-master 
		    ${PROJECT_SOURCE_DIR}/mavlink)

include_directories(${PROJECT_SOURCE_DIR}/3Dviz_image_studio/irrKlang-64bit-1.6.0/include
		    ${PROJECT_SOURCE_DIR}/3Dviz_image_studio/irrKlang-64bit-1.6.0/lib
		    ${PROJECT_SOURCE_DIR}/3Dviz_image_studio/irrKlang-64bit-1.6.0/bin/linux-gcc-64)

include_directories(${PROJECT_SOURCE_DIR}/libjpeg-turbo-main)

 
#GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(OpenGL REQUIRED)


#configuration for openCV
find_package( OpenCV REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS}
		     ${PROJECT_SOURCE_DIR}/3Dviz_image_studio/eigen
		     ${PROJECT_SOURCE_DIR}/3Dviz_image_studio/imgui
		     ${PROJECT_SOURCE_DIR}/3Dviz_image_studio/include
		     ${GLFW_INCLUDE_DIRS}
		     ${GLFW_LIBRARIES}
		     ${PROJECT_SOURCE_DIR}/include
		     ${PROJECT_SOURCE_DIR}/3Dviz_image_studio/include/glad
		     ${PROJECT_SOURCE_DIR}/camera_node/include
			)

add_executable( app
		src/app.cpp
		3Dviz_image_studio/src/viz.cpp
		3Dviz_image_studio/src/axes.cpp
		3Dviz_image_studio/src/pose.cpp
    		3Dviz_image_studio/src/viz_utils.cpp
		3Dviz_image_studio/src/viz_utils2.cpp
    		3Dviz_image_studio/src/modules.cpp
		3Dviz_image_studio/src/mavlink_module.cpp

		3Dviz_image_studio/imgui/imgui.cpp
		3Dviz_image_studio/imgui/imgui_draw.cpp
		3Dviz_image_studio/imgui/imgui_demo.cpp
		3Dviz_image_studio/imgui/imgui_impl_glfw.cpp
		3Dviz_image_studio/imgui/imgui_impl_opengl3.cpp
		3Dviz_image_studio/imgui/imgui_widgets.cpp
		3Dviz_image_studio/imgui/imgui_tables.cpp
		3Dviz_image_studio/src/glad.c

		camera_node/src/camera_node.cpp
		camera_node/src/stereo_node.cpp
		camera_node/src/stereo_node_tripple.cpp
		camera_node/src/mono_camera_node.cpp
		camera_node/src/trigger.cpp
 		
		mavlink/serial_port.cpp
		mavlink/autopilot_interface.cpp
		mavlink/udp_port.cpp
		mavlink/mavlink_node.cpp)

target_link_libraries(app
			${GLFW_STATIC_LIBRARIES}
			${OPENGL_LIBRARIES}
			${OpenCV_LIBS}
			"${PROJECT_SOURCE_DIR}/3Dviz_image_studio/irrKlang-64bit-1.6.0/bin/linux-gcc-64/libIrrKlang.so"
			"${PROJECT_SOURCE_DIR}/libjpeg-turbo-main/build/libturbojpeg.so")
#building main



add_executable( mavlink_control
		mavlink/mavlink_control.cpp
		mavlink/serial_port.cpp
		mavlink/autopilot_interface.cpp
		mavlink/udp_port.cpp)


