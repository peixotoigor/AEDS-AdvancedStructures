#!/usr/bin/env python3
"""
Gera gráficos individuais no padrão puro McKinsey para inserção direta no LaTeX:
- SEM títulos (para uso exclusivo com caption e label do LaTeX)
- SEM notas de rodapé / fontes no gráfico
- Estilo estrito McKinsey:
  * Paleta clássica: Navy (#00205B) para Heap de Fibonacci e Teal (#00828A) para vEB
  * Sem bordas superior, direita e esquerda
  * Grade horizontal muito sutil (#E5E5E5)
  * Legenda sem moldura (frameon=False)
  * Eixos legíveis e escala logarítmica com formatação elegante
"""

import csv
import os
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

# Paleta clássica McKinsey
MCK_NAVY = "#00205B"       # Heap de Fibonacci
MCK_TEAL = "#00828A"       # Árvore vEB
MCK_DARK = "#2B2B2B"       # Rótulos e textos
MCK_GRAY = "#555555"       # Eixos
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
    """Aplica o decluttering estrito da McKinsey."""
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)
    ax.spines["left"].set_visible(False)
    ax.spines["bottom"].set_color(MCK_GRAY)
    ax.spines["bottom"].set_linewidth(0.8)
    ax.grid(axis="y", color=MCK_GRID, linestyle="-", linewidth=0.7)
    ax.grid(axis="x", visible=False)
    ax.tick_params(left=False)

def gerar_grafico_operacao(dados, nome_op, aliases, padrao_chave):
    sub_fib = [d for d in dados if d["estrutura"] == "HeapFibonacci" and d["padrao"] == padrao_chave and d["operacao"] in aliases]
    sub_veb = [d for d in dados if d["estrutura"] == "vanEmdeBoas" and d["padrao"] == padrao_chave and d["operacao"] in aliases]

    if not sub_fib or not sub_veb:
        return

    sub_fib.sort(key=lambda x: x["n"])
    sub_veb.sort(key=lambda x: x["n"])

    x = [d["n"] for d in sub_fib]
    y_fib_ms = [d["tempo_medio_us"] / 1000.0 for d in sub_fib]
    y_veb_ms = [d["tempo_medio_us"] / 1000.0 for d in sub_veb]

    fig, ax = plt.subplots(figsize=(6.5, 4.4), facecolor="#FFFFFF")
    # Margens balanceadas sem espaço desperdiçado no topo/rodapé
    fig.subplots_adjust(top=0.96, bottom=0.14, left=0.14, right=0.95)

    limpar_eixos_mckinsey(ax)

    ax.plot(x, y_fib_ms, marker="o", markersize=6, color=MCK_NAVY, linewidth=2.0, label="Heap de Fibonacci")
    ax.plot(x, y_veb_ms, marker="s", markersize=6, color=MCK_TEAL, linewidth=2.0, label="Árvore vEB")

    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xlabel("Número de elementos ($n$)", fontsize=11, labelpad=7)
    ax.set_ylabel("Tempo total (ms)", fontsize=11, labelpad=7)

    ax.set_xticks([100, 1000, 10000, 100000, 1000000])
    ax.get_xaxis().set_major_formatter(ticker.FuncFormatter(lambda val, pos: f"{int(val):,}"))

    # Legenda limpa no estilo McKinsey
    ax.legend(frameon=False, fontsize=10, loc="upper left")

    caminho_base = f"results/grafico_{nome_op}_{padrao_chave}"
    plt.savefig(f"{caminho_base}.pdf")
    plt.savefig(f"{caminho_base}.png", dpi=300)
    plt.close()
    print(f"[OK] {caminho_base}.pdf e .png")

def gerar_grafico_custo_rho(dados, padrao_chave):
    sub_fib = [d for d in dados if d["estrutura"] == "HeapFibonacci" and d["padrao"] == padrao_chave and "extract" in d["operacao"]]
    sub_veb = [d for d in dados if d["estrutura"] == "vanEmdeBoas" and d["padrao"] == padrao_chave and ("extract" in d["operacao"] or "delete(minimum())" in d["operacao"])]

    if not sub_fib or not sub_veb:
        return

    sub_fib.sort(key=lambda x: x["rho"])
    sub_veb.sort(key=lambda x: x["rho"])

    fig, ax = plt.subplots(figsize=(6.5, 4.4), facecolor="#FFFFFF")
    fig.subplots_adjust(top=0.96, bottom=0.14, left=0.14, right=0.95)

    limpar_eixos_mckinsey(ax)

    ax.plot([d["rho"] for d in sub_fib], [d["tempo_por_op_ns"] for d in sub_fib],
            marker="o", markersize=6, color=MCK_NAVY, linewidth=2.0, label="Heap de Fibonacci — extractMin()")
    ax.plot([d["rho"] for d in sub_veb], [d["tempo_por_op_ns"] for d in sub_veb],
            marker="s", markersize=6, color=MCK_TEAL, linewidth=2.0, label="Árvore vEB — delete(minimum())")

    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xlabel("Razão de ocupação ($\\rho = n / U$)", fontsize=11, labelpad=7)
    ax.set_ylabel("Custo unitário amortizado (ns / op)", fontsize=11, labelpad=7)
    ax.legend(frameon=False, fontsize=10, loc="center left")

    caminho_base = f"results/grafico_custo_unitario_rho_{padrao_chave}"
    plt.savefig(f"{caminho_base}.pdf")
    plt.savefig(f"{caminho_base}.png", dpi=300)
    plt.close()
    print(f"[OK] {caminho_base}.pdf e .png")

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

    print("Gerando gráficos INDIVIDUAIS no padrão puro McKinsey (sem título, sem fonte):")
    for op_nome, aliases in operacoes:
        for p in padroes:
            gerar_grafico_operacao(dados, op_nome, aliases, p)

    for p in padroes:
        gerar_grafico_custo_rho(dados, p)

    print("\nConcluído com sucesso!")

if __name__ == "__main__":
    main()

