# set(HEADER_LIST "${PROJECT_SOURCE_DIR}/include/PRL.h")

# set(ENGINE_SOURCE_FILES
# Game.cpp 
# TextureManager.cpp
# GameObject.cpp
# Vector2D.cpp
# Collision.cpp
# TileMap.cpp
# Animation.cpp
# AssetManager.cpp
# PhysicsManager.cpp
# Geometry.cpp
# ECS/ECS.cpp
# ECS/TransformComponent.cpp
# ECS/SpriteComponent.cpp
# ECS/KeyboardController.cpp
# ECS/ColliderComponent.cpp
# ECS/TileComponent.cpp
# ECS/ProjectileComponent.cpp
# ECS/UILabel.cpp
# ECS/PhysicsComponent.cpp)

# add_library(PRL ${ENGINE_SOURCE_FILES} ${HEADER_LIST})

# target_include_directories(PRL PUBLIC ../include)

# target_link_libraries(PRL PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

# target_compile_features(PRL PUBLIC cxx_std_11)

# source_group(
#     TREE "${PROJECT_SOURCE_DIR}/include" 
#     PREFIX "Header Files"
#     FILES ${HEADER_LIST})


add_library(prl_library)

target_sources(prl_library
  PRIVATE
    Game.cpp 
    TextureManager.cpp
    Types.cpp
    Vector2D.cpp
    Collision.cpp
    Logging.cpp
    TileMap.cpp
    Animation.cpp
    AssetManager.cpp
    PhysicsManager.cpp
    Geometry.cpp
    ECS/ECS.cpp
    ECS/TransformComponent.cpp
    ECS/SpriteComponent.cpp
    ECS/KeyboardController.cpp
    ECS/ColliderComponent.cpp
    ECS/TileComponent.cpp
    ECS/ProjectileComponent.cpp
    ECS/UILabel.cpp
    ECS/PhysicsComponent.cpp
    ECS/ButtonComponent.cpp
    ECS/FadeComponent.cpp
    ECS/VelocityComponent.cpp
    Systems/Systems.cpp
    Systems/RenderSystem.cpp
    Systems/AnimationSystem.cpp
  PUBLIC
    ${HEADER_LIST}
)

# Allow to include files relative to the root of the project
if (WIN32)
  target_include_directories(prl_library
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/PRL
    PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
  )
else()
  target_include_directories(prl_library
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/PRL
  )
endif()
