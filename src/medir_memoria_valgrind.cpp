#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "avl.hpp"
#include "rb.hpp"
#include "veb.hpp"
#include "heap_dario.hpp"
#include "heap_fibonacci.hpp"
#include "dijkstra.hpp"
#include "leitor_dados.hpp"

using namespace std;

// Este programa e executado isoladamente sob o Valgrind Massif para registrar
// com precisao o pico de memoria dinamica alocada sem contaminar o tempo.
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <veb|avl|rb|fibonacci|dario2|dario4|dario8|dijkstra_esparso|dijkstra_denso> [N]" << endl;
        return 1;
    }

    string alvo = argv[1];
    int n = (argc >= 3) ? stoi(argv[2]) : 100000;

    // Carregar prefixo do dataset aleatorio
    vector<int> dados;
    {
        ifstream arq("data/aleatorio.dat");
        int x;
        while (dados.size() < static_cast<size_t>(n) && arq >> x) {
            dados.push_back(x);
        }
    }

    if (alvo == "veb") {
        cout << "[Valgrind] Alocando vanEmdeBoas(1048576) com " << n << " chaves..." << endl;
        vanEmdeBoas* V = new vanEmdeBoas(1048576);
        for (int x : dados) veb_insert(V, x);
        delete V;
    } else if (alvo == "avl") {
        cout << "[Valgrind] Alocando AVLTree com " << n << " nos..." << endl;
        AVLTree* tree = new AVLTree();
        for (int x : dados) tree->insert(x);
        delete tree;
    } else if (alvo == "rb") {
        cout << "[Valgrind] Alocando RedBlackTree com " << n << " nos..." << endl;
        RedBlackTree* tree = new RedBlackTree();
        for (int x : dados) tree->insert(x);
        delete tree;
    } else if (alvo == "fibonacci") {
        cout << "[Valgrind] Alocando HeapFibonacci com " << n << " nos..." << endl;
        HeapFibonacci* H = new HeapFibonacci();
        for (int x : dados) H->insert(x, x);
        delete H;
    } else if (alvo == "dario2") {
        cout << "[Valgrind] Alocando HeapDArio(2) com " << n << " chaves..." << endl;
        HeapDArio* H = new HeapDArio(2);
        for (int x : dados) H->insert(x, x);
        delete H;
    } else if (alvo == "dario4") {
        cout << "[Valgrind] Alocando HeapDArio(4) com " << n << " chaves..." << endl;
        HeapDArio* H = new HeapDArio(4);
        for (int x : dados) H->insert(x, x);
        delete H;
    } else if (alvo == "dario8") {
        cout << "[Valgrind] Alocando HeapDArio(8) com " << n << " chaves..." << endl;
        HeapDArio* H = new HeapDArio(8);
        for (int x : dados) H->insert(x, x);
        delete H;
    } else if (alvo == "dijkstra_esparso") {
        cout << "[Valgrind] Executando Dijkstra no Grafo Esparso..." << endl;
        Grafo g = lerGrafoArquivo("data/grafo_esparso.dat");
        dijkstra_heap_fibonacci(g, 0);
        dijkstra_heap_dario(g, 0, 2);
    } else if (alvo == "dijkstra_denso") {
        cout << "[Valgrind] Executando Dijkstra no Grafo Denso..." << endl;
        Grafo g = lerGrafoArquivo("data/grafo_denso.dat");
        dijkstra_heap_fibonacci(g, 0);
        dijkstra_heap_dario(g, 0, 2);
    } else {
        cerr << "Alvo desconhecido: " << alvo << endl;
        return 1;
    }

    cout << "[Valgrind] Finalizado com sucesso!" << endl;
    return 0;
}

