#include "experimento.hpp"

#include <fstream>
#include <chrono>
#include <utility>
#include <filesystem>
#include <climits>

using namespace std;
using namespace std::chrono;


Experimento::Experimento(){

    filesystem::create_directories("resultados");
    ofstream arquivo("resultados/execucoes.csv");

    arquivo << "heap,operacao,tipo_entrada,n,repeticoes,"
            << "semente,media_tempo_micro,chamadas,operacoes_validas,certo\n";
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


void Experimento::registrar_resultado(const MetricasExecucao& m){

    ofstream arquivo("resultados/execucoes.csv", ios::app);

    arquivo << m.heap << ','
            << m.operacao << ','
            << m.tipo_entrada << ','
            << m.n << ','
            << m.execucao << ','
            << m.semente << ','
            << m.tempo_micro << ','
            << m.chamadas << ','
            << m.operacoes_validas << ','
            << (m.certo ? "sim" : "nao")
            << '\n';
}


void Experimento::testar_insercao(const vector<int>& dados, const string& tipo, const string& heap, int d){

    double soma_tempos = 0.0;

    for (int execucao = 0; execucao <= 10; execucao++){

        auto inicio = steady_clock::now();

        if (heap == "fibonacci") {

            HeapFibonacci H;

            for (int i = 0; i < (int)dados.size(); i++){
                H.insert(dados[i], i);
            }

        } else {

            HeapDArio H(d);

            for (int i = 0; i < (int)dados.size(); i++){
                H.insert(dados[i], i);
            }
        }

        auto fim = steady_clock::now();
        double tempo = duration<double, micro>(fim - inicio).count();

        if (execucao > 0) {
            soma_tempos += tempo;
        }
    }

    MetricasExecucao resultado;

    resultado.heap = heap;
    resultado.operacao = "INSERT";
    resultado.tipo_entrada = tipo;
    resultado.n = dados.size();
    resultado.execucao = 10;
    resultado.chamadas = dados.size();
    resultado.operacoes_validas = dados.size();
    resultado.tempo_micro = soma_tempos / 10.0;
    resultado.certo = true;

    registrar_resultado(resultado);

}


void Experimento::testar_extract_min(const vector<int>& dados, const string& tipo, const string& heap, int d){

    double soma_tempos = 0.0;
    bool todas_corretas = true;

    for (int execucao = 0; execucao <= 10; execucao++){

        bool primeiro = true;
        int ultimo = INT_MIN;
        int operacoes_validas = 0;
        bool correto = true;

        double tempo;

        if (heap == "fibonacci"){

            HeapFibonacci H;

            for (int i = 0; i < (int)dados.size(); i++){
                H.insert(dados[i], i);
            }

            auto inicio = steady_clock::now();

            while (!H.esta_vazio()){

                auto removido = H.extract_min();

                if (removido == nullptr){
                    correto = false;
                    break;
                }

                if (!primeiro && removido->chave < ultimo){
                    correto = false;
                }

                ultimo = removido->chave;
                primeiro = false;
                delete removido;
                operacoes_validas++;
            }

            auto fim = steady_clock::now();
            tempo = duration<double, micro>(fim - inicio).count();

        } else {

            HeapDArio H(d);

            for (int i = 0; i < (int)dados.size(); i++){
                H.insert(dados[i], i);
            }

            auto inicio = steady_clock::now();

            while (!H.esta_vazio()){

                auto removido = H.extract_min();

                if (!primeiro && removido.chave < ultimo){
                    correto = false;
                }

                ultimo = removido.chave;
                primeiro = false;
                operacoes_validas++;
            }

            auto fim = steady_clock::now();
            tempo = duration<double, micro>(fim - inicio).count();
        }

        if (operacoes_validas != (int)dados.size()){
            correto = false;
        }

        if (execucao > 0){
            soma_tempos += tempo;

            if (!correto) {
                todas_corretas = false;
            }
        }
    }

    MetricasExecucao resultado;

    resultado.heap = heap;
    resultado.operacao = "EXTRACT_MIN";
    resultado.tipo_entrada = tipo;
    resultado.n = dados.size();
    resultado.execucao = 10;
    resultado.chamadas = dados.size();
    resultado.operacoes_validas = dados.size();
    resultado.tempo_micro = soma_tempos / 10.0;
    resultado.certo = todas_corretas;

    registrar_resultado(resultado);

}


void Experimento::testar_decrease_key(const vector<int>& dados, const string& tipo, const string& heap, int d){

    vector<pair<int, int>> operacoes;
    ifstream arquivo("data/decrease_key.dat");

    if (!arquivo.is_open()){
        return;
    }

    int id, nova_chave;
    uint64_t chamadas = 0;

    while (arquivo >> id >> nova_chave){

        chamadas++;

        if (id >= 0 && id < (int)dados.size()
            && nova_chave < dados[id]) {

            operacoes.push_back({id, nova_chave});
        }
    }

    if (operacoes.empty()){
        return;
    }

    double soma_tempos = 0.0;

    for (int execucao = 0; execucao <= 10; execucao++){

        double tempo;

        if (heap == "fibonacci"){

            HeapFibonacci H;
            vector<HeapFibonacci::NoFibonacci*> nos(dados.size());

            for (int i = 0; i < (int)dados.size(); i++){
                nos[i] = H.insert(dados[i], i);
            }

            auto inicio = steady_clock::now();

            for (const auto& op : operacoes){
                H.decrease_key(nos[op.first], op.second);
            }

            auto fim = steady_clock::now();
            tempo = duration<double, micro>(fim - inicio).count();

        } else {

            HeapDArio H(d);

            for (int i = 0; i < (int)dados.size(); i++){
                H.insert(dados[i], i);
            }

            auto inicio = steady_clock::now();

            for (const auto& op : operacoes){
                H.decrease_key(op.first, op.second);
            }

            auto fim = steady_clock::now();
            tempo = duration<double, micro>(fim - inicio).count();
        }

        if (execucao > 0) {
            soma_tempos += tempo;
        }
    }

    MetricasExecucao resultado;

    resultado.heap = heap;
    resultado.operacao = "DECREASE_KEY";
    resultado.tipo_entrada = tipo;
    resultado.n = dados.size();
    resultado.execucao = 10;
    resultado.chamadas = chamadas;
    resultado.operacoes_validas = operacoes.size();
    resultado.tempo_micro = soma_tempos / 10.0;
    resultado.certo = true;

    registrar_resultado(resultado);

}


void Experimento::testar_dijkstra(){

    struct Entrada {

        string arquivo;
        string tipo;
    };

    vector<Entrada> entradas = {{"data/grafo_esparso.dat", "esparso"},{"data/grafo_denso.dat", "denso"}};

    for (const auto& entrada : entradas){

        Grafo g = lerGrafoArquivo(entrada.arquivo);

        int origem = 0;

        double soma2 = 0.0;
        double soma4 = 0.0;
        double soma8 = 0.0;
        double somaf = 0.0;

        bool correto2 = true;
        bool correto4 = true;
        bool correto8 = true;
        bool correto_f = true;

        int operacoes2 = 0;
        int operacoes4 = 0;
        int operacoes8 = 0;
        int operacoesf = 0;

        for (int execucao = 0; execucao <= 10; execucao++){

            auto inicio = steady_clock::now();
            ResultadoDijkstra res2 = dijkstra_heap_dario(g, origem, 2);
            auto fim = steady_clock::now();

            double tempo2 = duration<double, micro>(fim - inicio).count();

            inicio = steady_clock::now();
            ResultadoDijkstra res4 = dijkstra_heap_dario(g, origem, 4);
            fim = steady_clock::now();

            double tempo4 = duration<double, micro>(fim - inicio).count();

            inicio = steady_clock::now();
            ResultadoDijkstra res8 = dijkstra_heap_dario(g, origem, 8);
            fim = steady_clock::now();

            double tempo8 = duration<double, micro>(fim - inicio).count();

            inicio = steady_clock::now();
            ResultadoDijkstra resf = dijkstra_heap_fibonacci(g, origem);
            fim = steady_clock::now();

            double tempof = duration<double, micro>(fim - inicio).count();

            bool certo4 = (res4.distancias == res2.distancias);
            bool certo8 = (res8.distancias == res2.distancias);
            bool certo_f = (resf.distancias == res2.distancias);

            if (execucao > 0){

                soma2 += tempo2;
                soma4 += tempo4;
                soma8 += tempo8;
                somaf += tempof;

                if (!certo4){
                    correto4 = false;
                }

                if (!certo8){
                    correto8 = false;
                }

                if (!certo_f){
                    correto_f = false;
                }

                operacoes2 = res2.distancias.size();
                operacoes4 = res4.distancias.size();
                operacoes8 = res8.distancias.size();
                operacoesf = resf.distancias.size();
            }
        }

        MetricasExecucao m2;
        MetricasExecucao m4;
        MetricasExecucao m8;
        MetricasExecucao mf;

        m2.heap = "binario";
        m4.heap = "4-ary";
        m8.heap = "8-ary";
        mf.heap = "fibonacci";

        m2.operacao = "DIJKSTRA";
        m4.operacao = "DIJKSTRA";
        m8.operacao = "DIJKSTRA";
        mf.operacao = "DIJKSTRA";

        m2.tipo_entrada = entrada.tipo;
        m4.tipo_entrada = entrada.tipo;
        m8.tipo_entrada = entrada.tipo;
        mf.tipo_entrada = entrada.tipo;

        m2.n = g.obter_num_vertices();
        m4.n = g.obter_num_vertices();
        m8.n = g.obter_num_vertices();
        mf.n = g.obter_num_vertices();

        m2.execucao = 10;
        m4.execucao = 10;
        m8.execucao = 10;
        mf.execucao = 10;

        m2.chamadas = 10;
        m4.chamadas = 10;
        m8.chamadas = 10;
        mf.chamadas = 10;

        m2.operacoes_validas = operacoes2;
        m4.operacoes_validas = operacoes4;
        m8.operacoes_validas = operacoes8;
        mf.operacoes_validas = operacoesf;

        m2.tempo_micro = soma2 / 10.0;
        m4.tempo_micro = soma4 / 10.0;
        m8.tempo_micro = soma8 / 10.0;
        mf.tempo_micro = somaf / 10.0;

        m2.certo = correto2;
        m4.certo = correto4;
        m8.certo = correto8;
        mf.certo = correto_f;

        registrar_resultado(m2);
        registrar_resultado(m4);
        registrar_resultado(m8);
        registrar_resultado(mf);

    }
}



void Experimento::testar_heaps(){

    struct Entrada {

        string arquivo;
        string tipo;
    };

    vector<Entrada> entradas = {{"data/crescente.dat", "crescente"},{"data/aleatorio.dat", "aleatoria"},{"data/decrescente.dat", "decrescente"}};
    vector<int> tamanhos = { 100, 1000, 10000, 100000, 1000000};

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

            testar_insercao(dados, entrada.tipo, "fibonacci", 0);
            testar_extract_min(dados, entrada.tipo, "fibonacci", 0);

            testar_insercao(dados, entrada.tipo, "binario", 2);
            testar_extract_min(dados, entrada.tipo, "binario", 2);

            testar_insercao(dados, entrada.tipo, "4-ary", 4);
            testar_extract_min(dados, entrada.tipo, "4-ary", 4);

            testar_insercao(dados, entrada.tipo, "8-ary", 8);
            testar_extract_min(dados, entrada.tipo, "8-ary", 8);

            testar_decrease_key(dados, entrada.tipo, "fibonacci", 0);
            testar_decrease_key(dados, entrada.tipo, "binario", 2);
            testar_decrease_key(dados, entrada.tipo, "4-ary", 4);
            testar_decrease_key(dados, entrada.tipo, "8-ary", 8);
        }
    }

    testar_dijkstra();

}