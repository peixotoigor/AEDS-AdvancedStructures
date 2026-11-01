#include <iostream>
#include <exception>
#include "experimento.hpp"

using namespace std;

int main() {
    cout << "==================================================" << endl;
    cout << "   INICIANDO BENCHMARK DE ESTRUTURAS DE HEAP      " << endl;
    cout << "==================================================" << endl;

    try {
        Experimento exp;

        cout << "[1/2] Rodando testes de operacoes isoladas nos Heaps..." << endl;
        cout << "[2/2] Rodando testes do Algoritmo de Dijkstra..." << endl;
        
        exp.testar_heaps();

        cout << "\n==================================================" << endl;
        cout << " BENCHMARK CONCLUIDO COM SUCESSO!                 " << endl;
        cout << " Resultados salvos em: results/execucoes.csv   " << endl;
        cout << "==================================================" << endl;

    } catch (const exception& e) {
        cerr << "\n[ERRO CRITICO] O experimento foi interrompido: " << e.what() << endl;
        return 1;
    }

    return 0;
}