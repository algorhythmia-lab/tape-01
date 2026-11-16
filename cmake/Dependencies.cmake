include_guard(GLOBAL)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()
include(FetchContent)
find_package(Python3 REQUIRED COMPONENTS Interpreter)
file(READ "${PROJECT_SOURCE_DIR}/cmake/dependencies.lock.json" sys01_dependency_lock)

function(sys01_dependency name)
    string(JSON url GET "${sys01_dependency_lock}" dependencies ${name} url)
    string(JSON digest GET "${sys01_dependency_lock}" dependencies ${name} archive_sha256)
    # Full revision URLs and archive hashes are both mandatory. No global cache
    # path is configured; each build can retrieve its declared dependencies.
    FetchContent_Declare(${name} URL "${url}" URL_HASH "SHA256=${digest}" TLS_VERIFY TRUE TIMEOUT 90)
endfunction()

function(sys01_verify_dependency name source)
    execute_process(COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/verify_dependency.py"
        "${name}" "${source}" --output "${CMAKE_BINARY_DIR}/dependencies/${name}.json"
        COMMAND_ERROR_IS_FATAL ANY)
endfunction()
