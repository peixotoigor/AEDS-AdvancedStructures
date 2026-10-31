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
    int execucao = 0;
    int semente = 42;

    double tempo_micro = 0.0;

    uint64_t chamadas = 0;
    uint64_t operacoes_validas = 0;

    bool certo = true;
};


class Experimento{

    private:

        vector<int> carregar_dados(const string& arquivo);
        void registrar_resultado(const MetricasExecucao& resultado);
        void testar_insercao(const vector<int>& dados, const string& tipo, const string& heap, int d);
        void testar_extract_min(const vector<int>& dados, const string& tipo, const string& heap, int d);
        void testar_decrease_key(const vector<int>& dados, const string& tipo, const string& heap, int d);
        void testar_dijkstra();

    public:

        Experimento();
        void testar_heaps();

};

#endif








