#!/usr/bin/env python3
import csv
import math
import os
import matplotlib.pyplot as plt

def main():
    csv_file = "results/estudo1_filas_prioridade.csv"
    if not os.path.exists(csv_file):
        print(f"[ERRO] Arquivo {csv_file} nao encontrado. Execute o benchmark primeiro.")
        return

    data = []
    with open(csv_file, mode="r", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for row in reader:
            data.append({
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

    os.makedirs("results", exist_ok=True)

    # 1. Gerar Tabela LaTeX consolidada
    tex_file = "results/tabela_estudo1.tex"
    with open(tex_file, "w", encoding="utf-8") as f:
        f.write("% Tabela gerada automaticamente pelo benchmark do Estudo 1\n")
        f.write("\\begin{table}[htbp]\n")
        f.write("  \\centering\n")
        f.write("  \\caption{Desempenho comparativo (média $\\pm$ desvio padrão em $\\mu$s) entre Heap de Fibonacci e Árvore vEB.}\n")
        f.write("  \\label{tab:resultados-estudo1}\n")
        f.write("  \\small\n")
        f.write("  \\begin{tabular}{lllrrrr}\n")
        f.write("    \\toprule\n")
        f.write("    \\textbf{Estrutura} & \\textbf{Padrão} & \\textbf{$n$} & \\textbf{Inserção ($\\mu$s)} & \\textbf{Mínimo ($\\mu$s)} & \\textbf{Extrair Mín ($\\mu$s)} & \\textbf{Remover ($\\mu$s)} \\\\\n")
        f.write("    \\midrule\n")

        estruturas = ["HeapFibonacci", "vanEmdeBoas"]
        padroes = ["aleatorio", "cresce", "decresce"]
        ns = [100, 1000, 10000, 100000, 1000000]

        for est in estruturas:
            for p in padroes:
                for n_val in ns:
                    # Encontrar operações
                    sub = [r for r in data if r["estrutura"] == est and r["padrao"] == p and r["n"] == n_val]
                    if not sub:
                        continue
                    ins = next((r for r in sub if "insert" in r["operacao"]), None)
                    fmin = next((r for r in sub if "minimum" in r["operacao"] or "findMin" in r["operacao"]), None)
                    ext = next((r for r in sub if "extract" in r["operacao"] or "delete(minimum())" in r["operacao"]), None)
                    dele = next((r for r in sub if r["operacao"] == "delete(x)"), None)

                    def fmt(reg):
                        if not reg: return "-"
                        return f"{reg['tempo_medio_us']:.1f} \\pm {reg['desvio_padrao_us']:.1f}"

                    nome_est = "Heap Fib." if est == "HeapFibonacci" else "vEB"
                    f.write(f"    {nome_est} & {p} & {n_val} & {fmt(ins)} & {fmt(fmin)} & {fmt(ext)} & {fmt(dele)} \\\\\n")
                f.write("    \\midrule\n")

        f.write("    \\bottomrule\n")
        f.write("  \\end{tabular}\n")
        f.write("\\end{table}\n")

    print(f"[OK] Tabela LaTeX gerada com sucesso em: {tex_file}")

    # 2. Gerar Gráficos com Matplotlib
    # Configuração de estilo visual
    plt.rcParams.update({'font.size': 11, 'figure.autolayout': True})
    cores = {"HeapFibonacci": "#1f77b4", "vanEmdeBoas": "#ff7f0e"}
    marcadores = {"HeapFibonacci": "o", "vanEmdeBoas": "s"}

    # Gráfico A: Comparação de tempo total por operação no padrão aleatório (Escala Log-Log)
    operacoes_map = [
        ("Inserção", ["insert(x)"]),
        ("Obter Mínimo", ["findMin()", "minimum()"]),
        ("Remover Mínimo", ["extractMin()", "delete(minimum())"]),
        ("Remover Específico", ["delete(x)"])
    ]

    fig, axes = plt.subplots(2, 2, figsize=(12, 10))
    axes = axes.flatten()

    for idx, (titulo_op, aliases) in enumerate(operacoes_map):
        ax = axes[idx]
        for est in ["HeapFibonacci", "vanEmdeBoas"]:
            sub = [r for r in data if r["estrutura"] == est and r["padrao"] == "aleatorio" and r["operacao"] in aliases]
            sub.sort(key=lambda x: x["n"])
            x_vals = [r["n"] for r in sub]
            y_vals = [r["tempo_medio_us"] for r in sub]
            y_err = [r["desvio_padrao_us"] for r in sub]

            label = "Heap de Fibonacci" if est == "HeapFibonacci" else "Árvore vEB"
            ax.errorbar(x_vals, y_vals, yerr=y_err, label=label,
                         color=cores[est], marker=marcadores[est], linewidth=2, capsize=4)

        ax.set_xscale("log")
        ax.set_yscale("log")
        ax.set_xlabel("Número de elementos (n)")
        ax.set_ylabel("Tempo Total (μs)")
        ax.set_title(f"Operação: {titulo_op} (Padrão Aleatório)")
        ax.grid(True, which="both", ls="--", alpha=0.5)
        ax.legend()

    plt.savefig("results/grafico_estudo1_4operacoes.png", dpi=300)
    plt.savefig("results/grafico_estudo1_4operacoes.pdf")
    plt.close()
    print("[OK] Gráfico das 4 operações gerado: results/grafico_estudo1_4operacoes.png")

    # Gráfico B: Custo por operação (ns/op) vs Densidade rho (n/U)
    plt.figure(figsize=(9, 6))
    for est in ["HeapFibonacci", "vanEmdeBoas"]:
        sub_ext = [r for r in data if r["estrutura"] == est and r["padrao"] == "aleatorio" and ("extract" in r["operacao"] or "delete(minimum())" in r["operacao"])]
        sub_ext.sort(key=lambda x: x["rho"])
        x_rho = [r["rho"] for r in sub_ext]
        y_ns = [r["tempo_por_op_ns"] for r in sub_ext]

        label = "Heap Fib - extractMin()" if est == "HeapFibonacci" else "vEB - delete(minimum())"
        plt.plot(x_rho, y_ns, label=label, color=cores[est], marker=marcadores[est], linewidth=2)

    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("Razão de Ocupação ρ = n / U (U = 1.048.576)")
    plt.ylabel("Tempo médio por operação (ns/op)")
    plt.title("Efeito da Razão de Ocupação ρ no Custo Unitário de Remoção do Mínimo")
    plt.grid(True, which="both", ls="--", alpha=0.5)
    plt.legend()
    plt.savefig("results/grafico_estudo1_escalabilidade_rho.png", dpi=300)
    plt.savefig("results/grafico_estudo1_escalabilidade_rho.pdf")
    plt.close()
    print("[OK] Gráfico de escalabilidade rho gerado: results/grafico_estudo1_escalabilidade_rho.png")

if __name__ == "__main__":
    main()

