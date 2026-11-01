import os
import sys
import textwrap

import numpy as np
import pandas as pd
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import Patch
from matplotlib.ticker import FuncFormatter

OUTPUT_DIR = sys.argv[1] if len(sys.argv) > 1 else "results"
DPI = 300
LARGURA = 6.3

# ---------------------------------------------------------------- estilo -----
COR_TEXTO = "#1F2933"
COR_SUB = "#5F6B7A"
COR_EIXO = "#9AA5B1"
COR_GRADE = "#E4E7EB"
COR_DESTAQUE = "#2251FF"

HEAPS = ["binario", "4-ary", "8-ary", "fibonacci"]
NOME = {"binario": "Binário (d=2)", "4-ary": "4-ário (d=4)",
        "8-ary": "8-ário (d=8)", "fibonacci": "Fibonacci"}
NOME_CURTO = {"binario": "Binário", "4-ary": "4-ário",
              "8-ary": "8-ário", "fibonacci": "Fibonacci"}
COR = {"binario": "#C3C9D0", "4-ary": "#98A2AD",
       "8-ary": "#6B7682", "fibonacci": COR_DESTAQUE}
DARIOS = ["binario", "4-ary", "8-ary"]
TIPOS = ["crescente", "aleatoria", "decrescente"]
TIPO_NOME = {"crescente": "Crescente", "aleatoria": "Aleatória",
             "decrescente": "Decrescente"}

FONTE_VEC = "Fonte: experimentos do grupo (repositório AEDS-AdvancedStructures), implementação com std::vector"
FONTE_AMBAS = "Fonte: experimentos do grupo (repositório AEDS-AdvancedStructures), duas versões da tabela de posições"

plt.rcParams.update({
    "font.family": "sans-serif",
    "font.sans-serif": ["Arial", "Helvetica", "Liberation Sans", "DejaVu Sans"],
    "font.size": 8,
    "text.color": COR_TEXTO,
    "axes.labelcolor": COR_SUB,
    "xtick.color": COR_TEXTO,
    "ytick.color": COR_SUB,
    "axes.unicode_minus": False,
    "savefig.facecolor": "white",
})


# ------------------------------------------------------------- utilitários ---
def br(x, casas=1):
    """Formata número no padrão brasileiro: 1.963,9"""
    s = f"{x:,.{casas}f}"
    return s.replace(",", "§").replace(".", ",").replace("§", ".")


def vezes(x):
    return br(x, 1) + "×"


def rotulo_ms(v):
    return br(v, 0) if v >= 100 else br(v, 1)


def ms(df, heap, operacao, tipo=None, n=None):
    """Mediana (ms) de uma linha do CSV; falha se a linha não for única."""
    q = (df["heap"] == heap) & (df["operacao"] == operacao)
    if tipo is not None:
        q &= df["tipo_entrada"] == tipo
    if n is not None:
        q &= df["n"] == n
    sel = df[q]
    if len(sel) != 1:
        raise ValueError(f"esperava 1 linha para {heap}/{operacao}/{tipo}/{n}, achei {len(sel)}")
    return float(sel.iloc[0]["mediana_micro"]) / 1000.0


def montar_moldura(titulo, subtitulo, fonte, nota=None, altura=4.2,
                   esq_ax=0.35, dir_ax=0.25, topo_extra=0.42):
    """Cria a figura com título de ação, subtítulo, rodapé e o eixo principal."""
    linhas = textwrap.wrap(titulo, 78)
    fig = plt.figure(figsize=(LARGURA, altura))
    x0 = 0.35 / LARGURA
    y = 0.16
    for linha in linhas:
        fig.text(x0, 1 - y / altura, linha, ha="left", va="top",
                 fontsize=10.2, fontweight="bold", color=COR_TEXTO)
        y += 0.19
    fig.text(x0, 1 - (y + 0.02) / altura, subtitulo, ha="left", va="top",
             fontsize=7.6, color=COR_SUB)
    y += 0.26
    topo = y + topo_extra
    rodape = 0.12
    fig.text(x0, rodape / altura, fonte, ha="left", va="bottom",
             fontsize=6.0, color=COR_SUB)
    base = rodape + 0.14 + 0.42
    if nota:
        fig.text(x0, (rodape + 0.14) / altura, nota, ha="left", va="bottom",
                 fontsize=6.0, color=COR_SUB)
        base += 0.14
    ax = fig.add_axes([esq_ax / LARGURA, base / altura,
                       (LARGURA - esq_ax - dir_ax) / LARGURA,
                       (altura - topo - base) / altura])
    ax.patch.set_alpha(0)
    return fig, ax


