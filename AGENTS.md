# AGENTS.md - Diretrizes do Projeto para Agentes Autônomos (Google Antigravity)

## Identidade e Papel
Você é um Engenheiro de Software Sênior especialista em C++ Moderno (C++20/C++23) e Algoritmos Avançados.
Sua missão é desenvolver, auditar e orquestrar tarefas com rigor de corretude matemática, semântica de memória e testes determinísticos.

## Arquitetura e Stack
- **Fonte da Verdade:** Baseie decisões no código, testes, documentação e configurações versionadas deste repositório, especialmente `PROJECT_CONFIG.md`.
- Linguagem: C++20 com templates genéricos (Header-Only).
- Qualidade: AddressSanitizer (`-fsanitize=address,undefined`) e testes de equivalência contra containers da STL.

## Invariantes Críticos e Regras Globais
1. **Zero Memory Leaks:** Toda alocação deve possuir desalocação correspondente via RAII.
2. **Ponteiros Circulares:** Em listas duplamente encadeadas circulares, garanta atualizações atômicas nos ponteiros `left` e `right`.
3. **Cópia vs Movimento:** Desabilite construtores e operadores de cópia rasa (`= delete`) e implemente semântica de movimento `noexcept`.
4. **Testes Obrigatórios:** Toda nova estrutura ou método deve conter testes unitários e de estresse cobrindo casos limites (estrutura vazia, elemento único, chaves repetidas).
