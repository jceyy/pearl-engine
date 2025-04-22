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
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic -std=c++14") # -fsanitize=address
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++14 -O3 -DNDEBUG")

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

  set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})

  if (WIN32)
    set(SDL2_DIR "D:/SDL2/cmake")
    set(SDL2_image_DIR "D:/SDL2/cmake")
    set(SDL2_ttf_DIR "D:/SDL2/cmake")
    set(GTest_DIR "D:/googletest-1.16.0/googletest/cmake")
    set(CMAKE_C_COMPILER "C:/MinGW/mingw64/bin/gcc.exe")
    set(CMAKE_CXX_COMPILER "C:/MinGW/mingw64/bin/g++.exe")
    set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES "C:/MinGW/mingw64/lib/gcc/x86_64-w64-mingw32/14.2.0/include")

    # Set up SDL2-related packages
    find_package(SDL2 REQUIRED CONFIG)
    find_package(SDL2_image REQUIRED CONFIG)
    find_package(SDL2_ttf REQUIRED CONFIG)
  else()
    # Set up SDL2-related packages
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED CFIG)
    find_package(SDL2_ttf REQUIRED)
  endif()

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
  if (WIN32)
    find_package(GTest REQUIRED CONFIG)
    # Create alias target
    add_library(GTest::GTest INTERFACE IMPORTED)
    add_library(GTest::gtest_main INTERFACE IMPORTED)
  else()
    find_package(GTest REQUIRED)
  endif()

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

