#include <iostream>
#include <fstream>
#include <vector>
#include <random>
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

int main(){
    vector<int> crescente = gerarCrescente(N);
    vector<int> decrescente = gerarDecrescente(N);
    vector<int> aleatorio = gerarCrescente(N);
    embaralhar(aleatorio);

    gravarDadosArquivo(crescente, "data/crescente.dat");
    gravarDadosArquivo(decrescente, "data/decrescente.dat");
    gravarDadosArquivo(aleatorio, "data/aleatorio.dat");
    gravarDecreaseKey(N, "data/decrease_key.dat");

    cout << "Arquivos gerados com N = " << N << " e semente = " << SEED << endl;
    return 0;
}