def estilo_barras(ax):
    for s in ("top", "right", "left"):
        ax.spines[s].set_visible(False)
    ax.spines["bottom"].set_color(COR_EIXO)
    ax.spines["bottom"].set_linewidth(0.8)
    ax.tick_params(axis="y", left=False, labelleft=False)
    ax.tick_params(axis="x", length=0, labelsize=7.6, pad=4)


def estilo_linhas(ax):
    for s in ("top", "right", "left"):
        ax.spines[s].set_visible(False)
    ax.spines["bottom"].set_color(COR_EIXO)
    ax.spines["bottom"].set_linewidth(0.8)
    ax.yaxis.grid(True, color=COR_GRADE, linewidth=0.6)
    ax.set_axisbelow(True)
    ax.tick_params(axis="both", length=0, labelsize=7)


def barras_agrupadas(ax, categorias, series, valores, cores, destacar=None,
                     fmt=rotulo_ms, folga=1.16):
    """valores[serie] = lista com um valor por categoria."""
    k = len(series)
    largura_total = 0.82
    w = largura_total / k
    ymax = max(max(valores[s]) for s in series)
    pad = ymax * 0.012
    for j, s in enumerate(series):
        xs = np.arange(len(categorias)) - largura_total / 2 + w * (j + 0.5)
        ax.bar(xs, valores[s], width=w * 0.92, color=cores[s], linewidth=0)
        for x, v in zip(xs, valores[s]):
            ax.text(x, v + pad, fmt(v), ha="center", va="bottom", fontsize=6.2,
                    color=COR_DESTAQUE if s == destacar else COR_TEXTO,
                    fontweight="bold" if s == destacar else "normal")
    ax.set_xticks(range(len(categorias)))
    ax.set_xticklabels(categorias)
    ax.set_ylim(0, ymax * folga)
    ax.set_xlim(-0.55, len(categorias) - 0.45)


def legenda_topo(ax, series, nomes, cores):
    handles = [Patch(facecolor=cores[s], label=nomes[s]) for s in series]
    ax.legend(handles=handles, loc="lower left", bbox_to_anchor=(0, 1.04),
              ncol=len(series), frameon=False, fontsize=7, handlelength=0.9,
              handleheight=0.9, columnspacing=1.3, borderaxespad=0)


def rotular_final(ax, itens, fontsize=7.2, gap=8.5):
    """Rótulos diretos à direita das linhas, sem sobreposição.
    itens: lista de (texto, x, y, cor, negrito)."""
    fig = ax.figure
    fig.canvas.draw()
    pts = []
    for t, x, y, c, b in itens:
        py = ax.transData.transform((x, y))[1] * 72.0 / fig.dpi
        pts.append([py, py, t, x, y, c, b])
    pts.sort(key=lambda p: p[0])
    for i in range(1, len(pts)):
        if pts[i][0] - pts[i - 1][0] < gap:
            pts[i][0] = pts[i - 1][0] + gap
    for ypt, y0, t, x, y, c, b in pts:
        ax.annotate(t, (x, y), xytext=(7, ypt - y0), textcoords="offset points",
                    va="center", ha="left", fontsize=fontsize, color=c,
                    fontweight="bold" if b else "normal", annotation_clip=False)


def salvar(fig, nome):
    fig.savefig(os.path.join(OUTPUT_DIR, nome), dpi=DPI)
    plt.close(fig)


