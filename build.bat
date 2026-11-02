@echo off
setlocal enabledelayedexpansion

if "%1"=="" goto all
if "%1"=="test" goto test
if "%1"=="estudo1" goto estudo1
if "%1"=="estudo2_c1" goto estudo2_c1
if "%1"=="estudo2_c2" goto estudo2_c2
if "%1"=="clean" goto clean

:all
echo [BUILD] Compilando executaveis do projeto em bin/...
if not exist bin mkdir bin
g++ -O3 -std=c++20 -I src/include tests/test_estruturas.cpp src/core/veb.cpp src/core/heap_fibonacci.cpp src/core/heap_dario.cpp src/core/dijkstra.cpp -o bin\test_estruturas.exe
g++ -O3 -std=c++20 -I src/include src/benchmark_estudo1.cpp src/core/veb.cpp src/core/heap_fibonacci.cpp -o bin\benchmark_estudo1.exe
g++ -O3 -std=c++20 -I src/include src/benchmark_estudo2_cenario1.cpp src/core/veb.cpp -o bin\benchmark_estudo2_cenario1.exe
g++ -O3 -std=c++20 -I src/include src/benchmark_estudo2_cenario2.cpp src/core/dijkstra.cpp src/core/heap_fibonacci.cpp src/core/heap_dario.cpp -o bin\benchmark_estudo2_cenario2.exe
echo [OK] Compilacao concluida com sucesso!
echo Para rodar os testes: build.bat test
goto end

:test
if not exist bin mkdir bin
echo [BUILD] Compilando testes unitarios...
g++ -O3 -std=c++20 -I src/include tests/test_estruturas.cpp src/core/veb.cpp src/core/heap_fibonacci.cpp src/core/heap_dario.cpp src/core/dijkstra.cpp -o bin\test_estruturas.exe
if %ERRORLEVEL% EQU 0 (
    echo [RUN] Executando testes...
    bin\test_estruturas.exe
)
goto end

:estudo1
if not exist bin mkdir bin
g++ -O3 -std=c++20 -I src/include src/benchmark_estudo1.cpp src/core/veb.cpp src/core/heap_fibonacci.cpp -o bin\benchmark_estudo1.exe
if %ERRORLEVEL% EQU 0 (
    echo [RUN] Executando Estudo 1 (Filas de Prioridade)...
    bin\benchmark_estudo1.exe
)
goto end

:estudo2_c1
if not exist bin mkdir bin
g++ -O3 -std=c++20 -I src/include src/benchmark_estudo2_cenario1.cpp src/core/veb.cpp -o bin\benchmark_estudo2_cenario1.exe
if %ERRORLEVEL% EQU 0 (
    echo [RUN] Executando Estudo 2 Cenario 1 (Arvores de Busca)...
    bin\benchmark_estudo2_cenario1.exe
)
goto end

:estudo2_c2
if not exist bin mkdir bin
g++ -O3 -std=c++20 -I src/include src/benchmark_estudo2_cenario2.cpp src/core/dijkstra.cpp src/core/heap_fibonacci.cpp src/core/heap_dario.cpp -o bin\benchmark_estudo2_cenario2.exe
if %ERRORLEVEL% EQU 0 (
    echo [RUN] Executando Estudo 2 Cenario 2 (Dijkstra)...
    bin\benchmark_estudo2_cenario2.exe
)
goto end

:clean
if exist bin rmdir /s /q bin
echo [OK] Binarios removidos.
goto end

:end
