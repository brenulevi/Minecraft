cmake_minimum_required(VERSION 3.20)

project(game)

add_subdirectory(deps/glfw)

add_executable(game
    src/main.cpp

    src/game.cpp
    src/game.h
    src/window.cpp
    src/window.h
    src/renderer.cpp
    src/renderer.h
    src/input.cpp
    src/input.h

    src/world.cpp
    src/world.h
    src/player.cpp
    src/player.h
    src/chunk.cpp
    src/chunk.h
    src/block.cpp
    src/block.h
    src/chunkmanager.cpp
    src/chunkmanager.h

    src/transform.h
    src/camera.h
    src/raycast.cpp
    src/raycast.h

    src/vertexbuffer.cpp
    src/vertexbuffer.h
    src/indexbuffer.cpp
    src/indexbuffer.h
    src/vertexarray.cpp
    src/vertexarray.h
    src/bufferlayout.h
    src/shader.cpp
    src/shader.h
    src/mesh.cpp
    src/mesh.h
    src/texture.cpp
    src/texture.h
    src/frustum.cpp
    src/frustum.h

    src/noises/opensimplexnoise.cpp
    src/noises/opensimplexnoise.h

    deps/glad/src/glad.c
    deps/stb_image/stb_image.c
)

target_link_libraries(game PRIVATE glfw)

target_include_directories(game PRIVATE
    deps/glad/include
    deps/glfw/include
    deps/glm
    deps/stb_image/include
    deps/FastNoiseLite
)

add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:game>/assets
)