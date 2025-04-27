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
        Minigin/Input/XInputControllerImpl.cpp
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
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG docking # Use the 'docking' branch
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui.h
        ${imgui_SOURCE_DIR}/imconfig.h
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_internal.h
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imstb_rectpack.h
        ${imgui_SOURCE_DIR}/imstb_textedit.h
        ${imgui_SOURCE_DIR}/imstb_truetype.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.h
)


target_sources(imgui PUBLIC
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

FetchContent_Declare(
        SDL2
        URL https://www.libsdl.org/release/SDL2-devel-2.30.10-VC.zip
        DOWNLOAD_NO_PROGRESS ON
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)
FetchContent_GetProperties(SDL2)
if (NOT SDL2_POPULATED)
    FetchContent_MakeAvailable(SDL2)
    set(SDL2_INCLUDE_DIR ${sdl2_SOURCE_DIR}/include)
    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x64/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x64/SDL2main.lib")
        set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x64/SDL2.dll")
    else ()
        set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x86/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x86/SDL2main.lib")
        set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x86/SDL2.dll")
    endif ()
endif ()

# add SDL2_image
FetchContent_Declare(
        SDL2_image
        URL https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.8.3-VC.zip
        DOWNLOAD_NO_PROGRESS ON
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)
FetchContent_GetProperties(SDL2_image)
if (NOT SDL2_image_POPULATED)
    FetchContent_MakeAvailable(SDL2_image)
    set(SDL2_IMAGE_INCLUDE_DIR ${sdl2_image_SOURCE_DIR}/include)
    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.lib")
        set(SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.dll")
    else ()
        set(SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.lib")
        set(SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.dll")
    endif ()
endif ()

# add SDL2_ttf
FetchContent_Declare(
        SDL2_ttf
        URL https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.22.0-VC.zip
        DOWNLOAD_NO_PROGRESS ON
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)
FetchContent_GetProperties(SDL2_ttf)
if (NOT sdl2_ttf_POPULATED)
    FetchContent_MakeAvailable(SDL2_ttf)
    set(SDL2_TTF_INCLUDE_DIR ${sdl2_ttf_SOURCE_DIR}/include)
    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.lib)
        set(SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.dll")
    else ()
        set(SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.lib)
        set(SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.dll")
    endif ()
endif ()


# add SDL2_mixer
FetchContent_Declare(
        SDL2_mixer
        URL https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.8.1-VC.zip
        DOWNLOAD_NO_PROGRESS ON
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)
FetchContent_GetProperties(SDL2_mixer)
if (NOT sdl2_mixer_POPULATED)
    FetchContent_MakeAvailable(SDL2_mixer)
    set(SDL2_MIXER_INCLUDE_DIR ${sdl2_mixer_SOURCE_DIR}/include)
    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(SDL2_MIXER_LIBRARIES ${sdl2_mixer_SOURCE_DIR}/lib/x64/SDL2_mixer.lib)
        set(SDL2_MIXER_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x64/SDL2_mixer.dll")
    else ()
        set(SDL2_MIXER_LIBRARIES ${sdl2_mixer_SOURCE_DIR}/lib/x86/SDL2_mixer.lib)
        set(SDL2_MIXER_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x86/SDL2_mixer.dll")
    endif ()
endif ()


target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${SDL2_INCLUDE_DIR})
target_link_libraries(imgui PRIVATE
        ${SDL2_LIBRARIES}
)

find_package(VLD CONFIG)

if (NOT VLD_FOUND)
    message("VLD NOT FOUND!")
endif ()


# set all include directories
target_include_directories(${TARGET_NAME} PUBLIC
        ${VLD_INCLUDE_DIR}
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR}
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
        ${glm_SOURCE_DIR}
)

# set libraries to link with
target_link_libraries(${TARGET_NAME} PUBLIC
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${VLD_LIBRARY}
)
target_link_libraries(${TARGET_NAME} PUBLIC imgui)

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

set(GAME_NAME "Pengo")

add_executable(${GAME_NAME} WIN32
        Game/main.cpp
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

        Game/Components/ScoreComponent.h
        Game/Components/ScoreComponent.cpp
)  # Your main game file

target_include_directories(${GAME_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Minigin/
        ${CMAKE_SOURCE_DIR}/Game/
)

target_compile_options(${GAME_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${GAME_NAME})

# Link Pengo as a library
target_link_libraries(${GAME_NAME} PRIVATE Fovy)
