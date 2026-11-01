#include "heap_dario.hpp"
#include <climits>
#include <algorithm>

using namespace std;

// métodos privados --------------------------------------------------------------------------------------
int HeapDArio::pai(int indiceFilho) const {
    return (indiceFilho - 1) / d;
}

int HeapDArio::primeiro_filho(int indicePai) const {
    return (indicePai * d) + 1;
}

void HeapDArio::swap_nos(int a, int b) {
    posicao[vetor[a].valor] = b;
    posicao[vetor[b].valor] = a;

    NoDario aux = vetor[a];
    vetor[a] = vetor[b];
    vetor[b] = aux;
}

void HeapDArio::heapify_down(int indicePai) {
    int indiceMelhor = indicePai;
    int n = vetor.size();
    int prim_filho = primeiro_filho(indicePai);

    for (int k = 0; k < d; k++) {
        int filho_atual = prim_filho + k;
        if (filho_atual < n && vetor[filho_atual].chave < vetor[indiceMelhor].chave) {
            indiceMelhor = filho_atual;
        }
    }

    if (indiceMelhor != indicePai) {
        swap_nos(indicePai, indiceMelhor);
        heapify_down(indiceMelhor);
    }
}

void HeapDArio::heapify_up(int indiceFilho) {
    while (indiceFilho > 0 && vetor[pai(indiceFilho)].chave > vetor[indiceFilho].chave) {
        swap_nos(indiceFilho, pai(indiceFilho));
        indiceFilho = pai(indiceFilho);
    }
}

// métodos públicos --------------------------------------------------------------------------------------
HeapDArio::HeapDArio(int d) : d(d) {
    if (d < 2) {
        throw invalid_argument("A aridade d deve ser no minimo 2.");
    }
}

HeapDArio::~HeapDArio() {}

void HeapDArio::insert(int chave, int valor) {
    vetor.push_back(NoDario(chave, valor));
    int indice_atual = vetor.size() - 1;

    if (valor >= (int)posicao.size()) {
        posicao.resize(valor + 1, -1);
    }
    
    posicao[valor] = indice_atual;
    heapify_up(indice_atual);
}

HeapDArio::NoDario HeapDArio::minimum() const {
    if (esta_vazio()) {
        throw runtime_error("Heap vazio");
    } 

    return vetor[0];
}

HeapDArio::NoDario HeapDArio::extract_min() {
    if (esta_vazio()) {
        throw runtime_error("Heap vazio");
    }
    
    NoDario min_node = vetor[0];
    posicao[min_node.valor] = -1;

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

void HeapDArio::decrease_key(int valor, int chave_nova) {
    if (valor < 0 || valor >= (int)posicao.size() || posicao[valor] == -1) {
        return;
    }

    int i = posicao[valor];
    if (chave_nova > vetor[i].chave) {
        return;
    }

    vetor[i].chave = chave_nova;
    heapify_up(i);
}

HeapDArio::NoDario HeapDArio::delete_no(int valor) {
        if (valor < 0 || valor >= (int)posicao.size() || posicao[valor] == -1) {
        throw runtime_error("Valor não encontrado");
    }

    decrease_key(valor, INT_MIN);
    return extract_min();
}

HeapDArio* HeapDArio::union_heaps(HeapDArio& H1, HeapDArio& H2) {
    if (H1.d != H2.d) {
        throw invalid_argument("Os heaps devem possuir a mesma aridade d para a uniao.");
    }

    HeapDArio* H = new HeapDArio(H1.d);
    H->posicao.resize(max(H1.posicao.size(), H2.posicao.size()), -1);
    
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
    for (int i = (n - 2) / H1.d; i >= 0; i--) { 
        H->heapify_down(i);
    }

    H1.vetor.clear(); H1.posicao.clear();
    H2.vetor.clear(); H2.posicao.clear();

    return H;
}

int HeapDArio::tamanho_heap() const {
    return vetor.size();
}

bool HeapDArio::esta_vazio() const {
    return vetor.empty();
}