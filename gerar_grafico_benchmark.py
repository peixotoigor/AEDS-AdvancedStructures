import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

# Configuração de tema e fonte legível
sns.set_theme(style="whitegrid", palette="colorblind")
plt.rcParams["font.sans-serif"] = "DejaVu Sans"

# ==============================================================================
# DADOS DO BENCHMARK (Insira ou ajuste aqui os seus resultados medidos)
# ==============================================================================

# Tamanhos de universo de teste (u = 2^k)
universos = [r'$2^8$' + '\n(256)', r'$2^{16}$' + '\n(65k)', r'$2^{24}$' + '\n(16M)', r'$2^{32}$' + '\n(4.2B)']
k_values = [8, 16, 24, 32]

# 1. Passos Recursivos no Pior Caso (Comparações de Nó)
passos_veb = [np.ceil(np.log2(k)) for k in k_values]  # O(log log u) -> ~3, 4, 5, 5
passos_avl = [k for k in k_values]                   # O(log u) -> 8, 16, 24, 32
passos_rb = [1.2 * k for k in k_values]               # O(log u) -> ~10, 19, 29, 38

# 2. Tempos de Execução Medidos em Milissegundos (Substitua pelos seus dados!)
tempo_veb = [50.46, 0.00, 33.51, 33.54]  # Exemplo em ms para 100k buscas
tempo_avl = [62.5, 15.53, 15.63, 31.25]
tempo_rb  = [31.25, 15.63, 15.63, 31.25]

# ==============================================================================
# CONSTRUÇÃO DO GRÁFICO DUAL (Passos Teóricos vs Tempo Prático)
# ==============================================================================

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

# --- GRÁFICO 1: Passos Recursivos / Comparações por Busca ---
ax1.plot(universos, passos_veb, marker='o', linewidth=2.5, label='vEB Tree - O(log log u)', color='#1f77b4')
ax1.plot(universos, passos_avl, marker='s', linewidth=2.5, linestyle='--', label='Árvore AVL - O(log n)', color='#d62728')
ax1.plot(universos, passos_rb, marker='^', linewidth=2.5, linestyle=':', label='Árvore Red-Black - O(log n)', color='#2ca02c')

ax1.set_title("Complexidade Teórica: Passos de Recursão", fontsize=13, fontweight='bold', pad=12)
ax1.set_xlabel("Tamanho do Universo (u)", fontsize=11, labelpad=8)
ax1.set_ylabel("Passos / Comparações no Pior Caso", fontsize=11)
ax1.legend(frameon=True, facecolor='white', edgecolor='none')
ax1.set_ylim(0, 45)

# Destacar valores em cada ponto do vEB
for i, txt in enumerate(passos_veb):
    ax1.annotate(f"{int(txt)} p", (universos[i], passos_veb[i] + 1.5), ha='center', fontsize=9, fontweight='bold', color='#1f77b4')

# --- GRÁFICO 2: Tempo Real de Execução (Milissegundos) ---
ax2.plot(universos, tempo_veb, marker='o', linewidth=2.5, label='vEB Tree', color='#1f77b4')
ax2.plot(universos, tempo_avl, marker='s', linewidth=2.5, linestyle='--', label='Árvore AVL', color='#d62728')
ax2.plot(universos, tempo_rb, marker='^', linewidth=2.5, linestyle=':', label='Árvore Red-Black', color='#2ca02c')

ax2.set_title("Desempenho Prático: Tempo de Consulta (ms)", fontsize=13, fontweight='bold', pad=12)
ax2.set_xlabel("Tamanho do Universo (u)", fontsize=11, labelpad=8)
ax2.set_ylabel("Tempo de Execução (ms)", fontsize=11)
ax2.legend(frameon=True, facecolor='white', edgecolor='none')

# Título Geral
fig.suptitle("vEB Tree vs. Árvores Binárias de Busca (AVL e Red-Black)", fontsize=15, fontweight='bold', y=0.98)

sns.despine()
plt.tight_layout(pad=2.0)

# Salvar figura
plt.savefig("benchmark_resultados.png", dpi=300, bbox_inches='tight')
print("Gráfico salvo com sucesso como 'benchmark_resultados.png'!")
plt.show()
