#include "experimento.hpp"

#include <algorithm>
#include <chrono>
#include <climits>
#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <malloc.h>
#include <random>
#include <utility>

using namespace std;
using namespace std::chrono;

const int SEMENTE = 42;

static long long bytes_em_uso(){

    struct mallinfo info = mallinfo();

    return (long long)info.uordblks + (long long)info.hblkhd;
}

// Poucas repeticoes para n grande (demora muito), mais para n pequeno (o timer e pouco preciso).
static int repeticoes_para(int n){

    if (n <= 1000){
        return 100;
    }

    return 10;
}


static double micros(steady_clock::time_point inicio, steady_clock::time_point fim){

    return duration<double, micro>(fim - inicio).count();
}


static double calcular_media(const vector<double>& v){

    if (v.empty()){
        return 0.0;
    }

    double soma = 0.0;

    for (double x : v){
        soma += x;
    }

    return soma / v.size();
}


static double calcular_desvio(const vector<double>& v){

    if (v.size() < 2){
        return 0.0;
    }

    double media = calcular_media(v);
    double soma = 0.0;

    for (double x : v){
        soma += (x - media) * (x - media);
    }

    return sqrt(soma / (v.size() - 1));
}


static double calcular_mediana(vector<double> v){

    if (v.empty()){
        return 0.0;
    }

    sort(v.begin(), v.end());

    int meio = v.size() / 2;

    if (v.size() % 2 == 1){
        return v[meio];
    }

    return (v[meio - 1] + v[meio]) / 2.0;
}


static long long somar(const vector<int>& v){

    long long soma = 0;

    for (int x : v){
        soma += x;
    }

    return soma;
}


// Esvazia o heap conferindo: ordem crescente, quantidade e soma das chaves.
static bool conferir_esvaziando(HeapFibonacci& H, int esperado, long long soma_esperada){

    int quantidade = 0;
    long long soma = 0;
    int ultimo = INT_MIN;
    bool correto = true;

    while (!H.esta_vazio()){

        auto removido = H.extract_min();

        if (removido == nullptr){
            correto = false;
            break;
        }

        if (removido->chave < ultimo){
            correto = false;
        }

        ultimo = removido->chave;
        soma += removido->chave;
        quantidade++;
        delete removido;
    }

    return correto && quantidade == esperado && soma == soma_esperada;
}


static bool conferir_esvaziando(HeapDArio& H, int esperado, long long soma_esperada){

    int quantidade = 0;
    long long soma = 0;
    int ultimo = INT_MIN;
    bool correto = true;

    while (!H.esta_vazio()){

        auto removido = H.extract_min();

        if (removido.chave < ultimo){
            correto = false;
        }

        ultimo = removido.chave;
        soma += removido.chave;
        quantidade++;
    }

    return correto && quantidade == esperado && soma == soma_esperada;
}


// Preenche as estatisticas a partir dos tempos medidos.
static MetricasExecucao montar_resultado(const string& heap, const string& operacao, const string& tipo,
                                         int n, const vector<double>& tempos,
                                         uint64_t chamadas, uint64_t validas, bool certo){

    MetricasExecucao r;

    r.heap = heap;
    r.operacao = operacao;
    r.tipo_entrada = tipo;
    r.n = n;
    r.execucao = tempos.size();
    r.semente = SEMENTE;
    r.tempo_micro = calcular_media(tempos);
    r.desvio_micro = calcular_desvio(tempos);
    r.mediana_micro = calcular_mediana(tempos);

    if (chamadas > 0){
        r.micro_por_op = r.tempo_micro / chamadas;
    }

    r.chamadas = chamadas;
    r.operacoes_validas = validas;
    r.certo = certo;

    return r;
}


// Numero de arestas do grafo.
static long long contar_arestas(const Grafo& g){

    return g.obter_num_arestas();
}

// h: 0 = binario, 1 = 4-ary, 2 = 8-ary, 3 = fibonacci
static ResultadoDijkstra rodar_dijkstra(const Grafo& g, int origem, int h){

    if (h == 0){
        return dijkstra_heap_dario(g, origem, 2);
    }

    if (h == 1){
        return dijkstra_heap_dario(g, origem, 4);
    }

    if (h == 2){
        return dijkstra_heap_dario(g, origem, 8);
    }

    return dijkstra_heap_fibonacci(g, origem);
}


