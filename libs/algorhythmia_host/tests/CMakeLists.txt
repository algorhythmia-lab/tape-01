add_executable(algorhythmia_host_tests host_tests.cpp
    ../../algorhythmia_dsp/tests/test_main.cpp)
target_link_libraries(algorhythmia_host_tests PRIVATE algorhythmia::host doctest::doctest)
algorhythmia_target_defaults(algorhythmia_host_tests)
add_test(NAME host_contract COMMAND algorhythmia_host_tests)
add_executable(sys01_musical_tests musical_tests.cpp)
target_link_libraries(sys01_musical_tests PRIVATE algorhythmia::host)
algorhythmia_target_defaults(sys01_musical_tests)
add_test(NAME musical_behavior COMMAND sys01_musical_tests)
set_tests_properties(musical_behavior PROPERTIES TIMEOUT 240)
if(UNIX AND CMAKE_NM)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    add_test(NAME host_runtime_symbols
        COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/check_rt_symbols.py"
            --nm "${CMAKE_NM}" $<TARGET_FILE:algorhythmia_host>)
endif()
add_executable(host_catalogue catalogue_export.cpp)
target_link_libraries(host_catalogue PRIVATE algorhythmia::host)
algorhythmia_target_defaults(host_catalogue)
find_package(Python3 REQUIRED COMPONENTS Interpreter)
add_test(NAME catalogue_documentation
    COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/check_catalogue.py"
        $<TARGET_FILE:host_catalogue> "${PROJECT_SOURCE_DIR}/docs/parameters.md")
