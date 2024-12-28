cmake_minimum_required(VERSION 3.15)
project(texfmt)

# C++ standard version
set(CMAKE_CXX_STANDARD 17 CACHE STRING "" FORCE)

# Executable target
add_executable(texfmt)
set_target_properties(texfmt PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})

# Add compiler (debug & release) definitions
target_compile_definitions(texfmt PRIVATE $<$<CONFIG:Debug>:DEBUG=1>)
target_compile_definitions(texfmt PRIVATE $<$<CONFIG:Release>:NDEBUG=1>)

# Include directories & pre-compiled header
target_include_directories(texfmt PRIVATE "src/")
# target_precompile_headers(texfmt PRIVATE "src/pch.hh")

# Glob all source files
file(GLOB_RECURSE src "src/*.cc")
target_sources(texfmt PRIVATE ${src})
