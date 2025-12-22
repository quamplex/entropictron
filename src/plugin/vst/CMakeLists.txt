set(ENT_VST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SMTG_ENABLE_VST3_HOSTING_EXAMPLES OFF CACHE BOOL "disable hosting examples")
set(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES OFF CACHE BOOL "disable plugin examples")
set(SMTG_ENABLE_VSTGUI_SUPPORT OFF CACHE BOOL "disable vstgui examples")

list(APPEND CMAKE_MODULE_PATH "${ENT_VST_SDK_PATH}/cmake/modules")

include_directories(${ENT_VST_SDK_PATH})
include_directories(${ENT_REDKITE_SDK_PATH}/include)
include_directories(${ENT_DSP_WRAPPER_DIR})
link_directories(${ENT_REDKITE_SDK_PATH}/lib)

get_filename_component(VST3SDK_FOLDER_NAME ${ENT_VST_SDK_PATH} NAME)
if(NAME_FROM_PATH STREQUAL "" AND (VST3SDK_FOLDER_NAME STREQUAL "." OR VST3SDK_FOLDER_NAME STREQUAL ".."))
    message(FATAL_ERROR "The folder name '${VST3SDK_FOLDER_NAME}' of VST3 SDK is invalid or empty.")
else()
    link_directories(${CMAKE_BINARY_DIR}/${VST3SDK_FOLDER_NAME}/build/lib)
endif()

add_subdirectory(${ENT_VST_SDK_PATH} EXTERN)

set(ENT_VST_HEADERS
  ${ENT_VST_SDK_PATH}/public.sdk/source/vst/vstcomponentbase.h
  ${ENT_VST_SDK_PATH}/public.sdk/source/vst/vstaudioeffect.h
  ${ENT_VST_DIR}/VstIds.h
  ${ENT_VST_DIR}/DspVstProxy.h
  ${ENT_VST_DIR}/DspNoiseProxyVst.h
  ${ENT_VST_DIR}/DspCrackleProxyVst.h
  ${ENT_VST_DIR}/DspGlitchProxyVst.h
  ${ENT_VST_DIR}/EntVstParameters.h
  ${ENT_VST_DIR}/EntVstProcessor.h
  ${ENT_VST_DIR}/EntVstController.h
  ${ENT_VST_DIR}/EntVstPluginView.h)


set(ENT_VST_SOURCES
  ${ENT_VST_SDK_PATH}/public.sdk/source/vst/vstcomponentbase.cpp
  ${ENT_VST_SDK_PATH}/public.sdk/source/vst/vstaudioeffect.cpp
  ${ENT_VST_DIR}/VstFactory.cpp
  ${ENT_VST_DIR}/DspNoiseProxyVst.cpp
  ${ENT_VST_DIR}/DspCrackleProxyVst.cpp
  ${ENT_VST_DIR}/DspGlitchProxyVst.cpp
  ${ENT_VST_DIR}/DspVstProxy.cpp
  ${ENT_VST_DIR}/EntVstProcessor.cpp
  ${ENT_VST_DIR}/EntVstController.cpp
  ${ENT_VST_DIR}/EntVstPluginView.cpp)

if (NOT CMAKE_SYSTEM_NAME MATCHES Windows)
    list(APPEND ENT_VST_HEADERS ${ENT_VST_DIR}/EntVstLoopTimer.h)
    list(APPEND ENT_VST_SOURCES ${ENT_VST_DIR}/EntVstLoopTimer.cpp)
endif()

set(target entropictron_vst)

link_directories(${PTHREAD_LIB_PATH})

add_library(${target} SHARED ${ENT_VST_HEADERS} ${ENT_VST_SOURCES})

set_target_properties(${target} PROPERTIES PREFIX "")
set_target_properties(${target} PROPERTIES OUTPUT_NAME "Entropictron")
if (CMAKE_SYSTEM_NAME MATCHES Windows)
    set_target_properties(${target} PROPERTIES SUFFIX ".dll")
else ()
    set_target_properties(${target} PROPERTIES SUFFIX ".so")
endif ()

target_link_libraries(${target} PRIVATE base sdk sdk_common pluginterfaces)
target_link_libraries(${target} PRIVATE entropictron_common dsp_wrapper redkite)
target_link_options(${target} PRIVATE -Wl,--no-undefined)

if (CMAKE_SYSTEM_NAME MATCHES Windows)
target_link_libraries(${target} PRIVATE
      "libcairo.a"
      "libmsimg32.a"
      "libfontconfig.a"
      "libfreetype.a"
      "libpixman-1.a"
      "libpng.a"
      "libintl.a"
      "libiconv.a"
      "libexpat.a"
      "libbrotlidec.a"
      "libharfbuzz.a"
      "libbz2.a"
      "libbrotlicommon.a"
      "libgraphite2.a"
      "librpcrt4.a"
      "libsndfile.a"
      "libvorbis.a"
      "libvorbisenc.a"
      "libogg.a"
      "libFLAC.a"
      "libmp3lame.a"
      "libopus.a"
      "libmpg123.a"
      "libshlwapi.a"
      "libz.a"
      "libwinpthread.a"
      "libdwrite.a"
      "-static-libstdc++"
      "-static-libgcc"
      )
else()
      target_link_libraries(${target} PRIVATE X11)
      target_link_libraries(${target} PRIVATE "-lstdc++ -lstdc++fs -lsndfile -lcairo -lm -lpthread")
endif()

add_dependencies(${target} entropictron_common)

install(TARGETS ${target} DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/vst3/Entropictron.vst3/Contents/${ENT_ARCHITECTURE}-linux)
