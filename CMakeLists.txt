cmake_minimum_required(VERSION 3.21)
project(Checkers_in_Cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(checkers boardly.cpp boardly.h main.cpp piecely.cpp piecely.h shapely.cpp shapely.h)
add_executable(testing testing.cpp boardly.cpp boardly.h piecely.cpp piecely.h shapely.cpp shapely.h)