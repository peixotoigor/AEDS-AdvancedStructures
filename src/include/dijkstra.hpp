#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include <vector>
#include <climits>

struct Aresta {
    int destino;
    int peso;

    Aresta(int d, int p) : destino(d), peso(p) {}
};

class Grafo {
    private:
        int num_vertices;
        int num_arestas;
        std::vector<std::vector<Aresta>> adjacencia;

    public:
        explicit Grafo(int vertices);

        void adicionar_aresta(int u, int v, int peso, bool direcionado = true);

        int obter_num_vertices() const;
        int obter_num_arestas() const;
        const std::vector<Aresta>& obter_adjacentes(int u) const;
};

struct ResultadoDijkstra {
    std::vector<int> distancias;
    std::vector<int> predecessores;

    std::vector<int> reconstruir_caminho(int destino) const;
};

ResultadoDijkstra dijkstra_heap_binario(const Grafo& grafo, int origem);
ResultadoDijkstra dijkstra_heap_fibonacci(const Grafo& grafo, int origem);

#endif
