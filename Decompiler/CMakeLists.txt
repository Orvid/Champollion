file(GLOB HEADER_FILES "*.hpp")
file(GLOB SOURCE_FILES "*.cpp")

file(GLOB NODE_HEADER_FILES "Node/*.hpp")
file(GLOB NODE_SOURCE_FILES "Node/*.cpp")

add_library(Decompiler STATIC ${HEADER_FILES} ${SOURCE_FILES} ${NODE_HEADER_FILES} ${NODE_SOURCE_FILES})
add_dependencies(Decompiler Pex)
auto_source_group("Decompiler" ${CMAKE_CURRENT_SOURCE_DIR} ${HEADER_FILES} ${SOURCE_FILES} ${NODE_HEADER_FILES} ${NODE_SOURCE_FILES})
