cmake_minimum_required(VERSION 3.16)
project(AEDS_AdvancedStructures CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/O2 /W3)
else()
    add_compile_options(-O3 -Wall)
endif()

include_directories(src/include)

# Testes de corretude das estruturas
add_executable(test_estruturas
    tests/test_estruturas.cpp
    src/core/veb.cpp
    src/core/heap_fibonacci.cpp
    src/core/heap_dario.cpp
    src/core/dijkstra.cpp
)

# Estudo 1: Filas de Prioridade (Fibonacci vs vEB)
add_executable(benchmark_estudo1
    src/benchmark_estudo1.cpp
    src/core/veb.cpp
    src/core/heap_fibonacci.cpp
)

# Estudo 2 Cenario 1: Arvores de Busca (vEB vs AVL vs Rubro-Negra)
add_executable(benchmark_estudo2_cenario1
    src/benchmark_estudo2_cenario1.cpp
    src/core/veb.cpp
)

# Estudo 2 Cenario 2: Algoritmo de Dijkstra com Heaps
add_executable(benchmark_estudo2_cenario2
    src/benchmark_estudo2_cenario2.cpp
    src/core/dijkstra.cpp
    src/core/heap_fibonacci.cpp
    src/core/heap_dario.cpp
)

# Gerador de Dados Sinteticos
add_executable(gerar_dados
    scripts/gerar_dados.cpp
)

# Medidor de Memoria com Valgrind
add_executable(medir_memoria
    src/medir_memoria_valgrind.cpp
    src/core/veb.cpp
    src/core/heap_fibonacci.cpp
    src/core/dijkstra.cpp
    src/core/heap_dario.cpp
)

