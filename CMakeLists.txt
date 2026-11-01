cmake_minimum_required(VERSION 3.20)
project(AEDS_VanEmdeBoas CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Biblioteca de suporte procedural da Arvore de van Emde Boas
add_library(van_emde_boas STATIC
    src/core/van_emde_boas.cpp
)
target_include_directories(van_emde_boas PUBLIC
    src/include
)

# Demonstracao e exemplo de uso (main)
add_executable(veb_demo
    src/core/main.cpp
)
target_link_libraries(veb_demo PRIVATE van_emde_boas)

# Suite de testes e verificacao de invariantes
add_executable(veb_tests
    tests/veb_tests.cpp
)
target_link_libraries(veb_tests PRIVATE van_emde_boas)

# Suite de benchmarks de desempenho e consumo de memoria
add_executable(benchmark_veb
    tests/benchmark_veb.cpp
)
target_link_libraries(benchmark_veb PRIVATE van_emde_boas)

enable_testing()
add_test(NAME TestesVanEmdeBoas COMMAND veb_tests)
