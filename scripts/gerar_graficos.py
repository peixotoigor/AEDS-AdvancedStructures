import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

sns.set_style("whitegrid")
plt.rcParams.update({
    'font.family': 'sans-serif',
    'font.size': 11,
    'axes.labelsize': 12,
    'axes.titlesize': 13,
    'xtick.labelsize': 10,
    'ytick.labelsize': 10,
    'legend.fontsize': 10,
    'figure.titlesize': 14
})

HEAP_LABELS = {
    'binario': 'Heap Binário (d=2)',
    '4-ary': '4-ary Heap (d=4)',
    '8-ary': '8-ary Heap (d=8)',
    'fibonacci': 'Heap de Fibonacci'
}
PALETTE = sns.color_palette("Set2", 4)
OUTPUT_DIR = "results"

# -----------------------------------------------------------------------------
# 1. CARGA DOS DADOS DA VERSÃO VECTOR
# -----------------------------------------------------------------------------
df_vec = pd.read_csv(f"{OUTPUT_DIR}/execucoes.csv")
df_vec['heap_nome'] = df_vec['heap'].map(HEAP_LABELS)
df_vec['tempo_ms'] = df_vec['mediana_micro'] / 1000.0

# --- GRÁFICO 1: Dijkstra por Cenário (Vector) ---
plt.figure(figsize=(9, 5))
df1 = df_vec[df_vec['operacao'] == 'DIJKSTRA'].copy()
df1['tipo_entrada'] = df1['tipo_entrada'].map({'esparso': 'Grafo Esparso (V=100k, E=300k)', 'denso': 'Grafo Denso (V=5k, E=5M)'})
sns.barplot(data=df1, x='tipo_entrada', y='tempo_ms', hue='heap_nome', palette=PALETTE)
plt.title('Gráfico 1: Desempenho do Dijkstra (Mapeamento Direto por Vector)')
plt.xlabel('Cenário de Grafo')
plt.ylabel('Tempo Mediano (ms)')
plt.legend(title='Estrutura de Heap')
plt.tight_layout()
plt.savefig(f'{OUTPUT_DIR}/grafico1_dijkstra_cenarios.png', dpi=300)
plt.close()

# --- GRÁFICO 2: DECREASE_KEY (N = 1.000.000) ---
plt.figure(figsize=(9, 5))
df2 = df_vec[(df_vec['operacao'] == 'DECREASE_KEY') & (df_vec['n'] == 1000000)].copy()
sns.barplot(data=df2, x='tipo_entrada', y='tempo_ms', hue='heap_nome', palette=PALETTE)
plt.title('Gráfico 2: Desempenho do DECREASE_KEY em N=1.000.000 (Vector)')
plt.xlabel('Padrão da Entrada')
plt.ylabel('Tempo Mediano (ms)')
plt.legend(title='Estrutura de Heap')
plt.tight_layout()
plt.savefig(f'{OUTPUT_DIR}/grafico2_decrease_key.png', dpi=300)
plt.close()

# --- GRÁFICO 3: Escalabilidade EXTRACT_MIN ---
plt.figure(figsize=(9, 5))
df3 = df_vec[(df_vec['operacao'] == 'EXTRACT_MIN') & (df_vec['tipo_entrada'] == 'aleatoria')].copy()
sns.lineplot(data=df3, x='n', y='tempo_ms', hue='heap_nome', marker='o', linewidth=2, palette=PALETTE)
plt.xscale('log')
plt.yscale('log')
plt.title('Gráfico 3: Escalabilidade do EXTRACT_MIN (Entrada Aleatória)')
plt.xlabel('Tamanho N (Escala Logarítmica)')
plt.ylabel('Tempo Mediano em ms (Escala Logarítmica)')
plt.legend(title='Estrutura de Heap')
plt.tight_layout()
plt.savefig(f'{OUTPUT_DIR}/grafico3_extract_min_escalabilidade.png', dpi=300)
plt.close()

# --- GRÁFICO 4: INSERT (N = 1.000.000) ---
plt.figure(figsize=(9, 5))
df4 = df_vec[(df_vec['operacao'] == 'INSERT') & (df_vec['n'] == 1000000)].copy()
sns.barplot(data=df4, x='tipo_entrada', y='tempo_ms', hue='heap_nome', palette=PALETTE)
plt.title('Gráfico 4: Tempo de Inserção de 1.000.000 Elementos (Vector)')
plt.xlabel('Ordenação da Carga de Entrada')
plt.ylabel('Tempo Mediano (ms)')
plt.legend(title='Estrutura de Heap')
plt.tight_layout()
plt.savefig(f'{OUTPUT_DIR}/grafico4_insert_padrao_entrada.png', dpi=300)
plt.close()

