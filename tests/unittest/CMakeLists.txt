include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Tests utils/AnyTests.cpp utils/OptionalTests.cpp utils/EitherTests.cpp utils/PlaceholderTests.cpp utils/ToStringTests.cpp utils/RawStorageTests.cpp)

target_include_directories(
    Tests PRIVATE ${gtest_SOURCE_DIR},
    Tests PRIVATE ../libs/spdlog-1.x/include
    Tests PRIVATE ../libs/include
    Tests PRIVATE ../../include
)

target_link_libraries(Tests gtest gtest_main)
#target_link_libraries(Tests StandardClassLibrary)