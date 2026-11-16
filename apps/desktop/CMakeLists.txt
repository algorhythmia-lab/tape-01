set(ALGORHYTHMIA_JUCE_SOURCE "" CACHE PATH "Optional verified JUCE 8.0.12 source checkout")
if(ALGORHYTHMIA_JUCE_SOURCE)
    sys01_verify_dependency(juce "${ALGORHYTHMIA_JUCE_SOURCE}")
    add_subdirectory("${ALGORHYTHMIA_JUCE_SOURCE}" "${CMAKE_BINARY_DIR}/juce")
else()
    sys01_dependency(juce)
    if(FETCHCONTENT_SOURCE_DIR_JUCE)
        sys01_verify_dependency(juce "${FETCHCONTENT_SOURCE_DIR_JUCE}")
    endif()
    FetchContent_MakeAvailable(juce)
    sys01_verify_dependency(juce "${juce_SOURCE_DIR}")
endif()
set(sys01_formats VST3 Standalone)
if(APPLE)
    list(APPEND sys01_formats AU)
endif()
juce_add_plugin(sys01
    COMPANY_NAME algorhythmia
    BUNDLE_ID org.algorhythmia.sys01
    PLUGIN_MANUFACTURER_CODE Algr
    PLUGIN_CODE Sys1
    PRODUCT_NAME "SYS 01"
    ICON_BIG "assets/sys01-icon.svg"
    ICON_SMALL "assets/sys01-icon-small.svg"
    VERSION ${PROJECT_VERSION}
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    MICROPHONE_PERMISSION_ENABLED TRUE
    MICROPHONE_PERMISSION_TEXT "SYS 01 uses your audio input for live tape processing."
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CATEGORIES Fx Generator
    AU_MAIN_TYPE kAudioUnitType_MusicEffect
    FORMATS ${sys01_formats})
# Icon resources are generated during configuration; refresh them when artwork changes.
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/sys01-icon.svg"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/sys01-icon-small.svg")
juce_add_binary_data(sys01_fonts NAMESPACE Sys01Fonts HEADER_NAME Sys01Fonts.h
    SOURCES ../../licenses/fonts/barlow-condensed.ttf
            ../../licenses/fonts/jetbrains-mono.ttf ../../licenses/fonts/inter.ttf)
