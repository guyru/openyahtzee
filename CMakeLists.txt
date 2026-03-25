cmake_minimum_required(VERSION 3.16)
project(openyahtzee VERSION 1.9.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

# Options
option(PORTABLE "Build portable edition (config stored alongside executable)" OFF)

# Dependencies
find_package(wxWidgets 3.2 REQUIRED COMPONENTS core base adv)
include(${wxWidgets_USE_FILE})

add_subdirectory(src)

# Install targets (Linux/Unix only)
if(NOT WIN32)
    install(FILES icons/openyahtzee.png DESTINATION ${CMAKE_INSTALL_DATADIR}/pixmaps)
    install(FILES openyahtzee.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
    install(FILES man/openyahtzee.6 DESTINATION ${CMAKE_INSTALL_MANDIR}/man6)
endif()

# i18n - compile .po files
find_package(Gettext)
if(GETTEXT_FOUND)
    file(GLOB PO_FILES ${CMAKE_SOURCE_DIR}/po/*.po)
    foreach(PO_FILE ${PO_FILES})
        get_filename_component(LANG ${PO_FILE} NAME_WE)
        gettext_process_po_files(${LANG} ALL PO_FILES ${PO_FILE})
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${LANG}.gmo
                DESTINATION ${CMAKE_INSTALL_LOCALEDIR}/${LANG}/LC_MESSAGES
                RENAME openyahtzee.mo)
    endforeach()
endif()
