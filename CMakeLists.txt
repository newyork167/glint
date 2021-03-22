cmake_minimum_required(VERSION 3.0)
project(Glint)

set(CMAKE_CXX_STANDARD 17)
set(_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)

set(DEBUG TRUE)

# Run conan install on conanfile.py
set(conan_setup_file "Scripts/conan_setup.py")
execute_process(COMMAND python ${conan_setup_file} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ ) 

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

# If we are debugging, symlink the shaders directory to allow live reloading/editing
if (DEBUG)
    message("Symlinking shaders for live reloading")
    message("Link from ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/Shaders to ${CMAKE_CURRENT_BINARY_DIR}/bin")
    execute_process(COMMAND python Scripts/debug_SymlinkShaders.py 
        -s "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/Shaders"
        -d "${CMAKE_CURRENT_BINARY_DIR}/bin/Shaders"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ )
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /std:c++17")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++17")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(Glint/Headers/)

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
                               
                               Glint/bindings/imgui_impl_glfw.cpp
                               Glint/bindings/imgui_impl_opengl3.cpp
                               Glint/bindings/imgui_impl_glfw.h
                               Glint/bindings/imgui_impl_opengl3.h

                               ${VENDORS_SOURCES} )
target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
#target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
conan_target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS}
                                PRIVATE
                               Boost::filesystem)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Glint/Shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders
        DEPENDS ${PROJECT_SHADERS})
