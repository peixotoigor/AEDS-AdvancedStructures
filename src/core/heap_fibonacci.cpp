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

void HeapFibonacci::juntar_listas_circulares(NoFibonacci& H1, NoFibonacci& H2);{}

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

void HeapFibonacci::consolidate(){}
void HeapFibonacci::cut(NoFibonacci* x, NoFibonacci* y){}
void HeapFibonacci::cascading_cut(NoFibonacci* y){}
void HeapFibonacci::heap_link(NoFibonacci* y, NoFibonacci* x){}

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

NoFibonacci* HeapFibonacci::insert(int chave, int valor){}
NoFibonacci* HeapFibonacci::minimum() const {}
NoFibonacci* HeapFibonacci::extract_min(){}
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