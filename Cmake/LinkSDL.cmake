include(FetchContent)

macro(linkSDL TARGET_NAME ACCESS)
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

    target_include_directories(${TARGET_NAME} ${ACCESS}
            ${SDL2_INCLUDE_DIR}
            ${SDL2_IMAGE_INCLUDE_DIR}
            ${SDL2_TTF_INCLUDE_DIR}
            ${SDL2_MIXER_INCLUDE_DIR}
    )
    target_link_libraries(${TARGET_NAME} ${ACCESS}
            ${SDL2_LIBRARIES}
            ${SDL2_IMAGE_LIBRARIES}
            ${SDL2_TTF_LIBRARIES}
            ${SDL2_MIXER_LIBRARIES}
    )

endmacro()