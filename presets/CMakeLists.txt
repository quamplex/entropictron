string(TOLOWER "${PROJECT_NAME}" PROJECT_NAME_LOWER)

set(ENT_FACTORY_PRESETS
    "${CMAKE_CURRENT_SOURCE_DIR}/EntropictronPresets.tar.xz"
)

set(outputDir "${CMAKE_BINARY_DIR}/presets")
set(stampFile "${outputDir}/.entropictron_presets_extracted")
set(factoryFile "${outputDir}/EntropictronPresets/factory.txt")

message(STATUS "Uncompress file: ${ENT_FACTORY_PRESETS}")

add_custom_command(
    OUTPUT "${stampFile}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${outputDir}"
    COMMAND ${CMAKE_COMMAND} -E chdir "${outputDir}"
            ${CMAKE_COMMAND} -E tar xvf "${ENT_FACTORY_PRESETS}"
    COMMAND ${CMAKE_COMMAND} -E touch "${factoryFile}"
    COMMAND ${CMAKE_COMMAND} -E touch "${stampFile}"
    COMMENT "Extracting ${ENT_FACTORY_PRESETS}"
    VERBATIM
)

add_custom_target(entropictron_presets ALL
    DEPENDS "${stampFile}"
)

install(
    DIRECTORY "${outputDir}/EntropictronPresets/"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME_LOWER}/presets"
)
