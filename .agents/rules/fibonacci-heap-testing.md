---
trigger: glob
description: "Testes e invariantes do Fibonacci Heap"
globs: "src/core/**/*.cpp, src/include/**/*.hpp, tests/**/*.cpp"
---

# Testes do Fibonacci Heap

- Testar heap vazio e heap com um nó.
- Testar múltiplas inserções e chaves repetidas.
- Testar consulta e extração do mínimo.
- Testar união, diminuir chave, corte e corte em cascata.
- Testar remoção e destruição de heaps vazios e não vazios.
- Validar listas circulares após cada operação estrutural importante.
- Verificar pais, filhos, graus, marcas, mínimo e quantidade de nós.
- Comparar com uma referência somente quando as operações forem equivalentes.
- Usar entradas determinísticas e registrar sementes quando houver aleatoriedade.
- Executar sanitizers quando houver risco de erro de memória.
