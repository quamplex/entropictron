cmake_minimum_required (VERSION 3.7)

project(entropictron VERSION 1.0.0)

math(EXPR ENT_VERSION_HEX "0x${PROJECT_VERSION_MAJOR} * 0x10000 + 0x${PROJECT_VERSION_MINOR} * 0x100 + 0x${PROJECT_VERSION_PATCH}")
set(ENT_VERSION_STR "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

message(STATUS "Entropictron version: ${ENT_VERSION_STR} (${ENT_VERSION_HEX})")

add_definitions(-DENT_VERSION_HEX=${ENT_VERSION_HEX})
add_definitions(-DENT_VERSION_STR="${ENT_VERSION_STR}")

if (NOT CMAKE_SYSTEM_NAME MATCHES Windows)
    include(GNUInstallDirs)
endif()

if (NOT CMAKE_SYSTEM_NAME MATCHES Windows)
   option(ENT_STANDALONE "Enable build for standalone" OFF)
else()
   option(ENT_STANDALONE "Disable build for standalone" OFF)
endif()

set(REDKITE_INSTALL OFF CACHE BOOL "Disable Redkite install" FORCE)
option(ENTROPICTRON_LIMITED_VERSION "Enable entropictron limited version" OFF)
option(ENT_PLUGIN "Enable build for plugin" ON)
option(ENT_PRESETS "Enable presets" OFF)

if (ENT_PLUGIN)
  option(ENT_PLUGIN_LV2 "Enable build LV2 plugin" OFF)
  if (VST3_SDK_PATH)
    option(ENT_PLUGIN_VST "Enable build VST plugin" ON)
    set(ENT_VST_SDK_PATH ${VST3_SDK_PATH})
  endif(VST3_SDK_PATH)
endif (ENT_PLUGIN)

option(ENT_DOCUMENTATION "Enable build documentation" OFF)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "no build type selected, set default to Release")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type Release" FORCE)
endif()

if (NOT ENT_ARCHITECTURE)
  message(STATUS "ENT_ARCHITECTURE variable was not set use default ${CMAKE_HOST_SYSTEM_PROCESSOR}")
  set(ENT_ARCHITECTURE ${CMAKE_HOST_SYSTEM_PROCESSOR})
endif ()

if (ENT_ARCHITECTURE MATCHES x86_64)
  message(STATUS "set optimisation compiler flags for ${ENT_ARCHITECTURE}")
  set(ENT_OPTIMISATION_FLAGS "-O3 -msse -msse2 -mfpmath=sse -funsafe-math-optimizations -fno-math-errno -fno-trapping-math -fomit-frame-pointer")
else ()
  message(STATUS "set optimisation compiler flags for ${ENT_ARCHITECTURE}")
  set(ENT_OPTIMISATION_FLAGS "-O3 -funsafe-math-optimizations -fno-math-errno -fno-trapping-math -fomit-frame-pointer")
endif ()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall ${ENT_OPTIMISATION_FLAGS} ${CMAKE_CXX_FLAGS} -fvisibility=hidden")
set(CMAKE_C_FLAGS "-Wall ${ENT_OPTIMISATION_FLAGS} ${CMAKE_C_FLAGS} -fvisibility=hidden")

if (ENTROPICTRON_LIMITED_VERSION)
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DENTROPICTRON_LIMITED_VERSION")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENTROPICTRON_LIMITED_VERSION")
endif(ENTROPICTRON_LIMITED_VERSION)

if (CMAKE_SYSTEM_NAME MATCHES Windows)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENTROPICTRON_OS_WINDOWS")
   set(CMAKE_C_FLAGS "${ENT_OPTIMISATION_FLAGS} -DENTROPICTRON_OS_WINDOWS")
else()
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENTROPICTRON_OS_GNU")
   set(CMAKE_C_FLAGS "${ENT_OPTIMISATION_FLAGS} -DENTROPICTRON_OS_GNU")
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set (CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "default install path" FORCE)
endif()

if (NOT CMAKE_INSTALL_FULL_DATADIR)
  message(STATUS "CMAKE_INSTALL_FULL_DATADIR is not set, use default ${CMAKE_INSTALL_PREFIX}/share")
  set(CMAKE_INSTALL_FULL_DATADIR "${CMAKE_INSTALL_PREFIX}/share" CACHE PATH "default data dir path" FORCE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENTROPICTRON_DATA_DIR=\\\"${CMAKE_INSTALL_FULL_DATADIR}\\\"")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DENTROPICTRON_DATA_DIR=\\\"${CMAKE_INSTALL_FULL_DATADIR}\\\"")

