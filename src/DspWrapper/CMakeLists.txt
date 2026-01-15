set(ENT_DSP_WRAPPER_HEADERS
    ${ENT_DSP_WRAPPER_DIR}/DspTypes.h
    ${ENT_DSP_WRAPPER_DIR}/DspFrameTimer.h
    ${ENT_DSP_WRAPPER_DIR}/DspWrapperNoise.h
    ${ENT_DSP_WRAPPER_DIR}/DspWrapperCrackle.h
    ${ENT_DSP_WRAPPER_DIR}/DspWrapperGlitch.h
    ${ENT_DSP_WRAPPER_DIR}/DspWrapperPitch.h
    ${ENT_DSP_WRAPPER_DIR}/DspWrapper.h)

set(ENT_DSP_WRAPPER_SOURCES
    ${ENT_DSP_WRAPPER_DIR}/DspFrameTimer.cpp
    ${ENT_DSP_WRAPPER_DIR}/DspWrapperNoise.cpp
    ${ENT_DSP_WRAPPER_DIR}/DspWrapperCrackle.cpp
    ${ENT_DSP_WRAPPER_DIR}/DspWrapperGlitch.cpp
    ${ENT_DSP_WRAPPER_DIR}/DspWrapperPitch.cpp
    ${ENT_DSP_WRAPPER_DIR}/DspWrapper.cpp)

include_directories(${ENT_DSP_WRAPPER_DIR})
include_directories(${ENT_COMMON_DIR})

add_library(dsp_wrapper STATIC
		${ENT_DSP_WRAPPER_HEADERS}
		${ENT_DSP_WRAPPER_SOURCES})
add_dependencies(dsp_wrapper dsp_plugin)
target_link_libraries(dsp_wrapper PRIVATE dsp_plugin)
