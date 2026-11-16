add_library(algorhythmia_dsp STATIC src/engine.cpp src/bytebeat.cpp src/cellular.cpp src/transform.cpp)
add_library(algorhythmia::dsp ALIAS algorhythmia_dsp)
target_include_directories(algorhythmia_dsp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_features(algorhythmia_dsp PUBLIC cxx_std_20)
set_target_properties(algorhythmia_dsp PROPERTIES CXX_EXTENSIONS OFF)
# Desktop externals need PIC; bare-metal firmware need not pay for indirection.
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Generic")
    set_target_properties(algorhythmia_dsp PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
algorhythmia_target_defaults(algorhythmia_dsp)
# Keeping exceptions/RTTI out of this boundary also validates bare-metal use.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(algorhythmia_dsp PRIVATE -fno-exceptions -fno-rtti)
endif()