# --- GRÁFICO 5: Impacto da Aridade d ---
plt.figure(figsize=(8, 5))
df5 = df_vec[(df_vec['operacao'] == 'DIJKSTRA') & (df_vec['heap'].isin(['binario', '4-ary', '8-ary']))].copy()
df5['aridade'] = df5['heap'].map({'binario': 2, '4-ary': 4, '8-ary': 8})
df5['tipo_entrada'] = df5['tipo_entrada'].map({'esparso': 'Esparso (V=100k)', 'denso': 'Denso (V=5k)'})
sns.lineplot(data=df5, x='aridade', y='tempo_ms', hue='tipo_entrada', marker='s', markersize=8, linewidth=2.5, palette=['#2b5c8f', '#d95f02'])
plt.xticks([2, 4, 8], labels=['d=2', 'd=4', 'd=8'])
plt.title('Gráfico 5: Impacto da Aridade d no Dijkstra (Vector)')
plt.xlabel('Grau de Ramificação (d)')
plt.ylabel('Tempo Mediano (ms)')
plt.legend(title='Cenário')
plt.tight_layout()
plt.savefig(f'{OUTPUT_DIR}/grafico5_efeito_aridade.png', dpi=300)
plt.close()

# --- GRÁFICO 6: Ocupação Real de Memória por Nó (Bytes/Nó) ---
plt.figure(figsize=(9, 5))
df6 = df_vec[(df_vec['operacao'] == 'INSERT') & (df_vec['n'] == 1000000)].copy()
sns.barplot(data=df6, x='tipo_entrada', y='bytes_por_no', hue='heap_nome', palette=PALETTE)
plt.title('Gráfico 6: Pegada de Memória por Nó em N=1.000.000 (Vector)')
plt.xlabel('Padrão da Carga de Entrada')
plt.ylabel('Ocupação Física (Bytes / Nó)')
plt.legend(title='Estrutura de Heap')
plt.tight_layout()
plt.savefig(f'{OUTPUT_DIR}/grafico6_consumo_memoria.png', dpi=300)
plt.close()

# -----------------------------------------------------------------------------
# 2. GRÁFICO 7: COMPARATIVO DIRETO VECTOR VS UNORDERED MAP
# -----------------------------------------------------------------------------
path_map = f"{OUTPUT_DIR}/execucoes_heapmap.csv"

if os.path.exists(path_map):
    df_map = pd.read_csv(path_map)
    
    df_vec['implementacao'] = 'std::vector (Contíguo)'
    df_map['implementacao'] = 'std::unordered_map (Hash)'
    
    df_comp = pd.concat([df_vec, df_map], ignore_index=True, sort=False)
    df_comp['tempo_ms'] = df_comp['mediana_micro'] / 1000.0
    df_comp['heap_nome'] = df_comp['heap'].map(HEAP_LABELS)
    
    # Filtra apenas o Grafo Esparso para o comparativo sem misturar com o denso
    df_comp_esparso = df_comp[(df_comp['operacao'] == 'DIJKSTRA') & (df_comp['tipo_entrada'] == 'esparso')].copy()

    plt.figure(figsize=(10, 6))
    sns.barplot(
        data=df_comp_esparso,
        x='heap_nome',
        y='tempo_ms',
        hue='implementacao',
        palette=['#2ca02c', '#d62728'],
        ci=None  
    )
    plt.title('Gráfico 7: Impacto do Mapeamento no Dijkstra (Grafo Esparso - V=100k)')
    plt.xlabel('Estrutura de Heap')
    plt.ylabel('Tempo Mediano de Execução (ms)')
    plt.legend(title='Mapeamento de Posições')
    plt.xticks(rotation=10)
    plt.tight_layout()
    plt.savefig(f'{OUTPUT_DIR}/grafico7_comparativo_vector_vs_map.png', dpi=300)
    plt.close()
    
    print("Sucesso: 7 gráficos gerados com sucesso na pasta 'results/'.")
else:
    print("Aviso: 'results/execucoes_heapmap.csv' não encontrado. Foram gerados apenas os gráficos 1 a 6.")