set(ENT_DOC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc)
set(ENT_COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(ENT_DSP_DIR ${ENT_COMMON_DIR}/dsp)
set(ENT_RC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/resources)
set(ENT_STANDALONE_DIR ${ENT_COMMON_DIR}/standalone)
set(ENT_DSP_WRAPPER_DIR ${ENT_COMMON_DIR}/DspWrapper)

if (REDKITE_PATH)
  get_filename_component(REDKITE_FOLDER_NAME ${REDKITE_PATH} NAME)
if(NAME_FROM_PATH STREQUAL "" AND (REDKITE_FOLDER_NAME STREQUAL "." OR REDKITE_FOLDER_NAME STREQUAL ".."))
  message(FATAL_ERROR "The folder name '${REDKITE_FOLDER_NAME}' of Redkite is invalid or empty.")
endif()
  set(ENT_REDKITE_DIR ${REDKITE_PATH})
else(REDKITE_PATH)
  set(ENT_REDKITE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/redkite)
endif(REDKITE_PATH)

if (ENT_PLUGIN)
    if (ENT_PLUGIN_VST)
      set(ENT_VST_DIR ${ENT_COMMON_DIR}/plugin/vst)
      set(ENT_VST_SDK_PATH ${VST3_SDK_PATH})
    endif (ENT_PLUGIN_VST)
    if (ENT_PLUGIN_LV2)
      set(ENT_LV2_DIR ${ENT_COMMON_DIR}/plugin/lv2)
    endif (ENT_PLUGIN_LV2)
endif(ENT_PLUGIN)

set(ENT_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
set(ENT_PRESETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/presets)

set(ENT_COMMON_HEADERS
  ${ENT_COMMON_DIR}/globals.h
  ${ENT_COMMON_DIR}/GuiTypes.h
  ${ENT_COMMON_DIR}/DspProxy.h
  ${ENT_COMMON_DIR}/DspNoiseProxy.h
  ${ENT_COMMON_DIR}/EntAbstractModel.h
  ${ENT_COMMON_DIR}/EntropictronModel.h
  ${ENT_COMMON_DIR}/NoiseModel.h
  ${ENT_COMMON_DIR}/EntWidget.h
  ${ENT_COMMON_DIR}/EntAbstractView.h
  ${ENT_COMMON_DIR}/EntWidget.h
  ${ENT_COMMON_DIR}/Knob.h
  ${ENT_COMMON_DIR}/NoiseWidget.h
  ${ENT_COMMON_DIR}/MainWindow.h
)

set(ENT_COMMON_SOURCES
  ${ENT_COMMON_DIR}/DspProxy.cpp
  ${ENT_COMMON_DIR}/DspNoiseProxy.cpp
  ${ENT_COMMON_DIR}/EntAbstractModel.cpp
  ${ENT_COMMON_DIR}/EntropictronModel.cpp
  ${ENT_COMMON_DIR}/NoiseModel.cpp
  ${ENT_COMMON_DIR}/EntWidget.cpp
  ${ENT_COMMON_DIR}/EntAbstractView.cpp
  ${ENT_COMMON_DIR}/Knob.cpp
  ${ENT_COMMON_DIR}/NoiseWidget.cpp
  ${ENT_COMMON_DIR}/MainWindow.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Enable application logging")
    option(ENABLE_LOGGING    "Enable logging for API and GUI" ON)
endif()

if (ENTROPICTRON_LIMITED_VERSION)
  list(APPEND ENT_DSP_FLAGS "-DENTROPICTRON_LIMITED_VERSION")
  list(APPEND ENT_DSP_FLAGS "-DENTROPICTRON_LIMITED_VERSION")
  list(APPEND ENT_DSP_PLUGIN_FLAGS "-DENTROPICTRON_LIMITED_VERSION")
  list(APPEND ENT_DSP_PLUGIN_FLAGS "-DENTROPICTRON_LIMITED_VERSION")
endif (ENTROPICTRON_LIMITED_VERSION)

if (ENABLE_LOGGING)
  message(STATUS "set debug level")
  #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fsanitize=undefined -fsanitize=leak -DENT_LOG_DEBUG_LEVEL")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DENT_LOG_DEBUG_LEVEL")
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fsanitize=leak -DENT_LOG_DEBUG_LEVEL")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENT_LOG_DEBUG_LEVEL")
  list(APPEND ENT_DSP_FLAGS "-DENT_LIB_LOG_LEVEL_DEBUG")
  list(APPEND ENT_DSP_FLAGS "-DENT_LIB_LOG_FUNCTION")
  list(APPEND ENT_DSP_PLUGIN_FLAGS "-DENT_LIB_LOG_LEVEL_DEBUG")
  list(APPEND ENT_DSP_PLUGIN_FLAGS "-DENT_LIB_LOG_FUNCTION")
else(ENABLE_LOGGING)
  list(APPEND ENT_DSP_FLAGS "-DENT_LIB_LOG_LEVEL_ERROR")
  list(APPEND ENT_DSP_FLAGS "-DENT_LIB_LOG_FUNCTION")
  list(APPEND ENT_DSP_PLUGIN_FLAGS "-DENT_LIB_LOG_LEVEL_ERROR")
  list(APPEND ENT_DSP_PLUGIN_FLAGS "-DENT_LIB_LOG_FUNCTION")
endif(ENABLE_LOGGING)

find_package(PkgConfig REQUIRED)
find_package(RapidJSON REQUIRED)
if (CMAKE_SYSTEM_NAME MATCHES Windows)
   include_directories(${RapidJSON_INCLUDE_DIR})
endif()

pkg_check_modules(SNDFILE sndfile)
if (NOT SNDFILE_FOUND)
    message(FATAL_ERROR "can't find sndfile library")
endif(NOT SNDFILE_FOUND)

if (ENT_STANDALONE)
  pkg_check_modules(JACK jack)
  if (NOT JACK_FOUND)
    message(STATUS "can't find jack library")
    set(ENT_STANDALONE OFF)
    set(ENT_JACK_NOT_FOUND ON)
  endif(NOT JACK_FOUND)
  list(APPEND ENT_DSP_FLAGS "-DENTROPICTRON_AUDIO_JACK")
endif()

if (ENT_PLUGIN)
  message(STATUS "checking for LV2 library...")
  pkg_check_modules(LV2 lv2)
  if (NOT LV2_FOUND)
      message(FATAL_ERROR "can't find LV2 library")
  endif(NOT LV2_FOUND)
endif(ENT_PLUGIN)

if (CMAKE_SYSTEM_NAME MATCHES Windows)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRK_OS_WIN")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRK_OS_WIN")
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRK_OS_GNU")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRK_OS_GNU")
endif ()

