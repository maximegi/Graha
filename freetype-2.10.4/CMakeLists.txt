include_directories(include)
file(GLOB_RECURSE SRC_FILES *.c *.h)
add_library(freetype-2.10.4 ${SRC_FILES})
