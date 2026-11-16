cmake_minimum_required(VERSION 3.22)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    include("${CMAKE_CURRENT_LIST_DIR}/../../cmake/Version.cmake")
    project(algorhythmia_max_adapter VERSION ${ALGORHYTHMIA_VERSION} LANGUAGES CXX)
    include("${CMAKE_CURRENT_LIST_DIR}/../../cmake/ProjectOptions.cmake")
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../../libs/algorhythmia_dsp"
                     "${CMAKE_CURRENT_BINARY_DIR}/algorhythmia_dsp")
endif()

if(NOT APPLE AND NOT WIN32)
    message(FATAL_ERROR "Max externals require macOS or Windows; use the CLI harness on Linux")
endif()

set(ALGORHYTHMIA_MAX_SDK_BASE "" CACHE PATH "Path to a supplied Cycling74/max-sdk-base checkout")
set(max_support "${ALGORHYTHMIA_MAX_SDK_BASE}/c74support")
if(NOT EXISTS "${max_support}/max-includes/ext.h" OR
   NOT EXISTS "${max_support}/msp-includes/z_dsp.h")
    message(FATAL_ERROR "Set ALGORHYTHMIA_MAX_SDK_BASE to a complete max-sdk-base checkout")
endif()

add_library(algorhythmia_max MODULE external/algorhythmia_tilde.cpp)
target_link_libraries(algorhythmia_max PRIVATE algorhythmia::dsp)
target_include_directories(algorhythmia_max SYSTEM PRIVATE
    "${max_support}/max-includes" "${max_support}/msp-includes")
target_compile_features(algorhythmia_max PRIVATE cxx_std_20)
algorhythmia_enable_sanitizers(algorhythmia_max)
set_target_properties(algorhythmia_max PROPERTIES
    CXX_EXTENSIONS OFF
    PREFIX ""
    OUTPUT_NAME "algorhythmia~"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/max/externals")

# SDK callback registration casts incompatible function types by design. Keep
# these host ABI casts outside the strict, portable DSP compilation target.
if(APPLE)
    find_library(max_audio_api MaxAudioAPI REQUIRED
        PATHS "${max_support}/msp-includes" NO_DEFAULT_PATH)
    target_link_libraries(algorhythmia_max PRIVATE "${max_audio_api}")
    target_link_options(algorhythmia_max PRIVATE "-undefined" "dynamic_lookup")
    set_target_properties(algorhythmia_max PROPERTIES
        BUNDLE TRUE
        BUNDLE_EXTENSION mxo
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/external/Info.plist.in")
else()
    target_compile_definitions(algorhythmia_max PRIVATE MAXAPI_USE_MSCRT WIN_VERSION)
    target_link_libraries(algorhythmia_max PRIVATE
        "${max_support}/max-includes/x64/MaxAPI.lib"
        "${max_support}/msp-includes/x64/MaxAudio.lib")
    set_target_properties(algorhythmia_max PROPERTIES SUFFIX ".mxe64")
endif()
