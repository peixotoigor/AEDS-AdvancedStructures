#ifndef EXPERIMENTO_HPP
#define EXPERIMENTO_HPP

#include <string>
#include <vector>
#include <cstdint>

#include "heap_dario.hpp"
#include "heap_fibonacci.hpp"
#include "dijkstra.hpp"
#include "leitor_dados.hpp"

using namespace std;


struct MetricasExecucao{

    string heap;
    string operacao;
    string tipo_entrada;

    int n = 0;
    int execucao = 0;          // numero de repeticoes medidas (sem contar o aquecimento)
    int semente = 42;

    double tempo_micro = 0.0;  // media das repeticoes
    double desvio_micro = 0.0; // desvio padrao das repeticoes
    double mediana_micro = 0.0;
    double micro_por_op = 0.0; // tempo medio dividido pelo numero de chamadas

    uint64_t chamadas = 0;
    uint64_t operacoes_validas = 0;

    uint64_t memoria_bytes = 0; // bytes usados pelo heap montado (so na linha INSERT)
    double bytes_por_no = 0.0;
    long long m = 0;            // numero de arestas (so no Dijkstra)

    bool certo = true;
};


class Experimento{

    private:

        vector<int> carregar_dados(const string& arquivo);
        void registrar_resultado(const MetricasExecucao& resultado);
        void imprimir_resultado(const MetricasExecucao& resultado);
        void testar_insercao(const vector<int>& dados, const string& tipo, const string& heap, int d);
        void testar_extract_min(const vector<int>& dados, const string& tipo, const string& heap, int d);
        void testar_decrease_key(const vector<int>& dados, const string& tipo, const string& heap, int d);
        void testar_dijkstra();

    public:

        Experimento();
        void testar_heaps();

};

#endif