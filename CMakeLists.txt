cmake_minimum_required(VERSION 3.0)

project(Graha)

add_definitions(-std=c++11)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(ASSIMP REQUIRED)
#sudo apt install libfreetype6-dev
find_package(Freetype REQUIRED)

# Pour gérer un bug a la fac, a supprimer sur machine perso:
#set(OPENGL_LIBRARIES /usr/lib/x86_64-linux-gnu/libGL.so.1)

include_directories(${SDL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR} glimac/include modellib/include third-party/include freetype-2.10.4/include)

set(ALL_LIBRARIES modellib glimac ${FREETYPE_LIBRARIES} ${SDL_LIBRARY} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${ASSIMP_LIBRARIES})

add_subdirectory(modellib)

add_subdirectory(glimac)

file(GLOB HEADER_FILES ${CMAKE_SOURCE_DIR}/src/*.hpp)
file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE SHADER_FILES ${CMAKE_SOURCE_DIR}/assets/shaders/*.glsl)

set(OUTPUT ${PROJECT_NAME})
add_executable(${OUTPUT} ${HEADER_FILES} ${SRC_FILES} ${SHADER_FILES})
target_link_libraries(${OUTPUT} ${ALL_LIBRARIES})

file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
target_compile_options(${OUTPUT} PRIVATE -I/usr/include/freetype2 -I/usr/include/libpng16 -Wall -O2 -D VERBOSE) # -D VERBOSE to activate VERBOSE mode
