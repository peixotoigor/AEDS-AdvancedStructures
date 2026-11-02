#!/usr/bin/env python3
"""
Gera gráficos minimalistas no padrão McKinsey separados para os 3 padrões de entrada:
1. Ordem Aleatória (aleatorio)
2. Ordem Crescente (cresce)
3. Ordem Decrescente (decresce)

E um gráfico comparativo consolidado entre os 3 padrões.
"""

import csv
import os
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

# Paleta clássica McKinsey
MCK_NAVY = "#00205B"       # Azul Marinho McKinsey (Fibonacci)
MCK_TEAL = "#00828A"       # Teal McKinsey (vEB)
MCK_DARK = "#2B2B2B"       # Texto principal
MCK_GRAY = "#6E6E6E"       # Eixos e subtítulos
MCK_GRID = "#E5E5E5"       # Grade sutil

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

NOMES_PADROES = {
    "aleatorio": "Ordem Aleatória",
    "cresce": "Ordem Crescente",
    "decresce": "Ordem Decrescente"
}

def carregar_dados(caminho_csv="results/estudo1_filas_prioridade.csv"):
    dados = []
    with open(caminho_csv, "r", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for row in reader:
            dados.append({
                "estrutura": row["estrutura"],
                "padrao": row["padrao_entrada"],
                "n": int(row["n"]),
                "u": int(row["universo_u"]),
                "rho": float(row["razao_ocupacao_rho"]),
                "operacao": row["operacao"],
                "tempo_medio_us": float(row["tempo_medio_us"]),
                "desvio_padrao_us": float(row["desvio_padrao_us"]),
                "tempo_por_op_ns": float(row["tempo_por_op_ns"]),
                "memoria_kb": int(row["memoria_kb"])
            })
    return dados

def limpar_eixos(ax):
    """Aplica o decluttering característico da McKinsey."""
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)
    ax.spines["left"].set_visible(False)
    ax.spines["bottom"].set_color(MCK_GRAY)
    ax.spines["bottom"].set_linewidth(0.8)
    ax.grid(axis="y", color=MCK_GRID, linestyle="-", linewidth=0.7)
    ax.grid(axis="x", visible=False)
    ax.tick_params(left=False)

# -----------------------------------------------------------------------------
# 1. PAINEL DAS 4 OPERAÇÕES POR PADRÃO DE ENTRADA
# -----------------------------------------------------------------------------
def gerar_painel_por_padrao(dados, padrao_chave):
    nome_padrao = NOMES_PADROES.get(padrao_chave, padrao_chave)
    fig, axes = plt.subplots(2, 2, figsize=(12, 9), facecolor="#FFFFFF")
    fig.subplots_adjust(top=0.86, hspace=0.32, wspace=0.22, bottom=0.09)

    fig.text(0.08, 0.94, f"Desempenho por Operação: Heap de Fibonacci vs. Árvore vEB ({nome_padrao})",
             fontsize=14.5, fontweight="bold", color=MCK_NAVY)
    fig.text(0.08, 0.91, f"Tempo total de execução (ms) para N = 10² a 10⁶ | Entrada em {nome_padrao.lower()} (escala logarítmica)",
             fontsize=10.5, color=MCK_GRAY)

    ops = [
        ("Inserção: insert(x)", ["insert(x)"], 0, 0),
        ("Consulta de Mínimo: findMin() / minimum()", ["findMin()", "minimum()"], 0, 1),
        ("Extração de Mínimo: extractMin() / delete(minimum())", ["extractMin()", "delete(minimum())"], 1, 0),
        ("Remoção Específica: delete(x)", ["delete(x)"], 1, 1)
    ]

    for titulo, aliases, r, c in ops:
        ax = axes[r, c]
        limpar_eixos(ax)

        sub_fib = [d for d in dados if d["estrutura"] == "HeapFibonacci" and d["padrao"] == padrao_chave and d["operacao"] in aliases]
        sub_veb = [d for d in dados if d["estrutura"] == "vanEmdeBoas" and d["padrao"] == padrao_chave and d["operacao"] in aliases]
        sub_fib.sort(key=lambda x: x["n"])
        sub_veb.sort(key=lambda x: x["n"])

        x = [d["n"] for d in sub_fib]
        y_fib = [d["tempo_medio_us"] / 1000.0 for d in sub_fib]
        y_veb = [d["tempo_medio_us"] / 1000.0 for d in sub_veb]

        ax.plot(x, y_fib, marker="o", markersize=5, color=MCK_NAVY, linewidth=1.8, label="Heap de Fibonacci")
        ax.plot(x, y_veb, marker="s", markersize=5, color=MCK_TEAL, linewidth=1.8, label="Árvore vEB")

        ax.set_xscale("log")
        ax.set_yscale("log")
        ax.set_title(titulo, loc="left", fontsize=11, fontweight="bold", color=MCK_NAVY, pad=8)
        ax.set_xlabel("Número de Elementos (N)", fontsize=9.5)
        ax.set_ylabel("Tempo (ms)", fontsize=9.5)

        ax.set_xticks([100, 1000, 10000, 100000, 1000000])
        ax.get_xaxis().set_major_formatter(ticker.FuncFormatter(lambda val, pos: f"{int(val):,}"))
        ax.legend(frameon=False, fontsize=9, loc="upper left")

    fig.text(0.08, 0.03, f"Fonte: Protocolo Experimental Estudo 1 | Padrão: {nome_padrao} | 10 repetições (g++ 13, -O3, C++20).",
             fontsize=8.5, color=MCK_GRAY)

    caminho_base = f"results/grafico_estudo1_mckinsey_4operacoes_{padrao_chave}"
    plt.savefig(f"{caminho_base}.png", dpi=300)
    plt.savefig(f"{caminho_base}.pdf")
    plt.close()
    print(f"[OK] {caminho_base}.png")

