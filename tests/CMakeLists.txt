# include(FetchContent)

# FetchContent_Declare(
#   googletest
#   GIT_REPOSITORY https://github.com/google/googletest.git
#   GIT_TAG        release-1.11.0
# )
# FetchContent_MakeAvailable(googletest)


# set(ENGINE_TEST_FILES 
# tests/main_test.cpp 
# tests/main_test.cpp)

# add_executable(engine-gtests ${ENGINE_SOURCE_FILES} ${ENGINE_TEST_FILES})

# add_library(GTest::GTest INTERFACE IMPORTED)
# target_link_libraries(GTest::GTest INTERFACE gtest_main)

# target_link_libraries(engine-gtests ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} GTest::GTest)
# include(GoogleTest)
# gtest_discover_tests(engine-gtests)

# add_test(NAME engine-gtests COMMAND engine-gtests)


# Alternative -----------------------



# # Testing library
# FetchContent_Declare(
#   catch2
#   GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#   GIT_TAG v2.13.10)
# FetchContent_MakeAvailable(catch2)
# # Adds Catch2::Catch2

# Tests need to be added as executables first
add_executable(geometry-test geometry_test.cpp)

# add_library(GTest::GTest INTERFACE IMPORTED)
# target_link_libraries(GTest::GTest INTERFACE gtest_main)
# include(GoogleTest)
# gtest_discover_tests(engine-gtests)

# Using C++11 in the test
# target_compile_features(geometry-test PRIVATE cxx_std_11)

# Should be linked to the main library, as well as the SDL2 related libraries
target_link_libraries(geometry-test PRIVATE prl_library 
${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} 
GTest::gtest_main) 

# If you register a test, then ctest and make test will run it. You can also run
# examples and check the output, as well.
include(GoogleTest)
gtest_discover_tests(geometry-test)

# add_test(NAME GeometryTest COMMAND geometry-test) # Command can be a target
