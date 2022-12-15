cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# package information
set(PACKAGE_NAME      "Champollion")
set(PACKAGE_VERSION   "1.0.8")
set(PACKAGE_STRING    "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_TARNAME   "${PACKAGE_NAME}-${PACKAGE_VERSION}")
set(PACKAGE_BUGREPORT "https://github.com/Orvid/champollion/issues")

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

project(${PACKAGE_NAME} CXX)

if (VCPKG_TARGET_TRIPLET MATCHES "static")
  if(MSVC)
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_MINSIZEREL ${CMAKE_CXX_FLAGS_MINSIZEREL})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
  else(MSVC)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_STATIC_RUNTIME ON)
  endif(MSVC)
endif()

find_package(Boost 1.73.0 REQUIRED COMPONENTS system program_options filesystem)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Automatically create source_group directives for the sources passed in.
function(auto_source_group rootName rootDir)
  file(TO_CMAKE_PATH "${rootDir}" rootDir)
  string(LENGTH "${rootDir}" rootDirLength)
  set(sourceGroups)
  foreach (fil ${ARGN})
    file(TO_CMAKE_PATH "${fil}" filePath)
    string(FIND "${filePath}" "/" rIdx REVERSE)
    if (rIdx EQUAL -1)
      message(FATAL_ERROR "Unable to locate the final forward slash in '${filePath}'!")
    endif()
    string(SUBSTRING "${filePath}" 0 ${rIdx} filePath)
    
    string(LENGTH "${filePath}" filePathLength)
    string(FIND "${filePath}" "${rootDir}" rIdx)
    if (NOT rIdx EQUAL 0)
      continue()
      #message(FATAL_ERROR "Source '${fil}' is outside of the root directory, '${rootDir}', that was passed to auto_source_group!")
    endif()
    math(EXPR filePathLength "${filePathLength} - ${rootDirLength}")
    string(SUBSTRING "${filePath}" ${rootDirLength} ${filePathLength} fileGroup)
    
    string(REPLACE "/" "\\" fileGroup "${fileGroup}")
    set(fileGroup "\\${rootName}${fileGroup}")
    
    list(FIND sourceGroups "${fileGroup}" rIdx)
    if (rIdx EQUAL -1)
      list(APPEND sourceGroups "${fileGroup}")
      source_group("${fileGroup}" REGULAR_EXPRESSION "${filePath}/[^/.]+(.(idl|tab|yy))?.(c|cc|cpp|h|hpp|json|ll|php|tcc|y)$")
    endif()
  endforeach()
endfunction()

add_subdirectory(Decompiler)
add_subdirectory(Pex)
add_subdirectory(Champollion)
