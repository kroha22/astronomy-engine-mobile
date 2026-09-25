cmake_minimum_required(VERSION 3.22.1)
project(astronomy_engine_mobile C)

get_filename_component(REPOSITORY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../.." ABSOLUTE)

add_library(
    astronomy_engine_mobile
    SHARED
    astronomy_engine_mobile_jni.c
    "${REPOSITORY_ROOT}/Sources/CAstronomyEngine/astronomy_engine_mobile.c"
    "${REPOSITORY_ROOT}/Sources/CAstronomyEngine/astronomy.c"
)

target_include_directories(
    astronomy_engine_mobile
    PRIVATE
    "${REPOSITORY_ROOT}/Sources/CAstronomyEngine/include"
)

target_compile_definitions(
    astronomy_engine_mobile
    PRIVATE
    ASTRONOMY_ENGINE_NO_CURRENT_TIME
)

target_link_libraries(astronomy_engine_mobile m)
