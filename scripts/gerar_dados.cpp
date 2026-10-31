#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <set>
using namespace std;

const int N = 1000000;
const unsigned long long SEED = 42;
const int PERCENTUAL_DECREASE_KEY = 50;

mt19937_64 gerador(SEED);

int sortear(int limite){
    return gerador() % limite;
}

vector<int> gerarCrescente(int n){
    vector<int> dados(n);
    for(int i=0;i<n;i++){
        dados[i] = i + 1;
    }
    
    return dados;
}

vector<int> gerarDecrescente(int n){
    vector<int> dados(n);
    for(int i=0;i<n;i++){
        dados[i] = n - i;
    }

    return dados;
}

void embaralhar(vector<int> &dados){
    for(int i=dados.size()-1;i>0;i--){
        int j = sortear(i + 1);
        int temp = dados[i];
        dados[i] = dados[j];
        dados[j] = temp;
    }
}

void gravarDadosArquivo(vector<int> &dados, string caminho_arquivo){
    ofstream escrever(caminho_arquivo);
    if(!escrever){
        cout << "Erro ao gravar dados no arquivo: " << caminho_arquivo << endl;
        return;
    }

    for(int i=0;i<(int)dados.size();i++){
        escrever << dados[i] << "\n";
    }
}

void gravarDecreaseKey(int n, string caminho_arquivo){
    int quantidade = (long long)n * PERCENTUAL_DECREASE_KEY / 100;

    vector<int> ids(n);
    for(int i=0;i<n;i++){
        ids[i] = i;
    }
    embaralhar(ids);

    ofstream escrever(caminho_arquivo);
    if(!escrever){
        cout << "Erro ao gravar dados no arquivo: " << caminho_arquivo << endl;
        return;
    }

    for(int i=0;i<quantidade;i++){
        int id = ids[i];
        int diminuir = sortear(n) + 1;      // de 1 até N
        escrever << id << " " << diminuir << "\n";
    }
}

// gerando dados para o grafo -------------------------------------------------
struct Aresta {
    int origem;
    int destino;
    int peso;
};

void gravarGrafo(int n_vertices, long long n_arestas, string caminho_arquivo, int peso_max = 100){
    ofstream escrever(caminho_arquivo);
    if(!escrever){
        cout << "Erro ao gravar dados no arquivo: " << caminho_arquivo << endl;
        return;
    }

    long long max_possivel = (long long)n_vertices * (n_vertices - 1);
    if(n_arestas > max_possivel){
        n_arestas = max_possivel;
    }

    vector<Aresta> arestas;
    set<pair<int,int>> existes;

    // 1. Garante conectividade inicial (caminho 0 -> 1 -> 2 ... -> n-1)
    for(int i = 0; i < n_vertices - 1; i++){
        int u = i;
        int v = i + 1;
        int peso = sortear(peso_max) + 1; // Pesos entre 1 e peso_max
        existes.insert({u, v});
        arestas.push_back({u, v, peso});
    }

    // 2. Preenche o restante das arestas sem duplicatas
    while((long long)arestas.size() < n_arestas){
        int u = sortear(n_vertices);
        int v = sortear(n_vertices);
        
        if(u != v && existes.find({u, v}) == existes.end()){
            existes.insert({u, v});
            int peso = sortear(peso_max) + 1;
            arestas.push_back({u, v, peso});
        }
    }

    escrever << n_vertices << " " << arestas.size() << "\n";

    for(int i = 0; i < (int)arestas.size(); i++){
        escrever << arestas[i].origem << " " << arestas[i].destino << " " << arestas[i].peso << "\n";
    }

    cout << "Grafo gerado: " << caminho_arquivo << " (" << n_vertices << " V, " << arestas.size() << " E)" << endl;
}

int main(){
    vector<int> crescente = gerarCrescente(N);
    vector<int> decrescente = gerarDecrescente(N);
    vector<int> aleatorio = gerarCrescente(N);
    embaralhar(aleatorio);

    gravarDadosArquivo(crescente, "data/crescente.dat");
    gravarDadosArquivo(decrescente, "data/decrescente.dat");
    gravarDadosArquivo(aleatorio, "data/aleatorio.dat");
    gravarDecreaseKey(N, "data/decrease_key.dat");
    
    // 1. Grafo Esparso (E ~ 3 * V) para 100.000 vértices
    int v_esparso = 100000;
    long long e_esparso = (long long)v_esparso * 3;
    gravarGrafo(v_esparso, e_esparso, "data/grafo_esparso.dat");

    // 2. Grafo Denso (E ~ 20% do máximo) para 5.000 vértices e ~5.000.000 de arestas 
    int v_denso = 5000;
    long long e_denso = (long long)(v_denso * (v_denso - 1)) * 20 / 100; // 20% de densidade
    gravarGrafo(v_denso, e_denso, "data/grafo_denso.dat");

    cout << "Arquivos gerados com N = " << N << " e semente = " << SEED << endl;
    return 0;
}