cmake_minimum_required(VERSION 3.22)

include(cmake/Version.cmake)
project(algorhythmia VERSION ${ALGORHYTHMIA_VERSION} LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(ALGORHYTHMIA_BUILD_CLI "Build the offline WAV renderer" ON)
option(ALGORHYTHMIA_BUILD_FIRMWARE_CONTRACT "Build the host firmware contract check" ON)
option(ALGORHYTHMIA_BUILD_DESKTOP "Build native VST3/AU and development standalone" OFF)
option(ALGORHYTHMIA_BUILD_MAX "Build the Max external using a local Max SDK checkout" OFF)
option(ALGORHYTHMIA_ENABLE_TSAN "Enable ThreadSanitizer (separate from ASan)" OFF)
option(ALGORHYTHMIA_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(ALGORHYTHMIA_ENABLE_CLANG_TIDY "Run clang-tidy while compiling project targets" OFF)

include(ProjectOptions)
include(CTest)
if(BUILD_TESTING OR ALGORHYTHMIA_BUILD_DESKTOP)
    include(Dependencies)
endif()

if(ALGORHYTHMIA_ENABLE_CLANG_TIDY)
    find_program(ALGORHYTHMIA_CLANG_TIDY NAMES clang-tidy-18 clang-tidy REQUIRED)
endif()

add_subdirectory(libs/algorhythmia_dsp)
add_subdirectory(libs/algorhythmia_host)

if(ALGORHYTHMIA_BUILD_DESKTOP)
    add_subdirectory(apps/desktop)
endif()

if(ALGORHYTHMIA_BUILD_CLI)
    add_subdirectory(apps/cli_harness)
endif()

if(ALGORHYTHMIA_BUILD_FIRMWARE_CONTRACT)
    add_subdirectory(apps/firmware_stub)
endif()

if(ALGORHYTHMIA_BUILD_MAX)
    add_subdirectory(apps/max_for_live)
endif()

if(BUILD_TESTING)
    sys01_dependency(doctest)
    set(DOCTEST_WITH_TESTS OFF CACHE BOOL "" FORCE)
    set(DOCTEST_WITH_MAIN_IN_STATIC_LIB OFF CACHE BOOL "" FORCE)
    if(FETCHCONTENT_SOURCE_DIR_DOCTEST)
        sys01_verify_dependency(doctest "${FETCHCONTENT_SOURCE_DIR_DOCTEST}")
    endif()
    FetchContent_MakeAvailable(doctest)
    sys01_verify_dependency(doctest "${doctest_SOURCE_DIR}")
    add_test(NAME dependency_pin_contract COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/test_dependency.py" "${doctest_SOURCE_DIR}")
    add_subdirectory(libs/algorhythmia_dsp/tests)
    add_subdirectory(libs/algorhythmia_host/tests)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    add_test(NAME native_theme_contract COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/export_theme.py" --check)
    add_test(NAME max_patch_contract COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/check_max_contract.py")
    add_test(NAME dependency_notice_contract COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/check_notices.py")
    add_test(NAME release_evidence_contract COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/test_release_evidence.py")
    add_test(NAME source_package_contract COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/test_source_package.py")
endif()

find_package(Python3 QUIET COMPONENTS Interpreter)
find_program(ALGORHYTHMIA_CLANG_FORMAT NAMES clang-format-18 clang-format)
if(Python3_Interpreter_FOUND AND ALGORHYTHMIA_CLANG_FORMAT)
    add_custom_target(format-check
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/scripts/check_format.py"
            --check --clang-format "${ALGORHYTHMIA_CLANG_FORMAT}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Checking C/C++ formatting"
        VERBATIM
    )
    add_custom_target(format
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/scripts/check_format.py"
            --fix --clang-format "${ALGORHYTHMIA_CLANG_FORMAT}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Formatting project C/C++ sources"
        VERBATIM
    )
endif()
