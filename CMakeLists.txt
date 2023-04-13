cmake_minimum_required(VERSION 2.6)
project(PearlEngine)

if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES debug)
	set(CMAKE_BUILD_TYPE Debug)
else(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES debug)
	set(CMAKE_BUILD_TYPE Release)
endif(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES debug)

message("\n")
message("Build type is ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic -std=c++11") # -fsanitize=address
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11")

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	message("Flags are: ${CMAKE_CXX_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
	message("Flags are: ${CMAKE_CXX_FLAGS_RELEASE}")
endif(CMAKE_BUILD_TYPE MATCHES "Debug")

message("\n")

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)


include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

add_executable(engine-test 
src/main.cpp 
src-engine/Game.cpp 
src-engine/TextureManager.cpp
src-engine/GameObject.cpp
src-engine/Vector2D.cpp
src-engine/Collision.cpp
src-engine/ECS/ECS.cpp
src-engine/ECS/TransformComponent.cpp
src-engine/ECS/SpriteComponent.cpp
src-engine/ECS/KeyboardController.cpp
src-engine/ECS/ColliderComponent.cpp
)

target_link_libraries(engine-test ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

