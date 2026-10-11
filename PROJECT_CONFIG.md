# Configuração do Projeto

Configuração mínima e oficial do repositório **AEDS-AdvancedStructures**.
Decisões devem ser baseadas neste arquivo, no código, nos testes e nas demais
configurações versionadas do próprio repositório.

## 1. Projeto

```yaml
name: AEDS-AdvancedStructures
language: C++
standard: C++20
architecture: header-only
namespace: <definir quando a API pública for criada>
build_system: CMake
```

### Objetivo

Implementar e estudar estruturas de dados e algoritmos avançados em C++,
priorizando corretude, segurança de memória, eficiência e código genérico.

## 2. Organização

```text
include/       Headers públicos da biblioteca
src/           Código compilado, caso seja necessário
tests/         Testes unitários, de estresse e de equivalência
examples/      Exemplos de utilização
docs/          Documentação adicional
build/         Arquivos gerados pelo CMake; não versionar
```

Quando não houver uma estrutura de diretórios criada, seguir este padrão ao
adicionar novos arquivos.

## 3. Regras de implementação

- Usar C++20 e templates genéricos quando fizer sentido.
- Preferir a regra dos cinco ou, idealmente, a regra dos zero.
- Usar RAII e evitar ownership manual.
- Não permitir vazamentos, double-free, uso após liberação ou comportamento
  indefinido.
- Cópia rasa é proibida para estruturas que possuem recursos próprios.
- Operações de movimento devem ser `noexcept` quando essa garantia for válida.
- Manter invariantes da estrutura após toda operação.
- Em listas duplamente encadeadas circulares, atualizar corretamente os
  ponteiros `left` e `right`.
- Manter headers públicos autocontidos e sem dependência de includes
  transitivos.
- Usar nomes e padrões já adotados pelo código existente.
- Não adicionar dependências sem necessidade.

Cada nova estrutura deve documentar, de forma breve:

- operações disponíveis;
- complexidade de tempo e espaço;
- comportamento para estrutura vazia;
- tratamento de elementos repetidos;
- regras de cópia, movimento e invalidação de iteradores, quando aplicável.

## 4. Testes obrigatórios

Toda nova estrutura ou método público deve possuir testes para:

- estrutura vazia;
- elemento único;
- inserção e remoção;
- primeiro e último elemento;
- chaves ou valores repetidos;
- remoção ou busca de elemento inexistente;
- limites relevantes;
- sequência de operações;
- comportamento após mover ou copiar, quando suportado.

Quando houver container equivalente, comparar os resultados com a STL.
Testes aleatórios devem usar semente determinística ou registrar a semente
utilizada.

## 5. Qualidade

Compilar sem warnings relevantes e formatar o código com `clang-format`.
Executar análise estática com `clang-tidy` quando a ferramenta estiver
disponível.

Validações prioritárias:

1. compilação em Debug;
2. testes unitários;
3. testes de estresse;
4. AddressSanitizer e UndefinedBehaviorSanitizer;
5. compilação em Release.

## 6. Comandos padrão

```powershell
# Configurar
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

# Compilar
cmake --build build --parallel

# Executar testes
ctest --test-dir build --output-on-failure
```

Se o projeto ainda não possuir `CMakeLists.txt` ou testes configurados,
adicionar a infraestrutura mínima necessária antes de criar uma estrutura
nova.

## 7. Checklist de alteração

- [ ] A alteração segue C++20 e os padrões existentes.
- [ ] Os invariantes e a ownership foram preservados.
- [ ] Testes de casos normais e limites foram adicionados.
- [ ] Testes contra a STL foram adicionados quando aplicável.
- [ ] Sanitizers foram executados quando houver código de memória.
- [ ] Documentação e exemplos foram atualizados quando necessário.
- [ ] `git diff --check` não apresenta problemas.
