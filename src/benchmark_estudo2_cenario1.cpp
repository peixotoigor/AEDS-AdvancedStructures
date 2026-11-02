#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <filesystem>
#include <random>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#elif defined(__linux__)
#include <sys/resource.h>
#endif

#include "avl.hpp"
#include "rb.hpp"
#include "veb.hpp"

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

const int REPETICOES = 10;
const int SEMENTE = 42;
const int UNIVERSO_U = 1048576; // 2^20
const vector<int> TAMANHOS_N = {100, 1000, 10000, 100000, 1000000};
const vector<pair<string, string>> PADROES_ENTRADA = {
    {"cresce", "data/crescente.dat"},
    {"decresce", "data/decrescente.dat"},
    {"aleatorio", "data/aleatorio.dat"}
};

static size_t obter_memoria_kb() {
#if defined(_WIN32)
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize / 1024;
    }
    return 0;
#elif defined(__linux__)
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        return usage.ru_maxrss;
    }
    return 0;
#else
    return 0;
#endif
}

struct RegistroResultado {
    string estrutura;
    string padrao_entrada;
    int n;
    int universo_u;
    double razao_ocupacao_rho;
    string operacao;
    double tempo_medio_us;
    double desvio_padrao_us;
    double tempo_por_op_ns;
    size_t memoria_kb;
};

static double calcular_media(const vector<double>& v) {
    if (v.empty()) return 0.0;
    double s = 0.0;
    for (double x : v) s += x;
    return s / v.size();
}

static double calcular_desvio_padrao(const vector<double>& v, double media) {
    if (v.size() < 2) return 0.0;
    double s = 0.0;
    for (double x : v) s += (x - media) * (x - media);
    return std::sqrt(s / (v.size() - 1));
}

static vector<int> carregar_prefixo(const string& caminho, int n) {
    vector<int> dados;
    dados.reserve(n);
    ifstream arq(caminho);
    if (!arq.is_open()) {
        cerr << "[ERRO] Arquivo nao encontrado: " << caminho << endl;
        exit(1);
    }
    int val;
    while (dados.size() < static_cast<size_t>(n) && arq >> val) {
        dados.push_back(val);
    }
    return dados;
}

volatile uint64_t g_acumulador_estudo2 = 0;

