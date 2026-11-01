import os
import numpy as np
import matplotlib.pyplot as plt

def carregar_dados_tempo(caminho):
    dados = {}
    with open(caminho, 'r', encoding='utf-8') as f:
        for linha in f:
            linha = linha.strip()
            if not linha or linha.startswith('#'):
                continue
            partes = linha.split()
            if len(partes) < 5:
                continue
            est, op, n_str, rep_str, tempo_str = partes[0], partes[1], partes[2], partes[3], partes[4]
            n = int(n_str)
            tempo_us = float(tempo_str)
            chave = (est, op, n)
            if chave not in dados:
                dados[chave] = []
            dados[chave].append(tempo_us)
    return dados

def carregar_dados_memoria(caminho):
    dados = {}
    with open(caminho, 'r', encoding='utf-8') as f:
        for linha in f:
            linha = linha.strip()
            if not linha or linha.startswith('#'):
                continue
            partes = linha.split()
            if len(partes) < 5:
                continue
            est, n_str, rep_str, mem_teor_str, mem_proc_str = partes[0], partes[1], partes[2], partes[3], partes[4]
            n = int(n_str)
            mem_teor = float(mem_teor_str)
            mem_proc = float(mem_proc_str)
            chave = (est, n)
            if chave not in dados:
                dados[chave] = {'teorica': [], 'processo': []}
            dados[chave]['teorica'].append(mem_teor)
            dados[chave]['processo'].append(mem_proc)
    return dados

