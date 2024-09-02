cmake_minimum_required (VERSION 3.8)
include(FetchContent)

function(GroupSourcesByFolder target)
  set(SOURCE_GROUP_DELIMITER "/")
  set(last_dir "")
  set(files "")

  get_target_property(sources ${target} SOURCES)

  foreach(file ${sources})
    file(RELATIVE_PATH relative_file "${PROJECT_SOURCE_DIR}" ${file})
    get_filename_component(dir "${relative_file}" PATH)
    if(NOT "${dir}" STREQUAL "${last_dir}")
      if(files)
        source_group("${last_dir}" FILES ${files})
      endif()
      set(files "")
    endif()
    set(files ${files} ${file})
    set(last_dir "${dir}")
  endforeach()

  if(files)
    source_group("${last_dir}" FILES ${files})
  endif()
endfunction()

project("Void")

file(GLOB_RECURSE FILES_SRC_E "src/*.cpp")
file(GLOB_RECURSE FILES_H_E "include/*.h")
file(GLOB MAIN_GLOBAL_SRC "mains/main.cpp")
file(GLOB MAIN_RCF_SRC "mains/RCFMain.cpp")
file(GLOB MAIN_REPLAYS_SRC "mains/VoidReplaysConverter.cpp")

add_library(Void ${FILES_SRC_E} ${FILES_H_E})
add_executable(Void_Main ${MAIN_GLOBAL_SRC})
add_executable(Void_RCF ${MAIN_RCF_SRC})
add_executable(Void_Replay_Converter ${MAIN_REPLAYS_SRC})

set_target_properties(Void PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(Void PROPERTIES CXX_STANDARD 20)
GroupSourcesByFolder(Void)

list(APPEND TARGETS Void_Main Void_RCF Void_Replay_Converter)

target_include_directories(Void PUBLIC "include")
target_link_libraries(Void_Main Void)

# Make sure RLGymPPO_CPP is going to build in the same directory as us
# Otherwise, we won't be able to import it at runtime
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

# Include RLGymSim_PPO
add_subdirectory(RLGymPPO_CPP)
target_link_libraries(Void RLGymPPO_CPP)

target_link_libraries(Void RLBotCPP)


FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG 0.8.0 # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
)
FetchContent_GetProperties(yaml-cpp)

if(NOT yaml-cpp_POPULATED)
  message(STATUS "Fetching yaml-cpp...")
  FetchContent_Populate(yaml-cpp)
  add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()

target_link_libraries(Void yaml-cpp::yaml-cpp) # The library or executable that require yaml-cpp library

file(GLOB MAIN_RLBOT "mains/RLBotMain.cpp")
add_executable(Void_RLBot ${MAIN_RLBOT})
list(APPEND TARGETS Void_RLBot)

file(GLOB MAIN_SELECTOR_SRC "mains/SelectorMain.cpp")
add_executable(Void_Selector ${MAIN_SELECTOR_SRC})
list(APPEND TARGETS Void_Selector)

file(GLOB MAIN_DOUBLETAP_SRC "mains/DoubleTapMain.cpp")
add_executable(Void_DoubleTap ${MAIN_DOUBLETAP_SRC})
list(APPEND TARGETS Void_DoubleTap)

file(GLOB MAIN_RECOVERY_SRC "mains/RecoveryMain.cpp" "Configs/RecoveryConfig.yaml")
add_executable(Void_Recovery ${MAIN_RECOVERY_SRC})
list(APPEND TARGETS Void_Recovery)

file(GLOB MAIN_CEILINGPINCH_SRC "mains/CeilingPinchMain.cpp")
add_executable(Void_CeilingPinch ${MAIN_CEILINGPINCH_SRC})
list(APPEND TARGETS Void_CeilingPinch)

file(GLOB MAIN_WALLPINCH_SRC "mains/WallPinchMain.cpp")
add_executable(Void_WallPinch ${MAIN_WALLPINCH_SRC})
list(APPEND TARGETS Void_WallPinch)

file(GLOB MAIN_TEAMPINCH_SRC "mains/TeamPinchMain.cpp")
add_executable(Void_TeamPinch ${MAIN_TEAMPINCH_SRC})
list(APPEND TARGETS Void_TeamPinch)

file(GLOB MAIN_GROUNDPINCH_SRC "mains/GroundPinchMain.cpp")
add_executable(Void_GroundPinch ${MAIN_GROUNDPINCH_SRC})
list(APPEND TARGETS Void_GroundPinch)

# Replace here

foreach(tgt ${TARGETS})
    set_target_properties(${tgt} PROPERTIES LINKER_LANGUAGE CXX)
    set_target_properties(${tgt} PROPERTIES CXX_STANDARD 20)
    GroupSourcesByFolder(${tgt})
    target_link_libraries(${tgt} Void)
endforeach()