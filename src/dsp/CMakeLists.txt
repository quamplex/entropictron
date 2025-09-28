set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")

set(ENT_DSP_HEADERS
#	${ENT_DSP_DIR}/src/audio_output.h
#	${ENT_DSP_DIR}/src/distortion.h
#	${ENT_DSP_DIR}/src/envelope.h
#	${ENT_DSP_DIR}/src/filter.h
	${ENT_DSP_DIR}/src/entropictron.h
#	${ENT_DSP_DIR}/src/entropictron_internal.h
#	${ENT_DSP_DIR}/src/ent_audio.h
#        ${ENT_DSP_DIR}/src/mixer.h
#	${ENT_DSP_DIR}/src/ent_buffer.h
#        ${ENT_DSP_DIR}/src/ring_buffer.h
	${ENT_DSP_DIR}/src/ent_log.h
#	${ENT_DSP_DIR}/src/oscillator.h
#	${ENT_DSP_DIR}/src/synthesizer.h
#        ${ENT_DSP_DIR}/src/worker.h
        )

set(ENT_DSP_SOURCES
#	${ENT_DSP_DIR}/src/audio_output.c
#	${ENT_DSP_DIR}/src/distortion.c
#	${ENT_DSP_DIR}/src/envelope.c
#	${ENT_DSP_DIR}/src/filter.c
	${ENT_DSP_DIR}/src/entropictron.c
#	${ENT_DSP_DIR}/src/ent_audio.c
#        ${ENT_DSP_DIR}/src/mixer.c
#	${ENT_DSP_DIR}/src/ent_buffer.c
#        ${ENT_DSP_DIR}/src/ring_buffer.c
	${ENT_DSP_DIR}/src/ent_log.c
#	${ENT_DSP_DIR}/src/oscillator.c
#	${ENT_DSP_DIR}/src/synthesizer.c
#        ${ENT_DSP_DIR}/src/worker.c
        )

include_directories(${ENT_DSP_DIR}/src)

if (ENT_PLUGIN)
   	add_library(dsp_plugin STATIC
		${ENT_DSP_HEADERS}
		${ENT_DSP_SOURCES})
	target_compile_options(dsp_plugin PRIVATE ${ENT_DSP_PLUGIN_FLAGS})
endif (ENT_PLUGIN)