// ---------------------------------------------------------------------
// Classe Experimento
// ---------------------------------------------------------------------

Experimento::Experimento(){

    filesystem::create_directories("results");
    ofstream arquivo("results/execucoes.csv");

    arquivo << "heap,operacao,tipo_entrada,n,repeticoes,semente,"
            << "media_tempo_micro,desvio_micro,mediana_micro,micro_por_op,"
            << "chamadas,operacoes_validas,certo,memoria_bytes,bytes_por_no,m\n";
}


vector<int> Experimento::carregar_dados(const string& caminho){

    vector<int> dados;
    ifstream arquivo(caminho);

    if (!arquivo.is_open()){
        return dados;
    }

    int valor;

    while (arquivo >> valor){
        dados.push_back(valor);
    }

    return dados;
}


void Experimento::imprimir_resultado(const MetricasExecucao& m){

    /*printf("  %-12s  %-11s  n=%-8d  %-10s %12.2f us  (+- %9.2f)  %.4f us/op  %s\n",
           m.operacao.c_str(), m.tipo_entrada.c_str(), m.n, m.heap.c_str(),
           m.tempo_micro, m.desvio_micro, m.micro_por_op, m.certo ? "ok" : "ERRO");

    fflush(stdout);*/
}


void Experimento::registrar_resultado(const MetricasExecucao& m){

    //imprimir_resultado(m);

    ofstream arquivo("results/execucoes.csv", ios::app);

    arquivo << fixed << setprecision(4);

    arquivo << m.heap << ','
            << m.operacao << ','
            << m.tipo_entrada << ','
            << m.n << ','
            << m.execucao << ','
            << m.semente << ','
            << m.tempo_micro << ','
            << m.desvio_micro << ','
            << m.mediana_micro << ','
            << m.micro_por_op << ','
            << m.chamadas << ','
            << m.operacoes_validas << ','
            << (m.certo ? "sim" : "nao") << ','
            << m.memoria_bytes << ','
            << m.bytes_por_no << ','
            << m.m
            << '\n';
}


// A execucao 0 e de aquecimento: nao entra na media.
// O heap e destruido DEPOIS de parar o cronometro.
void Experimento::testar_insercao(const vector<int>& dados, const string& tipo, const string& heap, int d){

    int n = dados.size();
    int repeticoes = repeticoes_para(n);
    long long soma_esperada = somar(dados);

    vector<double> tempos;
    bool certo = true;
    uint64_t memoria = 0;

    for (int execucao = 0; execucao <= repeticoes; execucao++){

        double tempo;

        if (heap == "fibonacci"){

            long long antes = bytes_em_uso();

            HeapFibonacci* H = new HeapFibonacci();

            auto inicio = steady_clock::now();

            for (int i = 0; i < n; i++){
                H->insert(dados[i], i);
            }

            auto fim = steady_clock::now();
            tempo = micros(inicio, fim);

            if (execucao == 0){
                memoria = bytes_em_uso() - antes;
            }

            if (execucao == repeticoes){
                certo = conferir_esvaziando(*H, n, soma_esperada);
            }

            delete H;

        } else {

            long long antes = bytes_em_uso();

            HeapDArio* H = new HeapDArio(d);

            auto inicio = steady_clock::now();

            for (int i = 0; i < n; i++){
                H->insert(dados[i], i);
            }

            auto fim = steady_clock::now();
            tempo = micros(inicio, fim);


            if (execucao == 0){
                memoria = bytes_em_uso() - antes;
            }

            if (execucao == repeticoes){
                certo = conferir_esvaziando(*H, n, soma_esperada);
            }

            delete H;
        }

        if (execucao > 0){
            tempos.push_back(tempo);
        }
    }

    MetricasExecucao resultado = montar_resultado(heap, "INSERT", tipo, n, tempos, n, n, certo);

    resultado.memoria_bytes = memoria;
    resultado.bytes_por_no = (double)memoria / n;

    registrar_resultado(resultado);
}


