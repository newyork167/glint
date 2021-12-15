cmake_minimum_required(VERSION 3.0)
project(Glint)

# Set some CMake commands to ignore certain C++17 issues
set(CMAKE_CXX_STANDARD 17)
set(_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)

# CHANGEME when you want to not run in debug
set(DEBUG TRUE)

# Run conan install on conanfile.py
execute_process(COMMAND conan install -if . conanfile.py WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ )

# Import the cmake that was made and run the setup on it
include(${CMAKE_CURRENT_SOURCE_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Find all the packages - Add a find_package here for others you want to include
find_package(imgui CONFIG)
find_package(glad CONFIG)
find_package(glfw3 CONFIG)
find_package(assimp CONFIG)
find_package(stb CONFIG)
find_package(glm CONFIG)

# Dependencies -- Boost
Set(Boost_INSTALL_DIR ${CONAN_BOOST_ROOT})
find_package(Boost 1.75.0 COMPONENTS filesystem)

# Set compiler flags to C++17 with all warnings
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /std:c++17")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++17")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(Glint/Headers/)

# Glob all the source/headers/compiles files
file(GLOB_RECURSE PROJECT_HEADERS Glint/Headers/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES Glint/Sources/*.cpp)
file(GLOB PROJECT_SHADERS Glint/Shaders/*.comp
                          Glint/Shaders/*.frag
                          Glint/Shaders/*.geom
                          Glint/Shaders/*.vert)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

# If we are debugging we don't want to group shaders
# since we want to be able to live edit within the IDE
if (DEBUG)
    source_group("Shaders")
else()
    source_group("Shaders" FILES ${PROJECT_SHADERS})
endif()

message("SOURCES: ${PROJECT_SOURCES}")

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               
                               Glint/Vendors/imgui/imgui_impl_glfw.cpp
                               Glint/Vendors/imgui/imgui_impl_opengl3.cpp
                               Glint/Vendors/imgui/imgui_impl_glfw.h
                               Glint/Vendors/imgui/imgui_impl_opengl3.h

                               ${VENDORS_SOURCES} )

target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

# Use conan's target_link_libraries instead!
#target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
conan_target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS}
                                PRIVATE
                               Boost::filesystem)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

# Copy Assets
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    ${CMAKE_CURRENT_LIST_DIR}/Glint/Assets ${CMAKE_CURRENT_BINARY_DIR}/Assets
)
add_dependencies(${PROJECT_NAME} copy_assets)

# If we are debugging, symlink the shaders directory to allow live reloading/editing
if (DEBUG)
    message("Attempting to symlink shaders for live reloading from ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/Shaders to ${CMAKE_CURRENT_BINARY_DIR}/bin/Shaders")
    find_package (Python3 COMPONENTS Interpreter Development REQUIRED)
    message("Python3 found for linking shaders?: ${Python3_FOUND}")
    if(Python3_FOUND)
        add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/Scripts/debug_SymlinkShaders.py"
                -s "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/Shaders"
                -d "${CMAKE_CURRENT_BINARY_DIR}/bin/Shaders")
    else()
        message("Symlinking requires python 3")
    endif()
endif()
