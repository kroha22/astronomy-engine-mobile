cmake_minimum_required(VERSION 3.22)
project(astronomy_engine_mobile C)

add_library(
    astronomy_engine_mobile
    STATIC
    Sources/CAstronomyEngine/astronomy_engine_mobile.c
    Sources/CAstronomyEngine/astronomy.c
)
target_include_directories(
    astronomy_engine_mobile
    PUBLIC Sources/CAstronomyEngine/include
)
target_compile_definitions(
    astronomy_engine_mobile
    PRIVATE ASTRONOMY_ENGINE_NO_CURRENT_TIME
)
target_link_libraries(astronomy_engine_mobile PUBLIC m)

enable_testing()
add_executable(astronomy_engine_mobile_test native-tests/contract_test.c)
target_link_libraries(astronomy_engine_mobile_test PRIVATE astronomy_engine_mobile)
add_test(NAME contract COMMAND astronomy_engine_mobile_test)
