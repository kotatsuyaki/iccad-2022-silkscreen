cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(main VERSION 1.0)

add_executable(
    ${PROJECT_NAME}
    ./src/main.cpp
)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)


# External libs
include_directories(./external/fmt/include/)

# LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)
if( supported )
  message(STATUS "LTO enabled")
  set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(STATUS "LTO not supported: ${error}")
endif()


# Linux-specific options
if(NOT WIN32)
  add_compile_options(-Wall -Wextra)
  install(TARGETS ${PROJECT_NAME} DESTINATION bin)
endif()


# Export compile commands for clangd
if(DEFINED ENV{CMAKE_EXPORT_COMPILE_COMMANDS})
  message(STATUS "Exporting compile commands")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()
