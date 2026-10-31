#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <climits>
#include "heap_binario.hpp"
#include "leitor_dados.hpp"

using namespace std;

// Executa o ciclo completo de testes para o HeapBinario em um conjunto de dados
void testarHeapBinario(const string& nome_conjunto, const string& caminho_dados, const vector<ParDecreaseKey>& decrease_pairs, int n) {
    cout << "\n==================================================" << endl;
    cout << " Testando HeapBinario - " << nome_conjunto << endl;
    cout << "==================================================" << endl;

    // 1. Carregamento do arquivo de dados
    vector<int> dados = lerDadosArquivo(n, caminho_dados);
    HeapBinario heap;
    
    // Vetor auxiliar para acompanhar a chave atual de cada ID (vertice)
    vector<int> chaves_atuais(n);

    // 2. Teste de Inserção (insert)
    auto t1 = chrono::steady_clock::now();
    for (int i = 0; i < n; i++) {
        chaves_atuais[i] = dados[i];
        heap.insert(dados[i], i); // chave = valor do arquivo, valor = id/índice (i)
    }
    auto t2 = chrono::steady_clock::now();
    chrono::duration<double, milli> tempo_insert = t2 - t1;
    cout << " [1] Insert (" << n << " elementos): " << fixed << setprecision(2) << tempo_insert.count() << " ms" << endl;

    // 3. Teste de Redução de Chave (decrease_key)
    auto t3 = chrono::steady_clock::now();
    for (const auto& par : decrease_pairs) {
        if (par.id < n) {
            chaves_atuais[par.id] -= par.diminuir;
            heap.decrease_key(par.id, chaves_atuais[par.id]);
        }
    }
    auto t4 = chrono::steady_clock::now();
    chrono::duration<double, milli> tempo_decrease = t4 - t3;
    cout << " [2] Decrease-Key (" << decrease_pairs.size() << " operacoes): " << tempo_decrease.count() << " ms" << endl;

    // 4. Teste de Extração do Mínimo (extract_min)
    bool ordenado_corretamente = true;
    int chave_anterior = INT_MIN;

    auto t5 = chrono::steady_clock::now();
    while (!heap.esta_vazio()) {
        HeapBinario::NoBinario min_node = heap.extract_min();
        if (min_node.chave < chave_anterior) {
            ordenado_corretamente = false;
        }
        chave_anterior = min_node.chave;
    }
    auto t6 = chrono::steady_clock::now();
    chrono::duration<double, milli> tempo_extract = t6 - t5;
    cout << " [3] Extract-Min (" << n << " elementos): " << tempo_extract.count() << " ms" << endl;

    // Validação da ordenação
    if (ordenado_corretamente) {
        cout << " [OK] Extracao validada: elementos removidos em ordem crescente!" << endl;
    } else {
        cout << " [ERRO] Inconsistencia detectada durante a extracao!" << endl;
    }

    chrono::duration<double, milli> tempo_total = tempo_insert + tempo_decrease + tempo_extract;
    cout << " -> Tempo total do Heap: " << tempo_total.count() << " ms" << endl;
}

int main() {
    cout << "==================================================" << endl;
    cout << "       BENCHMARK HEAP BINARIO COM ARQUIVOS       " << endl;
    cout << "==================================================" << endl;

    int N = 1000000; // Quantidade de elementos a testar

    string file_crescente   = "data/crescente.dat";
    string file_decrescente = "data/decrescente.dat";
    string file_aleatorio   = "data/aleatorio.dat";
    string file_decrease    = "data/decrease_key.dat";

    try {
        // Carrega o arquivo de decrease_key apenas uma vez para reusar nos testes
        cout << "\nLendo arquivo de decrease-key..." << endl;
        vector<ParDecreaseKey> decrease_pairs = lerDecreaseKey(N, file_decrease);
        cout << "[OK] " << decrease_pairs.size() << " pares carregados." << endl;

        // Executa os testes nos 3 cenários
        testarHeapBinario("Cenario Crescente", file_crescente, decrease_pairs, N);
        testarHeapBinario("Cenario Decrescente", file_decrescente, decrease_pairs, N);
        testarHeapBinario("Cenario Aleatorio", file_aleatorio, decrease_pairs, N);

    } catch (const exception& e) {
        cerr << "\n[ERRO CRITICO] " << e.what() << endl;
        return 1;
    }

    cout << "\n==================================================" << endl;
    cout << "              BENCHMARK FINALIZADO               " << endl;
    cout << "==================================================" << endl;

    return 0;
}