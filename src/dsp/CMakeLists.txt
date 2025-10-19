set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")

set(QUAMPLEX_DSP_TOOLS_PATH ${ENT_DSP_DIR}/src/quamplex_dsp_tools)

set(ENT_DSP_HEADERS
    ${QUAMPLEX_DSP_TOOLS_PATH}/qx_math.h
    ${QUAMPLEX_DSP_TOOLS_PATH}/qx_fader.h
    ${QUAMPLEX_DSP_TOOLS_PATH}/qx_randomizer.h
    ${ENT_DSP_DIR}/src/ent_shelf_filter.h
    ${ENT_DSP_DIR}/src/ent_noise.h
    ${ENT_DSP_DIR}/src/ent_crackle.h
    ${ENT_DSP_DIR}/src/ent_glitch.h
    ${ENT_DSP_DIR}/src/entropictron.h
    ${ENT_DSP_DIR}/src/ent_log.h)

set(ENT_DSP_SOURCES
    ${ENT_DSP_DIR}/src/ent_shelf_filter.c
    ${ENT_DSP_DIR}/src/ent_noise.c
    ${ENT_DSP_DIR}/src/ent_crackle.c
    ${ENT_DSP_DIR}/src/entropictron.c
    ${ENT_DSP_DIR}/src/ent_glitch.c
    ${ENT_DSP_DIR}/src/ent_log.c)

include_directories(${QUAMPLEX_DSP_TOOLS_PATH})
include_directories(${ENT_DSP_DIR}/src)

if (ENT_PLUGIN)
   	add_library(dsp_plugin STATIC
		${ENT_DSP_HEADERS}
		${ENT_DSP_SOURCES})
	target_compile_options(dsp_plugin PRIVATE ${ENT_DSP_PLUGIN_FLAGS})
endif (ENT_PLUGIN)

