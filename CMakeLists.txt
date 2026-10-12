cmake_minimum_required(VERSION 3.22)
set(CMAKE_OSX_ARCHITECTURES arm64 CACHE STRING "Apple Silicon")
set(CMAKE_OSX_DEPLOYMENT_TARGET 12.0 CACHE STRING "Minimum macOS")
project(KoenichUltraViolet VERSION 1.0.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(JUCE_SOURCE_DIR "" CACHE PATH "Local JUCE 8.0.15 checkout")
if(JUCE_SOURCE_DIR)
 add_subdirectory("${JUCE_SOURCE_DIR}" JUCE)
else()
 include(FetchContent)
 FetchContent_Declare(JUCE GIT_REPOSITORY https://github.com/juce-framework/JUCE.git GIT_TAG 8.0.15 GIT_SHALLOW TRUE)
 FetchContent_MakeAvailable(JUCE)
endif()
juce_add_binary_data(UVEQAssets SOURCES Assets/frontpanel.png)
juce_add_plugin(KoenichUltraViolet COMPANY_NAME "Koenich Sound"
 BUNDLE_ID "sound.koenich.recall.ultraviolet" PLUGIN_MANUFACTURER_CODE Koen PLUGIN_CODE Kuv1
 FORMATS VST3 PRODUCT_NAME "Koenich Recall - SSL UltraViolet EQ"
 IS_SYNTH FALSE NEEDS_MIDI_INPUT FALSE NEEDS_MIDI_OUTPUT FALSE IS_MIDI_EFFECT FALSE
 EDITOR_WANTS_KEYBOARD_FOCUS FALSE COPY_PLUGIN_AFTER_BUILD FALSE VST3_CATEGORIES Fx Tools)
target_sources(KoenichUltraViolet PRIVATE Source/PluginProcessor.cpp Source/PluginEditor.cpp)
target_compile_definitions(KoenichUltraViolet PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)
target_link_libraries(KoenichUltraViolet PRIVATE UVEQAssets juce::juce_audio_utils PUBLIC juce::juce_recommended_config_flags juce::juce_recommended_warning_flags)
option(RECALL_BUILD_TESTS "Build functional tests and GUI capture" ON)
if(RECALL_BUILD_TESTS)
 juce_add_console_app(UVEQTests PRODUCT_NAME UVEQTests)
 target_sources(UVEQTests PRIVATE Tests/RecallTests.cpp Source/PluginProcessor.cpp Source/PluginEditor.cpp)
 target_compile_definitions(UVEQTests PRIVATE JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)
 target_link_libraries(UVEQTests PRIVATE UVEQAssets juce::juce_audio_utils juce::juce_recommended_config_flags)
 enable_testing()
 add_test(NAME UVEQTests COMMAND UVEQTests)
endif()
# JUCE generates moduleinfo.json after the linker signs the bundle; seal again last.
if(APPLE)
 add_custom_command(TARGET KoenichUltraViolet_VST3 POST_BUILD
  COMMAND /usr/bin/codesign --force --sign - --timestamp=none "$<TARGET_BUNDLE_DIR:KoenichUltraViolet_VST3>"
  VERBATIM)
endif()
