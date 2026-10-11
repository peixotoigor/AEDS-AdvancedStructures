# Desenvolvimento

## Fluxo específico

1. Implementar e testar primeiro as operações de lista circular.
2. Implementar criação, inserção e consulta do mínimo.
3. Implementar união de dois heaps.
4. Implementar extração do mínimo e consolidação.
5. Implementar diminuir chave, corte e corte em cascata.
6. Implementar remoção e destruição completa.
7. Validar invariantes após cada etapa.

## Fluxo recomendado

1. Entender o requisito e localizar o código relacionado.
2. Identificar o comportamento esperado e os casos-limite.
3. Implementar a menor mudança completa.
4. Adicionar ou atualizar testes.
5. Executar a validação relevante.
6. Atualizar a documentação relacionada.
7. Inspecionar o diff antes de concluir.

## Código

- Seguir o estilo existente.
- Preferir soluções simples e legíveis.
- Evitar duplicação.
- Tratar erros explicitamente.
- Não introduzir dependências sem necessidade.
- Não alterar arquivos fora do escopo.

## Compilação manual

Enquanto não houver outro comando definido pelo repositório:

```powershell
New-Item -ItemType Directory -Force build

g++ -std=c++20 -Wall -Wextra -pedantic `
    src\core\fibonacci_heap.cpp src\core\main.cpp `
    -Isrc\include `
    -o build\fibonacci_heap.exe

.\build\fibonacci_heap.exe
```

Para testar erros de memória, usar um compilador que ofereça os sanitizers:

```powershell
g++ -std=c++20 -Wall -Wextra -pedantic `
    -fsanitize=address,undefined -fno-omit-frame-pointer `
    src\core\fibonacci_heap.cpp src\core\main.cpp `
    -Isrc\include `
    -o build\fibonacci_heap-sanitized.exe
```

Se essa opção não estiver disponível no ambiente Windows, registrar a
limitação e usar outra ferramenta de diagnóstico disponível.

## Experimentos

Quando desempenho for parte do objetivo:

- definir entradas representativas;
- variar progressivamente o tamanho;
- repetir medições quando houver variação;
- registrar as condições do experimento;
- salvar dados brutos antes de gerar gráficos;
- explicar diferenças entre a teoria e os resultados observados.

Comparar, quando possível, o Fibonacci Heap com `std::priority_queue`,
deixando claro que `std::priority_queue` não oferece `decrease-key` diretamente.

## Commits e revisão

Commits devem representar mudanças coerentes. Antes de solicitar revisão,
descrever o que mudou, como foi validado e quais limitações permanecem.
