#!/usr/bin/env python3
"""
Gera gráficos e tabelas para o Estudo 2 (Cenário 1 e Cenário 2):
- Gráficos individuais separados
- SEM títulos (para uso em \caption no LaTeX)
- SEM fontes
- Estilo estrito McKinsey (Navy #00205B, Teal #00828A, Coral #D84315, Gray #6E6E6E)
- Formatos PDF vetorizado e PNG 300 DPI
- Tabelas LaTeX consolidadas com médias e desvios
"""

import csv
import os
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np

# Cores McKinsey
MCK_NAVY  = "#00205B"   # AVL / Fib
MCK_CORAL = "#D84315"   # Rubro-Negra / Heaps d-ários
MCK_TEAL  = "#00828A"   # vEB
MCK_SLATE = "#37474F"   # Heaps auxiliares
MCK_DARK  = "#2B2B2B"   # Textos
MCK_GRAY  = "#555555"   # Eixos
MCK_GRID  = "#E5E5E5"   # Linhas de grade sutis

plt.rcParams.update({
    "font.family": "sans-serif",
    "font.sans-serif": ["Segoe UI", "Arial", "Helvetica", "DejaVu Sans"],
    "text.color": MCK_DARK,
    "axes.labelcolor": MCK_GRAY,
    "xtick.color": MCK_GRAY,
    "ytick.color": MCK_GRAY,
    "axes.edgecolor": MCK_GRID,
    "axes.linewidth": 0.8,
    "figure.facecolor": "#FFFFFF",
    "axes.facecolor": "#FFFFFF"
})

def limpar_eixos(ax):
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)
    ax.spines["left"].set_visible(False)
    ax.spines["bottom"].set_color(MCK_GRAY)
    ax.spines["bottom"].set_linewidth(0.8)
    ax.grid(axis="y", color=MCK_GRID, linestyle="-", linewidth=0.7)
    ax.grid(axis="x", visible=False)
    ax.tick_params(left=False)

