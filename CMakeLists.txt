cmake_minimum_required (VERSION 2.8)

project (miniprof)

add_subdirectory (plugins)

file (GLOB sources src/*.cpp)

add_executable (miniprof ${sources})

target_link_libraries (miniprof lldb_plugin most_called_plugin lldb pthread cppnetlib-server-parsers)
include_directories (include)

