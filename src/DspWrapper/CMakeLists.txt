set(ENT_DSP_WRAPPER_HEADERS
    ${ENT_DSP_WRAPPER_DIR}/DspWrapper.h)

set(ENT_DSP_WRAPPER_SOURCES
    ${ENT_DSP_WRAPPER_DIR}/DspWrapper.cpp)

include_directories(${ENT_DSP_WRAPPER_DIR})

add_library(dsp_wrapper STATIC
		${ENT_DSP_WRAPPER_HEADERS}
		${ENT_DSP_WRAPPER_SOURCES})
add_dependencies(dsp_wrapper dsp_plugin)
