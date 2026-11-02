#!/usr/bin/env python3
"""
Gera gráficos INDIVIDUAIS no estilo McKinsey & Company para inclusão em LaTeX:
- SEM títulos nos gráficos (para uso com \caption e \label do LaTeX)
- Gráficos individuais (uma operação e um dataset por figura)
- Estilo estrito McKinsey:
  * Paleta: Navy (#00205B) para Heap de Fibonacci e Teal (#00828A) para vEB
  * Sem spines superior, direita e esquerda
  * Grade horizontal muito sutil (#E5E5E5)
  * Legenda sem moldura (frameon=False)
  * Identificação clara de eixos e grandezas
- Formatos: PDF vetorizado (para pdflatex) e PNG 300 DPI
"""

import csv
import os
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

# Paleta clássica McKinsey
MCK_NAVY = "#00205B"       # Azul Marinho McKinsey (Heap de Fibonacci)
MCK_TEAL = "#00828A"       # Teal McKinsey (Árvore vEB)
MCK_DARK = "#2B2B2B"       # Rótulos e valores
MCK_GRAY = "#555555"       # Eixos e subtítulos
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

def limpar_eixos_mckinsey(ax):
    """Aplica o decluttering rigoroso da McKinsey."""
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)
    ax.spines["left"].set_visible(False)
    ax.spines["bottom"].set_color(MCK_GRAY)
    ax.spines["bottom"].set_linewidth(0.8)
    ax.grid(axis="y", color=MCK_GRID, linestyle="-", linewidth=0.7)
    ax.grid(axis="x", visible=False)
    ax.tick_params(left=False)

def gerar_grafico_individual(dados, operacao_nome_arquivo, aliases, padrao_chave, ylabel_text="Tempo Total (ms)"):
    sub_fib = [d for d in dados if d["estrutura"] == "HeapFibonacci" and d["padrao"] == padrao_chave and d["operacao"] in aliases]
    sub_veb = [d for d in dados if d["estrutura"] == "vanEmdeBoas" and d["padrao"] == padrao_chave and d["operacao"] in aliases]
    
    if not sub_fib or not sub_veb:
        return

    sub_fib.sort(key=lambda x: x["n"])
    sub_veb.sort(key=lambda x: x["n"])

    x = [d["n"] for d in sub_fib]
    # Converter micro para milissegundos para facilitar leitura
    y_fib_ms = [d["tempo_medio_us"] / 1000.0 for d in sub_fib]
    y_veb_ms = [d["tempo_medio_us"] / 1000.0 for d in sub_veb]

    fig, ax = plt.subplots(figsize=(7, 4.8), facecolor="#FFFFFF")
    fig.subplots_adjust(top=0.92, bottom=0.15, left=0.14, right=0.94)

    limpar_eixos_mckinsey(ax)

    ax.plot(x, y_fib_ms, marker="o", markersize=6, color=MCK_NAVY, linewidth=2.0, label="Heap de Fibonacci")
    ax.plot(x, y_veb_ms, marker="s", markersize=6, color=MCK_TEAL, linewidth=2.0, label="Árvore vEB")

    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xlabel("Número de elementos ($n$)", fontsize=11, labelpad=8)
    ax.set_ylabel(ylabel_text, fontsize=11, labelpad=8)

    ax.set_xticks([100, 1000, 10000, 100000, 1000000])
    ax.get_xaxis().set_major_formatter(ticker.FuncFormatter(lambda val, pos: f"{int(val):,}"))

    # Legenda limpa no padrão McKinsey (sem caixa/moldura)
    ax.legend(frameon=False, fontsize=10, loc="upper left")

    # Nome do arquivo limpo e bem identificado
    nome_base = f"results/grafico_{operacao_nome_arquivo}_{padrao_chave}"
    plt.savefig(f"{nome_base}.pdf")
    plt.savefig(f"{nome_base}.png", dpi=300)
    plt.close()
    print(f"[OK] {nome_base}.pdf e .png")

def main():
    dados = carregar_dados()
    os.makedirs("results", exist_ok=True)

    padroes = ["aleatorio", "cresce", "decresce"]
    operacoes = [
        ("insercao", ["insert(x)"]),
        ("obter_minimo", ["findMin()", "minimum()"]),
        ("extrair_minimo", ["extractMin()", "delete(minimum())"]),
        ("remocao_especifica", ["delete(x)"])
    ]

    print("Gerando gráficos INDIVIDUAIS no padrão McKinsey (sem títulos, para LaTeX):")
    for op_nome, aliases in operacoes:
        for p in padroes:
            gerar_grafico_individual(dados, op_nome, aliases, p)

    # Gráfico individual de Custo Unitário vs Rho (sem título)
    for p in padroes:
        sub_fib = [d for d in dados if d["estrutura"] == "HeapFibonacci" and d["padrao"] == p and "extract" in d["operacao"]]
        sub_veb = [d for d in dados if d["estrutura"] == "vanEmdeBoas" and d["padrao"] == p and ("extract" in d["operacao"] or "delete(minimum())" in d["operacao"])]
        if not sub_fib or not sub_veb:
            continue
        sub_fib.sort(key=lambda x: x["rho"])
        sub_veb.sort(key=lambda x: x["rho"])

        fig, ax = plt.subplots(figsize=(7, 4.8), facecolor="#FFFFFF")
        fig.subplots_adjust(top=0.92, bottom=0.15, left=0.14, right=0.94)
        limpar_eixos_mckinsey(ax)

        ax.plot([d["rho"] for d in sub_fib], [d["tempo_por_op_ns"] for d in sub_fib],
                marker="o", markersize=6, color=MCK_NAVY, linewidth=2.0, label="Heap de Fibonacci — extractMin()")
        ax.plot([d["rho"] for d in sub_veb], [d["tempo_por_op_ns"] for d in sub_veb],
                marker="s", markersize=6, color=MCK_TEAL, linewidth=2.0, label="Árvore vEB — delete(minimum())")

        ax.set_xscale("log")
        ax.set_yscale("log")
        ax.set_xlabel("Razão de ocupação ($\\rho = n / U$)", fontsize=11, labelpad=8)
        ax.set_ylabel("Custo unitário amortizado (ns / op)", fontsize=11, labelpad=8)
        ax.legend(frameon=False, fontsize=10, loc="center left")

        nome_base = f"results/grafico_custo_unitario_rho_{p}"
        plt.savefig(f"{nome_base}.pdf")
        plt.savefig(f"{nome_base}.png", dpi=300)
        plt.close()
        print(f"[OK] {nome_base}.pdf e .png")

    print("\nTodos os gráficos individuais para LaTeX foram gerados com sucesso!")

if __name__ == "__main__":
    main()