def main():
    print("Processando dados e gerando graficos cientificos...")
    os.makedirs("benchmarks/figures", exist_ok=True)
    os.makedirs("benchmarks/tables", exist_ok=True)

    dados_tempo = carregar_dados_tempo("benchmarks/data/tempo_por_entrada.txt")
    dados_mem = carregar_dados_memoria("benchmarks/data/memoria_por_entrada.txt")

    tamanhos_n = sorted(list(set(k[2] for k in dados_tempo.keys())))
    print(f"Tamanhos identificados: {tamanhos_n}")

    # Configuração estética padronizada ABNT
    plt.rcParams.update({
        'font.size': 10,
        'axes.labelsize': 11,
        'axes.titlesize': 12,
        'xtick.labelsize': 9,
        'ytick.labelsize': 9,
        'legend.fontsize': 9,
        'figure.titlesize': 13,
        'lines.linewidth': 1.8,
        'lines.markersize': 6,
        'grid.alpha': 0.4
    })

    # ---------------------------------------------------------
    # FIGURA 1: TEMPO TOTAL POR TAMANHO DE ENTRADA (LOG-LOG)
    # ---------------------------------------------------------
    fig, ax = plt.subplots(figsize=(8, 5.2), dpi=300)

    # Inserção
    y_fib_ins = [np.mean(dados_tempo.get(('fibonacci_heap', 'insert', n), [0])) / 1000.0 for n in tamanhos_n]
    err_fib_ins = [np.std(dados_tempo.get(('fibonacci_heap', 'insert', n), [0])) / 1000.0 for n in tamanhos_n]
    y_pq_ins = [np.mean(dados_tempo.get(('priority_queue', 'insert', n), [0])) / 1000.0 for n in tamanhos_n]
    err_pq_ins = [np.std(dados_tempo.get(('priority_queue', 'insert', n), [0])) / 1000.0 for n in tamanhos_n]

    # Extração
    y_fib_ext = [np.mean(dados_tempo.get(('fibonacci_heap', 'extract_min', n), [0])) / 1000.0 for n in tamanhos_n]
    err_fib_ext = [np.std(dados_tempo.get(('fibonacci_heap', 'extract_min', n), [0])) / 1000.0 for n in tamanhos_n]
    y_pq_ext = [np.mean(dados_tempo.get(('priority_queue', 'extract_min', n), [0])) / 1000.0 for n in tamanhos_n]
    err_pq_ext = [np.std(dados_tempo.get(('priority_queue', 'extract_min', n), [0])) / 1000.0 for n in tamanhos_n]

    # Decrease-Key
    y_fib_dec = [np.mean(dados_tempo.get(('fibonacci_heap', 'decrease_key', n), [0])) / 1000.0 for n in tamanhos_n]
    err_fib_dec = [np.std(dados_tempo.get(('fibonacci_heap', 'decrease_key', n), [0])) / 1000.0 for n in tamanhos_n]

    ax.errorbar(tamanhos_n, y_fib_ins, yerr=err_fib_ins, fmt='-o', color='#1f4e79', label='Fibonacci Heap: Inserção', capsize=3)
    ax.errorbar(tamanhos_n, y_pq_ins, yerr=err_pq_ins, fmt='--s', color='#2e7d32', label='std::priority_queue: Inserção', capsize=3)
    ax.errorbar(tamanhos_n, y_fib_ext, yerr=err_fib_ext, fmt='-^', color='#c00000', label='Fibonacci Heap: Extração Mínimo', capsize=3)
    ax.errorbar(tamanhos_n, y_pq_ext, yerr=err_pq_ext, fmt='--v', color='#e36c09', label='std::priority_queue: Extração Mínimo', capsize=3)
    ax.errorbar(tamanhos_n, y_fib_dec, yerr=err_fib_dec, fmt='-d', color='#7030a0', label='Fibonacci Heap: Decrease-Key (N/10)', capsize=3)

    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.set_xlabel('Tamanho da Entrada (N elementos)')
    ax.set_ylabel('Tempo Total Médio (milissegundos)')
    ax.set_title('Tempo de Processamento vs. Tamanho da Entrada (10 Repetições)')
    ax.grid(True, which="both", ls=":")
    ax.legend(loc='upper left', frameon=True)
    plt.tight_layout()

    plt.savefig('benchmarks/figures/tempo_por_entrada.pdf')
    plt.savefig('benchmarks/figures/tempo_por_entrada.png')
    plt.close()
    print("Salvo: benchmarks/figures/tempo_por_entrada.pdf (.png)")

    # ---------------------------------------------------------
    # FIGURA 2: CONSUMO DE MEMÓRIA POR TAMANHO DE ENTRADA
    # ---------------------------------------------------------
    fig, ax = plt.subplots(figsize=(8, 5.2), dpi=300)

    # Memória teórica em Megabytes (MB)
    mem_fib_mb = [(n * 48.0 + 16.0) / (1024.0 * 1024.0) for n in tamanhos_n]
    mem_pq_mb = [(n * 4.0) / (1024.0 * 1024.0) for n in tamanhos_n]

    # Memória empírica do processo (Private Bytes em MB)
    mem_fib_proc = [np.mean(dados_mem.get(('fibonacci_heap', n), {}).get('processo', [0])) / (1024.0 * 1024.0) for n in tamanhos_n]
    mem_pq_proc = [np.mean(dados_mem.get(('priority_queue', n), {}).get('processo', [0])) / (1024.0 * 1024.0) for n in tamanhos_n]

    ax.plot(tamanhos_n, mem_fib_mb, '-o', color='#1f4e79', label='Fibonacci Heap: Modelo Teórico (48 B/nó)')
    ax.plot(tamanhos_n, mem_pq_mb, '--s', color='#2e7d32', label='std::priority_queue: Modelo Teórico (4 B/chave)')
    
    # Se houver delta mensurável de processo, plota
    if max(mem_fib_proc) > 0.1:
        ax.plot(tamanhos_n, mem_fib_proc, ':^', color='#41719c', label='Fibonacci Heap: Processo (SO)')
    if max(mem_pq_proc) > 0.1:
        ax.plot(tamanhos_n, mem_pq_proc, ':v', color='#70ad47', label='std::priority_queue: Processo (SO)')

    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.set_xlabel('Tamanho da Entrada (N elementos)')
    ax.set_ylabel('Consumo de Memória (Megabytes - MB)')
    ax.set_title('Consumo de Memória vs. Tamanho da Entrada')
    ax.grid(True, which="both", ls=":")
    ax.legend(loc='upper left', frameon=True)
    plt.tight_layout()

    plt.savefig('benchmarks/figures/memoria_por_entrada.pdf')
    plt.savefig('benchmarks/figures/memoria_por_entrada.png')
    plt.close()
    print("Salvo: benchmarks/figures/memoria_por_entrada.pdf (.png)")

    # ---------------------------------------------------------
    # FIGURA 3: CUSTO UNITÁRIO AMORTIZADO (NANOSSEGUNDOS POR OPERAÇÃO)
    # ---------------------------------------------------------
    fig, ax = plt.subplots(figsize=(8, 4.8), dpi=300)

    # Custo médio por operação individual em microssegundos
    custo_unit_fib_ins = [(y_fib_ins[i] * 1000.0) / tamanhos_n[i] for i in range(len(tamanhos_n))]
    custo_unit_pq_ins = [(y_pq_ins[i] * 1000.0) / tamanhos_n[i] for i in range(len(tamanhos_n))]
    custo_unit_fib_ext = [(y_fib_ext[i] * 1000.0) / tamanhos_n[i] for i in range(len(tamanhos_n))]
    custo_unit_pq_ext = [(y_pq_ext[i] * 1000.0) / tamanhos_n[i] for i in range(len(tamanhos_n))]
    custo_unit_fib_dec = [(y_fib_dec[i] * 1000.0) / max(10, tamanhos_n[i] / 10) for i in range(len(tamanhos_n))]

    ax.plot(tamanhos_n, custo_unit_fib_ins, '-o', color='#1f4e79', label='Fibonacci Heap: Inserção Unitária [O(1)]')
    ax.plot(tamanhos_n, custo_unit_fib_dec, '-d', color='#7030a0', label='Fibonacci Heap: Decrease-Key Unitária [O(1)]')
    ax.plot(tamanhos_n, custo_unit_pq_ins, '--s', color='#2e7d32', label='std::priority_queue: Inserção Unitária [O(log n)]')
    ax.plot(tamanhos_n, custo_unit_fib_ext, '-^', color='#c00000', label='Fibonacci Heap: Extração Unitária [O(log n)]')
    ax.plot(tamanhos_n, custo_unit_pq_ext, '--v', color='#e36c09', label='std::priority_queue: Extração Unitária [O(log n)]')

    ax.set_xscale('log')
    ax.set_xlabel('Tamanho da Entrada (N elementos)')
    ax.set_ylabel('Custo Unitário Médio (µs por operação)')
    ax.set_title('Custo Amortizado Unitário por Operação')
    ax.grid(True, which="both", ls=":")
    ax.legend(loc='upper right', frameon=True)
    plt.tight_layout()

    plt.savefig('benchmarks/figures/tempo_operacoes_fibonacci.pdf')
    plt.savefig('benchmarks/figures/tempo_operacoes_fibonacci.png')
    plt.close()
    print("Salvo: benchmarks/figures/tempo_operacoes_fibonacci.pdf (.png)")

    # ---------------------------------------------------------
    # TABELA LATEX CONSOLIDADA
    # ---------------------------------------------------------
    with open('benchmarks/tables/tabela_desempenho.tex', 'w', encoding='utf-8') as f:
        f.write("% Tabela de medições experimentais (10 repetições)\n")
        f.write("\\begin{tabular}{@{}rcccccc@{}}\n")
        f.write("\\toprule\n")
        f.write("$N$ & Ins. Fib (ms) & Ins. PQ (ms) & Ext. Fib (ms) & Ext. PQ (ms) & Dec. Fib (ms) & Mem. Fib (MB) \\\\\n")
        f.write("\\midrule\n")
        for i, n in enumerate(tamanhos_n):
            f.write(f"{n:,} & ")
            f.write(f"{y_fib_ins[i]:.2f} $\\pm$ {err_fib_ins[i]:.2f} & ")
            f.write(f"{y_pq_ins[i]:.2f} $\\pm$ {err_pq_ins[i]:.2f} & ")
            f.write(f"{y_fib_ext[i]:.2f} $\\pm$ {err_fib_ext[i]:.2f} & ")
            f.write(f"{y_pq_ext[i]:.2f} $\\pm$ {err_pq_ext[i]:.2f} & ")
            f.write(f"{y_fib_dec[i]:.2f} $\\pm$ {err_fib_dec[i]:.2f} & ")
            f.write(f"{mem_fib_mb[i]:.2f} \\\\\n")
        f.write("\\bottomrule\n")
        f.write("\\end{tabular}\n")
    print("Salvo: benchmarks/tables/tabela_desempenho.tex")
    print("Concluido com sucesso!")

if __name__ == '__main__':
    main()
