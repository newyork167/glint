cmake_minimum_required(VERSION 2.8.12)
project(dear-imgui-conan)

set(CMAKE_CXX_STANDARD 17)
set(_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)

# CHANGEME when you want to not run in debug
set(DEBUG TRUE)

# Set external src folder
set(CONAN_EXTERNAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external)

# Run conan install based on os/arch
if(APPLE)
    execute_process( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
    if(${ARCHITECTURE} STREQUAL "arm64")
        message("Building for Apple Silicon")
        execute_process(COMMAND tools/m1.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tools)
        execute_process(COMMAND conan install -if . ${CMAKE_SOURCE_DIR}/conanfile.py -e CONAN_EXTERNAL_PATH=${CONAN_EXTERNAL_PATH} --profile:build default --profile:host m1 --build=missing WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/)
    else()
        execute_process(COMMAND conan install -if . ${CMAKE_SOURCE_DIR}/conanfile.py -e CONAN_EXTERNAL_PATH=${CONAN_EXTERNAL_PATH} --build=missing WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/)
    endif()
else()
    message("Building for Windows/Linux")
    execute_process(COMMAND conan install -if . ${CMAKE_SOURCE_DIR}/conanfile.py -e CONAN_EXTERNAL_PATH=${CONAN_EXTERNAL_PATH} --build=missing WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/)
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set_directory_properties(PROPERTIES external/ ADDITIONAL_MAKE_CLEAN_FILES)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

find_package(imgui CONFIG)
find_package(glad MODULE)
find_package(glfw3 MODULE REQUIRED)
find_package(stb MODULE)
find_package(glm MODULE)
find_package(fmt MODULE)

# Dependencies -- Boost
Set(Boost_INSTALL_DIR ${CONAN_BOOST_ROOT})
find_package(Boost 1.79.0 COMPONENTS filesystem)

# Glob all the source/headers/compiles files
file(GLOB_RECURSE PROJECT_HEADERS include/*.h include/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp)

file(GLOB_RECURSE VENDOR_SOURCES external/*.cpp)
file(GLOB_RECURSE VENDOR_HEADERS external/*.h external/*.hpp)

# Glob all shader types
file(GLOB PROJECT_SHADERS data/shaders/*.comp
        data/shaders/*.frag data/shaders/*.fs
        data/shaders/*.geom
        data/shaders/*.vert data/shaders/*.vs)

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

if (DEBUG)
    message("SOURCES: ${PROJECT_SOURCES}")
    message("VENDOR_SOURCES: ${VENDOR_SOURCES}")
endif()

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendor" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
        ${PROJECT_SHADERS} ${PROJECT_CONFIGS}

        external/imgui/imgui_impl_glfw.cpp
        external/imgui/imgui_impl_opengl3.cpp
        external/imgui/imgui_impl_glfw.h
        external/imgui/imgui_impl_opengl3.h

        ${VENDORS_SOURCES} )

target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

# Use conan's target_link_libraries instead of target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
conan_target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS}
        PRIVATE
        Boost::filesystem)
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

#add_custom_command(TARGET dear-imgui-conan
#        POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy assets/shaders/simple-shader.vs ${PROJECT_BINARY_DIR}
#        COMMAND ${CMAKE_COMMAND} -E copy assets/shaders/simple-shader.fs ${PROJECT_BINARY_DIR}
#        )

#target_compile_definitions(dear-imgui-conan PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)

conan_target_link_libraries(dear-imgui-conan)

# Copy Assets
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# If we are debugging, symlink the shaders directory to allow live reloading/editing
if (DEBUG)
    set(DEBUG_SHADER_SOURCE \"${CMAKE_CURRENT_SOURCE_DIR}/data/shaders\")
    set(DEBUG_SHADER_DESTINATION \"${CMAKE_CURRENT_BINARY_DIR}/data/shaders\")

    find_package (Python3 COMPONENTS Interpreter Development REQUIRED)
    message("Python3 found for linking shaders?: ${Python3_FOUND}: ${Python3_EXECUTABLE}")

    if(Python3_FOUND)
        message("Attempting to symlink shaders for live reloading from ${DEBUG_SHADER_SOURCE} to ${DEBUG_SHADER_DESTINATION}")
        message("")
        add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/tools/debug_SymlinkShaders.py"
                -s ${CMAKE_CURRENT_SOURCE_DIR}/data/shaders
                -d ${CMAKE_CURRENT_BINARY_DIR}/bin/shaders)
    else()
        message("Symlinking shaders for debug requires python 3")
    endif()
endif()
