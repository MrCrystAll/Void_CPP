cmake_minimum_required (VERSION 3.8)

project("RLGymPPO_CPP_Example")

file(GLOB_RECURSE FILES_SRC_E "src/*.cpp")
file(GLOB_RECURSE FILES_H_E "include/*.h")

add_executable(RLGymPPO_CPP_Example ${FILES_SRC_E})

target_include_directories(RLGymPPO_CPP_Example PUBLIC "include")

# Set C++ version to 20
set_target_properties(RLGymPPO_CPP_Example PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(RLGymPPO_CPP_Example PROPERTIES CXX_STANDARD 20)

# Make sure RLGymPPO_CPP is going to build in the same directory as us
# Otherwise, we won't be able to import it at runtime
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

# Include RLGymSim_PPO
add_subdirectory(RLGymPPO_CPP)
target_link_libraries(RLGymPPO_CPP_Example RLGymPPO_CPP)

# Include RLBot
add_subdirectory(RLBotCPP)
target_link_libraries(RLGymPPO_CPP_Example RLBotCPP)