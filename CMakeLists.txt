
cmake_minimum_required(VERSION 3.7)
if(TARGET CraftEngineDepsLisp)
    return()
endif(TARGET CraftEngineDepsLisp)
Project(CraftEngineDepsLisp)

IF(NOT DEFINED CRAFT_ROOT_PROJECT)
	SET(CRAFT_ROOT_PROJECT CraftEngineDepsLisp)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
ENDIF()

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

# Dependent Projects
add_subdirectory(${CRAFT_ENGINE_ROOT_DIR}/deps/util CraftEngineDepsUtil)
add_subdirectory(${CRAFT_ENGINE_ROOT_DIR}/deps/types CraftEngineDepsTypes)


include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CRAFT_VENDOR_INCLUDE_DIRECTORY}
  ${CRAFT_ENGINE_ROOT_DIR}/deps/util/src
  ${CRAFT_ENGINE_ROOT_DIR}/deps/types/src
  
)

# File Operations
set(LibSources
	src/README.md
  src/lisp/BuiltinFunction.cpp
  src/lisp/BuiltinFunction.h
  src/lisp/common.h
  src/lisp/defines.h
  src/lisp/Environment.cpp
  src/lisp/Environment.h
  src/lisp/Function.cpp
  src/lisp/Function.h
  src/lisp/Keyword.cpp
  src/lisp/Keyword.h
  src/lisp/lisp.cpp
  src/lisp/lisp.h
  src/lisp/Macro.cpp
  src/lisp/Macro.h
  src/lisp/MultiMethod.cpp
  src/lisp/MultiMethod.h
  src/lisp/parser.h
  src/lisp/parser_lisp.cpp
  src/lisp/PSubroutine.hpp
  src/lisp/Scope.cpp
  src/lisp/Scope.h
  src/lisp/Sexpr.cpp
  src/lisp/Sexpr.h
  src/lisp/SpecialForm.cpp
  src/lisp/SpecialForm.h
  src/lisp/Symbol.cpp
  src/lisp/Symbol.h
  src/lisp/types.cpp
  src/lisp/types.h
  src/lisp/library/global.cpp
  src/lisp/library/libraries.h
  
)

set(TestSources
	test/main.cpp
  test/types.cult
  
)

set(EntrySources
	entry/main.cpp
  
)


link_directories(
  ${CRAFT_VENDOR_LIB_DIRECTORY}
)

add_library(CraftEngineDepsLisp SHARED
  ${LibSources}
  common.cpp
)

SET(CMAKE_CONFIGURATION_TYPES "Debug;Release;Trace")
SET(CMAKE_CXX_FLAGS_TRACE ${CMAKE_CXX_FLAGS_DEBUG})
SET(CMAKE_LINKER_FLAGS_TRACE ${CMAKE_LINKER_FLAGS_DEBUG})
SET(CMAKE_SHARED_LINKER_FLAGS_TRACE ${CMAKE_SHARED_LINKER_FLAGS_DEBUG})
SET(CMAKE_EXE_LINKER_FLAGS_TRACE ${CMAKE_EXE_LINKER_FLAGS_DEBUG})

set_property(TARGET CraftEngineDepsLisp
  APPEND PROPERTY COMPILE_DEFINITIONS
  $<$<CONFIG:Trace>:BUILD_TRACE>
  $<$<CONFIG:Debug>:BUILD_DEBUG>
  $<$<CONFIG:Release>:BUILD_RELEASE>
)


target_compile_definitions(CraftEngineDepsLisp PRIVATE CRAFT_LISP_DLL)

add_precompiled_header(CraftEngineDepsLisp src/lisp/common.h FORCEINCLUDE)

target_link_libraries (CraftEngineDepsLisp CraftEngineDepsUtil  CraftEngineDepsTypes  )

# Tests
IF(${CRAFT_ROOT_PROJECT} STREQUAL CraftEngineDepsLisp)
	add_executable(CraftEngineDepsLispTest ${TestSources})
	target_link_libraries(CraftEngineDepsLispTest CraftEngineDepsLisp)
ENDIF()
# Entry
IF(${CRAFT_ROOT_PROJECT} STREQUAL CraftEngineDepsLisp)
	add_executable(CraftEngineDepsLispEntry ${EntrySources})
	target_link_libraries(CraftEngineDepsLispEntry CraftEngineDepsLisp)
ENDIF()

# IDE
set(files ${LibSources} ${TestSources} ${EntrySources})
VSFILTERS(${files})

#Vendor Dependencies
include(cmake/vendor/${CRAFT_TRIPLE_CROSS}.cmake)

# Platform Specific
include(cmake/platform/${CRAFT_TRIPLE_CROSS}.cmake)
include(cmake/flags/${CRAFT_TRIPLE_CROSS}.cmake)
include(cmake/defines/${CRAFT_TRIPLE_CROSS}.cmake)
