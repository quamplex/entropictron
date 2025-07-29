set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")

set(ENT_API_HEADERS
	${ENT_API_DIR}/src/audio_output.h
	${ENT_API_DIR}/src/distortion.h
	${ENT_API_DIR}/src/envelope.h
	${ENT_API_DIR}/src/filter.h
	${ENT_API_DIR}/src/entropictron.h
	${ENT_API_DIR}/src/entropictron_internal.h
	${ENT_API_DIR}/src/ent_audio.h
        ${ENT_API_DIR}/src/mixer.h
	${ENT_API_DIR}/src/ent_buffer.h
        ${ENT_API_DIR}/src/ring_buffer.h
	${ENT_API_DIR}/src/ent_log.h
	${ENT_API_DIR}/src/oscillator.h
	${ENT_API_DIR}/src/synthesizer.h
        ${ENT_API_DIR}/src/worker.h)

if (ENT_STANDALONE)
  if (CMAKE_SYSTEM_NAME MATCHES Windows)
  	message(STATUS "No Jack implementation for Windows")
  else()
        list(APPEND ENT_API_HEADERS ${ENT_API_DIR}/src/ent_jack.h)
  endif()
endif(ENT_STANDALONE)

set(ENT_API_SOURCES
	${ENT_API_DIR}/src/audio_output.c
	${ENT_API_DIR}/src/distortion.c
	${ENT_API_DIR}/src/envelope.c
	${ENT_API_DIR}/src/filter.c
	${ENT_API_DIR}/src/entropictron.c
	${ENT_API_DIR}/src/ent_audio.c
        ${ENT_API_DIR}/src/mixer.c
	${ENT_API_DIR}/src/ent_buffer.c
        ${ENT_API_DIR}/src/ring_buffer.c
	${ENT_API_DIR}/src/ent_log.c
	${ENT_API_DIR}/src/oscillator.c
	${ENT_API_DIR}/src/synthesizer.c
        ${ENT_API_DIR}/src/worker.c)

if (ENT_STANDALONE)
  if (CMAKE_SYSTEM_NAME MATCHES Windows)
  	message(STATUS "No Jack implementation for Windows")
  else()
       list(APPEND ENT_API_SOURCES ${ENT_API_DIR}/src/ent_jack.c)
  endif()
endif(ENT_STANDALONE)

include_directories(${ENT_API_DIR}/src)

if (ENT_STANDALONE)
	add_library(api_standalone STATIC
		${ENT_API_HEADERS}
		${ENT_API_SOURCES})
        target_compile_options(api_standalone PUBLIC ${ENT_API_FLAGS})
endif (ENT_STANDALONE)

if (ENT_PLUGIN)
   	add_library(api_plugin STATIC
		${ENT_API_HEADERS}
		${ENT_API_SOURCES})
	target_compile_options(api_plugin PRIVATE ${ENT_API_PLUGIN_FLAGS})
endif (ENT_PLUGIN)