# ---------------------------------------------------------------- gráficos ---
def grafico1(df):
    esp = df[(df["operacao"] == "DIJKSTRA") & (df["tipo_entrada"] == "esparso")]
    den = df[(df["operacao"] == "DIJKSTRA") & (df["tipo_entrada"] == "denso")]
    cenarios = [("esparso", esp), ("denso", den)]
    valores = {h: [ms(df, h, "DIJKSTRA", t) for t, _ in cenarios] for h in HEAPS}
    r = []
    for i in range(2):
        melhor = min(valores[h][i] for h in DARIOS)
        r.append(valores["fibonacci"][i] / melhor)
    mais_lento = all(valores["fibonacci"][i] > max(valores[h][i] for h in DARIOS) for i in range(2))
    if mais_lento:
        titulo = (f"No Dijkstra, o heap de Fibonacci foi o mais lento nos dois grafos: "
                  f"{vezes(r[0])} o tempo do melhor d-ário no esparso e {vezes(r[1])} no denso")
    else:
        titulo = (f"No Dijkstra, o heap de Fibonacci levou {vezes(r[0])} o tempo do melhor d-ário "
                  f"no grafo esparso e {vezes(r[1])} no denso")
    cats = []
    for t, sub in cenarios:
        v, e = int(sub.iloc[0]["n"]), int(sub.iloc[0]["m"])
        cats.append(f"Grafo {t}\nV = {br(v, 0)}; E = {br(e, 0)}")
    fig, ax = montar_moldura(
        titulo, "Tempo mediano do algoritmo de Dijkstra, em ms",
        FONTE_VEC, nota="Nota: grafos direcionados, pesos de 1 a 100; 10 repetições após uma de aquecimento.")
    barras_agrupadas(ax, cats, HEAPS, valores, COR, destacar="fibonacci")
    estilo_barras(ax)
    legenda_topo(ax, HEAPS, NOME, COR)
    salvar(fig, "grafico1_dijkstra_cenarios.png")
    return titulo


def grafico2(df, n=1_000_000):
    valores = {h: [ms(df, h, "DECREASE_KEY", t, n) for t in TIPOS] for h in HEAPS}
    razoes = []
    for i in range(3):
        melhor = min(valores[h][i] for h in DARIOS)
        razoes.append(valores["fibonacci"][i] / melhor)
    titulo = (f"Em reduções de chave com $n = 10^6$, o Fibonacci levou de {vezes(min(razoes))} "
              f"a {vezes(max(razoes))} o tempo do melhor d-ário, nas três ordens de entrada")
    fig, ax = montar_moldura(
    titulo, "Tempo mediano de $10^6$ reduções de chave ($n = 10^6$), em ms",
        FONTE_VEC, nota="Nota: ordem da entrada = ordem das chaves inseridas antes das reduções.")
    barras_agrupadas(ax, [TIPO_NOME[t] for t in TIPOS], HEAPS, valores, COR, destacar="fibonacci")
    estilo_barras(ax)
    legenda_topo(ax, HEAPS, NOME, COR)
    salvar(fig, "grafico2_decrease_key.png")
    return titulo


def grafico3(df):
    sel = df[(df["operacao"] == "EXTRACT_MIN") & (df["tipo_entrada"] == "aleatoria")]
    ns = sorted(sel["n"].unique())
    nmax = ns[-1]
    fib = ms(df, "fibonacci", "EXTRACT_MIN", "aleatoria", nmax)
    bi = ms(df, "binario", "EXTRACT_MIN", "aleatoria", nmax)
    titulo = (f"Na extração do mínimo, o Fibonacci é {vezes(fib / bi)} mais lento que o binário "
              f"com n = {br(nmax, 0)}, apesar do mesmo custo O(log n)")
    fig, ax = montar_moldura(
        titulo, "Tempo mediano para esvaziar o heap (entrada aleatória), em ms; escala logarítmica nos dois eixos",
        FONTE_VEC, dir_ax=1.15, esq_ax=0.7)
    itens = []
    for h in HEAPS:
        ys = [ms(df, h, "EXTRACT_MIN", "aleatoria", n) for n in ns]
        ax.plot(ns, ys, color=COR[h], linewidth=2.2 if h == "fibonacci" else 1.4,
                marker="o", markersize=3.2, zorder=3 if h == "fibonacci" else 2)
        itens.append((NOME_CURTO[h], ns[-1], ys[-1], COR[h] if h == "fibonacci" else COR_SUB,
                      h == "fibonacci"))
    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xticks(ns)
    ax.xaxis.set_major_formatter(FuncFormatter(lambda v, _: br(v, 0)))
    ax.yaxis.set_major_formatter(FuncFormatter(lambda v, _: br(v, 0) if v >= 1 else br(v, 2 if v < 0.1 else 1)))
    ax.minorticks_off()
    ax.set_xlim(ns[0] * 0.8, ns[-1] * 1.25)
    estilo_linhas(ax)
    ax.set_xlabel("Número de elementos (n)", fontsize=7, labelpad=4)
    rotular_final(ax, itens)
    salvar(fig, "grafico3_extract_min_escalabilidade.png")
    return titulo