target_sources(sys01 PRIVATE PluginProcessor.cpp PluginEditor.cpp EditorControls.cpp)
target_link_libraries(sys01 PRIVATE algorhythmia::host juce::juce_audio_utils sys01_fonts)
target_compile_definitions(sys01 PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)
algorhythmia_target_defaults(sys01)
target_sources(sys01_Standalone PRIVATE StandaloneApp.cpp StandaloneWindow.cpp)
target_compile_definitions(sys01_Standalone PRIVATE JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=1)
target_include_directories(sys01_Standalone PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
algorhythmia_target_defaults(sys01_Standalone)
if(BUILD_TESTING)
    add_executable(sys01_notification_tests NotificationTests.cpp)
    target_link_libraries(sys01_notification_tests PRIVATE sys01 algorhythmia::host juce::juce_audio_utils)
    target_compile_definitions(sys01_notification_tests PRIVATE JUCE_MODAL_LOOPS_PERMITTED=1)
    algorhythmia_target_defaults(sys01_notification_tests)
    add_test(NAME native_notifications COMMAND sys01_notification_tests)
    add_executable(sys01_adapter_tests AdapterTests.cpp)
    target_link_libraries(sys01_adapter_tests PRIVATE sys01 algorhythmia::host juce::juce_audio_utils)
    algorhythmia_target_defaults(sys01_adapter_tests)
    add_executable(sys01_editor_tests EditorTests.cpp)
    target_link_libraries(sys01_editor_tests PRIVATE sys01 algorhythmia::host juce::juce_audio_utils juce::juce_cryptography)
    algorhythmia_target_defaults(sys01_editor_tests)
    add_test(NAME native_editor COMMAND sys01_editor_tests "${CMAKE_CURRENT_BINARY_DIR}/editor-evidence")
    set_tests_properties(native_editor PROPERTIES TIMEOUT 240)
    add_executable(sys01_standalone_tests StandaloneTests.cpp StandaloneWindow.cpp)
    target_link_libraries(sys01_standalone_tests PRIVATE sys01 algorhythmia::host juce::juce_audio_utils)
    target_compile_definitions(sys01_standalone_tests PRIVATE JUCE_MODAL_LOOPS_PERMITTED=1)
    if(APPLE)
        target_sources(sys01_standalone_tests PRIVATE StandaloneMacTests.mm)
    endif()
    algorhythmia_target_defaults(sys01_standalone_tests)
    option(ALGORHYTHMIA_INTERACTIVE_WINDOW_TESTS "Run window/Dock tests in an interactive desktop session" OFF)
    if(ALGORHYTHMIA_INTERACTIVE_WINDOW_TESTS)
        add_test(NAME native_standalone COMMAND sys01_standalone_tests "${CMAKE_CURRENT_BINARY_DIR}/standalone-evidence")
        set_tests_properties(native_standalone PROPERTIES TIMEOUT 120 LABELS interactive)
    endif()
    add_executable(sys01_recall_tests RecallTests.cpp)
    target_link_libraries(sys01_recall_tests PRIVATE sys01 algorhythmia::host juce::juce_audio_utils)
    algorhythmia_target_defaults(sys01_recall_tests)
    add_test(NAME native_recall COMMAND sys01_recall_tests)
    add_test(NAME desktop_adapter COMMAND sys01_adapter_tests)
    set_tests_properties(desktop_adapter PROPERTIES TIMEOUT 120)
endif()

# Reviewed format boundary; generated source keeps the upstream license and the
# pinned checkout remains untouched. The stock VST3 source is excluded only in
# this directory scope. AU MIDI ingress is bounded below; standalone audio/device
# handling stays upstream, with a custom cassette window in StandaloneWindow.cpp.
find_package(Python3 REQUIRED COMPONENTS Interpreter)
if(ALGORHYTHMIA_JUCE_SOURCE)
    set(sys01_juce_root "${ALGORHYTHMIA_JUCE_SOURCE}")
else()
    set(sys01_juce_root "${juce_SOURCE_DIR}")
endif()
set(sys01_vst3_source "${sys01_juce_root}/modules/juce_audio_plugin_client/juce_audio_plugin_client_VST3.cpp")
if(APPLE)
    set(sys01_vst3_extension mm)
else()
    set(sys01_vst3_extension cpp)
endif()
set(sys01_vst3_generated "${CMAKE_CURRENT_BINARY_DIR}/native/Sys01VST3.${sys01_vst3_extension}")
execute_process(COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/native_vst3_adapter.py"
    "${sys01_vst3_source}" "${sys01_vst3_generated}" COMMAND_ERROR_IS_FATAL ANY)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/scripts/native_vst3_adapter.py" "${sys01_vst3_source}")
option(ALGORHYTHMIA_POSITIONED_VST3 "Preserve every VST3 point (OFF reproduces the stock timing defect)" ON)
if(ALGORHYTHMIA_POSITIONED_VST3)
set_source_files_properties("${sys01_vst3_source}"
    "${sys01_juce_root}/modules/juce_audio_plugin_client/juce_audio_plugin_client_VST3.mm"
    PROPERTIES HEADER_FILE_ONLY TRUE)
target_sources(sys01_VST3 PRIVATE "${sys01_vst3_generated}")
endif()
target_include_directories(sys01_VST3 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(sys01_VST3 PRIVATE algorhythmia::host)
if(BUILD_TESTING)
    set(sys01_vst_sdk "${sys01_juce_root}/modules/juce_audio_processors_headless/format_types/VST3_SDK")
    add_executable(sys01_vst3_contract VST3ContractTests.cpp "${sys01_vst_sdk}/pluginterfaces/base/funknown.cpp")
    target_include_directories(sys01_vst3_contract SYSTEM PRIVATE "${sys01_vst_sdk}")
    target_link_libraries(sys01_vst3_contract PRIVATE algorhythmia::host)
    algorhythmia_target_defaults(sys01_vst3_contract)
    add_dependencies(sys01_vst3_contract sys01_VST3)
    if(APPLE)
        target_link_libraries(sys01_vst3_contract PRIVATE "-framework CoreFoundation")
        add_test(NAME vst3_positioned_automation COMMAND sys01_vst3_contract $<TARGET_BUNDLE_DIR:sys01_VST3>)
    else()
        add_test(NAME vst3_positioned_automation COMMAND sys01_vst3_contract $<TARGET_FILE:sys01_VST3>)
    endif()
endif()

# Diagnostic interposition belongs to fixtures, never the plugin or timing build.
option(ALGORHYTHMIA_NATIVE_RT_PROBE "macOS whole-wrapper allocation/lock diagnostic probe" OFF)
if(APPLE AND BUILD_TESTING)
    target_sources(sys01_vst3_contract PRIVATE NativeEditorHost.mm)
    target_link_libraries(sys01_vst3_contract PRIVATE "-framework Cocoa")
    if(ALGORHYTHMIA_NATIVE_RT_PROBE)
        add_library(sys01_realtime_probe SHARED RealtimeProbe.cpp)
        target_compile_features(sys01_realtime_probe PRIVATE cxx_std_20)
        foreach(fixture sys01_vst3_contract sys01_adapter_tests sys01_notification_tests)
            target_link_libraries(${fixture} PRIVATE sys01_realtime_probe)
            target_compile_definitions(${fixture} PRIVATE SYS01_NATIVE_RT_PROBE=1)
        endforeach()
    endif()
endif()
if(APPLE AND BUILD_TESTING)
    add_executable(sys01_au_updater AUUpdaterTests.cpp NativeEditorHost.mm)
    algorhythmia_target_defaults(sys01_au_updater)
    target_link_libraries(sys01_au_updater PRIVATE "-framework AudioToolbox" "-framework CoreFoundation" "-framework Cocoa")
    add_dependencies(sys01_au_updater sys01_AU)
    if(ALGORHYTHMIA_NATIVE_RT_PROBE)
        target_link_libraries(sys01_au_updater PRIVATE sys01_realtime_probe)
        target_compile_definitions(sys01_au_updater PRIVATE SYS01_NATIVE_RT_PROBE=1)
    endif()
    add_executable(sys01_au_registry AURegistryTests.cpp)
    algorhythmia_target_defaults(sys01_au_registry)
    target_link_libraries(sys01_au_registry PRIVATE "-framework AudioToolbox" "-framework CoreFoundation")
    add_dependencies(sys01_au_registry sys01_AU)
    if(ALGORHYTHMIA_NATIVE_RT_PROBE)
        target_link_libraries(sys01_au_registry PRIVATE sys01_realtime_probe)
        target_compile_definitions(sys01_au_registry PRIVATE SYS01_NATIVE_RT_PROBE=1)
    endif()
    add_executable(sys01_au_notifications AUNotificationTests.cpp)
    algorhythmia_target_defaults(sys01_au_notifications)
    target_link_libraries(sys01_au_notifications PRIVATE "-framework AudioToolbox" "-framework CoreFoundation")
    add_dependencies(sys01_au_notifications sys01_AU)
    if(ALGORHYTHMIA_NATIVE_RT_PROBE)
        target_link_libraries(sys01_au_notifications PRIVATE sys01_realtime_probe)
        target_compile_definitions(sys01_au_notifications PRIVATE SYS01_NATIVE_RT_PROBE=1)
    endif()
    add_executable(sys01_au_suppression AUSuppressionTests.cpp)
    algorhythmia_target_defaults(sys01_au_suppression)
    target_link_libraries(sys01_au_suppression PRIVATE "-framework AudioToolbox" "-framework CoreFoundation")
    add_dependencies(sys01_au_suppression sys01_AU)
    if(ALGORHYTHMIA_NATIVE_RT_PROBE)
        target_link_libraries(sys01_au_suppression PRIVATE sys01_realtime_probe)
        target_compile_definitions(sys01_au_suppression PRIVATE SYS01_NATIVE_RT_PROBE=1)
    endif()
    add_executable(sys01_au_contract AUContractTests.cpp)
    algorhythmia_target_defaults(sys01_au_contract)
    target_link_libraries(sys01_au_contract PRIVATE "-framework AudioToolbox" "-framework CoreFoundation" "-framework CoreMIDI")
    add_dependencies(sys01_au_contract sys01_AU)
    if(ALGORHYTHMIA_NATIVE_RT_PROBE)
        target_link_libraries(sys01_au_contract PRIVATE sys01_realtime_probe)
        target_compile_definitions(sys01_au_contract PRIVATE SYS01_NATIVE_RT_PROBE=1)
    endif()
    add_test(NAME au_native_callback COMMAND sys01_au_contract $<TARGET_BUNDLE_DIR:sys01_AU>)
    add_executable(sys01_au_offsets AUOffsetTests.cpp)
    algorhythmia_target_defaults(sys01_au_offsets)
    target_link_libraries(sys01_au_offsets PRIVATE algorhythmia::host "-framework AudioToolbox" "-framework CoreFoundation")
    add_dependencies(sys01_au_offsets sys01_AU)
    # Characterizes the known timing gap. Strict qualification is an explicit
    # --require-positioned run; a characterization pass never establishes parity.
    add_test(NAME au_offset_characterization COMMAND sys01_au_offsets $<TARGET_BUNDLE_DIR:sys01_AU>)
    add_test(NAME au_positioned_automation COMMAND sys01_au_offsets $<TARGET_BUNDLE_DIR:sys01_AU> --require-positioned)
    set_tests_properties(au_positioned_automation PROPERTIES TIMEOUT 240)
endif()

option(ALGORHYTHMIA_AU_OWNERSHIP_FIXTURE "Deterministic loaded-AU publication barrier; never shipping" OFF)
if(ALGORHYTHMIA_AU_OWNERSHIP_FIXTURE)
    if(NOT APPLE OR NOT BUILD_TESTING)
        message(FATAL_ERROR "AU ownership hooks require macOS fixtures")
    endif()
    target_compile_definitions(sys01_AU PRIVATE SYS01_AU_OWNERSHIP_FIXTURE=1)
    add_test(NAME au_host_notifications COMMAND sys01_au_notifications $<TARGET_BUNDLE_DIR:sys01_AU>)
    set_tests_properties(au_host_notifications PROPERTIES TIMEOUT 120)
    add_test(NAME au_native_suppression COMMAND sys01_au_suppression $<TARGET_BUNDLE_DIR:sys01_AU>)
    add_test(NAME au_captured_publication COMMAND sys01_au_offsets $<TARGET_BUNDLE_DIR:sys01_AU> --require-ownership)
    add_test(NAME au_reentrant_publication COMMAND sys01_au_offsets $<TARGET_BUNDLE_DIR:sys01_AU> --require-ownership-v3)
    set_tests_properties(au_captured_publication au_reentrant_publication PROPERTIES TIMEOUT 120)
endif()

option(ALGORHYTHMIA_VST3_OWNERSHIP_FIXTURE "Deterministic loaded-VST3 suppression hooks; never shipping" OFF)
if(ALGORHYTHMIA_VST3_OWNERSHIP_FIXTURE)
    if(NOT APPLE OR NOT BUILD_TESTING)
        message(FATAL_ERROR "VST3 ownership hooks require macOS fixtures")
    endif()
    target_compile_definitions(sys01_VST3 PRIVATE SYS01_VST3_OWNERSHIP_FIXTURE=1)
endif()
if(APPLE AND BUILD_TESTING)
    add_executable(sys01_vst3_suppression VST3SuppressionTests.cpp "${sys01_vst_sdk}/pluginterfaces/base/funknown.cpp")
    target_include_directories(sys01_vst3_suppression SYSTEM PRIVATE "${sys01_vst_sdk}")
    algorhythmia_target_defaults(sys01_vst3_suppression)
    target_link_libraries(sys01_vst3_suppression PRIVATE "-framework CoreFoundation")
    add_dependencies(sys01_vst3_suppression sys01_VST3)
    if(ALGORHYTHMIA_NATIVE_RT_PROBE)
        target_link_libraries(sys01_vst3_suppression PRIVATE sys01_realtime_probe)
        target_compile_definitions(sys01_vst3_suppression PRIVATE SYS01_NATIVE_RT_PROBE=1)
    endif()
    if(ALGORHYTHMIA_VST3_OWNERSHIP_FIXTURE)
        add_test(NAME vst3_native_suppression COMMAND sys01_vst3_suppression $<TARGET_BUNDLE_DIR:sys01_VST3>)
    endif()
endif()

# Reviewed AU ingress bounds, preserving upstream synchronization and notices.
if(APPLE)
    set(sys01_au_source "${sys01_juce_root}/modules/juce_audio_plugin_client/juce_audio_plugin_client_AU_1.mm")
    set(sys01_au_generated "${CMAKE_CURRENT_BINARY_DIR}/native/Sys01AU.mm")
    execute_process(COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/native_au_adapter.py"
        "${sys01_au_source}" "${sys01_au_generated}" COMMAND_ERROR_IS_FATAL ANY)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/scripts/native_au_adapter.py" "${sys01_au_source}")
    set_source_files_properties("${sys01_au_source}" PROPERTIES HEADER_FILE_ONLY TRUE)
    target_sources(sys01_AU PRIVATE "${sys01_au_generated}")
    target_include_directories(sys01_AU PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    target_link_libraries(sys01_AU PRIVATE algorhythmia::host)
    set(sys01_au_sdk_source "${sys01_juce_root}/modules/juce_audio_plugin_client/juce_audio_plugin_client_AU_2.mm")
    set(sys01_au_sdk_generated "${CMAKE_CURRENT_BINARY_DIR}/native/Sys01AUSDK.mm")
    execute_process(COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/native_au_dispatch.py"
        "${sys01_au_sdk_source}" "${sys01_au_sdk_generated}" COMMAND_ERROR_IS_FATAL ANY)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/scripts/native_au_dispatch.py" "${sys01_au_sdk_source}"
        "${sys01_juce_root}/modules/juce_audio_plugin_client/AU/AudioUnitSDK/AUPlugInDispatch.cpp")
    set_source_files_properties("${sys01_au_sdk_source}" PROPERTIES HEADER_FILE_ONLY TRUE)
    target_sources(sys01_AU PRIVATE "${sys01_au_sdk_generated}")
endif()
# Instrument generated format glue too; a sanitized core alone misses this sprint.
foreach(format VST3 AU Standalone)
    if(TARGET sys01_${format})
        algorhythmia_enable_sanitizers(sys01_${format})
    endif()
endforeach()

# JUCE creates VST3 moduleinfo.json after its initial ad-hoc signing step.
# Seal every final development bundle, including the standalone app's resource
# manifest. This is local ad-hoc integrity, not release signing.
if(APPLE)
    foreach(format VST3 AU Standalone)
        # Resource-only edits must relink so POST_BUILD reseals the bundle.
        set_property(TARGET sys01_${format} APPEND PROPERTY LINK_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/assets/sys01-icon.svg"
            "${CMAKE_CURRENT_SOURCE_DIR}/assets/sys01-icon-small.svg")
        add_custom_command(TARGET sys01_${format} POST_BUILD
            COMMAND /usr/bin/codesign --force --sign - "$<TARGET_BUNDLE_DIR:sys01_${format}>"
            COMMENT "Ad-hoc sealing final SYS 01 ${format} development bundle"
            VERBATIM)
    endforeach()
endif()

# Fingerprint entire bundles, including seals and metadata, on macOS/Windows.
set(sys01_provenance_arguments --artifact "$<TARGET_FILE:sys01_VST3>")
if(APPLE)
    set(sys01_provenance_arguments --artifact "$<TARGET_BUNDLE_DIR:sys01_VST3>"
        --artifact "$<TARGET_BUNDLE_DIR:sys01_AU>")
elseif(WIN32)
    set(sys01_provenance_arguments --artifact "$<TARGET_FILE_DIR:sys01_VST3>/../..")
endif()
add_custom_target(candidate-provenance
    COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/write_build_provenance.py"
        --build "${CMAKE_BINARY_DIR}" ${sys01_provenance_arguments}
    DEPENDS sys01_VST3
    VERBATIM)
if(APPLE)
    add_dependencies(candidate-provenance sys01_AU)
endif()
