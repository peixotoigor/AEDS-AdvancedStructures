#include <iostream>
#include "fibonacci_heap.hpp"

int main() {
    std::cout << "========================================\n";
    std::cout << " Demonstracao: Fibonacci Heap (Procedural)\n";
    std::cout << "========================================\n\n";

    HeapFibonacci heap = criar_heap();
    std::cout << "1. Heap criado. Vazio? " << (heap_vazio(&heap) ? "Sim" : "Nao") << "\n";

    // Sequência de inserção do exemplo do relatório: 7, 3, 18, 38, 10, 12
    std::cout << "\n2. Inserindo elementos: 7, 3, 18, 38, 10, 12...\n";
    inserir(&heap, 7);
    inserir(&heap, 3);
    inserir(&heap, 18);
    NoFibonacci* n38 = inserir(&heap, 38);
    inserir(&heap, 10);
    NoFibonacci* n12 = inserir(&heap, 12);

    int min_val = 0;
    if (consultar_minimo(&heap, &min_val)) {
        std::cout << "   Minimo consultado: " << min_val << " (esperado: 3)\n";
    }
    std::cout << "   Total de nos: " << heap.quantidade << " (esperado: 6)\n";
    std::cout << "   Invariantes validos? " << (verificar_invariantes(&heap) ? "Sim" : "Nao") << "\n";

    // Extrair mínimo (3) - provoca consolidação das raízes
    std::cout << "\n3. Extraindo minimo...\n";
    int extraido = 0;
    if (extrair_minimo(&heap, &extraido)) {
        std::cout << "   Elemento extraido: " << extraido << "\n";
    }
    if (consultar_minimo(&heap, &min_val)) {
        std::cout << "   Novo minimo: " << min_val << " (esperado: 7)\n";
    }
    std::cout << "   Total de nos restante: " << heap.quantidade << " (esperado: 5)\n";
    std::cout << "   Invariantes validos apos consolidacao? " 
              << (verificar_invariantes(&heap) ? "Sim" : "Nao") << "\n";

    // Diminuir chave: diminuir 38 para 5 (torna-se o novo mínimo)
    std::cout << "\n4. Diminuindo chave do no com valor 38 para 5...\n";
    if (diminuir_chave(&heap, n38, 5)) {
        std::cout << "   Chave diminuida com sucesso.\n";
    }
    if (consultar_minimo(&heap, &min_val)) {
        std::cout << "   Novo minimo: " << min_val << " (esperado: 5)\n";
    }
    std::cout << "   Invariantes validos? " << (verificar_invariantes(&heap) ? "Sim" : "Nao") << "\n";

    // Remover um nó arbitrário (ex: n12)
    std::cout << "\n5. Removendo no com valor 12...\n";
    if (remover_no(&heap, n12)) {
        std::cout << "   No removido com sucesso.\n";
    }
    std::cout << "   Total de nos restante: " << heap.quantidade << " (esperado: 4)\n";
    std::cout << "   Invariantes validos? " << (verificar_invariantes(&heap) ? "Sim" : "Nao") << "\n";

    // Destruição do heap
    std::cout << "\n6. Destruindo heap...\n";
    destruir_heap(&heap);
    std::cout << "   Heap destruido. Vazio? " << (heap_vazio(&heap) ? "Sim" : "Nao") << "\n";
    std::cout << "   Quantidade: " << heap.quantidade << ", Minimo: " << (void*)heap.minimo << "\n";

    std::cout << "\nDemonstracao finalizada com sucesso!\n";
    return 0;
}