def grafico4(df, n=1_000_000):
    valores = {h: [ms(df, h, "INSERT", t, n) for t in TIPOS] for h in HEAPS}
    f, b = valores["fibonacci"], valores["binario"]
    if max(f) / min(f) < max(b) / min(b):
        titulo = (f"A inserção do Fibonacci varia pouco com a ordem da entrada ({rotulo_ms(min(f))} a "
                  f"{rotulo_ms(max(f))} ms); a do binário varia de {rotulo_ms(min(b))} a {rotulo_ms(max(b))} ms")
    else:
        titulo = (f"Inserção com $n = 10^6$: Fibonacci de {rotulo_ms(min(f))} a {rotulo_ms(max(f))} ms; "
                  f"binário de {rotulo_ms(min(b))} a {rotulo_ms(max(b))} ms")
    fig, ax = montar_moldura(
    titulo, "Tempo mediano para inserir $10^6$ elementos ($n = 10^6$), em ms",
    FONTE_VEC, nota="Nota: a destruição do heap não entra na medição.")
    barras_agrupadas(ax, [TIPO_NOME[t] for t in TIPOS], HEAPS, valores, COR, destacar="fibonacci")
    estilo_barras(ax)
    legenda_topo(ax, HEAPS, NOME, COR)
    salvar(fig, "grafico4_insert_padrao_entrada.png")
    return titulo


def grafico5(df):
    aridades = [("binario", 2), ("4-ary", 4), ("8-ary", 8)]
    cen = [("esparso", "Grafo esparso", "#6B7682"), ("denso", "Grafo denso", COR_DESTAQUE)]
    dados = {}
    for t, _, _ in cen:
        dados[t] = [ms(df, h, "DIJKSTRA", t) for h, _ in aridades]
    red = {t: (dados[t][0] - dados[t][-1]) / dados[t][0] * 100 for t in dados}
    if all(v > 0 for v in red.values()):
        titulo = (f"Subir a aridade de 2 para 8 reduz o tempo do Dijkstra em apenas "
                  f"{br(red['esparso'], 0)}% (esparso) e {br(red['denso'], 0)}% (denso)")
    else:
        titulo = "Efeito da aridade d no tempo do Dijkstra"
    fig, ax = montar_moldura(
        titulo, "Tempo mediano do Dijkstra com heaps d-ários, em ms",
        FONTE_VEC, dir_ax=1.1, esq_ax=0.6)
    itens = []
    ymax = max(max(v) for v in dados.values())
    for t, nome, cor in cen:
        xs = [d for _, d in aridades]
        ax.plot(xs, dados[t], color=cor, linewidth=2.0, marker="o", markersize=4)
        for x, y in zip(xs, dados[t]):
            ax.text(x, y + ymax * 0.035, br(y, 1), ha="center", va="bottom", fontsize=6.5, color=cor)
        itens.append((nome, xs[-1], dados[t][-1], cor, False))
    ax.set_xticks([2, 4, 8])
    ax.set_xticklabels(["d = 2", "d = 4", "d = 8"])
    ax.set_xlim(1.5, 8.6)
    ax.set_ylim(0, ymax * 1.25)
    ax.yaxis.set_major_formatter(FuncFormatter(lambda v, _: br(v, 0)))
    estilo_linhas(ax)
    ax.tick_params(axis="x", labelsize=7.6, labelcolor=COR_TEXTO)
    rotular_final(ax, itens)
    salvar(fig, "grafico5_efeito_aridade.png")
    return titulo


