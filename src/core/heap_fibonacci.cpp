#include "heap_fibonacci.hpp"

using namespace std;

// métodos privados --------------------------------------------------------------------------------------
void HeapFibonacci::inserir_no_lista_circular(NoFibonacci*& referencia, NoFibonacci* x){
    if(referencia == nullptr){
        referencia = x;
        referencia->esquerda = x;
        referencia->direita = x;
        return;
    }

    NoFibonacci* vizinhoEsquerda = referencia->esquerda;
    referencia->esquerda = x;
    x->esquerda = vizinhoEsquerda;
    x->direita = referencia;
    vizinhoEsquerda->direita = x;
}

void HeapFibonacci::deletar_no_lista_circular(NoFibonacci* x){
    if(x == nullptr){
        return;
    }

    if(x->direita != x){
        NoFibonacci* vizinhoEsquerda = x->esquerda;
        NoFibonacci* vizinhoDireita = x->direita;

        vizinhoDireita->esquerda = vizinhoEsquerda;
        vizinhoEsquerda->direita = vizinhoDireita;
    }
    
    x->direita = x;
    x->esquerda = x;
}

void HeapFibonacci::juntar_listas_circulares(NoFibonacci& H1, NoFibonacci& H2){}

void HeapFibonacci::liberar_memoria(NoFibonacci *no){
    if(no == nullptr){
        return;
    }

    if(no->filho != nullptr){
        NoFibonacci* atual = no->filho;
        
        no->filho->esquerda->direita = nullptr;

        while(atual != nullptr){
            NoFibonacci* proximo = atual->direita;
            liberar_memoria(atual);
            atual = proximo;
        }
    }
    
    delete no;
}

void HeapFibonacci::consolidate(){
    int grauMaximo = static_cast<int>(floor(log2(this->n) / log2(1.61803398875)) + 2);
    vector<NoFibonacci*> auxiliarRaizes(grauMaximo);

    for(int i=0;i<grauMaximo;i++){
        auxiliarRaizes[i] = nullptr;
    }

    vector<NoFibonacci*> listaRaizes;
    NoFibonacci* proximo = this->min;
    if(proximo != nullptr){
        do {
            listaRaizes.push_back(proximo);
            proximo = proximo->direita;
        } while(proximo != this->min);
    } 

    for(size_t i=0;i < listaRaizes.size();i++){
        NoFibonacci* x = listaRaizes[i];
        int d = x->grau;

        while(d < grauMaximo && auxiliarRaizes[d] != nullptr){
            NoFibonacci* y = auxiliarRaizes[d];
            if(x->chave > y->chave){
                swap(x,y);
            }

            heap_link(y, x);
            auxiliarRaizes[d] = nullptr;
            d++;
        }

        auxiliarRaizes[d] = x;
    }

    this->min = nullptr;
    for(int i=0;i<grauMaximo;i++){
        if(auxiliarRaizes[i] != nullptr){
            auxiliarRaizes[i]->direita = auxiliarRaizes[i];
            auxiliarRaizes[i]->esquerda = auxiliarRaizes[i];
            if(this->min == nullptr){
                inserir_no_lista_circular(this->min, auxiliarRaizes[i]);
                this->min = auxiliarRaizes[i];
            } else {
                inserir_no_lista_circular(this->min, auxiliarRaizes[i]);
                if(auxiliarRaizes[i]->chave < this->min->chave){
                    this->min = auxiliarRaizes[i];
                } 
            }
        }
    }
}

void HeapFibonacci::cut(NoFibonacci* x, NoFibonacci* y){}
void HeapFibonacci::cascading_cut(NoFibonacci* y){}

void HeapFibonacci::heap_link(NoFibonacci* y, NoFibonacci* x){
    if(x == nullptr || y == nullptr){
        return;
    }

    deletar_no_lista_circular(y);
    inserir_no_lista_circular(x->filho, y);
    x->grau += 1;
    y->marca = false;
    y->pai = x;
}

// métodos públicos -----------------------------------------------------------------------------------
HeapFibonacci::HeapFibonacci(){}

HeapFibonacci::~HeapFibonacci(){
    if(this->min == nullptr){
        return;
    }

    NoFibonacci* atual = this->min;

    this->min->esquerda->direita = nullptr;

    while(atual != nullptr){
        NoFibonacci* proximo = atual->direita;
        liberar_memoria(atual);
        atual = proximo;
    }

}

NoFibonacci* HeapFibonacci::extract_min(){
    NoFibonacci* z = this->min;

    if(z != nullptr){
        vector<NoFibonacci*> auxiliarFilhos;
        if(z->filho != nullptr){
            NoFibonacci* proximo = z->filho;
            
            do {
                auxiliarFilhos.push_back(proximo);
                proximo = proximo->direita;
            } while(proximo != z->filho);
        }

        for(size_t i=0;i<auxiliarFilhos.size();i++){
            auxiliarFilhos[i]-> direita = auxiliarFilhos[i];
            auxiliarFilhos[i]->esquerda = auxiliarFilhos[i];
            inserir_no_lista_circular(this->min, auxiliarFilhos[i]);
            auxiliarFilhos[i]->pai = nullptr;
        }

        NoFibonacci* proxZ = z->direita;
        deletar_no_lista_circular(z);
        if(z == proxZ){
            this->min = nullptr;
        } else {
            this->min = proxZ;
            consolidate();
        }

        this->n -= 1;
    }

    return z;
}

NoFibonacci* HeapFibonacci::insert(int chave, int valor){
    NoFibonacci* novo_no = new NoFibonacci(chave, valor);
    inserir_no_lista_circular(this->min, novo_no);
    if(novo_no->chave < this->min->chave){
        this->min = novo_no;
    }
    this->n++;
    return novo_no;

}

NoFibonacci* HeapFibonacci::minimum() const {
    if (this->min == nullptr){ // Não é obrigatório essa parte, porém
        return nullptr;        // ficará mais fácil de explicar com essa verificação.
    }                          // Se tivesse ou não, daria o mesmo resultado.
    return this->min;
}

void HeapFibonacci::union_heaps(HeapFibonacci& H2){}
void HeapFibonacci::decrease_key(NoFibonacci* x, int chave_nova){}
NoFibonacci* HeapFibonacci::delete_no(NoFibonacci* x){}

int HeapFibonacci::tamanho_heap() const {
    return this->n;
}

bool HeapFibonacci::esta_vazio() const {
    if(this->min == nullptr){
        return true;
    }

    return false;
}