
add_executable(Champollion main.cpp)
add_dependencies(Champollion Decompiler Pex)
target_link_libraries(Champollion Decompiler Pex ${Boost_LIBRARIES})