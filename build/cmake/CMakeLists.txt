cmake_minimum_required (VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
enable_language(Pascal)

project (CMakeHelloWorld Pascal)

add_executable (hello hello.pp uhello.pas)
