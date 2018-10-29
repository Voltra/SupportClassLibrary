cmake_minimum_required(VERSION 3.12)
project(StandardClassLibrary)

set(CMAKE_CXX_STANDARD 11)

add_compile_definitions(SCL_USE_IOSTREAM_EXTEND)
add_executable(StandardClassLibrary main.cpp scl/scl.hpp scl/tools/tools.hpp scl/tools/make/make.hpp scl/tools/make/from.h scl/tools/make/ptr.h scl/tools/make/unique.h scl/tools/make/shared.h scl/tools/iostream/iostream.hpp scl/tools/iostream/nl.h scl/tools/iostream/log/log.hpp scl/tools/iostream/log/basic.h scl/tools/iostream/log/prefixed.h scl/macros.h scl/tools/iostream/log/surrounded.h scl/tools/iostream/extend.h scl/tools/meta/meta.hpp scl/tools/meta/void_t.h scl/tools/meta/enable_if.h scl/tools/meta/is_convertible.h scl/tools/meta/can_call.h scl/tools/meta/is_same.h scl/tools/meta/fn_cast.h scl/tools/meta/fn_type.h scl/tools/meta/type_check.h scl/tools/meta/is_base_of.h scl/tools/meta/type_mod.h scl/tools/meta/type_query.h scl/tools/meta/conditional.h scl/tools/make/atomic.h scl/tools/make/promise.h scl/concepts/concepts.hpp scl/concepts/is_concept.h scl/concepts/require.h scl/concepts/Same.h scl/concepts/DerivedFrom.h scl/concepts/ConvertibleTo.h)
target_include_directories(StandardClassLibrary PRIVATE ${CMAKE_SOURCE_DIR})