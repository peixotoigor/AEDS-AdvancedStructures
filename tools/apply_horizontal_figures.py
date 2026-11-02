import re

with open('build_relatorio.py', 'r', encoding='utf-8') as f:
    text = f.read()

figures_meta = {
    "fig:estudo1-insercao": {
        "suba_title": r"Padrão Aleatório\\ \scriptsize(Heap lazy supera vEB em 93,9\%)",
        "subb_title": r"Padrão Crescente\\ \scriptsize(Clusters contíguos aceleram vEB em 89,7\%)",
        "subc_title": r"Padrão Decrescente\\ \scriptsize(vEB supera Heap de Fibonacci em 12,3\%)",
        "caption": r"Estudo 1: Avaliação comparativa do tempo de inserção entre Heap de Fibonacci e Árvore vEB ($n \in [10^2, 10^6]$, 10 repetições com desvio padrão): (a) Padrão aleatório demonstrando a eficácia da política lazy $O(1)$ estrita do Heap de Fibonacci ($38{,}06$~ms contra $623{,}35$~ms da vEB em $n=10^6$); (b) Padrão crescente onde o encadeamento contíguo em clusters reduz o tempo da vEB em 89,70\% ($64{,}17$~ms); (c) Padrão decrescente onde a vEB supera o Heap em 12,29\% ($37{,}21$~ms vs $42{,}42$~ms) por meio da atualização direta de ponteiros mínimos em sub-árvores recursivas."
    },
    "fig:estudo1-obter-min": {
        "suba_title": r"Padrão Aleatório\\ \scriptsize(Consulta $O(1)$ estrita com vantagem do Heap de 31,8\%)",
        "subb_title": r"Padrão Crescente\\ \scriptsize(Estabilidade em $\mu$s independente de $n$)",
        "subc_title": r"Padrão Decrescente\\ \scriptsize(Localidade de cache favorece vEB em 42,6\%)",
        "caption": r"Estudo 1: Avaliação do tempo de consulta do elemento mínimo entre Heap de Fibonacci e Árvore vEB ($n \in [10^2, 10^6]$, 10 repetições com desvio padrão): (a) Padrão aleatório confirmando latência estritamente $O(1)$ no pior caso em ambas as estruturas ($2{,}41$~ms no Heap vs $3{,}54$~ms na vEB em $n=10^6$), com vantagem do Heap pela leitura direta de \texttt{minRoot}; (b) Padrão crescente exibindo independência temporal absoluta em relação à escala de chaves; (c) Padrão decrescente onde a vEB supera o Heap em 42,58\% ($2{,}38$~ms vs $4{,}14$~ms) decorrente de menor dispersão de ponteiros na hierarquia de memória."
    },
    "fig:estudo1-extrair-min": {
        "suba_title": r"Padrão Aleatório\\ \scriptsize(vEB $O(\log \log U)$ é 97,0\% mais rápida que Heap)",
        "subb_title": r"Padrão Crescente\\ \scriptsize(Consolidação melhora mas vEB lidera em 84,0\%)",
        "subc_title": r"Padrão Decrescente\\ \scriptsize(Dominância da vEB com 93,6\% de redução temporal)",
        "caption": r"Estudo 1: Avaliação do tempo de extração do mínimo entre Heap de Fibonacci e Árvore vEB ($n \in [10^2, 10^6]$, 10 repetições com desvio padrão): (a) Padrão aleatório evidenciando a superioridade da complexidade $O(\log \log U)$ da vEB ($64{,}33$~ms), sendo 33,35x mais rápida que o Heap ($2.145{,}75$~ms), cuja consolidação de raízes impõe alto custo amortizado; (b) Padrão crescente onde o menor número de árvores no Heap atenua a consolidação ($711{,}44$~ms), embora a vEB mantenha folga de 83,98\% ($113{,}95$~ms); (c) Padrão decrescente onde a vEB supera o Heap em 93,60\% ($53{,}24$~ms vs $832{,}00$~ms) pela eficiência de partição de clusters."
    },
    "fig:estudo1-remocao-esp": {
        "suba_title": r"Padrão Aleatório\\ \scriptsize(Referência direta favorece Heap em 22,2\%)",
        "subb_title": r"Padrão Crescente\\ \scriptsize(Clusters contíguos tornam vEB 87,6\% mais rápida)",
        "subc_title": r"Padrão Decrescente\\ \scriptsize(vEB consolida dominância de 92,3\% frente ao Heap)",
        "caption": r"Estudo 1: Avaliação do tempo de remoção de elemento específico entre Heap de Fibonacci e Árvore vEB ($n \in [10^2, 10^6]$, 10 repetições com desvio padrão): (a) Padrão aleatório com o Heap consumindo $597{,}89$~ms via ponteiro direto contra $768{,}37$~ms da vEB por busca de valor; (b) Padrão crescente onde a vEB reduz seu tempo para $89{,}44$~ms, superando o Heap ($721{,}72$~ms) em 87,61\%; (c) Padrão decrescente no qual a vEB atinge $39{,}36$~ms e supera o Heap ($508{,}83$~ms) em 92,27\% graças à eliminação imediata em sub-árvores recursivas sem cascata de cortes."
    },
    "fig:estudo1-memoria-pico": {
        "suba_title": r"Padrão Aleatório\\ \scriptsize(Heap $O(n)$ inicia com 5,5 MB vs 109,4 MB da vEB)",
        "subb_title": r"Padrão Crescente\\ \scriptsize(Economia de 95,0\% de memória no Heap em baixo $\rho$)",
        "subc_title": r"Padrão Decrescente\\ \scriptsize(vEB estabiliza em 120,5 MB dominada por $U$)",
        "caption": r"Estudo 1: Avaliação do consumo de memória total de processo entre Heap de Fibonacci e Árvore vEB ($n \in [10^2, 10^6]$, 10 repetições com desvio padrão): (a) Padrão aleatório comprovando alocação proporcional aos nós no Heap ($5{,}50$~MB a $71{,}23$~MB) contra custo quase constante dominado por $U = 2^{20}$ na vEB ($109{,}37$~MB a $120{,}46$~MB); (b) Padrão crescente demonstrando que o Heap economiza 94,97\% em $n=10^2$ em razão da penalidade de clusters vazios da vEB; (c) Padrão decrescente confirmando a invariância da vEB frente à distribuição das chaves."
    },
    "fig:estudo1-especificas-veb": {
        "suba_title": r"Padrão Aleatório\\ \scriptsize(Operações com invariância temporal $O(\log \log U)$)",
        "subb_title": r"Padrão Crescente\\ \scriptsize(Latência idêntica entre sucessor e predecessor)",
        "subc_title": r"Padrão Decrescente\\ \scriptsize(Consultas quase planas entre $10^2$ e $10^6$)",
        "caption": r"Estudo 1: Operações específicas da Árvore de van Emde Boas — Busca, Sucessor, Predecessor e Máximo ($n \in [10^2, 10^6]$, 10 repetições com desvio padrão): (a) Padrão aleatório ilustrando curvas rigorosamente planas decorrentes da complexidade $O(\log \log U)$, independente de $n$; (b) Padrão crescente demonstrando tempo uniforme de consulta de extremos e vizinhança; (c) Padrão decrescente confirmando a simetria operacional entre \texttt{successor} e \texttt{predecessor} em toda a faixa experimental."
    },
    "fig:estudo1-especificas-fib": {
        "suba_title": r"Padrão Aleatório\\ \scriptsize(União $O(1)$ estrita com latência sub-$\mu$s)",
        "subb_title": r"Padrão Crescente\\ \scriptsize(\texttt{decreaseKey} com cortes amortizados estáveis)",
        "subc_title": r"Padrão Decrescente\\ \scriptsize(Previsibilidade de \texttt{decreaseKey} e \texttt{union})",
        "caption": r"Estudo 1: Operações complementares do Heap de Fibonacci — Diminuição de Chave e União ($n \in [10^2, 10^6]$, 10 repetições com desvio padrão): (a) Padrão aleatório evidenciando união em tempo $O(1)$ estrito no pior caso via concatenação de listas circulares ($0{,}18$ a $0{,}32~\mu$s); (b) Padrão crescente demonstrando que a diminuição de chave opera em $O(1)$ amortizado mesmo com múltiplos cortes em cascata; (c) Padrão decrescente confirmando estabilidade operacional em larga escala."
    },
    "fig:estudo2-insercao": {
        "suba_title": r"Padrão Aleatório\\ \scriptsize(vEB supera AVL em 66,7\% e RB em 56,5\% em $10^6$)",
        "subb_title": r"Padrão Crescente\\ \scriptsize(Rotações aceleram árvores binárias até $n=10^5$)",
        "subc_title": r"Padrão Decrescente\\ \scriptsize(vEB lidera em $10^6$ consumindo 120,2 ms)",
        "caption": r"Estudo 2: Avaliação comparativa do tempo de inserção entre árvores AVL, Rubro-Negra e vEB ($n \in [10^2, 10^6]$, 10 repetições com desvio padrão): (a) Padrão aleatório evidenciando a inversão de desempenho em $n=10^6$, onde a vEB ($792{,}83$~ms) supera a AVL ($2.380{,}94$~ms) e a Rubro-Negra ($1.821{,}37$~ms) devido ao esgotamento do cache L3 das árvores de busca; (b) Padrão crescente com eficiência das árvores binárias sob dados pré-ordenados; (c) Padrão decrescente confirmando a supremacia assintótica de $O(\log \log U)$ em grande volume."
    },
    "fig:estudo2-busca": {
        "suba_title": r"Padrão Aleatório\\ \scriptsize(vEB é 64,5\% mais rápida que AVL em $n=10^6$)",
        "subb_title": r"Padrão Crescente\\ \scriptsize(Localidade no L1 favorece binárias para $n \le 10^3$)",
        "subc_title": r"Padrão Decrescente\\ \scriptsize(Inflexão assintótica em $10^5$ com transição para vEB)",
        "caption": r"Estudo 2: Avaliação do tempo de busca por pertinência entre árvores AVL, Rubro-Negra e vEB ($n \in [10^2, 10^6]$, 10 repetições com desvio padrão): (a) Padrão aleatório demonstrando redução de 64,49\% do tempo na vEB ($372{,}93$~ms vs $1.050{,}33$~ms da AVL) em $n=10^6$ pelo salto direto de apenas 5 níveis recursivos; (b) Padrão crescente destacando dominância das árvores binárias em volumes pequenos ($n \le 10^3$), onde a árvore inteira reside no cache L1; (c) Padrão decrescente com clara inversão de dominância à medida que a dispersão de ponteiros das árvores binárias gera cache misses."
    },
    "fig:estudo2-minmax": {
        "suba_title": r"Padrão Aleatório\\ \scriptsize(Consulta $O(1)$ na vEB supera AVL e RB em 88,9\%)",
        "subb_title": r"Padrão Crescente\\ \scriptsize(Travessia $O(\log n)$ até folhas penaliza binárias)",
        "subc_title": r"Padrão Decrescente\\ \scriptsize(Estabilidade quase absoluta da vEB em toda escala)",
        "caption": r"Estudo 2: Avaliação do tempo de consulta de mínimo e máximo entre AVL, Rubro-Negra e vEB ($n \in [10^2, 10^6]$, 10 repetições com desvio padrão): (a) Padrão aleatório com a vEB consumindo apenas $4{,}98$~ms contra $44{,}95$~ms da AVL e $43{,}28$~ms da Rubro-Negra em $n=10^6$, operando em tempo $O(1)$ estrito no pior caso; (b) Padrão crescente evidenciando o custo de percurso das árvores binárias até as folhas extremas; (c) Padrão decrescente comprovando que a manutenção de ponteiros na vEB elimina custos de travessia."
    },
    "fig:estudo2-sucpred": {
        "suba_title": r"Padrão Aleatório\\ \scriptsize(vEB é 64,1\% mais rápida que AVL em $n=10^6$)",
        "subb_title": r"Padrão Crescente\\ \scriptsize(Resumo de clusters supera percurso de ancestrais)",
        "subc_title": r"Padrão Decrescente\\ \scriptsize(vEB atinge 950,7 ms vs 2.475,2 ms da AVL)",
        "caption": r"Estudo 2: Avaliação do tempo de sucessor e predecessor entre AVL, Rubro-Negra e vEB ($n \in [10^2, 10^6]$, 10 repetições com desvio padrão): (a) Padrão aleatório comprovando vantagem expressiva da vEB ($1.100{,}52$~ms contra $3.063{,}42$~ms da AVL e $2.625{,}59$~ms da Rubro-Negra em $n=10^6$), guiada pelo resumo de clusters; (b) Padrão crescente onde o custo de retorno por ancestrais degrada as árvores binárias; (c) Padrão decrescente exibindo consistência assintótica $O(\log \log U)$ na localização de vizinhos imediatos."
    },
    "fig:estudo2-remocao": {
        "suba_title": r"Padrão Aleatório\\ \scriptsize(vEB supera AVL em 65,0\% e RB em 47,9\%)",
        "subb_title": r"Padrão Crescente\\ \scriptsize(Cache favorece Rubro-Negra em volumes médios)",
        "subc_title": r"Padrão Decrescente\\ \scriptsize(vEB lidera em $10^6$ consumindo 129,2 ms)",
        "caption": r"Estudo 2: Avaliação do tempo de remoção entre árvores AVL, Rubro-Negra e vEB ($n \in [10^2, 10^6]$, 10 repetições com desvio padrão): (a) Padrão aleatório demonstrando superioridade da vEB ($958{,}11$~ms vs $2.734{,}62$~ms da AVL e $1.837{,}90$~ms da Rubro-Negra em $n=10^6$), eliminando rotações de rebalanceamento; (b) Padrão crescente com menor custo de reorganização na Rubro-Negra em dados ordenados; (c) Padrão decrescente confirmando a rápida desativação de bits em clusters recursivos."
    },
    "fig:estudo2-arvores-memoria-pico": {
        "suba_title": r"Padrão Aleatório\\ \scriptsize(Linearidade $O(n)$ das binárias vs $O(U)$ da vEB)",
        "subb_title": r"Padrão Crescente\\ \scriptsize(vEB demanda 107,4 MB em $10^2$ vs 59,9 MB das binárias)",
        "subc_title": r"Padrão Decrescente\\ \scriptsize(Invariância da vEB dominada pelo universo de $2^{20}$)",
        "caption": r"Estudo 2: Avaliação do consumo de memória total de processo entre AVL, Rubro-Negra e vEB ($n \in [10^2, 10^6]$, 10 repetições com desvio padrão): (a) Padrão aleatório atestando que AVL e Rubro-Negra mantêm alocação estritamente proporcional aos nós ($59{,}94$~MB a $96{,}22$~MB), enquanto a vEB aloca $107{,}41$~MB a $120{,}46$~MB dominada por $U$; (b) Padrão crescente destacando o desperdício espacial da vEB quando a taxa de ocupação $\rho$ é reduzida; (c) Padrão decrescente evidenciando a convergência do custo amortizado por chave quando $\rho \to 1$."
    },
    "fig:estudo2-dijkstra-tempo": {
        "suba_title": r"Grafo Esparso ($|V|=100.000, |E|=300.000$)\\ \scriptsize(Heap Binário é 89,2\% mais veloz que Fibonacci)",
        "subb_title": r"Grafo Denso ($|V|=5.000, |E|=4.999.000$)\\ \scriptsize(Diminuição $O(1)$ reduz diferença para 1,49x)",
        "caption": r"Estudo 2: Avaliação temporal do Algoritmo de Dijkstra com diferentes filas de prioridade (seed=42, 10 repetições com desvio padrão): (a) Grafo esparso evidenciando a superioridade do Heap Binário ($50{,}85$~ms) e 4-ário ($56{,}77$~ms) frente ao Heap de Fibonacci ($471{,}37$~ms), resultante da contiguidade em cache e ausência de ponteiros dinâmicos; (b) Grafo denso com $\approx 5$ milhões de arestas onde a operação de diminuição de chave amortizada $O(1)$ do Heap de Fibonacci reduz a distância para o Heap Binário ($103{,}09$~ms vs $68{,}93$~ms)."
    },
    "fig:estudo2-dijkstra-memoria": {
        "suba_title": r"Grafo Esparso ($|V|=100.000, |E|=300.000$)\\ \scriptsize(Heaps em vetor consomem 14,88 MB)",
        "subb_title": r"Grafo Denso ($|V|=5.000, |E|=4.999.000$)\\ \scriptsize(Estrutura de adjacência domina 59,24 MB)",
        "caption": r"Estudo 2: Avaliação do consumo de memória de processo no Algoritmo de Dijkstra (seed=42, 10 repetições com desvio padrão): (a) Grafo esparso com alocação quase idêntica entre os heaps ($14{,}88$~MB a $15{,}16$~MB), com leve acréscimo de 1,88\% no Heap de Fibonacci pelos nós encadeados; (b) Grafo denso demonstrando que o volume de arestas representa a quase totalidade da pegada de memória ($59{,}24$~MB a $59{,}49$~MB), tornando o overhead da fila desprezível."
    },
    "fig:estudo2-valgrind-heap": {
        "caption": r"Estudo 2: Perfil comparativo do pico absoluto de memória dinâmica alocada no heap aferido via Valgrind Massif ($n=10^5$ elementos): comprovação experimental de que a Árvore de van Emde Boas demanda $112{,}28$~MB na instanciação estática de clusters para $U=2^{20}$, exigindo 7,74x mais memória dinâmica do que a Árvore AVL ($14{,}51$~MB) e 7,00x mais do que a Árvore Rubro-Negra ($16{,}05$~MB)."
    }
}

