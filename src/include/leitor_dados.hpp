#ifndef LEITOR_DADOS_HPP
#define LEITOR_DADOS_HPP

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
using namespace std;

const int LIMITE_MAXIMO = 1000000;

struct ParDecreaseKey {
    int id;        // posição do elemento no arquivo de dados (ordem de inserção, começando em 0)
    int diminuir;  // quanto subtrair da chave atual
};


inline vector<int> lerDadosArquivo(int n, string caminho_arquivo){
    if(n <= 0 || n > LIMITE_MAXIMO){
        throw invalid_argument("Erro ao ler dados do arquivo: tamanho errado - " + to_string(n) + " - de 1 até o limite máximo de " + to_string(LIMITE_MAXIMO));
    }

    ifstream ler(caminho_arquivo);
    if(!ler){
        throw runtime_error("Erro ao abrir o arquivo: " + caminho_arquivo);
    }

    vector<int> dados(n);
    for(int i=0;i<n;i++){
        if(!(ler >> dados[i])){
            throw runtime_error("Erro: o arquivo " + caminho_arquivo + " tem menos de " + to_string(n) + " valores.");
        }
    }
    return dados;
}

inline vector<ParDecreaseKey> lerDecreaseKey(int n, string caminho_arquivo){
    if(n <= 0 || n > LIMITE_MAXIMO){
        throw invalid_argument("Erro ao ler decrease-key: tamanho errado - " + to_string(n));
    }

    ifstream ler(caminho_arquivo);
    if(!ler){
        throw runtime_error("Erro ao abrir o arquivo: " + caminho_arquivo);
    }

    vector<ParDecreaseKey> pares;
    ParDecreaseKey par;
    while(ler >> par.id >> par.diminuir){
        if(par.id < n){
            pares.push_back(par);
        }
    }
    return pares;
}

#endif