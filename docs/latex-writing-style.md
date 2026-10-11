# Padrão de Escrita Acadêmica em LaTeX

Este documento define o padrão de redação, argumentação, referências e
apresentação visual do relatório técnico. O conteúdo deve permanecer
compatível com os resultados realmente obtidos na implementação.

## 1. Tom de voz e registro linguístico

### Linguagem direta e concreta

Usar termos técnicos precisos e verbos operacionais.

Preferir:

- "aloca nós na heap";
- "percorre a lista circular";
- "atualiza o ponteiro do pai";
- "opera em tempo constante";
- "desaloca o nó removido";
- "mede o tempo com `std::chrono`".

Evitar adjetivos vazios, rebuscados ou hiperbólicos, como:

- "solução transcendental";
- "beleza matemática fascinante";
- "desempenho incomparável";
- "estrutura revolucionária";
- "resultado extremamente impressionante".

### Eliminação de frases genéricas

Não usar frases de preenchimento, como:

- "ao longo da história da computação";
- "é importante ressaltar que as árvores são estruturas vitais";
- "nos dias atuais, a tecnologia está cada vez mais presente".

Substituir introduções genéricas por uma definição do problema, da operação ou
da propriedade que será analisada.

### Flexão verbal

Usar a terceira pessoa impessoal em afirmações acadêmicas:

- "observa-se";
- "constata-se";
- "verifica-se";
- "define-se";
- "considera-se".

Usar a primeira pessoa do plural somente para decisões tomadas no trabalho:

- "implementamos a consolidação de forma iterativa";
- "adotamos a representação por listas circulares";
- "medimos o tempo de execução em múltiplas repetições".

## 2. Argumentação científica e empírica

As análises devem seguir a sequência:

```text
Propriedade teórica → dado numérico medido (μ ± σ) → conclusão técnica
```

Nenhuma afirmação de desempenho deve ser apresentada somente de forma
qualitativa quando puder ser acompanhada por uma medição.

Preferir:

> Para `N = 100.000`, a implementação realizou `X ± Y` operações por segundo,
> enquanto a referência realizou `A ± B`. A diferença observada é compatível
> com `<explicação técnica>`.

Evitar:

> A implementação foi muito mais rápida.

Quando não houver medição suficiente, declarar a limitação em vez de apresentar
uma conclusão como fato.

### Honestidade intelectual

Todo resultado deve apresentar suas limitações. Registrar, quando aplicável:

- pior caso e caso médio;
- diferença entre complexidade amortizada e custo de uma operação isolada;
- custo de alocação dinâmica;
- influência de cache, compilador e hardware;
- entradas para as quais a estrutura perde eficiência;
- diferenças entre o modelo assintótico e o tempo observado.

Não apresentar nenhuma estrutura como ótima para todos os cenários.

## 3. Rigor bibliográfico e histórico

Toda definição, teorema, algoritmo ou estrutura baseada em trabalho externo deve
possuir referência bibliográfica.

Quando a origem histórica for relevante, registrar autor e ano com uma fonte
verificável. As referências devem ser citadas no corpo do texto e incluídas na
bibliografia final.

Não atribuir uma estrutura a um autor ou ano sem confirmação na fonte
consultada.

## 4. Padrão tipográfico e gráfico em LaTeX

### Referências cruzadas obrigatórias

Nenhuma figura, tabela ou algoritmo deve ficar sem citação no texto.

Usar referências nomeadas:

```latex
Como apresentado na Figura~\ref{fig:consolidacao}, a extração do mínimo
reorganiza a lista de raízes.

Os tempos médios estão na Tabela~\ref{tab:tempos-insercao}.

O procedimento utilizado é descrito no Algoritmo~\ref{alg:extrair-minimo}.
```

Toda figura, tabela ou algoritmo deve possuir:

- `\label{...}`;
- citação no corpo do texto;
- legenda informativa;
- origem ou descrição dos dados, quando aplicável.

### Legendas explicativas

As legendas devem descrever o conteúdo e a conclusão relevante do artefato.

Preferir:

```latex
\caption{Tempo médio de extração do mínimo em função do número de elementos,
com barras de erro referentes ao desvio-padrão das repetições.}
```

Evitar:

```latex
\caption{Gráfico de desempenho.}
```

### Paleta e legibilidade

- Usar paleta sóbria, com azul e cinza quando houver cores.
- Garantir contraste suficiente em impressão monocromática.
- Não usar cor como único meio de distinguir informações.
- Usar padrões, símbolos ou estilos de linha quando necessário.
- Manter fontes, espessuras e unidades consistentes.
- Informar a unidade de todas as grandezas nos eixos e tabelas.

### Dados experimentais

Gráficos devem informar:

- variável independente;
- variável medida;
- unidade;
- tamanho das entradas;
- número de repetições;
- média e dispersão, quando disponíveis.

Os dados brutos devem ser preservados para permitir a reprodução das figuras.
