#include "heap_binario.hpp"
#include <climits>
#include <algorithm>

using namespace std;

// métodos privados --------------------------------------------------------------------------------------
int HeapBinario::pai(int indiceFilho) const {
    return (indiceFilho - 1) / 2;
}

int HeapBinario::filho_esq(int indicePai) const {
    return (indicePai * 2) + 1;
}

int HeapBinario::filho_dir(int indicePai) const {
    return (indicePai * 2) + 2;
}

void HeapBinario::swap_nos(int a, int b) {
    posicao[vetor[a].valor] = b;
    posicao[vetor[b].valor] = a;

    NoBinario aux = vetor[a];
    vetor[a] = vetor[b];
    vetor[b] = aux;
}

void HeapBinario::heapify_down(int indicePai) {
    int indiceMelhor = indicePai;
    int n = vetor.size();
    
    int esq = filho_esq(indicePai);
    int dir = filho_dir(indicePai);

    if (esq < n && vetor[esq].chave < vetor[indiceMelhor].chave) {
        indiceMelhor = esq;
    }
    
    if (dir < n && vetor[dir].chave < vetor[indiceMelhor].chave) {
        indiceMelhor = dir;
    }

    if (indiceMelhor != indicePai) {
        swap_nos(indicePai, indiceMelhor);
        heapify_down(indiceMelhor);
    }
}

void HeapBinario::heapify_up(int indiceFilho) {
    while (indiceFilho > 0 && vetor[pai(indiceFilho)].chave > vetor[indiceFilho].chave) {
        swap_nos(indiceFilho, pai(indiceFilho));
        indiceFilho = pai(indiceFilho);
    }
}

// métodos públicos --------------------------------------------------------------------------------------
HeapBinario::HeapBinario() {}

HeapBinario::~HeapBinario() {}

void HeapBinario::insert(int chave, int valor) {
    vetor.push_back(NoBinario(chave, valor));
    int indice_atual = vetor.size() - 1;
    
    posicao[valor] = indice_atual;
    heapify_up(indice_atual);
}

HeapBinario::NoBinario HeapBinario::minimum() const {
    if (esta_vazio()){
        throw runtime_error("Heap vazio");
    } 

    return vetor[0];
}


HeapBinario::NoBinario HeapBinario::extract_min() {
    if (esta_vazio()){
         throw runtime_error("Heap vazio");
    }
    
    NoBinario min_node = vetor[0];
    posicao.erase(min_node.valor);

    if (vetor.size() > 1) {
        vetor[0] = vetor.back();
        posicao[vetor[0].valor] = 0;
        vetor.pop_back();
        heapify_down(0);
    } else {
        vetor.pop_back();
    }

    return min_node;
}

void HeapBinario::decrease_key(int valor, int chave_nova) {
    if (posicao.find(valor) == posicao.end()){
        return;
    }

    int i = posicao[valor];
    if (chave_nova > vetor[i].chave){
        return;
    }

    vetor[i].chave = chave_nova;
    heapify_up(i);
}

HeapBinario::NoBinario HeapBinario::delete_no(int valor) {
    if (posicao.find(valor) == posicao.end()) {
        throw runtime_error("Valor não encontrado");
    }

    decrease_key(valor, INT_MIN);
    return extract_min();
}

HeapBinario* HeapBinario::union_heaps(HeapBinario& H1, HeapBinario& H2) {
    HeapBinario* H = new HeapBinario();
    
    if (H1.esta_vazio() && H2.esta_vazio()) return H;

    for (size_t i = 0; i < H1.vetor.size(); i++) {
        H->vetor.push_back(H1.vetor[i]);
        H->posicao[H1.vetor[i].valor] = i;
    }

    for (size_t i = 0; i < H2.vetor.size(); i++) {
        int novo_indice = H->vetor.size();
        H->vetor.push_back(H2.vetor[i]);
        H->posicao[H2.vetor[i].valor] = novo_indice;
    }

    int n = H->vetor.size();
    for (int i = (n / 2) - 1; i >= 0; i--) {
        H->heapify_down(i);
    }

    H1.vetor.clear(); H1.posicao.clear();
    H2.vetor.clear(); H2.posicao.clear();

    return H;
}

int HeapBinario::tamanho_heap() const {
    return vetor.size();
}

bool HeapBinario::esta_vazio() const {
    return vetor.empty();
}