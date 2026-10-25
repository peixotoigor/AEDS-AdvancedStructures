#ifndef HEAP_FIBONACCI_HPP
#define HEAP_FIBONACCI_HPP

#include <climits>

class HeapFibonacci {
    public:
        struct NoFibonacci {
            int grau = 0;
            bool marca = false;
            int chave;
            int valor;

            NoFibonacci* pai = nullptr;
            NoFibonacci* filho = nullptr;
            NoFibonacci* esquerda = nullptr;
            NoFibonacci* direita = nullptr;

            NoFibonacci(int c, int v): chave(c), valor(v){
                esquerda = this; // os ponteiros já ficam encadeados para formar a lista circular
                direita = this;
            }

        };

    private: 
        int n;
        NoFibonacci* min;

        // métodos auxiliares para manipular a lista circular
        void inserir_no_lista_circular(NoFibonacci* x);
        void deletar_no_lista_circular(NoFibonacci* x);
        void juntar_listas_circulares(NoFibonacci* H1, NoFibonacci* H2);
        void liberar_memoria(NoFibonacci *no);

        // métodos auxiliares das funções do heap
        void consolidate();
        void cut(NoFibonacci* x, NoFibonacci* y);
        void cascading_cut(NoFibonacci* y);
        void heap_link(NoFibonacci* y, NoFibonacci* x);
        
       
    public:
        HeapFibonacci(); //substitui o make-heap()
        ~HeapFibonacci();

        NoFibonacci* insert(int chave, int valor);
        NoFibonacci* minimum() const;
        NoFibonacci* extract_min();
        void union_heaps(HeapFibonacci& H2);
        void decrease_key(NoFibonacci* x, int chave_nova);
        NoFibonacci* delete_no(NoFibonacci* x);
        int tamanho_heap() const;
        bool esta_vazio() const;
};

#endif