int main() {
    cout << "========================================================================\n";
    cout << "  ESTUDO 2 (CENARIO 1): vEB vs ARVORE AVL vs ARVORE RUBRO-NEGRA       \n";
    cout << "  Manutencao de Conjuntos Ordenados | seed = " << SEMENTE << " | C++20 (-O3)      \n";
    cout << "========================================================================\n\n";

    fs::create_directories("results");
    string caminho_csv = "results/estudo2_cenario1_arvores.csv";
    ofstream csv(caminho_csv);
    csv << "estrutura,padrao_entrada,n,universo_u,razao_ocupacao_rho,operacao,"
        << "tempo_medio_us,desvio_padrao_us,tempo_por_op_ns,memoria_kb\n";

    mt19937 rng(SEMENTE);

    for (const auto& [padrao_nome, padrao_arquivo] : PADROES_ENTRADA) {
        cout << "\n>>> PADRAO DE ENTRADA: " << padrao_nome << " (" << padrao_arquivo << ")\n";
        cout << "------------------------------------------------------------------------\n";

        for (int n : TAMANHOS_N) {
            double rho = static_cast<double>(n) / UNIVERSO_U;
            cout << "\n[N = " << setw(7) << n << " | U = " << UNIVERSO_U << " | rho = " << fixed << setprecision(6) << rho << "]\n";

            vector<int> dados = carregar_prefixo(padrao_arquivo, n);

            // Gerar chaves para busca por pertinência (50% presentes, 50% ausentes)
            vector<int> chaves_busca(n);
            for (int i = 0; i < n; i++) {
                if (i % 2 == 0) chaves_busca[i] = dados[i];
                else chaves_busca[i] = (dados[i] + UNIVERSO_U / 2) % UNIVERSO_U; // valor ausente
            }

            // Gerar amostra para consultas de sucessor e predecessor
            vector<int> chaves_consulta = dados;
            shuffle(chaves_consulta.begin(), chaves_consulta.end(), rng);
            int k_consultas = min(n, 10000);

            // =================================================================
            // 1. ÁRVORE AVL
            // =================================================================
            {
                vector<double> t_ins, t_search, t_minmax, t_succpred, t_del;
                size_t mem_pico = 0;

                // Warm-up
                {
                    AVLTree avl_w;
                    for (int x : dados) avl_w.insert(x);
                    g_acumulador_estudo2 += avl_w.search(dados[0]);
                    g_acumulador_estudo2 += avl_w.minimum() + avl_w.maximum();
                    g_acumulador_estudo2 += avl_w.successor(dados[0]) + avl_w.predecessor(dados[0]);
                    for (int x : dados) avl_w.remove(x);
                }

                for (int rep = 0; rep < REPETICOES; rep++) {
                    auto avl = new AVLTree();

                    // a) Insercao
                    auto t0 = high_resolution_clock::now();
                    for (int x : dados) avl->insert(x);
                    auto t1 = high_resolution_clock::now();
                    t_ins.push_back(duration<double, micro>(t1 - t0).count());
                    if (rep == 0) mem_pico = obter_memoria_kb();

                    // b) Busca por pertinencia
                    auto t2 = high_resolution_clock::now();
                    for (int x : chaves_busca) {
                        g_acumulador_estudo2 += avl->search(x);
                    }
                    auto t3 = high_resolution_clock::now();
                    t_search.push_back(duration<double, micro>(t3 - t2).count());

                    // c) Consulta de Minimo e Maximo
                    int repeticoes_minmax = max(n, 10000);
                    auto t4 = high_resolution_clock::now();
                    for (int c = 0; c < repeticoes_minmax; c++) {
                        g_acumulador_estudo2 += avl->minimum() + avl->maximum();
                    }
                    auto t5 = high_resolution_clock::now();
                    double t_mm_tot = duration<double, micro>(t5 - t4).count();
                    t_minmax.push_back(t_mm_tot * (static_cast<double>(n) / repeticoes_minmax));

                    // d) Sucessor e Predecessor
                    auto t6 = high_resolution_clock::now();
                    for (int i = 0; i < k_consultas; i++) {
                        g_acumulador_estudo2 += avl->successor(chaves_consulta[i]);
                        g_acumulador_estudo2 += avl->predecessor(chaves_consulta[i]);
                    }
                    auto t7 = high_resolution_clock::now();
                    double t_sp_tot = duration<double, micro>(t7 - t6).count();
                    t_succpred.push_back(t_sp_tot * (static_cast<double>(n) / k_consultas));

                    // e) Remocao
                    auto t8 = high_resolution_clock::now();
                    for (int x : dados) avl->remove(x);
                    auto t9 = high_resolution_clock::now();
                    t_del.push_back(duration<double, micro>(t9 - t8).count());

                    delete avl;
                }

                auto registrar = [&](const string& op, const vector<double>& tempos) {
                    double media = calcular_media(tempos);
                    double desvio = calcular_desvio_padrao(tempos, media);
                    double ns_op = (media * 1000.0) / n;
                    csv << "AVL," << padrao_nome << "," << n << "," << UNIVERSO_U << "," << rho << ","
                        << op << "," << media << "," << desvio << "," << ns_op << "," << mem_pico << "\n";
                };

                registrar("insert", t_ins);
                registrar("search", t_search);
                registrar("min_max", t_minmax);
                registrar("succ_pred", t_succpred);
                registrar("remove", t_del);

                cout << "  [AVL Tree    ] insert: " << setw(10) << fixed << setprecision(2) << calcular_media(t_ins) << " us | "
                     << "search: " << setw(10) << calcular_media(t_search) << " us | "
                     << "succ_pred: " << setw(10) << calcular_media(t_succpred) << " us | "
                     << "remove: " << setw(10) << calcular_media(t_del) << " us\n";
            }

            // =================================================================
            // 2. ÁRVORE RUBRO-NEGRA (Red-Black Tree)
            // =================================================================
            {
                vector<double> t_ins, t_search, t_minmax, t_succpred, t_del;
                size_t mem_pico = 0;

                // Warm-up
                {
                    RedBlackTree rb_w;
                    for (int x : dados) rb_w.insert(x);
                    g_acumulador_estudo2 += rb_w.search(dados[0]);
                    g_acumulador_estudo2 += rb_w.minimum() + rb_w.maximum();
                    g_acumulador_estudo2 += rb_w.successor(dados[0]) + rb_w.predecessor(dados[0]);
                    for (int x : dados) rb_w.remove(x);
                }

                for (int rep = 0; rep < REPETICOES; rep++) {
                    auto rb = new RedBlackTree();

                    // a) Insercao
                    auto t0 = high_resolution_clock::now();
                    for (int x : dados) rb->insert(x);
                    auto t1 = high_resolution_clock::now();
                    t_ins.push_back(duration<double, micro>(t1 - t0).count());
                    if (rep == 0) mem_pico = obter_memoria_kb();

                    // b) Busca por pertinencia
                    auto t2 = high_resolution_clock::now();
                    for (int x : chaves_busca) {
                        g_acumulador_estudo2 += rb->search(x);
                    }
                    auto t3 = high_resolution_clock::now();
                    t_search.push_back(duration<double, micro>(t3 - t2).count());

                    // c) Consulta de Minimo e Maximo
                    int repeticoes_minmax = max(n, 10000);
                    auto t4 = high_resolution_clock::now();
                    for (int c = 0; c < repeticoes_minmax; c++) {
                        g_acumulador_estudo2 += rb->minimum() + rb->maximum();
                    }
                    auto t5 = high_resolution_clock::now();
                    double t_mm_tot = duration<double, micro>(t5 - t4).count();
                    t_minmax.push_back(t_mm_tot * (static_cast<double>(n) / repeticoes_minmax));

                    // d) Sucessor e Predecessor
                    auto t6 = high_resolution_clock::now();
                    for (int i = 0; i < k_consultas; i++) {
                        g_acumulador_estudo2 += rb->successor(chaves_consulta[i]);
                        g_acumulador_estudo2 += rb->predecessor(chaves_consulta[i]);
                    }
                    auto t7 = high_resolution_clock::now();
                    double t_sp_tot = duration<double, micro>(t7 - t6).count();
                    t_succpred.push_back(t_sp_tot * (static_cast<double>(n) / k_consultas));

                    // e) Remocao
                    auto t8 = high_resolution_clock::now();
                    for (int x : dados) rb->remove(x);
                    auto t9 = high_resolution_clock::now();
                    t_del.push_back(duration<double, micro>(t9 - t8).count());

                    delete rb;
                }

                auto registrar = [&](const string& op, const vector<double>& tempos) {
                    double media = calcular_media(tempos);
                    double desvio = calcular_desvio_padrao(tempos, media);
                    double ns_op = (media * 1000.0) / n;
                    csv << "RedBlack," << padrao_nome << "," << n << "," << UNIVERSO_U << "," << rho << ","
                        << op << "," << media << "," << desvio << "," << ns_op << "," << mem_pico << "\n";
                };

                registrar("insert", t_ins);
                registrar("search", t_search);
                registrar("min_max", t_minmax);
                registrar("succ_pred", t_succpred);
                registrar("remove", t_del);

                cout << "  [Red-Black   ] insert: " << setw(10) << fixed << setprecision(2) << calcular_media(t_ins) << " us | "
                     << "search: " << setw(10) << calcular_media(t_search) << " us | "
                     << "succ_pred: " << setw(10) << calcular_media(t_succpred) << " us | "
                     << "remove: " << setw(10) << calcular_media(t_del) << " us\n";
            }

            // =================================================================
            // 3. ÁRVORE DE VAN EMDE BOAS (vEB)
            // =================================================================
            {
                vector<double> t_ins, t_search, t_minmax, t_succpred, t_del;
                size_t mem_pico = 0;

                // Warm-up
                {
                    vanEmdeBoas* veb_w = new vanEmdeBoas(UNIVERSO_U);
                    for (int x : dados) veb_insert(veb_w, x);
                    g_acumulador_estudo2 += veb_member(veb_w, dados[0]);
                    g_acumulador_estudo2 += veb_minimum(veb_w) + veb_maximum(veb_w);
                    g_acumulador_estudo2 += veb_successor(veb_w, dados[0]) + veb_predecessor(veb_w, dados[0]);
                    for (int x : dados) veb_delete(veb_w, x);
                    delete veb_w;
                }

                for (int rep = 0; rep < REPETICOES; rep++) {
                    vanEmdeBoas* veb = new vanEmdeBoas(UNIVERSO_U);

                    // a) Insercao
                    auto t0 = high_resolution_clock::now();
                    for (int x : dados) veb_insert(veb, x);
                    auto t1 = high_resolution_clock::now();
                    t_ins.push_back(duration<double, micro>(t1 - t0).count());
                    if (rep == 0) mem_pico = obter_memoria_kb();

                    // b) Busca por pertinencia
                    auto t2 = high_resolution_clock::now();
                    for (int x : chaves_busca) {
                        g_acumulador_estudo2 += veb_member(veb, x);
                    }
                    auto t3 = high_resolution_clock::now();
                    t_search.push_back(duration<double, micro>(t3 - t2).count());

                    // c) Consulta de Minimo e Maximo
                    int repeticoes_minmax = max(n, 10000);
                    auto t4 = high_resolution_clock::now();
                    for (int c = 0; c < repeticoes_minmax; c++) {
                        g_acumulador_estudo2 += veb_minimum(veb) + veb_maximum(veb);
                    }
                    auto t5 = high_resolution_clock::now();
                    double t_mm_tot = duration<double, micro>(t5 - t4).count();
                    t_minmax.push_back(t_mm_tot * (static_cast<double>(n) / repeticoes_minmax));

                    // d) Sucessor e Predecessor
                    auto t6 = high_resolution_clock::now();
                    for (int i = 0; i < k_consultas; i++) {
                        g_acumulador_estudo2 += veb_successor(veb, chaves_consulta[i]);
                        g_acumulador_estudo2 += veb_predecessor(veb, chaves_consulta[i]);
                    }
                    auto t7 = high_resolution_clock::now();
                    double t_sp_tot = duration<double, micro>(t7 - t6).count();
                    t_succpred.push_back(t_sp_tot * (static_cast<double>(n) / k_consultas));

                    // e) Remocao
                    auto t8 = high_resolution_clock::now();
                    for (int x : dados) veb_delete(veb, x);
                    auto t9 = high_resolution_clock::now();
                    t_del.push_back(duration<double, micro>(t9 - t8).count());

                    delete veb;
                }

                auto registrar = [&](const string& op, const vector<double>& tempos) {
                    double media = calcular_media(tempos);
                    double desvio = calcular_desvio_padrao(tempos, media);
                    double ns_op = (media * 1000.0) / n;
                    csv << "vanEmdeBoas," << padrao_nome << "," << n << "," << UNIVERSO_U << "," << rho << ","
                        << op << "," << media << "," << desvio << "," << ns_op << "," << mem_pico << "\n";
                };

                registrar("insert", t_ins);
                registrar("search", t_search);
                registrar("min_max", t_minmax);
                registrar("succ_pred", t_succpred);
                registrar("remove", t_del);

                cout << "  [vanEmdeBoas ] insert: " << setw(10) << fixed << setprecision(2) << calcular_media(t_ins) << " us | "
                     << "search: " << setw(10) << calcular_media(t_search) << " us | "
                     << "succ_pred: " << setw(10) << calcular_media(t_succpred) << " us | "
                     << "remove: " << setw(10) << calcular_media(t_del) << " us\n";
            }
        }
    }

    csv.close();
    cout << "\n========================================================================\n";
    cout << " CENARIO 1 CONCLUIDO COM SUCESSO!\n";
    cout << " Resultados gravados em: " << caminho_csv << "\n";
    cout << " Acumulador de integridade: " << g_acumulador_estudo2 << "\n";
    cout << "========================================================================\n";

    return 0;
}

