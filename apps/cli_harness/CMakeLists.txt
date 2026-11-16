add_executable(algorhythmia_render main.cpp)
target_link_libraries(algorhythmia_render PRIVATE algorhythmia::dsp)
algorhythmia_target_defaults(algorhythmia_render)

if(BUILD_TESTING)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    add_test(NAME cli.wav_smoke
        COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/check_wav.py"
            "$<TARGET_FILE:algorhythmia_render>"
    )
    set_tests_properties(cli.wav_smoke PROPERTIES TIMEOUT 30)
endif()
