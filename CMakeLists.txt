cmake_minimum_required(VERSION 3.13)
project(mp5)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(cs225)
include_directories(cs225/catch)
include_directories(cs225/lodepng)
include_directories(util)

add_executable(mp5
#        cs225/catch/catch.hpp
#        cs225/catch/catchmain.cpp
        cs225/lodepng/lodepng.cpp
        cs225/lodepng/lodepng.h
        cs225/HSLAPixel.cpp
        cs225/HSLAPixel.h
        cs225/PNG.cpp
        cs225/PNG.h
        cs225/RGB_HSL.h
        util/coloredout.cpp
        util/coloredout.h
        util/no_sort.h
        util/util.cpp
        util/util.h
#        kdtree.cpp
        kdtree.h
#        kdtree_extras.cpp
        main.cpp
        maptiles.cpp
        maptiles.h
        mosaiccanvas.cpp
        mosaiccanvas.h
#        point.cpp
        point.h
        sourceimage.cpp
        sourceimage.h
        tileimage.cpp
        tileimage.h)
