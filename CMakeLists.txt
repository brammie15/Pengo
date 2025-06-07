cmake_minimum_required(VERSION 3.26)
project(Fovy)
set(TARGET_NAME Fovy)
set(TARGET_PARAM WIN32)

# list source files here
add_library(${TARGET_NAME}
        Minigin/Font.cpp
        Minigin/Font.h
        Minigin/ObjectModel/GameObject.cpp
        Minigin/ObjectModel/GameObject.h
        Minigin/Input/InputManager.cpp
        Minigin/Input/InputManager.h
        Minigin/Minigin.cpp
        Minigin/Minigin.h
        Minigin/Managers/Renderer.cpp
        Minigin/Managers/Renderer.h
        Minigin/Managers/ResourceManager.cpp
        Minigin/Managers/ResourceManager.h
        Minigin/Scene/Scene.cpp
        Minigin/Scene/Scene.h
        Minigin/Scene/SceneManager.cpp
        Minigin/Scene/SceneManager.h
        Minigin/Texture2D.cpp
        Minigin/Texture2D.h
        Minigin/ObjectModel/Transform.cpp
        Minigin/ObjectModel/Transform.h
        Minigin/Timer.cpp
        Minigin/Timer.h
        Minigin/ObjectModel/Object.cpp
        Minigin/ObjectModel/Object.h
        Minigin/ObjectModel/Component.cpp
        Minigin/ObjectModel/Component.h
        Game/Components/TextComponent.cpp
        Game/Components/TextComponent.h
        Game/Components/TextureComponent.cpp
        Game/Components/TextureComponent.h
        Game/Components/FPSComponent.cpp
        Game/Components/FPSComponent.h
        Game/Components/Rotator.cpp
        Game/Components/Rotator.h
        Minigin/Command.cpp
        Minigin/Command.h
        Minigin/Input/InputAction.cpp
        Minigin/Input/InputAction.h
        Minigin/Input/InputBinding.cpp
        Minigin/Input/InputBinding.h
        Minigin/Input/SDLInputControllerImpl.cpp
        Minigin/Event.cpp
        Minigin/Event.h
        Game/Components/HealthComponent.cpp
        Game/Components/HealthComponent.h
        Game/Components/LivesDisplayComponent.cpp
        Game/Components/LivesDisplayComponent.h
        Game/Components/ScoreDisplayComponent.cpp
        Game/Components/ScoreDisplayComponent.h
        Game/Components/ScoreComponent.cpp
        Game/Components/ScoreComponent.h
        Minigin/Managers/AchievementManager.cpp
        Minigin/Managers/AchievementManager.h

        Minigin/Audio/ISoundSystem.h
        Minigin/Audio/SDLSoundSystem.h
        Minigin/Audio/SDLSoundSystem.cpp
        Minigin/ServiceLocator.h
        Minigin/Audio/SoundLogSystem.h
        Minigin/Audio/SoundLogSystem.cpp
)

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

include(FetchContent)

FetchContent_Declare(
        glm
        URL https://github.com/g-truc/glm/releases/download/1.0.1/glm-1.0.1-light.zip
        DOWNLOAD_NO_PROGRESS ON
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)

FetchContent_MakeAvailable(glm)

# add SDL2
target_link_libraries(${TARGET_NAME} PUBLIC xinput)

include(Cmake/LinkSDL.cmake)
linkSDL(${TARGET_NAME} PUBLIC)

include(Cmake/LinkImgui.cmake)
LinkImGui(${TARGET_NAME} PUBLIC)

find_package(VLD CONFIG)

if (NOT VLD_FOUND)
    message("VLD NOT FOUND!")
endif ()


# set all include directories
target_include_directories(${TARGET_NAME} PUBLIC
        ${VLD_INCLUDE_DIR}
        ${glm_SOURCE_DIR}
)

# set libraries to link with
target_link_libraries(${TARGET_NAME} PUBLIC
        ${VLD_LIBRARY}
)

target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Minigin/
)


target_compile_options(${TARGET_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")

# add post build commands
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_IMAGE_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_TTF_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_MIXER_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
)

include(Game/Game.cmake)
