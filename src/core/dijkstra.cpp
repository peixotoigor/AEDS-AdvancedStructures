#include "dijkstra.hpp"
#include "heap_binario.hpp"
#include "heap_fibonacci.hpp"
#include <algorithm>
#include <stdexcept>

using namespace std;

// Implementação do Grafo -------------------------------------------------------------------------------

Grafo::Grafo(int vertices) : num_vertices(vertices), num_arestas(0) {
    if (vertices <= 0) {
        throw invalid_argument("O numero de vertices deve ser maior que zero.");
    }
    adjacencia.resize(vertices);
}

void Grafo::adicionar_aresta(int u, int v, int peso, bool direcionado) {
    if (u < 0 || u >= num_vertices || v < 0 || v >= num_vertices) {
        throw out_of_range("Indice de vertice invalido.");
    }
    if (peso < 0) {
        throw invalid_argument("O algoritmo de Dijkstra nao aceita arestas com peso negativo.");
    }

    adjacencia[u].push_back(Aresta(v, peso));
    num_arestas++;

    if (!direcionado && u != v) {
        adjacencia[v].push_back(Aresta(u, peso));
        num_arestas++;
    }
}

int Grafo::obter_num_vertices() const {
    return num_vertices;
}

int Grafo::obter_num_arestas() const {
    return num_arestas;
}

const vector<Aresta>& Grafo::obter_adjacentes(int u) const {
    if (u < 0 || u >= num_vertices) {
        throw out_of_range("Indice de vertice invalido.");
    }
    return adjacencia[u];
}

// Reconstrução do caminho mínimo -----------------------------------------------------------------------

vector<int> ResultadoDijkstra::reconstruir_caminho(int destino) const {
    vector<int> caminho;
    if (destino < 0 || destino >= static_cast<int>(distancias.size())) {
        return caminho;
    }
    if (distancias[destino] == INT_MAX) {
        return caminho; // Vértice inalcançável a partir da origem
    }

    for (int atual = destino; atual != -1; atual = predecessores[atual]) {
        caminho.push_back(atual);
    }

    reverse(caminho.begin(), caminho.end());
    return caminho;
}

// Dijkstra com Heap Binário — Complexidade: O((V + E) log V) -------------------------------------------

ResultadoDijkstra dijkstra_heap_binario(const Grafo& grafo, int origem) {
    int V = grafo.obter_num_vertices();
    if (origem < 0 || origem >= V) {
        throw out_of_range("Vertice de origem invalido.");
    }

    // INITIALIZE-SINGLE-SOURCE(G, s) — Cormen Seção 24.1
    ResultadoDijkstra resultado;
    resultado.distancias.assign(V, INT_MAX);
    resultado.predecessores.assign(V, -1);
    resultado.distancias[origem] = 0;

    // Conjunto S de vértices cujo caminho mínimo final já foi determinado
    vector<bool> visitado(V, false);

    // Fila de prioridade Q = G.V (chave = distância, valor = vértice)
    HeapBinario Q;
    for (int v = 0; v < V; v++) {
        Q.insert(resultado.distancias[v], v);
    }

    while (!Q.esta_vazio()) {
        HeapBinario::NoBinario menor = Q.extract_min();
        int u = menor.valor;
        visitado[u] = true;

        // Se a menor distância for infinita, os vértices restantes são inalcançáveis
        if (resultado.distancias[u] == INT_MAX) {
            break;
        }

        // RELAX(u, v, w) para cada aresta (u, v) — Cormen Seção 24.3
        const vector<Aresta>& vizinhos = grafo.obter_adjacentes(u);
        for (size_t i = 0; i < vizinhos.size(); i++) {
            int v = vizinhos[i].destino;
            int peso = vizinhos[i].peso;

            if (!visitado[v]) {
                long long nova_distancia = static_cast<long long>(resultado.distancias[u]) + peso;
                if (nova_distancia < resultado.distancias[v]) {
                    resultado.distancias[v] = static_cast<int>(nova_distancia);
                    resultado.predecessores[v] = u;
                    Q.decrease_key(v, resultado.distancias[v]);
                }
            }
        }
    }

    return resultado;
}

// Dijkstra com Heap de Fibonacci — Complexidade Amortizada: O(V log V + E) -----------------------------

ResultadoDijkstra dijkstra_heap_fibonacci(const Grafo& grafo, int origem) {
    int V = grafo.obter_num_vertices();
    if (origem < 0 || origem >= V) {
        throw out_of_range("Vertice de origem invalido.");
    }

    // INITIALIZE-SINGLE-SOURCE(G, s) — Cormen Seção 24.1
    ResultadoDijkstra resultado;
    resultado.distancias.assign(V, INT_MAX);
    resultado.predecessores.assign(V, -1);
    resultado.distancias[origem] = 0;

    // Conjunto S de vértices cujo caminho mínimo final já foi determinado
    vector<bool> visitado(V, false);

    // Fila de prioridade Q = G.V e vetor de ponteiros diretos para decrease_key em O(1)
    HeapFibonacci Q;
    vector<HeapFibonacci::NoFibonacci*> nos(V, nullptr);

    for (int v = 0; v < V; v++) {
        nos[v] = Q.insert(resultado.distancias[v], v);
    }

    while (!Q.esta_vazio()) {
        HeapFibonacci::NoFibonacci* menor = Q.extract_min();
        int u = menor->valor;

        // extract_min remove o nó da estrutura, mas transfere a posse do ponteiro ao chamador
        delete menor;
        nos[u] = nullptr;
        visitado[u] = true;

        // Se a menor distância for infinita, os vértices restantes são inalcançáveis
        if (resultado.distancias[u] == INT_MAX) {
            break;
        }

        // RELAX(u, v, w) para cada aresta (u, v) — Cormen Seção 24.3
        const vector<Aresta>& vizinhos = grafo.obter_adjacentes(u);
        for (size_t i = 0; i < vizinhos.size(); i++) {
            int v = vizinhos[i].destino;
            int peso = vizinhos[i].peso;

            if (!visitado[v] && nos[v] != nullptr) {
                long long nova_distancia = static_cast<long long>(resultado.distancias[u]) + peso;
                if (nova_distancia < resultado.distancias[v]) {
                    resultado.distancias[v] = static_cast<int>(nova_distancia);
                    resultado.predecessores[v] = u;
                    Q.decrease_key(nos[v], resultado.distancias[v]);
                }
            }
        }
    }

    return resultado;
}
