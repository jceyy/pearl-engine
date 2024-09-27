# add_executable(engine-test main.cpp)  # ${ENGINE_SOURCE_FILES}
# target_compile_features(engine-test PRIVATE cxx_std_11)
# target_link_libraries(engine-test PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

add_executable(engine-test main.cpp)
target_compile_features(engine-test PRIVATE cxx_std_11)

target_link_libraries(engine-test PRIVATE prl_library ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})