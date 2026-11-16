find_package(Threads REQUIRED)

add_executable(algorhythmia_dsp_tests
    test_main.cpp
    engine_tests.cpp
    utility_tests.cpp
    scene_tests.cpp
)
target_link_libraries(algorhythmia_dsp_tests PRIVATE algorhythmia::dsp doctest::doctest)
algorhythmia_target_defaults(algorhythmia_dsp_tests)
add_test(NAME dsp_unit_tests COMMAND algorhythmia_dsp_tests)

add_executable(algorhythmia_realtime_tests realtime_tests.cpp)
target_link_libraries(algorhythmia_realtime_tests PRIVATE algorhythmia::host Threads::Threads)
algorhythmia_target_defaults(algorhythmia_realtime_tests)
if(ALGORHYTHMIA_ENABLE_TSAN)
    # TSan owns global new/delete; keep its concurrency run separate from the
    # allocation hooks exercised by ordinary and ASan builds.
    target_compile_definitions(algorhythmia_realtime_tests PRIVATE SYS01_TSAN=1)
endif()
add_test(NAME dsp_realtime_tests COMMAND algorhythmia_realtime_tests)
set_tests_properties(dsp_unit_tests dsp_realtime_tests PROPERTIES TIMEOUT 60)

if(UNIX AND CMAKE_NM)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    add_test(NAME dsp_runtime_symbols
        COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/check_rt_symbols.py"
            --nm "${CMAKE_NM}" $<TARGET_FILE:algorhythmia_dsp>)
endif()

option(ALGORHYTHMIA_BUILD_BENCHMARKS "Build the informational host DSP benchmark" OFF)
if(ALGORHYTHMIA_BUILD_BENCHMARKS)
    add_executable(algorhythmia_dsp_benchmark benchmark.cpp)
    target_link_libraries(algorhythmia_dsp_benchmark PRIVATE algorhythmia::dsp)
    algorhythmia_target_defaults(algorhythmia_dsp_benchmark)

    add_executable(algorhythmia_dsp_stress stress.cpp)
    target_link_libraries(algorhythmia_dsp_stress PRIVATE algorhythmia::dsp)
    algorhythmia_target_defaults(algorhythmia_dsp_stress)
    add_test(NAME dsp_stress_smoke COMMAND algorhythmia_dsp_stress 0.0006)
    set_tests_properties(dsp_stress_smoke PROPERTIES TIMEOUT 60)
    add_custom_target(stress-eight-hours
        COMMAND $<TARGET_FILE:algorhythmia_dsp_stress> 8
        DEPENDS algorhythmia_dsp_stress
        USES_TERMINAL)
endif()