def grafico6(df, n=1_000_000):
    sel = df[(df["operacao"] == "INSERT") & (df["n"] == n)]
    media = {h: float(sel[sel["heap"] == h]["bytes_por_no"].mean()) for h in HEAPS}
    var = max(float(sel[sel["heap"] == h]["bytes_por_no"].max() - sel[sel["heap"] == h]["bytes_por_no"].min())
              / media[h] for h in HEAPS)
    d_med = float(np.mean([media[h] for h in DARIOS]))
    titulo = (f"O heap de Fibonacci ocupa {vezes(media['fibonacci'] / d_med)} mais memória por nó "
              f"que os heaps d-ários ({br(media['fibonacci'], 0)} contra {br(d_med, 1)} bytes)")
    nota = ("Nota: valores iguais nas três ordens de entrada (variação < 1%); mede capacidade alocada (mallinfo), "
            "não tamanho lógico." if var < 0.01 else
            "Nota: média das três ordens de entrada; mede capacidade alocada (mallinfo), não tamanho lógico.")
    fig, ax = montar_moldura(
        titulo, "Memória alocada por nó, em bytes ($n = 10^6$)", FONTE_VEC, nota=nota)
    xs = np.arange(len(HEAPS))
    ax.bar(xs, [media[h] for h in HEAPS], width=0.55, color=[COR[h] for h in HEAPS], linewidth=0)
    ymax = max(media.values())
    for x, h in zip(xs, HEAPS):
        ax.text(x, media[h] + ymax * 0.015, br(media[h], 1), ha="center", va="bottom", fontsize=8,
                fontweight="bold" if h == "fibonacci" else "normal",
                color=COR_DESTAQUE if h == "fibonacci" else COR_TEXTO)
    ax.set_xticks(xs)
    ax.set_xticklabels([NOME[h] for h in HEAPS])
    ax.set_ylim(0, ymax * 1.15)
    estilo_barras(ax)
    salvar(fig, "grafico6_consumo_memoria.png")
    return titulo


def grafico7(df_vec, df_map):
    cor_impl = {"map": "#B8BFC7", "vec": COR_DESTAQUE}
    nome_impl = {"map": "std::unordered_map", "vec": "std::vector"}
    v = {h: ms(df_vec, h, "DIJKSTRA", "esparso") for h in HEAPS}
    m = {h: ms(df_map, h, "DIJKSTRA", "esparso") for h in HEAPS}
    ganho = {h: m[h] / v[h] for h in HEAPS}
    gd = [ganho[h] for h in DARIOS]
    titulo = (f"Trocar unordered_map por vector acelerou os heaps d-ários em {vezes(min(gd))} a {vezes(max(gd))} "
              f"e o de Fibonacci em apenas {vezes(ganho['fibonacci'])}")
    fig, ax = montar_moldura(
        titulo, "Tempo mediano do Dijkstra no grafo esparso (V = 100.000; E = 300.000), em ms",
        FONTE_AMBAS, nota="Nota: o ganho indicado acima de cada grupo é o tempo com unordered_map dividido pelo tempo com vector.")
    cats = [NOME[h] for h in HEAPS]
    valores = {"map": [m[h] for h in HEAPS], "vec": [v[h] for h in HEAPS]}
    barras_agrupadas(ax, cats, ["map", "vec"], valores, cor_impl, destacar="vec", folga=1.28)
    for i, h in enumerate(HEAPS):
        topo = max(m[h], v[h])
        ax.text(i, topo * 1.11, "ganho " + vezes(ganho[h]), ha="center", va="bottom",
                fontsize=6.8, color=COR_TEXTO, fontweight="bold")
    estilo_barras(ax)
    legenda_topo(ax, ["map", "vec"], nome_impl, cor_impl)
    salvar(fig, "grafico7_comparativo_vector_vs_map.png")
    return titulo


# -------------------------------------------------------------------- main ---
def main():
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    caminho_vec = os.path.join(OUTPUT_DIR, "execucoes.csv")
    caminho_map = os.path.join(OUTPUT_DIR, "execucoes_heapmap.csv")
    df = pd.read_csv(caminho_vec)

    titulos = []
    for fn in (grafico1, grafico2, grafico3, grafico4, grafico5, grafico6):
        titulos.append(fn(df))

    if os.path.exists(caminho_map):
        titulos.append(grafico7(df, pd.read_csv(caminho_map)))
        print("Sucesso: 7 gráficos gerados em", OUTPUT_DIR)
    else:
        print(f"Aviso: {caminho_map} não encontrado. Foram gerados apenas os gráficos 1 a 6.")


if __name__ == "__main__":
    main()