# =============================================================================
# 1. CENÁRIO 1: ÁRVORES (AVL vs RB vs vEB)
# =============================================================================
def processar_cenario1():
    csv_file = "results/estudo2_cenario1_arvores.csv"
    if not os.path.exists(csv_file):
        return

    dados = []
    with open(csv_file, "r", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for r in reader:
            dados.append({
                "estrutura": r["estrutura"],
                "padrao": r["padrao_entrada"],
                "n": int(r["n"]),
                "operacao": r["operacao"],
                "tempo_ms": float(r["tempo_medio_us"]) / 1000.0,
                "desvio_ms": float(r["desvio_padrao_us"]) / 1000.0,
                "tempo_medio_us": float(r["tempo_medio_us"]),
                "desvio_padrao_us": float(r["desvio_padrao_us"]),
                "memoria_kb": int(r["memoria_kb"])
            })

    operacoes = [
        ("insercao", "insert"),
        ("busca", "search"),
        ("min_max", "min_max"),
        ("sucessor_predecessor", "succ_pred"),
        ("remocao", "remove")
    ]
    padroes = ["aleatorio", "cresce", "decresce"]

    # Gerar gráficos individuais
    for op_chave, op_csv in operacoes:
        for p in padroes:
            sub = [d for d in dados if d["padrao"] == p and d["operacao"] == op_csv]
            if not sub:
                continue

            fig, ax = plt.subplots(figsize=(6.5, 4.4), facecolor="#FFFFFF")
            fig.subplots_adjust(top=0.96, bottom=0.14, left=0.14, right=0.95)
            limpar_eixos(ax)

            for est, cor, mk, lbl in [
                ("AVL", MCK_NAVY, "o", "Árvore AVL"),
                ("RedBlack", MCK_CORAL, "^", "Árvore Rubro-Negra"),
                ("vanEmdeBoas", MCK_TEAL, "s", "Árvore vEB")
            ]:
                s_est = [d for d in sub if d["estrutura"] == est]
                s_est.sort(key=lambda x: x["n"])
                x_vals = [d["n"] for d in s_est]
                y_vals = [d["tempo_ms"] for d in s_est]
                ax.plot(x_vals, y_vals, marker=mk, markersize=5.5, color=cor, linewidth=1.8, label=lbl)

            ax.set_xscale("log")
            ax.set_yscale("log")
            ax.set_xlabel("Número de elementos ($n$)", fontsize=11, labelpad=7)
            ax.set_ylabel("Tempo total (ms)", fontsize=11, labelpad=7)
            ax.set_xticks([100, 1000, 10000, 100000, 1000000])
            ax.get_xaxis().set_major_formatter(ticker.FuncFormatter(lambda val, pos: f"{int(val):,}"))
            ax.legend(frameon=False, fontsize=9.5, loc="upper left")

            caminho_base = f"results/grafico_estudo2_arvores_{op_chave}_{p}"
            plt.savefig(f"{caminho_base}.pdf")
            plt.savefig(f"{caminho_base}.png", dpi=300)
            plt.close()
            print(f"[OK] {caminho_base}.pdf e .png")

    # Tabela LaTeX Cenário 1
    tex_file = "results/tabela_estudo2_cenario1.tex"
    with open(tex_file, "w", encoding="utf-8") as f:
        f.write("% Tabela gerada automaticamente para o Estudo 2 (Cenário 1: Conjuntos Ordenados)\n")
        f.write("\\begin{table}[htbp]\n")
        f.write("  \\centering\n")
        f.write("  \\caption{Tempo de execução (média $\\pm$ desvio padrão em $\\mu$s) para AVL, Rubro-Negra e vEB.}\n")
        f.write("  \\label{tab:estudo2-cenario1}\n")
        f.write("  \\small\n")
        f.write("  \\begin{tabular}{lllrrrrr}\n")
        f.write("    \\toprule\n")
        f.write("    \\textbf{Estrutura} & \\textbf{Padrão} & \\textbf{$n$} & \\textbf{Inserção} & \\textbf{Busca} & \\textbf{Mín/Máx} & \\textbf{Suc/Pred} & \\textbf{Remoção} \\\\\n")
        f.write("    \\midrule\n")

        for est in ["AVL", "RedBlack", "vanEmdeBoas"]:
            lbl_est = "AVL" if est == "AVL" else ("Rubro-Negra" if est == "RedBlack" else "vEB")
            for p in padroes:
                for n_val in [100, 1000, 10000, 100000, 1000000]:
                    s = [d for d in dados if d["estrutura"] == est and d["padrao"] == p and d["n"] == n_val]
                    if not s: continue
                    get_op = lambda op: next((d for d in s if d["operacao"] == op), None)
                    fmt = lambda d: f"{d['tempo_medio_us']:.1f} \\pm {d['desvio_padrao_us']:.1f}" if d else "-"
                    f.write(f"    {lbl_est} & {p} & {n_val} & {fmt(get_op('insert'))} & {fmt(get_op('search'))} & {fmt(get_op('min_max'))} & {fmt(get_op('succ_pred'))} & {fmt(get_op('remove'))} \\\\\n")
                f.write("    \\midrule\n")
        f.write("    \\bottomrule\n")
        f.write("  \\end{tabular}\n")
        f.write("\\end{table}\n")
    print(f"[OK] {tex_file}")

# =============================================================================
# 2. CENÁRIO 2: DIJKSTRA (Fibonacci vs Heaps d-ários)
# =============================================================================
def processar_cenario2():
    csv_file = "results/estudo2_cenario2_dijkstra.csv"
    if not os.path.exists(csv_file):
        return

    dados = []
    with open(csv_file, "r", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for r in reader:
            dados.append({
                "grafo": r["tipo_grafo"],
                "v": int(r["num_vertices"]),
                "e": int(r["num_arestas"]),
                "fila": r["fila_prioridade"],
                "tempo_ms": float(r["tempo_medio_ms"]),
                "desvio_ms": float(r["desvio_padrao_ms"]),
                "memoria_kb": int(r["memoria_kb"])
            })

    for g_tipo, g_label in [("esparso", "Grafo Esparso (V=100k, E=300k)"), ("denso", "Grafo Denso (V=5k, E=5M)")]:
        sub = [d for d in dados if d["grafo"] == g_tipo]
        if not sub:
            continue

        fig, ax = plt.subplots(figsize=(6.5, 4.2), facecolor="#FFFFFF")
        fig.subplots_adjust(top=0.94, bottom=0.18, left=0.14, right=0.95)
        limpar_eixos(ax)

        labels = [d["fila"].replace(" (d=2)", "\n(d=2)").replace(" (d=4)", "\n(d=4)").replace(" (d=8)", "\n(d=8)") for d in sub]
        tempos = [d["tempo_ms"] for d in sub]
        desvios = [d["desvio_ms"] for d in sub]
        cores = [MCK_NAVY, MCK_SLATE, MCK_CORAL, MCK_TEAL]

        bars = ax.bar(range(len(labels)), tempos, yerr=desvios, capsize=4, color=cores, width=0.55)
        ax.set_xticks(range(len(labels)))
        ax.set_xticklabels(labels, fontsize=9.5)
        ax.set_ylabel("Tempo de execução (ms)", fontsize=11, labelpad=7)

        for b in bars:
            y = b.get_height()
            ax.text(b.get_x() + b.get_width()/2.0, y + max(tempos)*0.02, f"{y:.1f} ms",
                    ha="center", va="bottom", fontsize=9.5, fontweight="bold", color=MCK_DARK)
        ax.set_ylim(0, max(tempos) * 1.25)

        caminho_base = f"results/grafico_estudo2_dijkstra_{g_tipo}"
        plt.savefig(f"{caminho_base}.pdf")
        plt.savefig(f"{caminho_base}.png", dpi=300)
        plt.close()
        print(f"[OK] {caminho_base}.pdf e .png")

    # Tabela LaTeX Cenário 2
    tex_file = "results/tabela_estudo2_cenario2.tex"
    with open(tex_file, "w", encoding="utf-8") as f:
        f.write("% Tabela gerada automaticamente para o Estudo 2 (Cenário 2: Algoritmo de Dijkstra)\n")
        f.write("\\begin{table}[htbp]\n")
        f.write("  \\centering\n")
        f.write("  \\caption{Desempenho do Algoritmo de Dijkstra com diferentes filas de prioridade (seed=42).}\n")
        f.write("  \\label{tab:estudo2-cenario2}\n")
        f.write("  \\begin{tabular}{llrrrr}\n")
        f.write("    \\toprule\n")
        f.write("    \\textbf{Tipo de Grafo} & \\textbf{Fila de Prioridade} & \\textbf{$|V|$} & \\textbf{$|E|$} & \\textbf{Tempo (ms)} & \\textbf{Memória (KB)} \\\\\n")
        f.write("    \\midrule\n")
        for g_tipo in ["esparso", "denso"]:
            sub = [d for d in dados if d["grafo"] == g_tipo]
            lbl_g = "Esparso" if g_tipo == "esparso" else "Denso"
            for d in sub:
                f.write(f"    {lbl_g} & {d['fila']} & {d['v']} & {d['e']} & {d['tempo_ms']:.2f} \\pm {d['desvio_ms']:.2f} & {d['memoria_kb']:,} \\\\\n")
            f.write("    \\midrule\n")
        f.write("    \\bottomrule\n")
        f.write("  \\end{tabular}\n")
        f.write("\\end{table}\n")
    print(f"[OK] {tex_file}")

# =============================================================================
# 3. TABELA E GRÁFICO VALGRIND MASSIF
# =============================================================================
def processar_valgrind():
    picos = {
        "vanEmdeBoas (U=2^{20})": 112.28,
        "HeapDArio (d=2, N=100k)": 19.61,
        "HeapFibonacci (N=100k)": 17.60,
        "RedBlackTree (N=100k)": 16.05,
        "AVLTree (N=100k)": 14.51,
        "Dijkstra Grafo Esparso": 28.61,
        "Dijkstra Grafo Denso": 66.28
    }

    # Gráfico de barras de memória Valgrind Massif (sem título, sem fonte)
    fig, ax = plt.subplots(figsize=(7, 4.4), facecolor="#FFFFFF")
    fig.subplots_adjust(top=0.96, bottom=0.25, left=0.14, right=0.95)
    limpar_eixos(ax)

    nomes = ["vEB", "AVL", "Rubro-Negra", "Heap Fib.", "Heap Binário"]
    vals = [112.28, 14.51, 16.05, 17.60, 19.61]
    cores = [MCK_TEAL, MCK_NAVY, MCK_CORAL, MCK_NAVY, MCK_SLATE]

    bars = ax.bar(range(len(nomes)), vals, color=cores, width=0.52)
    ax.set_xticks(range(len(nomes)))
    ax.set_xticklabels(nomes, fontsize=10)
    ax.set_ylabel("Pico de Memória Dinâmica (MB)", fontsize=11, labelpad=7)

    for b in bars:
        y = b.get_height()
        ax.text(b.get_x() + b.get_width()/2.0, y + 2, f"{y:.1f} MB",
                ha="center", va="bottom", fontsize=9.5, fontweight="bold", color=MCK_DARK)
    ax.set_ylim(0, max(vals) * 1.18)

    caminho_base = "results/grafico_estudo2_valgrind_memoria"
    plt.savefig(f"{caminho_base}.pdf")
    plt.savefig(f"{caminho_base}.png", dpi=300)
    plt.close()
    print(f"[OK] {caminho_base}.pdf e .png")

    # Tabela LaTeX Valgrind
    tex_file = "results/tabela_estudo2_valgrind_memoria.tex"
    with open(tex_file, "w", encoding="utf-8") as f:
        f.write("% Tabela gerada a partir das medicoes do Valgrind 3.22 Massif\n")
        f.write("\\begin{table}[htbp]\n")
        f.write("  \\centering\n")
        f.write("  \\caption{Pico de memória dinâmica alocada no heap aferido com Valgrind Massif.}\n")
        f.write("  \\label{tab:valgrind-massif}\n")
        f.write("  \\begin{tabular}{llr}\n")
        f.write("    \\toprule\n")
        f.write("    \\textbf{Estrutura / Algoritmo} & \\textbf{Parâmetro de Carga} & \\textbf{Pico Massif (MB)} \\\\\n")
        f.write("    \\midrule\n")
        f.write("    Árvore van Emde Boas & $U = 1.048.576, N = 100.000$ & 112,28 \\\\\n")
        f.write("    Árvore AVL & $N = 100.000$ & 14,51 \\\\\n")
        f.write("    Árvore Rubro-Negra & $N = 100.000$ & 16,05 \\\\\n")
        f.write("    Heap de Fibonacci & $N = 100.000$ & 17,60 \\\\\n")
        f.write("    Heap Binário ($d=2$) & $N = 100.000$ & 19,61 \\\\\n")
        f.write("    Dijkstra (Grafo Esparso) & $|V|=100.000, |E|=300.000$ & 28,61 \\\\\n")
        f.write("    Dijkstra (Grafo Denso) & $|V|=5.000, |E|=4.999.000$ & 66,28 \\\\\n")
        f.write("    \\bottomrule\n")
        f.write("  \\end{tabular}\n")
        f.write("\\end{table}\n")
    print(f"[OK] {tex_file}")

def main():
    print("Processando Estudo 2 (Cenário 1: Árvores)...")
    processar_cenario1()
    print("\nProcessando Estudo 2 (Cenário 2: Dijkstra)...")
    processar_cenario2()
    print("\nProcessando Memória Valgrind Massif...")
    processar_valgrind()
    print("\nProcessamento do Estudo 2 finalizado com sucesso!")

if __name__ == "__main__":
    main()

