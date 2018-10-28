cmake_minimum_required(VERSION 3.12)
project(StandardClassLibrary)

set(CMAKE_CXX_STANDARD 11)

add_executable(StandardClassLibrary main.cpp scl/scl.hpp scl/tools/tools.hpp scl/tools/make/make.hpp scl/tools/make/from.h scl/tools/make/ptr.h scl/tools/make/unique.h scl/tools/make/shared.h scl/tools/iostream/iostream.hpp scl/tools/iostream/nl.h)
target_include_directories(StandardClassLibrary PRIVATE ${CMAKE_SOURCE_DIR})