#ifndef HEAP_BINARIO_HPP
#define HEAP_BINARIO_HPP

#include <vector>
#include <unordered_map>
#include <stdexcept>

class HeapBinario {
    public:
        struct NoBinario {
            int chave;
            int valor;
            NoBinario(int c, int v) : chave(c), valor(v) {}
        };

    private:
        std::vector<NoBinario> vetor;
        std::unordered_map<int, int> posicao;

        int pai(int indiceFilho) const;
        int filho_esq(int indicePai) const;
        int filho_dir(int indicePai) const;

        void swap_nos(int a, int b);
        void heapify_down(int indicePai);
        void heapify_up(int indiceFilho);

    public:
        HeapBinario();
        ~HeapBinario();

        void insert(int chave, int valor);
        NoBinario minimum() const;
        NoBinario extract_min();
        void decrease_key(int valor, int chave_nova);
        NoBinario delete_no(int valor);
        HeapBinario* union_heaps(HeapBinario& H1, HeapBinario& H2);
        int tamanho_heap() const;
        bool esta_vazio() const;
};

#endif