cmake_minimum_required(VERSION 3.20)
project(Void_Tests)

find_package (Boost COMPONENTS system REQUIRED)

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
  message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
execute_process(COMMAND "${CMAKE_COMMAND}" "-E" "environment")

add_definitions (-DBOOST_TEST_DYN_LINK)
add_executable (Tests ${FILES_SRC_TESTS})
target_include_directories(Tests PUBLIC "include")
target_include_directories(Tests PUBLIC ${BOOST_LIBRARY_INCLUDES})
target_link_libraries (Tests
                       "../Release/Void"
                       ${boost_BINARY_DIR}
                       )