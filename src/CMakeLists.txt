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
)

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
    target_sources(openyahtzee PRIVATE openyahtzee.rc)
    target_link_options(openyahtzee PRIVATE -static)
endif()

install(TARGETS openyahtzee DESTINATION ${CMAKE_INSTALL_BINDIR})
