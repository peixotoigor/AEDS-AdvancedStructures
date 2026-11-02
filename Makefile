CXX ?= g++
CXXFLAGS = -O3 -std=c++20 -Wall -I src/include
PYTHON ?= python3

BIN_DIR = bin
SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core

TARGET_TEST = $(BIN_DIR)/test_estruturas
TARGET_ESTUDO1 = $(BIN_DIR)/benchmark_estudo1
TARGET_ESTUDO2_C1 = $(BIN_DIR)/benchmark_estudo2_cenario1
TARGET_ESTUDO2_C2 = $(BIN_DIR)/benchmark_estudo2_cenario2
TARGET_GERAR_DADOS = $(BIN_DIR)/gerar_dados

.PHONY: all clean test benchmarks estudo1 estudo2_c1 estudo2_c2 graficos help

all: directories $(TARGET_TEST) $(TARGET_ESTUDO1) $(TARGET_ESTUDO2_C1) $(TARGET_ESTUDO2_C2) $(TARGET_GERAR_DADOS)
	@echo =======================================================
	@echo  Compilacao concluida com sucesso em $(BIN_DIR)/
	@echo  Execute 'make test' para validar a corretude.
	@echo =======================================================

directories:
	@mkdir -p $(BIN_DIR) 2>nul || mkdir $(BIN_DIR) 2>nul || true

$(TARGET_TEST): tests/test_estruturas.cpp $(CORE_DIR)/veb.cpp $(CORE_DIR)/heap_fibonacci.cpp $(CORE_DIR)/heap_dario.cpp $(CORE_DIR)/dijkstra.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test: $(TARGET_TEST)
	@echo Executando testes de corretude das estruturas...
	@$(TARGET_TEST)

$(TARGET_ESTUDO1): $(SRC_DIR)/benchmark_estudo1.cpp $(CORE_DIR)/veb.cpp $(CORE_DIR)/heap_fibonacci.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

estudo1: $(TARGET_ESTUDO1)
	@$(TARGET_ESTUDO1)

$(TARGET_ESTUDO2_C1): $(SRC_DIR)/benchmark_estudo2_cenario1.cpp $(CORE_DIR)/veb.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

estudo2_c1: $(TARGET_ESTUDO2_C1)
	@$(TARGET_ESTUDO2_C1)

$(TARGET_ESTUDO2_C2): $(SRC_DIR)/benchmark_estudo2_cenario2.cpp $(CORE_DIR)/dijkstra.cpp $(CORE_DIR)/heap_fibonacci.cpp $(CORE_DIR)/heap_dario.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

estudo2_c2: $(TARGET_ESTUDO2_C2)
	@$(TARGET_ESTUDO2_C2)

$(TARGET_GERAR_DADOS): scripts/gerar_dados.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

benchmarks: estudo1 estudo2_c1 estudo2_c2

clean:
	@rm -rf $(BIN_DIR) 2>nul || rmdir /s /q $(BIN_DIR) 2>nul || true

help:
	@echo Comandos disponiveis:
	@echo   make              - Compila todos os programas para a pasta bin/
	@echo   make test         - Roda os testes unitarios de corretude (rapido)
	@echo   make estudo1      - Executa o benchmark do Estudo 1 (Fibonacci vs vEB)
	@echo   make estudo2_c1   - Executa o benchmark do Estudo 2 Cenario 1 (vEB vs AVL vs RB)
	@echo   make estudo2_c2   - Executa o benchmark do Estudo 2 Cenario 2 (Dijkstra)
	@echo   make clean        - Remove os binarios compilados

