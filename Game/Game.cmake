set(GAME_NAME "Pengo")

add_executable(${GAME_NAME} WIN32
        Game/main.cpp

        Game/GameCommands.h
        Game/GameCommands.cpp

        Game/Components/FPSComponent.cpp
        Game/Components/FPSComponent.h

        Game/Components/Rotator.cpp
        Game/Components/Rotator.h

        Game/Components/TextureComponent.cpp
        Game/Components/TextureComponent.h

        Game/Components/TextComponent.cpp
        Game/Components/TextComponent.h

        Game/Components/HealthComponent.cpp
        Game/Components/HealthComponent.h

        Game/Components/LivesDisplayComponent.cpp
        Game/Components/LivesDisplayComponent.h

        Game/Components/ScoreDisplayComponent.cpp
        Game/Components/ScoreDisplayComponent.h

        Game/Components/ScoreComponent.cpp
        Game/Components/ScoreComponent.h

        Game/Components/GridComponent.cpp
        Game/Components/GridComponent.h

        Game/Components/Pengo/PengoComponent.cpp
        Game/Components/Pengo/PengoComponent.h

        Game/Components/SpriteRenderer.cpp
        Game/Components/SpriteRenderer.h

        Game/Components/Tile/TileComponent.h
        Game/Components/Tile/TileComponent.cpp

        Game/Components/Tile/IceBlockComponent.h
        Game/Components/Tile/IceBlockComponent.cpp

        Game/Components/Tile/IceblockStates.h
        Game/Components/Tile/IceblockStates.cpp

        Game/Components/Pengo/PengoStates.h
        Game/Components/Pengo/PengoStates.cpp
)

target_include_directories(${GAME_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Minigin/
        ${CMAKE_SOURCE_DIR}/Game/
)

target_compile_options(${GAME_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

target_link_libraries(${GAME_NAME} PRIVATE Fovy)

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${GAME_NAME})
