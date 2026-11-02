#!/usr/bin/env bash
set -e

CMD=${1:-all}

mkdir -p bin

case "$CMD" in
    all)
        echo "[BUILD] Compilando executaveis do projeto em bin/..."
        g++ -O3 -std=c++20 -Wall -I src/include tests/test_estruturas.cpp src/core/veb.cpp src/core/heap_fibonacci.cpp src/core/heap_dario.cpp src/core/dijkstra.cpp -o bin/test_estruturas
        g++ -O3 -std=c++20 -Wall -I src/include src/benchmark_estudo1.cpp src/core/veb.cpp src/core/heap_fibonacci.cpp -o bin/benchmark_estudo1
        g++ -O3 -std=c++20 -Wall -I src/include src/benchmark_estudo2_cenario1.cpp src/core/veb.cpp -o bin/benchmark_estudo2_cenario1
        g++ -O3 -std=c++20 -Wall -I src/include src/benchmark_estudo2_cenario2.cpp src/core/dijkstra.cpp src/core/heap_fibonacci.cpp src/core/heap_dario.cpp -o bin/benchmark_estudo2_cenario2
        echo "[OK] Compilacao concluida com sucesso em bin/!"
        echo "Para executar os testes: ./build.sh test"
        ;;
    test)
        echo "[BUILD] Compilando testes unitarios..."
        g++ -O3 -std=c++20 -Wall -I src/include tests/test_estruturas.cpp src/core/veb.cpp src/core/heap_fibonacci.cpp src/core/heap_dario.cpp src/core/dijkstra.cpp -o bin/test_estruturas
        echo "[RUN] Executando testes..."
        ./bin/test_estruturas
        ;;
    estudo1)
        g++ -O3 -std=c++20 -Wall -I src/include src/benchmark_estudo1.cpp src/core/veb.cpp src/core/heap_fibonacci.cpp -o bin/benchmark_estudo1
        echo "[RUN] Executando Estudo 1..."
        ./bin/benchmark_estudo1
        ;;
    estudo2_c1)
        g++ -O3 -std=c++20 -Wall -I src/include src/benchmark_estudo2_cenario1.cpp src/core/veb.cpp -o bin/benchmark_estudo2_cenario1
        echo "[RUN] Executando Estudo 2 Cenario 1..."
        ./bin/benchmark_estudo2_cenario1
        ;;
    estudo2_c2)
        g++ -O3 -std=c++20 -Wall -I src/include src/benchmark_estudo2_cenario2.cpp src/core/dijkstra.cpp src/core/heap_fibonacci.cpp src/core/heap_dario.cpp -o bin/benchmark_estudo2_cenario2
        echo "[RUN] Executando Estudo 2 Cenario 2..."
        ./bin/benchmark_estudo2_cenario2
        ;;
    clean)
        rm -rf bin
        echo "[OK] Pasta bin/ removida."
        ;;
    *)
        echo "Opcao invalida: $CMD"
        echo "Uso: ./build.sh [all|test|estudo1|estudo2_c1|estudo2_c2|clean]"
        exit 1
        ;;
esac

