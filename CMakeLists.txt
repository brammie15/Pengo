cmake_minimum_required(VERSION 3.26)
project(Fovy)
set(TARGET_NAME Fovy)
set(TARGET_PARAM WIN32)

add_library(${TARGET_NAME}
        Minigin/Minigin.h Minigin/Minigin.cpp
        Minigin/Event.h Minigin/Event.cpp
        Minigin/Command.h Minigin/Command.cpp
        Minigin/Timer.h Minigin/Timer.cpp
        Minigin/Singleton.h
        Minigin/ServiceLocator.h

        Minigin/Audio/ISoundSystem.h
        Minigin/Audio/SDLSoundSystem.h Minigin/Audio/SDLSoundSystem.cpp
        Minigin/Audio/SoundLogSystem.h Minigin/Audio/SoundLogSystem.cpp

        Minigin/Input/InputAction.h Minigin/Input/InputAction.cpp
        Minigin/Input/InputBinding.h Minigin/Input/InputBinding.cpp
        Minigin/Input/InputManager.h Minigin/Input/InputManager.cpp
        Minigin/Input/SDLInputControllerImpl.cpp
#        Minigin/Input/XInputControllerImpl.cpp

        Minigin/Managers/ResourceManager.h Minigin/Managers/ResourceManager.cpp
        Minigin/Managers/Renderer.h Minigin/Managers/Renderer.cpp

        Minigin/ObjectModel/Object.h Minigin/ObjectModel/Object.cpp
        Minigin/ObjectModel/Component.h Minigin/ObjectModel/Component.cpp
        Minigin/ObjectModel/Transform.h Minigin/ObjectModel/Transform.cpp
        Minigin/ObjectModel/GameObject.h Minigin/ObjectModel/GameObject.cpp

        Minigin/Resources/Font.h Minigin/Resources/Font.cpp
        Minigin/Resources/Texture2D.h Minigin/Resources/Texture2D.cpp

        Minigin/Scene/Scene.h Minigin/Scene/Scene.cpp
        Minigin/Scene/SceneManager.h Minigin/Scene/SceneManager.cpp


        Minigin/Components/SpriteRenderer.h Minigin/Components/SpriteRenderer.cpp
        Minigin/Components/TextComponent.h Minigin/Components/TextComponent.cpp
        Minigin/Components/TextureComponent.h Minigin/Components/TextureComponent.cpp

        Minigin/UI/Canvas.h Minigin/UI/Canvas.cpp
        Minigin/UI/Focusable.h Minigin/UI/Focusable.cpp

        Minigin/UI/Button.h Minigin/UI/Button.cpp
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
