import subprocess

tex_template = r"""\documentclass[12pt,a4paper]{report}
\usepackage[utf8]{inputenc}
\usepackage[T1]{fontenc}
\usepackage{lmodern}
\usepackage[brazilian]{babel}
\usepackage{graphicx}
\usepackage{subcaption}
\usepackage{geometry}

\geometry{top=3.0cm, left=3.0cm, bottom=2.0cm, right=2.0cm}
\graphicspath{{../results/}{results/}}

\begin{document}

\begin{figure}[htbp]
  \centering
  \begin{subfigure}[b]{0.33\textwidth}
    \centering
    \includegraphics[width=\textwidth]{estudo1_tempo_insercao_aleatorio.pdf}
    \caption{Padrão Aleatório\\ \scriptsize(Heap lazy supera vEB em 93,9\%)}
    \label{fig:ins-aleat}
  \end{subfigure}\hfill%
  \begin{subfigure}[b]{0.33\textwidth}
    \centering
    \includegraphics[width=\textwidth]{estudo1_tempo_insercao_cresce.pdf}
    \caption{Padrão Crescente\\ \scriptsize(Clusters contíguos aceleram vEB em 89,7\%)}
    \label{fig:ins-cresc}
  \end{subfigure}\hfill%
  \begin{subfigure}[b]{0.33\textwidth}
    \centering
    \includegraphics[width=\textwidth]{estudo1_tempo_insercao_decresce.pdf}
    \caption{Padrão Decrescente\\ \scriptsize(vEB supera Heap de Fibonacci em 12,3\%)}
    \label{fig:ins-decresc}
  \end{subfigure}
  \caption{Estudo 1: Avaliação comparativa do tempo de inserção entre Heap de Fibonacci e Árvore vEB ($n \in [10^2, 10^6]$, 10 repetições com desvio padrão): (a) Padrão aleatório demonstrando a eficácia da política lazy $O(1)$ estrita do Heap de Fibonacci ($38{,}06$~ms contra $623{,}35$~ms da vEB em $n=10^6$); (b) Padrão crescente onde o encadeamento contíguo em clusters reduz o tempo da vEB em 89,70\% ($64{,}17$~ms); (c) Padrão decrescente onde a vEB supera o Heap em 12,29\% ($37{,}21$~ms vs $42{,}42$~ms) por meio da atualização direta de ponteiros mínimos em sub-árvores recursivas.}
  \label{fig:estudo1-insercao}
  \vspace{0.1cm}
  
  \raggedright\footnotesize Fonte: Elaborado pelo autor a partir dos dados experimentais (2026).
\end{figure}

\end{document}
"""

with open('test_fig.tex', 'w', encoding='utf-8') as f:
    f.write(tex_template)

res = subprocess.run(['pdflatex', '-interaction=nonstopmode', 'test_fig.tex'], capture_output=True, text=True)
print("Exit code:", res.returncode)
for line in res.stdout.splitlines():
    if "Overfull" in line or "Underfull" in line or "Warning" in line or "Output written on" in line:
        print(line)

