set(ENT_PRESETS Entropictron.tar.xz )

string(TOLOWER "${PROJECT_NAME}" PROJECT_NAME_LOWER)

set(ENT_PRESETS_OUTPUT "")

foreach(compressedFolder ${ENT_PRESETS})
   message(STATUS "Uncompress file: ${compressedFolder}")

   get_filename_component(presetsFolder "${compressedFolder}" NAME_WE)

   set(outputDir "${CMAKE_BINARY_DIR}/presets/${presetsFolder}")

    add_custom_command(
        OUTPUT "${outputDir}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${outputDir}"
        COMMAND ${CMAKE_COMMAND} -E tar xvf
        "${ENT_PRESETS_DIR}/${compressedFolder}"
        --directory="${outputDir}"
        COMMENT "Extracting ${compressedFolder}"
        VERBATIM
   )

   list(APPEND ENT_PRESETS_OUTPUT "${outputDir}")
endforeach()

add_custom_target(gkick_presets ALL
    DEPENDS ${ENT_PRESETS_OUTPUT})

install(DIRECTORY ${ENT_PRESETS_OUTPUT}
        DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/${PROJECT_NAME_LOWER}/presets)
