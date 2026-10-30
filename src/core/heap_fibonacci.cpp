#include "heap_fibonacci.hpp"
#include <algorithm>

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

void HeapFibonacci::juntar_listas_circulares(NoFibonacci* H1, NoFibonacci* H2){
    NoFibonacci* temp = H1 -> esquerda;
    NoFibonacci* temp2 = H2 -> esquerda;

    temp -> direita = H2;
    H2 -> esquerda = temp;

    temp2 -> direita = H1;
    H1 -> esquerda = temp2;

}

void HeapFibonacci::liberar_memoria(NoFibonacci* no){
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

    if(this->n <= 0){

        return;
    }
    int grauMaximo = static_cast<int>(floor(log2(this->n) / log2(1.61803398875)) + 2);

    if(grauMaximo < 1){

        grauMaximo = 1;
    }

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
    while(d >= static_cast<int>(auxiliarRaizes.size())){
        auxiliarRaizes.push_back(nullptr);
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

void HeapFibonacci::cut(NoFibonacci* x, NoFibonacci* y){
    if(x->direita == x){
        y->filho = nullptr;
    }else{
        
        x -> direita -> esquerda = x -> esquerda;
        x -> esquerda -> direita = x -> direita;

        if(y->filho == x){
            y->filho = x->direita;
        }
    }

    y->grau -= 1;
    x->direita = x;
    x->esquerda = x;
    inserir_no_lista_circular(this->min, x);

    x->pai = nullptr;
    x->marca = false;
}

void HeapFibonacci::cascading_cut(NoFibonacci* y){
    NoFibonacci* z = y->pai;

    if(z != nullptr){
       if(y-> marca == false){

          y-> marca = true;

       }else{
          cut(y,z);
          cascading_cut(z);
       }
    }
}

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
HeapFibonacci::HeapFibonacci(): n(0), min(nullptr) {}

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

int HeapFibonacci::tamanho_heap() const {
    return this->n;
}

HeapFibonacci::NoFibonacci* HeapFibonacci::extract_min(){
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

        z->filho = nullptr;
        z->grau = 0;

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

HeapFibonacci::NoFibonacci* HeapFibonacci::insert(int chave, int valor){
    NoFibonacci* novo_no = new NoFibonacci(chave, valor);

    inserir_no_lista_circular(this->min, novo_no);
    if(novo_no->chave < this->min->chave){
        this->min = novo_no;
    }
    this->n++;
    return novo_no;

}

HeapFibonacci::NoFibonacci* HeapFibonacci::minimum() const {
    if (this->min == nullptr){ 
        return nullptr;        
    }                          
    return this->min;
}

bool HeapFibonacci::esta_vazio() const {
    if(this->min == nullptr){
        return true;
    }

    return false;
}

// Cria um novo heap que reúne os elementos de H1 e H2.
HeapFibonacci* HeapFibonacci::union_heaps(HeapFibonacci& H1,HeapFibonacci& H2){
    HeapFibonacci *H = new HeapFibonacci();

    if(H1.esta_vazio() && H2.esta_vazio()){
        return H;
    }
    // Se H1 estiver vazio, transfere os elementos de H2 para H.
    if(H1.esta_vazio()){

        H->min = H2.min;
        H->n = H2.n;

        H2.min = nullptr;
        H2.n = 0;

        return H;
    }
    // Se H2 estiver vazio, transfere os elementos de H1 para H.
    if(H2.esta_vazio()){
            
        H->min = H1.min;
        H->n = H1.n;

        H1.min = nullptr;
        H1.n = 0;

        return H;

    }
    juntar_listas_circulares(H1.min, H2.min);

    if(H1.min -> chave < H2.min -> chave){
        H -> min = H1.min;
    }else{   
        H -> min = H2.min;
    }           
    H->n = H1.n + H2.n;

    H1.min = nullptr;
    H1.n = 0;

    H2.min = nullptr;
    H2.n = 0;
    return H;

}

void HeapFibonacci::decrease_key(NoFibonacci* x, int chave_nova){
    if(x == nullptr){
        return;
    }
    if(chave_nova > x->chave){
        return;
    }
    
    x->chave = chave_nova;
    NoFibonacci* y = x->pai;
    
    if(y != nullptr && x->chave < y->chave){
        cut(x,y);
        cascading_cut(y);

    }
    if(this->min == nullptr || x->chave < this->min->chave){
        this->min = x;

    }
}

HeapFibonacci::NoFibonacci* HeapFibonacci::delete_no(HeapFibonacci::NoFibonacci* x){
    if(this->min == nullptr|| x == nullptr){
        return nullptr;
    }

    decrease_key(x, INT_MIN);
    NoFibonacci* z = extract_min();
    return z;
}



