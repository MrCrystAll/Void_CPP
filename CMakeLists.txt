cmake_minimum_required (VERSION 3.8)

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

add_library(Void ${FILES_SRC_E} ${FILES_H_E})
add_executable(Void_Main ${MAIN_GLOBAL_SRC})
add_executable(Void_RCF ${MAIN_RCF_SRC})

set_target_properties(Void PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(Void PROPERTIES CXX_STANDARD 20)
GroupSourcesByFolder(Void)

list(APPEND TARGETS Void_Main Void_RCF)

target_include_directories(Void PUBLIC "include")
target_link_libraries(Void_Main Void)


# Make sure RLGymPPO_CPP is going to build in the same directory as us
# Otherwise, we won't be able to import it at runtime
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

file(COPY "submodels" DESTINATION "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

# Include RLGymSim_PPO
add_subdirectory(RLGymPPO_CPP)
target_link_libraries(Void RLGymPPO_CPP)

# Include RLBot
add_subdirectory(RLBotCPP)
target_link_libraries(Void RLBotCPP)

file(GLOB MAIN_SELECTOR_SRC "mains/SelectorMain.cpp")
add_executable(Void_Selector ${MAIN_SELECTOR_SRC})
list(APPEND TARGETS Void_Selector)

file(GLOB MAIN_DOUBLETAP_SRC "mains/DoubleTapMain.cpp")
add_executable(Void_DoubleTap ${MAIN_DOUBLETAP_SRC})
list(APPEND TARGETS Void_DoubleTap)

file(GLOB MAIN_RECOVERY_SRC "mains/RecoveryMain.cpp")
add_executable(Void_Recovery ${MAIN_RECOVERY_SRC})
list(APPEND TARGETS Void_Recovery)

# Replace here

foreach(tgt ${TARGETS})
    set_target_properties(${tgt} PROPERTIES LINKER_LANGUAGE CXX)
    set_target_properties(${tgt} PROPERTIES CXX_STANDARD 20)
    GroupSourcesByFolder(${tgt})
    target_link_libraries(${tgt} Void)
endforeach()