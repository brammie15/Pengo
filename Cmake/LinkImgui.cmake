include(FetchContent)

macro(LinkImGui TARGET ACCESS)
    FetchContent_Declare(
            imgui
            GIT_REPOSITORY https://github.com/ocornut/imgui.git
            GIT_TAG docking # Use the 'docking' branch
    )

    # Fetch and make ImGui available
    FetchContent_MakeAvailable(imgui)

    add_library(imgui STATIC
            ${imgui_SOURCE_DIR}/imgui.cpp
            ${imgui_SOURCE_DIR}/imgui_demo.cpp
            ${imgui_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_SOURCE_DIR}/imgui_tables.cpp
            ${imgui_SOURCE_DIR}/imgui_widgets.cpp

            ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )

    target_link_libraries(${TARGET} ${ACCESS} imgui)

    target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
    set_target_properties(imgui PROPERTIES CXX_STANDARD 20)


    include(Cmake/LinkSDL.cmake)
    # Link SDL2 with the imgui target
    linkSDL(imgui PUBLIC)

endmacro()