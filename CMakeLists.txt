cmake_minimum_required(VERSION 3.11) # 2.6
project(
  PearlEngine
  VERSION 0.1
  LANGUAGES CXX
  DESCRIPTION "A simple 2D game engine")

# If main CMakeLists.txt file, then set some global properties
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  OPTION(BUILD_TESTING "Build the tests" ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # Set compile flags
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic -std=c++11") # -fsanitize=address
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11")

  # Test for build type
  if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES debug)
    set(CMAKE_BUILD_TYPE Debug)
    message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")
    message(STATUS "Compile flags are: ${CMAKE_CXX_FLAGS_DEBUG}")
  else(CMAKE_BUILD_TYPE MATCHES Release OR CMAKE_BUILD_TYPE MATCHES release)
    set(CMAKE_BUILD_TYPE Release)
    message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")
    message(STATUS "Compile flags are: ${CMAKE_CXX_FLAGS_RELEASE}")
  endif()

  # Find packages
  find_package(Doxygen)
    if(Doxygen_FOUND)
      add_subdirectory(docs)
    else()
      message(STATUS "Doxygen not found, not building docs")
    endif()

  set(CMAKE_C_COMPILER /usr/bin/gcc)
  set(CMAKE_CXX_COMPILER /usr/bin/g++)

  # set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
  set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})

  # Set up SDL2-related packages
  find_package(SDL2 REQUIRED)
  find_package(SDL2_image REQUIRED)
  find_package(SDL2_ttf REQUIRED)

  include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
  # target_include_directories(include PRIVATE "{CMAKE_CURRENT_SOURCE_DIR}")  
  endif()

# Libraries in here
add_subdirectory(src)

# Executable code is located in here
add_subdirectory(apps)

# Build tests only if required
if(BUILD_TESTING)
  enable_testing()
  find_package(GTest REQUIRED)
  # include(FetchContent)
  # FetchContent_Declare(
  #   googletest
  #   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  # )
  # # For Windows: Prevent overriding the parent project's compiler/linker settings
  # set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  # FetchContent_MakeAvailable(googletest)
  add_subdirectory(tests)
endif()

