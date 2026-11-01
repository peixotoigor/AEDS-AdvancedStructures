#ifndef HEAP_DARIO_HPP
#define HEAP_DARIO_HPP

#include <vector>
#include <stdexcept>

class HeapDArio {
    public:
        struct NoDario {
            int chave;
            int valor;
            NoDario(int c, int v) : chave(c), valor(v) {}
        };

    private:
        std::vector<NoDario> vetor;
        std::vector<int> posicao;   // posicao[valor] = indice no vetor, ou -1 se o valor nao esta no heap
        int d; // grau de aridade do heap 

        int pai(int indiceFilho) const;
        int primeiro_filho(int indicePai) const;

        void swap_nos(int a, int b);
        void heapify_down(int indicePai);
        void heapify_up(int indiceFilho);

    public:
        explicit HeapDArio(int d = 2);
        ~HeapDArio();

        void insert(int chave, int valor);
        NoDario minimum() const;
        NoDario extract_min();
        void decrease_key(int valor, int chave_nova);
        NoDario delete_no(int valor);
        HeapDArio* union_heaps(HeapDArio& H1, HeapDArio& H2);
        int tamanho_heap() const;
        bool esta_vazio() const;
        int obter_d() const { return d; }
};

#endif