include_directories(${ENT_DSP_DIR}/src)
include_directories(${ENT_DSP_WRAPPER_DIR})
include_directories(${ENT_REDKITE_DIR}/include)
link_directories(${ENT_REDKITE_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_definitions(-fPIC)
add_subdirectory(${ENT_DSP_DIR})
add_subdirectory(${ENT_DSP_WRAPPER_DIR})
add_subdirectory(${ENT_REDKITE_DIR} redkite)
include(${ENT_RC_DIR}/resources.rc)

foreach(pngFile ${ent_rc_png_files})
  message(STATUS "convert file ${pngFile}")
  get_filename_component(filename ${ENT_RC_DIR}/${pngFile} NAME)
  string(REPLACE "." "_" filename ${filename})
  string(REPLACE " " "_" filename ${filename})
  add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/resources/${filename}.c
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/resources
    && ${CMAKE_BINARY_DIR}/redkite/tools/rkpng2c
    "${ENT_RC_DIR}/${pngFile}" "${CMAKE_BINARY_DIR}/resources/${filename}.c" "rk__${filename}")
    set(ENT_RC_OUTPUT ${ENT_RC_OUTPUT} ${CMAKE_BINARY_DIR}/resources/${filename}.c)
endforeach()

add_custom_target(ent_resources ALL DEPENDS ${ENT_RC_OUTPUT})
add_dependencies(ent_resources redkite rkpng2c)

include_directories(${ENT_COMMON_DIR})
link_directories(${REDKITE_LIBRARY_DIR})
add_library(entropictron_common STATIC
  ${ENT_COMMON_HEADERS}
  ${ENT_COMMON_SOURCES}
  ${ENT_RC_OUTPUT})
add_dependencies(entropictron_common ent_resources)

if (ENT_STANDALONE)
   add_subdirectory(${ENT_STANDALONE_DIR})
endif (ENT_STANDALONE)

if (ENT_PLUGIN)
   add_subdirectory(${ENT_COMMON_DIR}/plugin)
endif (ENT_PLUGIN)

message(STATUS "------------ Summary ---------")

if (ENTROPICTRON_LIMITED_VERSION)
  message(STATUS "Entropictron limited version: yes" )
endif(ENTROPICTRON_LIMITED_VERSION)

if (ENT_STANDALONE)
  message(STATUS "Standalone: yes" )
else(ENT_STANDALONE)
  if (ENT_JACK_NOT_FOUND)
    message(STATUS "Standalone: no (install jack development library)")
  else ()
    message(STATUS "Standalone: no")
  endif()
endif(ENT_STANDALONE)

if (ENT_PLUGIN_LV2)
  message(STATUS "LV2 plugin: yes" )
else(ENT_PLUGIN_LV2)
  message(STATUS "LV2 plugin: no")
endif(ENT_PLUGIN_LV2)

if (ENT_PLUGIN_VST)
  message(STATUS "VST plugin: yes" )
else(ENT_PLUGIN_VST)
  message(STATUS "VST plugin: no")
endif(ENT_PLUGIN_VST)

if(ENT_PRESETS)
  message(STATUS "Pesets: yes" )
else(ENT_PRESETS)
  message(STATUS "Presets: no")
endif(ENT_PRESETS)

if (ENT_DOCUMENTATION)
  message(STATUS "Documentation: yes" )
else(ENT_DOCUMENTATION)
  message(STATUS "Documentation: no" )
endif(ENT_DOCUMENTATION)

if (ENABLE_LOGGING)
  message(STATUS "Debug enabled: yes" )
else(ENABLE_LOGGING)
  message(STATUS "Debug enabled: no" )
endif(ENABLE_LOGGING)
message(STATUS "------------------------------")