# -----------------------------------------------------------------------------
# 2. COMPARATIVO HEAD-TO-HEAD (BARRAS N = 10^6) POR PADRÃO DE ENTRADA
# -----------------------------------------------------------------------------
def gerar_head_to_head_por_padrao(dados, padrao_chave):
    nome_padrao = NOMES_PADROES.get(padrao_chave, padrao_chave)
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(11, 5.2), facecolor="#FFFFFF")
    fig.subplots_adjust(top=0.82, bottom=0.14, wspace=0.25)

    fig.text(0.08, 0.93, f"Comparativo Direto em N = 1.000.000 ({nome_padrao})",
             fontsize=14.5, fontweight="bold", color=MCK_NAVY)
    fig.text(0.08, 0.89, f"Tempo total (ms) para inserção em massa e extração de todos os mínimos ({nome_padrao.lower()})",
             fontsize=10.5, color=MCK_GRAY)

    # Inserção
    limpar_eixos(ax1)
    sub_ins = [d for d in dados if d["n"] == 1_000_000 and d["padrao"] == padrao_chave and "insert" in d["operacao"]]
    fib_ins = next(d["tempo_medio_us"] / 1000.0 for d in sub_ins if d["estrutura"] == "HeapFibonacci")
    veb_ins = next(d["tempo_medio_us"] / 1000.0 for d in sub_ins if d["estrutura"] == "vanEmdeBoas")

    max_ins = max(fib_ins, veb_ins)
    bar1 = ax1.bar(["Heap de Fibonacci", "Árvore vEB"], [fib_ins, veb_ins],
                   color=[MCK_NAVY, MCK_TEAL], width=0.48)
    ax1.set_ylabel("Tempo Total (ms)", fontsize=9.5)
    ax1.set_title("Inserção em Massa (1.000.000 chaves)", loc="left", fontsize=11, fontweight="bold", color=MCK_NAVY, pad=8)
    for b in bar1:
        y = b.get_height()
        ax1.text(b.get_x() + b.get_width()/2.0, y + (max_ins * 0.02), f"{y:.1f} ms",
                 ha="center", va="bottom", fontsize=9.5, fontweight="bold", color=MCK_DARK)
    ax1.set_ylim(0, max_ins * 1.15)

    # Extração de Mínimo
    limpar_eixos(ax2)
    sub_ext = [d for d in dados if d["n"] == 1_000_000 and d["padrao"] == padrao_chave and ("extract" in d["operacao"] or "delete(minimum())" in d["operacao"])]
    fib_ext = next(d["tempo_medio_us"] / 1000.0 for d in sub_ext if d["estrutura"] == "HeapFibonacci")
    veb_ext = next(d["tempo_medio_us"] / 1000.0 for d in sub_ext if d["estrutura"] == "vanEmdeBoas")

    max_ext = max(fib_ext, veb_ext)
    bar2 = ax2.bar(["Heap de Fibonacci", "Árvore vEB"], [fib_ext, veb_ext],
                   color=[MCK_NAVY, MCK_TEAL], width=0.48)
    ax2.set_ylabel("Tempo Total (ms)", fontsize=9.5)
    ax2.set_title("Extração de Mínimo (1.000.000 remoções)", loc="left", fontsize=11, fontweight="bold", color=MCK_NAVY, pad=8)
    for b in bar2:
        y = b.get_height()
        ax2.text(b.get_x() + b.get_width()/2.0, y + (max_ext * 0.02), f"{y:.1f} ms",
                 ha="center", va="bottom", fontsize=9.5, fontweight="bold", color=MCK_DARK)
    ax2.set_ylim(0, max_ext * 1.15)

    fig.text(0.08, 0.03, f"Fonte: Dados experimentais | Padrão: {nome_padrao} | Média de 10 execuções independentes.",
             fontsize=8.5, color=MCK_GRAY)

    caminho_base = f"results/grafico_estudo1_mckinsey_tradeoff_{padrao_chave}"
    plt.savefig(f"{caminho_base}.png", dpi=300)
    plt.savefig(f"{caminho_base}.pdf")
    plt.close()
    print(f"[OK] {caminho_base}.png")

