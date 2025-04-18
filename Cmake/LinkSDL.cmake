include(FetchContent)

function(linkSDL)

    set(FETCHCONTENT_QUIET OFF)

    include(FetchContent)

    FetchContent_Declare(
            glm
            GIT_REPOSITORY https://github.com/g-truc/glm
            GIT_TAG 1.0.1
            GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(glm)

    FetchContent_Declare(
            fmt
            GIT_REPOSITORY https://github.com/fmtlib/fmt.git
            GIT_TAG 10.2.1
            GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(fmt)

    if(WIN32)

        set(SDL_STATIC OFF)
        FetchContent_Declare(
                sdl2
                GIT_REPOSITORY https://github.com/libsdl-org/SDL
                GIT_TAG release-2.30.2
                GIT_SHALLOW TRUE
                OVERRIDE_FIND_PACKAGE TRUE)
        FetchContent_MakeAvailable(sdl2)

        # Windows requires building FreeType
        set(SDL2TTF_VENDORED ON)

        FetchContent_Declare(
                sdl2-ttf
                GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
                GIT_TAG release-2.22.0
                GIT_SHALLOW TRUE
                OVERRIDE_FIND_PACKAGE TRUE)
        FetchContent_MakeAvailable(sdl2-ttf)


        set(SDL2IMAGE_BMP ON)
        set(SDL2IMAGE_GIF OFF)
        set(SDL2IMAGE_JPG ON)
        set(SDL2IMAGE_PNG ON)


        set(SDL2IMAGE_VENDORED ON)
        FetchContent_Declare(
                sdl2-image
                GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
                GIT_TAG release-2.8.2
                GIT_SHALLOW TRUE
                OVERRIDE_FIND_PACKAGE TRUE)
        FetchContent_MakeAvailable(sdl2-image)

    endif()

endfunction()