cmake_minimum_required(VERSION 3.20)
project(Void_Tests)

include(CTest)

file(GLOB_RECURSE FILES_SRC_TESTS "src/*.cpp")
file(GLOB_RECURSE FILES_H_TESTS "include/*.h")

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.7.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)
add_executable (Tests ${FILES_SRC_TESTS} ${FILES_H_TESTS})
GroupSourcesByFolder(Tests)

set_target_properties(Tests PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(Tests PROPERTIES CXX_STANDARD 20)


target_include_directories(Tests PUBLIC "include")
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain Void)
