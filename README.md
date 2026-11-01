# AEDS-AdvancedStructures

Projeto acadêmico para estudo conceitual, especificação arquitetural, implementação procedural e análise comparativa de desempenho de **Árvores de Van Emde Boas (*vEB Trees*)**.

## Objetivos

- **Rigor Conceitual e Teórico:** Formalizar a estrutura de dados de Van Emde Boas para conjuntos dinâmicos de inteiros em um universo limitado $[0, u - 1]$, explorando a divisão recursiva em clusters e subárvore de resumo (*summary*).
- **Análise Assintótica:** Demonstrar a complexidade temporal de pior caso de $O(\log \log u)$ para busca (`member`), inserção (`insert`), remoção (`delete`), predecessor (`predecessor`) e sucessor (`successor`), além de consultas de mínimo e máximo em $O(1)$.
- **Arquitetura Procedural Limpa:** Definir contratos estritos, registros (`struct`) e funções livres em C++20 que priorizam clareza didática e viabilidade de inspeção acadêmica linha a linha.
- **Estratégias de Validação Robusta:** Estabelecer uma suíte completa de testes unitários funcionais, preservação estrita de invariantes estruturais e testes diferenciais com oráculos canônicos da STL (`std::set`, `std::vector<bool>`, `std::bitset`).
- **Benchmarking e Avaliação Empírica:** Mapear o comportamento experimental de tempo de execução e consumo de memória sob diferentes densidades de preenchimento e escalas de universo ($u$), comparando com árvores balanceadas de busca e bitmaps.

## Organização do Repositório

```text
src/
  core/       Implementação procedural (.cpp) e demonstração principal
  include/    Definição de structs, constantes e contratos públicos (.hpp)
tests/        Testes funcionais, testes de invariantes e suíte de benchmarks
benchmarks/   Dados brutos (.txt), gráficos (.pdf, .png), tabelas LaTeX e Jupyter Notebook
docs/         Requisitos, arquitetura formal, estilo de código, desenvolvimento e validação
report/       Relatório técnico-científico monográfico em LaTeX
```

Consulte a documentação detalhada em [`docs/`](docs/) para especificações formais de cada subsistema:

- [`docs/requirements.md`](docs/requirements.md): Requisitos funcionais, casos de fronteira e baselines.
- [`docs/architecture.md`](docs/architecture.md): Especificação matemática da decomposição recursiva, invariantes e análise de complexidade.
- [`docs/coding-style.md`](docs/coding-style.md): Convenções de estilo procedural em C++20 e gerenciamento de memória.
- [`docs/testing.md`](docs/testing.md): Metodologia de testes de corretude, oráculos diferenciais e validação de invariantes.
- [`docs/benchmarks.md`](docs/benchmarks.md): Protocolo experimental, métricas de memória e baselines comparativos.
- [`docs/development.md`](docs/development.md): Guia de compilação, execução e fluxo de desenvolvimento.

## Requisitos de Ambiente

- Compilador C++20 compatível (GCC/G++ $\ge 11$ ou Clang $\ge 13$).
- PowerShell 7+ ou terminal Bash no Windows/Linux.
- CMake 3.20+ (opcional para automação de compilação).
- Python 3.9+ com `matplotlib` e `numpy` para processamento de benchmarks e plotagem de gráficos.
- Distribuição LaTeX (MiKTeX ou TeX Live) com `pdflatex` e `bibtex` para geração do relatório monográfico.

## Diretrizes de Contribuição e Desenvolvimento

Antes de qualquer implementação ou execução:

1. Leia [`AGENTS.md`](AGENTS.md) para diretrizes de segurança, governança e conformidade.
2. Leia [`docs/requirements.md`](docs/requirements.md) para alinhar escopo funcional e restrições.
3. Estude [`docs/architecture.md`](docs/architecture.md) para compreender a aritmética de decomposição (`high`, `low`, `index`) e os invariantes de `min`/`max`.
4. Siga as convenções estabelecidas em [`docs/coding-style.md`](docs/coding-style.md).
5. Utilize os scripts e comandos catalogados em [`docs/development.md`](docs/development.md).

## Estado do Projeto

- **Modelagem Teórica e Requisitos:** 100% formalizados para Árvores de Van Emde Boas (*vEB Tree*).
- **Arquitetura e Contratos de Dados:** Especificação matemática concluída com recorrência assintótica $T(u) = T(\sqrt{u}) + O(1)$.
- **Protocolo de Testes e Validação:** Desenho de testes com oráculo diferencial (`std::set` / `std::bitset`) e verificação de invariantes estruturais devidamente documentados.
- **Plano de Benchmarks:** Protocolo de medições com 10 repetições ($10\times$), variação de densidade e modelagem de footprint de memória em elaboração para integração no relatório técnico.
