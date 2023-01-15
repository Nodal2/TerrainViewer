cmake_minimum_required( VERSION 3.15 )

project( mesh_tool )

## Find dependencies
find_package( OpenGL REQUIRED )

include_directories( src/graphic )
include_directories( src/math )
include_directories( src/util )
include_directories( src/io )
include_directories( src/terrain )

file( GLOB_RECURSE SOURCES src/*.cpp )

file( GLOB IMGUI_SOURCES dependancy/imgui/*.cpp )
file( GLOB IMGUI_INCLUDES dependancy/imgui )

file( GLOB STB_IMAGE_SOURCES dependancy/stb_image/lib/stb_image.cpp )
file( GLOB STB_IMAGE_INCLUDES dependancy/stb_image/include )

add_executable( ${PROJECT_NAME} src/main.cpp ${SOURCES} )

add_library( "imgui" STATIC ${IMGUI_SOURCES} )
include_directories( "imgui" PUBLIC ${IMGUI_INCLUDES} )

add_library( "stb_image" STATIC ${STB_IMAGE_SOURCES} )
include_directories( "stb_image" PUBLIC ${STB_IMAGE_INCLUDES} )

add_subdirectory( dependancy/glfw )
add_subdirectory( dependancy/glad )

target_link_libraries( ${PROJECT_NAME} PRIVATE OpenGL::GL )
target_link_libraries( ${PROJECT_NAME} PRIVATE glfw )
target_link_libraries( ${PROJECT_NAME} PRIVATE glad )
target_link_libraries( ${PROJECT_NAME} PRIVATE imgui )
target_link_libraries( ${PROJECT_NAME} PRIVATE stb_image )

target_compile_options( ${PROJECT_NAME} PRIVATE -std=c++11 -Wall -Wpedantic )