void Experimento::testar_extract_min(const vector<int>& dados, const string& tipo, const string& heap, int d){

    int n = dados.size();
    int repeticoes = repeticoes_para(n);

    vector<double> tempos;
    bool todas_corretas = true;

    for (int execucao = 0; execucao <= repeticoes; execucao++){

        bool primeiro = true;
        int ultimo = INT_MIN;
        int operacoes_validas = 0;
        bool correto = true;

        double tempo;

        if (heap == "fibonacci"){

            HeapFibonacci* H = new HeapFibonacci();

            for (int i = 0; i < n; i++){
                H->insert(dados[i], i);
            }

            auto inicio = steady_clock::now();

            while (!H->esta_vazio()){

                auto removido = H->extract_min();

                if (removido == nullptr){
                    correto = false;
                    break;
                }

                if (!primeiro && removido->chave < ultimo){
                    correto = false;
                }

                ultimo = removido->chave;
                primeiro = false;
                delete removido;      // desalocar o no faz parte do custo do Fibonacci
                operacoes_validas++;
            }

            auto fim = steady_clock::now();
            tempo = micros(inicio, fim);

            delete H;

        } else {

            HeapDArio* H = new HeapDArio(d);

            for (int i = 0; i < n; i++){
                H->insert(dados[i], i);
            }

            auto inicio = steady_clock::now();

            while (!H->esta_vazio()){

                auto removido = H->extract_min();

                if (!primeiro && removido.chave < ultimo){
                    correto = false;
                }

                ultimo = removido.chave;
                primeiro = false;
                operacoes_validas++;
            }

            auto fim = steady_clock::now();
            tempo = micros(inicio, fim);

            delete H;
        }

        if (operacoes_validas != n){
            correto = false;
        }

        if (!correto){
            todas_corretas = false;
        }

        if (execucao > 0){
            tempos.push_back(tempo);
        }
    }

    MetricasExecucao resultado = montar_resultado(heap, "EXTRACT_MIN", tipo, n, tempos, n, n, todas_corretas);

    registrar_resultado(resultado);
}

void Experimento::testar_decrease_key(const vector<int>& dados, const string& tipo, const string& heap, int d){

    int n = dados.size();

    if (n < 3){
        return;
    }

    int repeticoes = repeticoes_para(n);

    int minimo = *min_element(dados.begin(), dados.end());
    int maximo = *max_element(dados.begin(), dados.end());

    int max_delta = (maximo - minimo) / 10;

    if (max_delta < 1){
        max_delta = 1;
    }

    mt19937 gerador(SEMENTE + n);
    uniform_int_distribution<int> sorteio_id(1, n - 1);
    uniform_int_distribution<int> sorteio_delta(1, max_delta);

    vector<int> atual = dados;
    atual[0] = INT_MIN;

    vector<pair<int, int>> operacoes;

    for (int k = 0; k < n; k++){

        int id = sorteio_id(gerador);
        long long nova = (long long)atual[id] - sorteio_delta(gerador);

        if (nova <= (long long)INT_MIN + 1){
            continue;
        }

        atual[id] = (int)nova;
        operacoes.push_back({id, (int)nova});
    }

    long long soma_esperada = 0;

    for (int i = 1; i < n; i++){
        soma_esperada += atual[i];
    }

    vector<double> tempos;
    bool certo = true;

    for (int execucao = 0; execucao <= repeticoes; execucao++){

        double tempo;

        if (heap == "fibonacci"){

            HeapFibonacci* H = new HeapFibonacci();
            vector<HeapFibonacci::NoFibonacci*> nos(n);

            nos[0] = H->insert(INT_MIN, 0);

            for (int i = 1; i < n; i++){
                nos[i] = H->insert(dados[i], i);
            }

            auto sentinela = H->extract_min();
            bool ok = (sentinela == nos[0]);
            delete sentinela;

            auto inicio = steady_clock::now();

            for (const auto& op : operacoes){
                H->decrease_key(nos[op.first], op.second);
            }

            auto fim = steady_clock::now();
            tempo = micros(inicio, fim);

            if (!ok){
                certo = false;
            }

            if (execucao == repeticoes){
                certo = certo && conferir_esvaziando(*H, n - 1, soma_esperada);
            }

            delete H;

        } else {

            HeapDArio* H = new HeapDArio(d);

            H->insert(INT_MIN, 0);

            for (int i = 1; i < n; i++){
                H->insert(dados[i], i);
            }

            auto sentinela = H->extract_min();
            bool ok = (sentinela.chave == INT_MIN);

            auto inicio = steady_clock::now();

            for (const auto& op : operacoes){
                H->decrease_key(op.first, op.second);
            }

            auto fim = steady_clock::now();
            tempo = micros(inicio, fim);

            if (!ok){
                certo = false;
            }

            if (execucao == repeticoes){
                certo = certo && conferir_esvaziando(*H, n - 1, soma_esperada);
            }

            delete H;
        }

        if (execucao > 0){
            tempos.push_back(tempo);
        }
    }

    MetricasExecucao resultado = montar_resultado(heap, "DECREASE_KEY", tipo, n, tempos,
                                                  operacoes.size(), operacoes.size(), certo);

    registrar_resultado(resultado);
}


