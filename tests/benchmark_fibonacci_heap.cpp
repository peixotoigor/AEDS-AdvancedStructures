#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <chrono>
#include <random>
#include <numeric>
#include <iomanip>
#include <windows.h>
#include <psapi.h>

#include "fibonacci_heap.hpp"

// Obtém o consumo de memória do processo atual em bytes (PrivateUsage)
static size_t obter_memoria_processo_bytes() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.PrivateUsage;
    }
    return 0;
}

struct ResultadoTempo {
    std::string estrutura;
    std::string operacao;
    int n;
    int repeticao;
    double tempo_us;
};

struct ResultadoMemoria {
    std::string estrutura;
    int n;
    int repeticao;
    size_t memoria_teorica_bytes;
    size_t memoria_processo_bytes;
};

int main() {
    std::cout << "========================================================\n";
    std::cout << " INICIANDO BENCHMARK: FIBONACCI HEAP (10 REPETICOES)\n";
    std::cout << "========================================================\n\n";

    const std::vector<int> tamanhos_n = {
        100, 1000, 5000, 10000, 50000, 100000, 500000, 1000000
    };
    const int REPETICOES = 10;

    std::vector<ResultadoTempo> resultados_tempo;
    std::vector<ResultadoMemoria> resultados_memoria;

    // Gerador determinístico de números aleatórios
    std::mt19937 rng(1337);
    std::uniform_int_distribution<int> dist(-10000000, 10000000);

    for (int n : tamanhos_n) {
        std::cout << ">>> Avaliando N = " << std::setw(8) << n << " (" << REPETICOES << " repeticoes)... " << std::flush;

        for (int rep = 1; rep <= REPETICOES; ++rep) {
            // Gera vetor de chaves para este teste
            std::vector<int> dados(n);
            for (int i = 0; i < n; ++i) {
                dados[i] = dist(rng);
            }

            // ----------------------------------------------------
            // 1. FIBONACCI HEAP: INSERÇÃO
            // ----------------------------------------------------
            HeapFibonacci fheap = criar_heap();
            std::vector<NoFibonacci*> nos_inseridos(n);

            size_t mem_antes = obter_memoria_processo_bytes();
            auto t_ini = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < n; ++i) {
                nos_inseridos[i] = inserir(&fheap, dados[i]);
            }
            auto t_fim = std::chrono::high_resolution_clock::now();
            size_t mem_depois = obter_memoria_processo_bytes();

            double tempo_insert_us = std::chrono::duration<double, std::micro>(t_fim - t_ini).count();
            resultados_tempo.push_back({"fibonacci_heap", "insert", n, rep, tempo_insert_us});

            size_t mem_teorica_fib = (size_t)n * sizeof(NoFibonacci) + sizeof(HeapFibonacci);
            size_t mem_proc_diff = (mem_depois > mem_antes) ? (mem_depois - mem_antes) : 0;
            resultados_memoria.push_back({"fibonacci_heap", n, rep, mem_teorica_fib, mem_proc_diff});

            // ----------------------------------------------------
            // 2. FIBONACCI HEAP: DECREASE-KEY (em K = N / 10 elementos)
            // ----------------------------------------------------
            int qtd_dec = std::max(10, n / 10);
            t_ini = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < qtd_dec; ++i) {
                // Diminui chave para um valor menor que a chave atual
                int novo_val = nos_inseridos[i]->chave - 1000;
                diminuir_chave(&fheap, nos_inseridos[i], novo_val);
            }
            t_fim = std::chrono::high_resolution_clock::now();
            double tempo_decrease_us = std::chrono::duration<double, std::micro>(t_fim - t_ini).count();
            // Normaliza para o custo equivalente por operação ou armazena total
            resultados_tempo.push_back({"fibonacci_heap", "decrease_key", n, rep, tempo_decrease_us});

            // ----------------------------------------------------
            // 3. FIBONACCI HEAP: EXTRAÇÃO DO MÍNIMO (N extrações)
            // ----------------------------------------------------
            t_ini = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < n; ++i) {
                int dummy = 0;
                extrair_minimo(&fheap, &dummy);
            }
            t_fim = std::chrono::high_resolution_clock::now();
            double tempo_extract_us = std::chrono::duration<double, std::micro>(t_fim - t_ini).count();
            resultados_tempo.push_back({"fibonacci_heap", "extract_min", n, rep, tempo_extract_us});

            destruir_heap(&fheap);

            // ----------------------------------------------------
            // 4. STD::PRIORITY_QUEUE: INSERÇÃO
            // ----------------------------------------------------
            std::priority_queue<int, std::vector<int>, std::greater<int>> pq;

            size_t mem_pq_antes = obter_memoria_processo_bytes();
            t_ini = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < n; ++i) {
                pq.push(dados[i]);
            }
            t_fim = std::chrono::high_resolution_clock::now();
            size_t mem_pq_depois = obter_memoria_processo_bytes();

            double tempo_pq_insert_us = std::chrono::duration<double, std::micro>(t_fim - t_ini).count();
            resultados_tempo.push_back({"priority_queue", "insert", n, rep, tempo_pq_insert_us});

            size_t mem_teorica_pq = (size_t)n * sizeof(int);
            size_t mem_pq_diff = (mem_pq_depois > mem_pq_antes) ? (mem_pq_depois - mem_pq_antes) : 0;
            resultados_memoria.push_back({"priority_queue", n, rep, mem_teorica_pq, mem_pq_diff});

            // ----------------------------------------------------
            // 5. STD::PRIORITY_QUEUE: EXTRAÇÃO DO MÍNIMO
            // ----------------------------------------------------
            t_ini = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < n; ++i) {
                pq.pop();
            }
            t_fim = std::chrono::high_resolution_clock::now();
            double tempo_pq_extract_us = std::chrono::duration<double, std::micro>(t_fim - t_ini).count();
            resultados_tempo.push_back({"priority_queue", "extract_min", n, rep, tempo_pq_extract_us});
        }

        std::cout << "Concluido!\n";
    }

    std::cout << "\nSalvando resultados em benchmarks/data/...\n";

    // 1. Arquivo completo de tempos
    std::ofstream arq_tempo("benchmarks/data/tempo_por_entrada.txt");
    arq_tempo << "# estrutura operacao entrada_n repeticao tempo_us\n";
    for (const auto& r : resultados_tempo) {
        arq_tempo << r.estrutura << " " << r.operacao << " " << r.n << " " << r.repeticao << " " << std::fixed << std::setprecision(3) << r.tempo_us << "\n";
    }
    arq_tempo.close();

    // 2. Arquivo completo de memória
    std::ofstream arq_mem("benchmarks/data/memoria_por_entrada.txt");
    arq_mem << "# estrutura entrada_n repeticao memoria_teorica_bytes memoria_processo_bytes\n";
    for (const auto& r : resultados_memoria) {
        arq_mem << r.estrutura << " " << r.n << " " << r.repeticao << " " << r.memoria_teorica_bytes << " " << r.memoria_processo_bytes << "\n";
    }
    arq_mem.close();

    // 3. Arquivos diretos simplificados de 2 colunas com as médias
    auto calcular_medias_tempo = [&](const std::string& est, const std::string& op) {
        std::map<int, std::vector<double>> agrupado;
        for (const auto& r : resultados_tempo) {
            if (r.estrutura == est && r.operacao == op) {
                agrupado[r.n].push_back(r.tempo_us);
            }
        }
        std::vector<std::pair<int, double>> medias;
        for (auto& par : agrupado) {
            double soma = std::accumulate(par.second.begin(), par.second.end(), 0.0);
            medias.push_back({par.first, soma / par.second.size()});
        }
        return medias;
    };

    auto salvar_duas_colunas = [](const std::string& caminho, const std::vector<std::pair<int, double>>& dados, const std::string& col2) {
        std::ofstream arq(caminho);
        arq << "# entrada_n " << col2 << "\n";
        for (const auto& p : dados) {
            arq << p.first << " " << std::fixed << std::setprecision(3) << p.second << "\n";
        }
        arq.close();
    };

    salvar_duas_colunas("benchmarks/data/fibonacci_tempo_insercao.txt", calcular_medias_tempo("fibonacci_heap", "insert"), "tempo_medio_us");
    salvar_duas_colunas("benchmarks/data/fibonacci_tempo_extracao.txt", calcular_medias_tempo("fibonacci_heap", "extract_min"), "tempo_medio_us");
    salvar_duas_colunas("benchmarks/data/fibonacci_tempo_decrease_key.txt", calcular_medias_tempo("fibonacci_heap", "decrease_key"), "tempo_medio_us");
    salvar_duas_colunas("benchmarks/data/priority_queue_tempo_insercao.txt", calcular_medias_tempo("priority_queue", "insert"), "tempo_medio_us");
    salvar_duas_colunas("benchmarks/data/priority_queue_tempo_extracao.txt", calcular_medias_tempo("priority_queue", "extract_min"), "tempo_medio_us");

    // Salvar memória simplificada (N vs bytes teóricos)
    std::ofstream arq_mem_fib("benchmarks/data/fibonacci_memoria.txt");
    arq_mem_fib << "# entrada_n memoria_teorica_bytes\n";
    for (int n : tamanhos_n) {
        size_t mem_fib = (size_t)n * sizeof(NoFibonacci) + sizeof(HeapFibonacci);
        arq_mem_fib << n << " " << mem_fib << "\n";
    }
    arq_mem_fib.close();

    std::cout << "Todos os arquivos .txt foram gravados com sucesso em benchmarks/data/!\n";
    std::cout << "========================================================\n";
    return 0;
}
