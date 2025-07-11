set(GAME_NAME "Pengo")

add_executable(${GAME_NAME} WIN32
        Game/main.cpp

        Game/ScoreManager.h Game/ScoreManager.cpp

        Game/GameCommands.h Game/GameCommands.cpp

        Game/Components/FPSComponent.h Game/Components/FPSComponent.cpp

        Game/Components/Rotator.h Game/Components/Rotator.cpp

        Game/Components/HealthComponent.h Game/Components/HealthComponent.cpp

        Game/Components/LivesDisplayComponent.h Game/Components/LivesDisplayComponent.cpp

        Game/Components/ScoreDisplayComponent.h Game/Components/ScoreDisplayComponent.cpp

        Game/Components/ScoreComponent.h Game/Components/ScoreComponent.cpp

        Game/Components/GridComponent.h Game/Components/GridComponent.cpp

        Game/Components/Pengo/PengoComponent.h Game/Components/Pengo/PengoComponent.cpp

        Game/Components/Tile/TileComponent.cpp Game/Components/Tile/TileComponent.h

        Game/Components/Tile/IceBlockComponent.cpp Game/Components/Tile/IceBlockComponent.h

        Game/Components/Tile/IceblockStates.cpp Game/Components/Tile/IceblockStates.h

        Game/Components/Pengo/PengoStates.cpp Game/Components/Pengo/PengoStates.h

        Game/Components/SnoBee/SnoBeeComponent.h Game/Components/SnoBee/SnoBeeComponent.cpp

        Game/Components/SnoBee/SnoBeeStates.cpp Game/Components/SnoBee/SnoBeeStates.h

        Game/Components/UI/LetterRoller.h Game/Components/UI/LetterRoller.cpp

        Game/Components/UI/MainMenuUiManager.h Game/Components/UI/MainMenuUiManager.cpp

        Game/Components/ScoreScreenManager.h Game/Components/ScoreScreenManager.cpp

        Game/Components/SnoBee/SnoBeeController.h Game/Components/SnoBee/SnoBeeController.cpp

        Game/Components/MainGameController.h Game/Components/MainGameController.cpp

        Game/GameController.h Game/GameController.cpp

        Game/Components/PointsCollectedText.h Game/Components/PointsCollectedText.cpp

        Game/Prefabs.h Game/Prefabs.cpp

        Game/Components/Tile/WallComponent.h Game/Components/Tile/WallComponent.cpp

        Game/Components/EggDisplay.h Game/Components/EggDisplay.cpp

        Game/Components/PengoSlider.h Game/Components/PengoSlider.cpp
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
