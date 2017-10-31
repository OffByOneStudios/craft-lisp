cmake_minimum_required(VERSION 3.7)
if(TARGET CraftLisp)
    return()
endif(TARGET CraftLisp)
Project(CraftLisp)

# This Variable is the Simplest way to ensure your variables will work
include(cmake/ResolveCraftRoot.cmake)
# This is mandatory
include(cmake/ResolveCraftCmake.cmake)

# Use This To Support Multiplatforms or to use the Vendor Repo.
include(${CRAFT_CMAKE_DIR}/ResolveCraftTripleCross.cmake)


include(${CRAFT_CMAKE_DIR}/ResolveVendor.cmake)
# Use This is you depend on Vendors
IF(NOT DEFINED CRAFT_ENGINE_VENDOR_DIR)
  MESSAGE(FATAL_ERROR "Could not locate required dependency CRAFT_ENGINE_VENDOR_DIR")
ENDIF()


# External Cmake Deps
include(${CRAFT_CMAKE_DIR}/PrecompiledHeader.cmake)
include(${CRAFT_CMAKE_DIR}/MakeVSFilters.cmake)
include(${CRAFT_CMAKE_DIR}/MakeVSDocs.cmake)

# Use This Preprocess Def to Control exports
add_definitions(-D${CRAFT_TRIPLE_CROSS})

# Depdendent Projects
add_subdirectory(${CRAFT_ENGINE_ROOT_DIR}/deps/craft-util CraftUtil)
add_subdirectory(${CRAFT_ENGINE_ROOT_DIR}/deps/types CraftType)

# Test culling

# File Operations
file(GLOB_RECURSE includes
  src/*.h
  src/*.hpp
)

file(GLOB_RECURSE sources
  src/*.cpp
)

file(GLOB_RECURSE tests
  test/*.h
  test/*.cpp
)

file(GLOB_RECURSE docs
  src/*.md
)


include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CRAFT_VENDOR_INCLUDE_DIRECTORY}
  ${CRAFT_ENGINE_ROOT_DIR}/deps/craft-util/src
  ${CRAFT_ENGINE_ROOT_DIR}/deps/types/src
)

link_directories(
  ${CRAFT_VENDOR_LIB_DIRECTORY}
)

add_library(CraftLisp SHARED
  ${sources}
  ${includes}
  common.cpp
)
target_compile_definitions(CraftLisp PRIVATE CRAFT_LISP_DLL)

add_precompiled_header(CraftLisp src/lisp/common.h FORCEINCLUDE)

target_link_libraries (CraftLisp CraftUtil CraftType)

if(NOT DEFINED CRAFT_ENGINE_SUBDIRECTORY)

add_executable(CraftLispTests
  ${tests}
)
target_link_libraries (CraftLispTests CraftLisp)
endif()



# Platform Specific
if(CRAFT_TRIPLE_CROSS STREQUAL "osx_x64_clang")
  list(APPEND CMAKE_CXX_FLAGS "-std=c++1z -stdlib=libc++ -Wc++11-extensions")
elseif(MSVC)
  set(files ${includes} ${sources} ${docs})
  VSFILTERS(${files})
  GenerateDocs(CraftLisp)
else()
  list(APPEND CMAKE_CXX_FLAGS "-std=gnu++1z -pthread -fpermissive")
endif()
