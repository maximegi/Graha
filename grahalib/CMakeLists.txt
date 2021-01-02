include_directories(include)
file(GLOB_RECURSE SRC_FILES *.cpp *.hpp)
add_library(grahalib ${SRC_FILES})
target_compile_options(grahalib PRIVATE -I/usr/include/freetype2 -I/usr/include/libpng16 -Wall -O2 -D VERBOSE) # -D VERBOSE to activate VERBOSE mode