# -----------------------------------------------------------------------------
# 3. COMPARATIVO CONSOLIDADO ENTRE OS 3 PADRÕES (N = 1.000.000)
# -----------------------------------------------------------------------------
def gerar_comparativo_3padroes(dados):
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5.5), facecolor="#FFFFFF")
    fig.subplots_adjust(top=0.82, bottom=0.14, wspace=0.25)

    fig.text(0.08, 0.93, "Sensibilidade à Ordem de Entrada em N = 1.000.000",
             fontsize=14.5, fontweight="bold", color=MCK_NAVY)
    fig.text(0.08, 0.89, "Impacto dos três padrões de dados (Crescente, Decrescente e Aleatório) nas operações estruturais",
             fontsize=10.5, color=MCK_GRAY)

    padroes_ordem = ["cresce", "decresce", "aleatorio"]
    labels_padroes = ["Crescente", "Decrescente", "Aleatório"]

    # Inserção
    limpar_eixos(ax1)
    fib_ins_vals = []
    veb_ins_vals = []
    for p in padroes_ordem:
        sub = [d for d in dados if d["n"] == 1_000_000 and d["padrao"] == p and "insert" in d["operacao"]]
        fib_ins_vals.append(next(d["tempo_medio_us"] / 1000.0 for d in sub if d["estrutura"] == "HeapFibonacci"))
        veb_ins_vals.append(next(d["tempo_medio_us"] / 1000.0 for d in sub if d["estrutura"] == "vanEmdeBoas"))

    import numpy as np
    x = np.arange(len(padroes_ordem))
    width = 0.35

    ax1.bar(x - width/2, fib_ins_vals, width, label="Heap de Fibonacci", color=MCK_NAVY)
    ax1.bar(x + width/2, veb_ins_vals, width, label="Árvore vEB", color=MCK_TEAL)
    ax1.set_ylabel("Tempo Total (ms)", fontsize=9.5)
    ax1.set_title("Inserção em Massa (1.000.000 chaves)", loc="left", fontsize=11, fontweight="bold", color=MCK_NAVY, pad=8)
    ax1.set_xticks(x)
    ax1.set_xticklabels(labels_padroes, fontsize=9.5)
    ax1.legend(frameon=False, fontsize=9)

    # Extração de Mínimo
    limpar_eixos(ax2)
    fib_ext_vals = []
    veb_ext_vals = []
    for p in padroes_ordem:
        sub = [d for d in dados if d["n"] == 1_000_000 and d["padrao"] == p and ("extract" in d["operacao"] or "delete(minimum())" in d["operacao"])]
        fib_ext_vals.append(next(d["tempo_medio_us"] / 1000.0 for d in sub if d["estrutura"] == "HeapFibonacci"))
        veb_ext_vals.append(next(d["tempo_medio_us"] / 1000.0 for d in sub if d["estrutura"] == "vanEmdeBoas"))

    ax2.bar(x - width/2, fib_ext_vals, width, label="Heap de Fibonacci", color=MCK_NAVY)
    ax2.bar(x + width/2, veb_ext_vals, width, label="Árvore vEB", color=MCK_TEAL)
    ax2.set_ylabel("Tempo Total (ms)", fontsize=9.5)
    ax2.set_title("Extração de Mínimo (1.000.000 remoções)", loc="left", fontsize=11, fontweight="bold", color=MCK_NAVY, pad=8)
    ax2.set_xticks(x)
    ax2.set_xticklabels(labels_padroes, fontsize=9.5)
    ax2.legend(frameon=False, fontsize=9)

    fig.text(0.08, 0.03, "Fonte: Dados experimentais | C++20 (-O3), 10 repetições independentes com aquecimento prévio.",
             fontsize=8.5, color=MCK_GRAY)

    caminho_base = "results/grafico_estudo1_mckinsey_comparativo_3padroes"
    plt.savefig(f"{caminho_base}.png", dpi=300)
    plt.savefig(f"{caminho_base}.pdf")
    plt.close()
    print(f"[OK] {caminho_base}.png")

def main():
    dados = carregar_dados()
    padroes = ["aleatorio", "cresce", "decresce"]
    
    print("Gerando gráficos McKinsey separados por dataset:")
    for p in padroes:
        gerar_painel_por_padrao(dados, p)
        gerar_head_to_head_por_padrao(dados, p)

    print("\nGerando gráfico comparativo consolidado entre os 3 padrões:")
    gerar_comparativo_3padroes(dados)

if __name__ == "__main__":
    main()
