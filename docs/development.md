# Desenvolvimento — Árvore de Van Emde Boas (vEB Tree)

## Fluxo Específico de Implementação Procedural

A construção da Árvore de Van Emde Boas deve seguir uma sequência estritamente incremental, garantindo que cada camada conceitual seja validada antes do avanço para a seguinte:

1. **Aritmética de Bits e Decomposição:**
   - Implementar e testar unitariamente `veb_high(x, u)`, `veb_low(x, u)`, `veb_index(h, l, u)`, `veb_sqrt_up(u)` e `veb_sqrt_down(u)`.
2. **Alocação e Desalocação Recursiva:**
   - Implementar `veb_criar(u)` e `veb_destruir(V)`, validando ausência de vazamento de memória para $u = 2, 4, 16, 65536$.
3. **Caso Base ($u = 2$) e Consultas Elementares:**
   - Implementar `veb_vazio(V)`, `veb_minimo(V)` e `veb_maximo(V)` em $O(1)$.
   - Implementar pertinência (`veb_pertence`) para $u = 2$ e recursivo.
4. **Consultas Ordenadas:**
   - Implementar `veb_sucessor(V, x)` e `veb_predecessor(V, x)` com saltos via *summary* em $O(\log \log u)$.
5. **Inserção com Invariante de Mínimo:**
   - Implementar `veb_inserir(V, x)`, assegurando que $min$ nunca desça para os clusters e que no máximo uma chamada recursiva ocorra.
6. **Remoção e Atualização de Resumo:**
   - Implementar `veb_remover(V, x)`, tratando promoção de $new\_min$ e remoção no *summary* quando clusters esvaziarem.
7. **Validação de Invariantes e Differential Testing:**
   - Implementar `veb_verificar_invariantes(V)` e submeter ao estresse com `std::set` como oráculo.
8. **Benchmarks e Avaliação Empírica:**
   - Executar suíte de medição ($10\times$ repetições) sob variação de $u$ e densidade $\alpha$.
9. **Pós-Processamento e Relatório:**
   - Gerar gráficos e tabelas com script Python e compilar monografia LaTeX.

## Comandos de Compilação e Execução (PowerShell no Windows)

### 1. Demonstração e Uso Básico

```powershell
New-Item -ItemType Directory -Force build

g++ -std=c++20 -Wall -Wextra -pedantic `
    src\core\van_emde_boas.cpp src\core\main.cpp `
    -Isrc\include `
    -o build\van_emde_boas.exe

.\build\van_emde_boas.exe
```

### 2. Suíte de Testes Unitários, Invariantes e Differential Testing

```powershell
g++ -std=c++20 -Wall -Wextra -pedantic `
    src\core\van_emde_boas.cpp tests\van_emde_boas_tests.cpp `
    -Isrc\include `
    -o build\van_emde_boas_tests.exe

.\build\van_emde_boas_tests.exe
```

### 3. Benchmarks de Desempenho e Consumo de Memória ($10\times$ Repetições)

```powershell
New-Item -ItemType Directory -Force benchmarks/data, benchmarks/figures, benchmarks/tables

g++ -std=c++20 -O3 -Wall -Wextra `
    src\core\van_emde_boas.cpp tests\benchmark_van_emde_boas.cpp `
    -Isrc\include -lpsapi `
    -o build\benchmark_runner.exe

.\build\benchmark_runner.exe
```

### 4. Pós-Processamento e Geração de Gráficos via Python

```powershell
python benchmarks\gerar_graficos.py
```

Para exploração visual e interativa dos dados via Jupyter Notebook:

```powershell
jupyter notebook benchmarks\analise_graficos.ipynb
```

### 5. Compilação do Relatório Monográfico em LaTeX

```powershell
pdflatex -interaction=nonstopmode -output-directory=report report/main.tex
bibtex report/main
pdflatex -interaction=nonstopmode -output-directory=report report/main.tex
pdflatex -interaction=nonstopmode -output-directory=report report/main.tex
```

## Sanitizers de Memória e Depuração

Para verificação de ponteiros inválidos, vazamentos e undefined behavior em compiladores compatíveis:

```powershell
g++ -std=c++20 -Wall -Wextra -pedantic `
    "-fsanitize=address,undefined" -fno-omit-frame-pointer `
    src\core\van_emde_boas.cpp tests\van_emde_boas_tests.cpp `
    -Isrc\include `
    -o build\van_emde_boas-sanitized.exe
```

Caso o ambiente MinGW local não disponha de `libasan`, a ausência de vazamentos é comprovada pelo validador formal de invariantes estruturais e pela auditoria sistemática de `delete[]` em pós-ordem.
