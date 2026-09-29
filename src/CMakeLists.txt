# Rasterize the SVG icon and embed the resulting PNG as a C header
find_program(RSVG_CONVERT rsvg-convert REQUIRED)
set(ICON_SVG "${CMAKE_SOURCE_DIR}/icons/openyahtzee.svg")
set(ICON_PNG "${CMAKE_CURRENT_BINARY_DIR}/openyahtzee.png")
add_custom_command(
    OUTPUT "${ICON_PNG}"
    COMMAND ${RSVG_CONVERT} -w 256 -h 256 -o "${ICON_PNG}" "${ICON_SVG}"
    DEPENDS "${ICON_SVG}"
    COMMENT "Rendering openyahtzee.png from SVG"
)

set(ICON_HEADER "${CMAKE_CURRENT_BINARY_DIR}/openyahtzee_png.h")
add_custom_command(
    OUTPUT "${ICON_HEADER}"
    COMMAND ${CMAKE_COMMAND}
        -DINPUT_FILE=${ICON_PNG}
        -DOUTPUT_FILE=${ICON_HEADER}
        -DVAR_NAME=openyahtzee_png
        -P ${CMAKE_SOURCE_DIR}/cmake/bin2h.cmake
    DEPENDS "${ICON_PNG}" "${CMAKE_SOURCE_DIR}/cmake/bin2h.cmake"
    COMMENT "Embedding openyahtzee.png as C header"
)

add_executable(openyahtzee
    about.cpp
    configuration.cpp
    highscores_dialog.cpp
    MainFrame.cpp
    openyahtzee.cpp
    ScoreDice.cpp
    settings_dialog.cpp
    simple_pie_plot.cpp
    statistics.cpp
    statistics_dialog.cpp
    wxDynamicBitmap.cpp
    "${ICON_HEADER}"
)

target_include_directories(openyahtzee PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

target_compile_definitions(openyahtzee PRIVATE
    VERSION="${PROJECT_VERSION}"
    PACKAGE_NAME="${PROJECT_NAME}"
)

if(NOT WIN32)
    target_compile_definitions(openyahtzee PRIVATE
        LOCALEDIR="${CMAKE_INSTALL_FULL_LOCALEDIR}"
    )
endif()

if(PORTABLE)
    target_compile_definitions(openyahtzee PRIVATE PORTABLE)
endif()

target_link_libraries(openyahtzee PRIVATE ${wxWidgets_LIBRARIES})

if(WIN32)
    find_program(MAGICK convert REQUIRED)
    set(ICON_ICO "${CMAKE_CURRENT_BINARY_DIR}/openyahtzee.ico")
    add_custom_command(
        OUTPUT "${ICON_ICO}"
        COMMAND ${MAGICK} "${ICON_PNG}" -define icon:auto-resize=256,48,32,16 "${ICON_ICO}"
        DEPENDS "${ICON_PNG}"
        COMMENT "Generating openyahtzee.ico from PNG"
    )
    configure_file(openyahtzee.rc "${CMAKE_CURRENT_BINARY_DIR}/openyahtzee.rc" COPYONLY)
    target_sources(openyahtzee PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/openyahtzee.rc" "${ICON_ICO}")
    target_link_options(openyahtzee PRIVATE -static)
endif()

if(WIN32)
    install(TARGETS openyahtzee DESTINATION .)
else()
    install(TARGETS openyahtzee DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