void Experimento::testar_dijkstra(){

    struct Entrada {

        string arquivo;
        string tipo;
    };

    vector<Entrada> entradas = {{"data/grafo_esparso.dat", "esparso"},{"data/grafo_denso.dat", "denso"}};

    string nomes[4] = {"binario", "4-ary", "8-ary", "fibonacci"};

    //printf("== DIJKSTRA ==\n");

    for (const auto& entrada : entradas){

        Grafo g = lerGrafoArquivo(entrada.arquivo);

        int origem = 0;
        int n = g.obter_num_vertices();
        long long m = contar_arestas(g);

        //printf("  grafo %s: V=%d E=%lld densidade m/V=%.1f\n", entrada.tipo.c_str(), n, m, (double)m / n);

        // resultado de referencia: heap binario
        ResultadoDijkstra referencia = dijkstra_heap_dario(g, origem, 2);

        vector<double> tempos[4];
        bool correto[4] = {true, true, true, true};

        for (int execucao = 0; execucao <= 10; execucao++){

            // a ordem em que os heaps rodam gira a cada execucao
            for (int j = 0; j < 4; j++){

                int h = (j + execucao) % 4;

                auto inicio = steady_clock::now();
                ResultadoDijkstra res = rodar_dijkstra(g, origem, h);
                auto fim = steady_clock::now();

                if (execucao > 0){

                    tempos[h].push_back(micros(inicio, fim));

                    if (res.distancias != referencia.distancias){
                        correto[h] = false;
                    }
                }
            }
        }

        for (int h = 0; h < 4; h++){

            // chamadas = numero de arestas, entao micro_por_op = microssegundos por aresta
            MetricasExecucao resultado = montar_resultado(nomes[h], "DIJKSTRA", entrada.tipo, n,
                                                          tempos[h], m, n, correto[h]);

            resultado.m = m;

            registrar_resultado(resultado);
        }
    }
}

void Experimento::testar_heaps(){

    struct Entrada {

        string arquivo;
        string tipo;
    };

    vector<Entrada> entradas = {{"data/crescente.dat", "crescente"},{"data/aleatorio.dat", "aleatoria"},{"data/decrescente.dat", "decrescente"}};
    vector<int> tamanhos = { 100, 1000, 10000, 100000, 1000000};

    string nomes[4] = {"fibonacci", "binario", "4-ary", "8-ary"};
    int ds[4] = {0, 2, 4, 8};

    int deslocamento = 0;

    for (const auto& entrada : entradas){

        vector<int> dados_completos = carregar_dados(entrada.arquivo);

        if (dados_completos.empty()){
            continue;
        }

        for (int tamanho : tamanhos){

            if (tamanho > (int)dados_completos.size()){
                continue;
            }

            vector<int> dados(dados_completos.begin(),dados_completos.begin() + tamanho);

            //printf("== %s n=%d ==\n", entrada.tipo.c_str(), tamanho);

            // a ordem dos heaps muda a cada tamanho (evita favorecer quem roda primeiro)
            for (int j = 0; j < 4; j++){

                int h = (j + deslocamento) % 4;

                testar_insercao(dados, entrada.tipo, nomes[h], ds[h]);
                testar_extract_min(dados, entrada.tipo, nomes[h], ds[h]);
                testar_decrease_key(dados, entrada.tipo, nomes[h], ds[h]);
            }

            deslocamento++;
        }
    }

    testar_dijkstra();

}