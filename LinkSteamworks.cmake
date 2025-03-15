function(use_steamworks ENGINE_TARGET)
    set(STEAMWORKS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/steamworks")

    if(NOT EXISTS "${STEAMWORKS_DIR}")
        message(
                FATAL_ERROR
                "can't find steamworks folder numnut"
        )
    else()
        file(GLOB STEAMWORKS_FILES "${STEAMWORKS_DIR}/*")
        if(NOT STEAMWORKS_FILES)
            message(
                    FATAL_ERROR
                    "Steamworks directory ${STEAMWORKS_DIR} is empty fucker"
            )
        endif()
    endif()

    set(STEAMWORKS_INCLUDE_DIR
            ${CMAKE_CURRENT_SOURCE_DIR}/external/steamworks/public/steam)
    set(STEAMWORKS_LIB_DIR
            ${CMAKE_CURRENT_SOURCE_DIR}/external/steamworks/redistributable_bin)

    if(WIN32)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(STEAMWORKS_LIB ${STEAMWORKS_LIB_DIR}/win64/steam_api64.lib)

            add_custom_command(
                    TARGET ${ENGINE_TARGET}
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    ${STEAMWORKS_LIB_DIR}/win64/steam_api64.dll ${CMAKE_BINARY_DIR})

        else()
            set(STEAMWORKS_LIB ${STEAMWORKS_LIB_DIR}/steam_api.lib)

            add_custom_command(
                    TARGET ${ENGINE_TARGET}
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy ${STEAMWORKS_LIB_DIR}/steam_api.dll
                    ${CMAKE_BINARY_DIR})
        endif()
    endif()

    target_include_directories(${ENGINE_TARGET} PUBLIC ${STEAMWORKS_INCLUDE_DIR})
    target_link_libraries(${ENGINE_TARGET} PUBLIC ${STEAMWORKS_LIB})

    target_compile_definitions(${ENGINE_TARGET} PUBLIC USE_STEAMWORKS)
endfunction()