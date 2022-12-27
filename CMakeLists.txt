cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
cmake_policy(SET CMP0048 NEW)

# package information
set(PACKAGE_NAME      "Champollion")
set(PACKAGE_VERSION   "1.1.0")
set(PACKAGE_STRING    "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_TARNAME   "${PACKAGE_NAME}-${PACKAGE_VERSION}")
set(PACKAGE_BUGREPORT "https://github.com/Orvid/champollion/issues")

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

project(${PACKAGE_NAME} VERSION 1.1.0 LANGUAGES CXX)
include(GNUInstallDirs)


option(CHAMPOLLION_STATIC_LIBRARY "Build Champollion as a static library" OFF)

if (NOT CHAMPOLLION_STATIC_LIBRARY)
  set(CMAKE_CXX_STANDARD 20)
else()
  set(CMAKE_CXX_STANDARD 17)
endif()


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


add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set(CHAMPOLLION_TARGET_NAME               ${PROJECT_NAME})
set(CHAMPOLLION_CONFIG_INSTALL_DIR        "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE INTERNAL "")
set(CHAMPOLLION_INCLUDE_INSTALL_DIR       "${CMAKE_INSTALL_INCLUDEDIR}/Champollion")
set(CHAMPOLLION_TARGETS_EXPORT_NAME       "${PROJECT_NAME}-targets")
set(CHAMPOLLION_CMAKE_CONFIG_TEMPLATE     "cmake/Config.cmake.in")
set(CHAMPOLLION_CMAKE_CONFIG_DIR          "${CMAKE_CURRENT_BINARY_DIR}")
set(CHAMPOLLION_CMAKE_VERSION_CONFIG_FILE "${CHAMPOLLION_CMAKE_CONFIG_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(CHAMPOLLION_CMAKE_PROJECT_CONFIG_FILE "${CHAMPOLLION_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Config.cmake")
set(CHAMPOLLION_CMAKE_PROJECT_TARGETS_FILE "${CHAMPOLLION_CMAKE_CONFIG_DIR}/${PROJECT_NAME}-targets.cmake")

if(CHAMPOLLION_STATIC_LIBRARY)

  file(GLOB DECOMPILER_HEADER_FILES "Decompiler/*.hpp")
  file(GLOB PEX_HEADER_FILES "Pex/*.hpp")
  file(GLOB NODE_HEADER_FILES "Decompiler/Node/*.hpp")
  list(APPEND ALL_HEADERS ${DECOMPILER_HEADER_FILES} ${PEX_HEADER_FILES} ${NODE_HEADER_FILES})

  file(GLOB SOURCE_FILES "Pex/*.cpp" "Decompiler/*.cpp" "Decompiler/Node/*.cpp")

  add_library("${PROJECT_NAME}" STATIC ${ALL_HEADERS} ${SOURCE_FILES})

  target_include_directories(
    "${PROJECT_NAME}"
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CHAMPOLLION_INCLUDE_INSTALL_DIR}>
  )
  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
      ${CHAMPOLLION_CMAKE_VERSION_CONFIG_FILE} COMPATIBILITY SameMinorVersion
  )
  configure_package_config_file(
    ${CHAMPOLLION_CMAKE_CONFIG_TEMPLATE}
    "${CHAMPOLLION_CMAKE_PROJECT_CONFIG_FILE}" 
    INSTALL_DESTINATION ${CHAMPOLLION_CONFIG_INSTALL_DIR}
  )

  install(
    TARGETS "${CHAMPOLLION_TARGET_NAME}"
    EXPORT "${CHAMPOLLION_TARGETS_EXPORT_NAME}"
  )

  install(
    EXPORT "${CHAMPOLLION_TARGETS_EXPORT_NAME}"
    NAMESPACE "${CHAMPOLLION_TARGET_NAME}::"
    DESTINATION "${CHAMPOLLION_CONFIG_INSTALL_DIR}"
  )
  install(
    FILES ${DECOMPILER_HEADER_FILES}
    DESTINATION "${CHAMPOLLION_INCLUDE_INSTALL_DIR}/Decompiler"
  )
  install(
    FILES ${PEX_HEADER_FILES}
    DESTINATION "${CHAMPOLLION_INCLUDE_INSTALL_DIR}/Pex"
  )
  install(
    FILES ${NODE_HEADER_FILES}
    DESTINATION "${CHAMPOLLION_INCLUDE_INSTALL_DIR}/Decompiler/Node"
  )
  install(FILES ${CHAMPOLLION_CMAKE_VERSION_CONFIG_FILE} ${CHAMPOLLION_CMAKE_PROJECT_CONFIG_FILE}
  DESTINATION ${CHAMPOLLION_CONFIG_INSTALL_DIR})
else()
  if (VCPKG_TARGET_TRIPLET MATCHES "static")
    if(NOT MSVC)
      set(Boost_USE_STATIC_LIBS ON)
      set(Boost_USE_STATIC_RUNTIME ON)
    endif()
  endif()
  find_package(Boost REQUIRED COMPONENTS program_options)
  include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
  )
  add_subdirectory(Decompiler)
  add_subdirectory(Pex)
  add_subdirectory(Champollion)
  install(
    TARGETS Champollion
  )
endif()