# CMakeLists.txt

# Copyright (C) 2007-2014 Glenn Randers-Pehrson

# This code is released under the libpng license.
# For conditions of distribution and use, see the disclaimer
# and license in png.h

cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_CONFIG_H -DFPM_ARM -ffast-math -O3")
project("renderer")
include_directories(../libcommon)
include_directories(../libpng)
include_directories(./)
file(GLOB SOURCE_FILES
        ./*.cpp
)
add_library(renderer SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        ${SOURCE_FILES})

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(renderer
        # List libraries link to the target library
        pngdecoder
        GLESv2
        EGL
        z
        android
        log)