# Find each \begin{figure} ... \end{figure}
pattern = re.compile(r'\\begin\{figure\}.*?\\end\{figure\}', re.DOTALL)
matches = list(pattern.finditer(text))

new_text = text
for m in reversed(matches):
    block = m.group(0)
    all_lbls = re.findall(r'\\label\{([^}]+)\}', block)
    imgs = re.findall(r'\\includegraphics.*?\{([^}]+)\}', block)
    
    main_lbl = None
    for l in all_lbls:
        if l in figures_meta:
            main_lbl = l
            break
            
    if not main_lbl:
        continue
        
    meta = figures_meta[main_lbl]
    
    if len(imgs) == 3:
        lbl_a, lbl_b, lbl_c = all_lbls[0], all_lbls[1], all_lbls[2]
        img_a, img_b, img_c = imgs[0], imgs[1], imgs[2]
        
        replacement = f"""\\begin{{figure}}[htbp]
  \\centering
  \\begin{{subfigure}}[b]{{0.33\\textwidth}}
    \\centering
    \\includegraphics[width=\\textwidth]{{{img_a}}}
    \\caption{{{meta['suba_title']}}}
    \\label{{{lbl_a}}}
  \\end{{subfigure}}\\hfill%
  \\begin{{subfigure}}[b]{{0.33\\textwidth}}
    \\centering
    \\includegraphics[width=\\textwidth]{{{img_b}}}
    \\caption{{{meta['subb_title']}}}
    \\label{{{lbl_b}}}
  \\end{{subfigure}}\\hfill%
  \\begin{{subfigure}}[b]{{0.33\\textwidth}}
    \\centering
    \\includegraphics[width=\\textwidth]{{{img_c}}}
    \\caption{{{meta['subc_title']}}}
    \\label{{{lbl_c}}}
  \\end{{subfigure}}
  \\caption{{{meta['caption']}}}
  \\label{{{main_lbl}}}
  \\vspace{{0.1cm}}
  
  \\raggedright\\footnotesize Fonte: Elaborado pelo autor a partir dos dados experimentais (2026).
\\end{{figure}}"""
    elif len(imgs) == 2:
        lbl_a, lbl_b = all_lbls[0], all_lbls[1]
        img_a, img_b = imgs[0], imgs[1]
        replacement = f"""\\begin{{figure}}[htbp]
  \\centering
  \\begin{{subfigure}}[b]{{0.49\\textwidth}}
    \\centering
    \\includegraphics[width=\\textwidth]{{{img_a}}}
    \\caption{{{meta['suba_title']}}}
    \\label{{{lbl_a}}}
  \\end{{subfigure}}\\hfill%
  \\begin{{subfigure}}[b]{{0.49\\textwidth}}
    \\centering
    \\includegraphics[width=\\textwidth]{{{img_b}}}
    \\caption{{{meta['subb_title']}}}
    \\label{{{lbl_b}}}
  \\end{{subfigure}}
  \\caption{{{meta['caption']}}}
  \\label{{{main_lbl}}}
  \\vspace{{0.1cm}}
  
  \\raggedright\\footnotesize Fonte: Elaborado pelo autor a partir dos testes de benchmark (2026).
\\end{{figure}}"""
    elif len(imgs) == 1:
        img_a = imgs[0]
        replacement = f"""\\begin{{figure}}[htbp]
  \\centering
  \\includegraphics[width=0.80\\textwidth]{{{img_a}}}
  \\caption{{{meta['caption']}}}
  \\label{{{main_lbl}}}
  \\vspace{{0.1cm}}
  
  \\raggedright\\footnotesize Fonte: Elaborado pelo autor a partir do perfilamento com Valgrind Massif (2026).
\\end{{figure}}"""
    else:
        continue
        
    start, end = m.span()
    new_text = new_text[:start] + replacement + new_text[end:]
    print(f"Replaced {main_lbl} with horizontal maximum width layout.")

with open('build_relatorio.py', 'w', encoding='utf-8') as f:
    f.write(new_text)

print("Updated build_relatorio.py successfully!")

