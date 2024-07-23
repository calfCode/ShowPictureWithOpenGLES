# CMakeLists.txt

# Copyright (C) 2007-2014 Glenn Randers-Pehrson

# This code is released under the libpng license.
# For conditions of distribution and use, see the disclaimer
# and license in png.h

cmake_minimum_required(VERSION 3.22.1)
project("pngdecoder")
include_directories(./)
file(GLOB SOURCE_FILES
        ./png.c
        ./pngerror.c
        ./pngget.c
        ./pngmem.c
        ./pngpread.c
        ./pngread.c
        ./pngrio.c
        ./pngrtran.c
        ./pngrutil.c
        ./pngset.c
        ./pngtrans.c
        ./pngwio.c
        ./pngwrite.c
        ./pngwtran.c
        ./pngwutil.c
        ./image.c
        ./png_decoder.cpp
)
add_library(pngdecoder SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        ${SOURCE_FILES})

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(pngdecoder
        # List libraries link to the target library
        z
        android
        log)
