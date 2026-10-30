#include <iostream>
#include <chrono>
#include <iomanip>
#include "leitor_dados.hpp"

using namespace std;

void testarArquivoValores(const string& nome_teste, const string& caminho, int n) {
    cout << "\n--------------------------------------------------" << endl;
    cout << " Lendo: " << nome_teste << endl;
    cout << " Caminho: " << caminho << endl;

    auto inicio = chrono::high_resolution_clock::now();
    
    try {
        vector<int> dados = lerDadosArquivo(n, caminho);
        auto fim = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> tempo = fim - inicio;

        cout << " [OK] Lido com sucesso em " << fixed << setprecision(2) << tempo.count() << " ms" << endl;
        cout << " Total de elementos lidos: " << dados.size() << endl;
        cout << " Primeiro elemento [0]: " << dados.front() << endl;
        cout << " Ultimo elemento [" << n - 1 << "]: " << dados.back() << endl;
    } catch (const exception& e) {
        cout << " [ERRO] " << e.what() << endl;
    }
}

void testarArquivoDecreaseKey(const string& caminho, int n) {
    cout << "\n--------------------------------------------------" << endl;
    cout << " Lendo: Decrease-Key" << endl;
    cout << " Caminho: " << caminho << endl;

    auto inicio = chrono::high_resolution_clock::now();

    try {
        vector<ParDecreaseKey> pares = lerDecreaseKey(n, caminho);
        auto fim = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> tempo = fim - inicio;

        cout << " [OK] Lido com sucesso em " << fixed << setprecision(2) << tempo.count() << " ms" << endl;
        cout << " Total de pares validos lidos (id < " << n << "): " << pares.size() << endl;
        
        if (!pares.empty()) {
            cout << " Primeiro par: id=" << pares.front().id << ", diminuir=" << pares.front().diminuir << endl;
            cout << " Ultimo par:   id=" << pares.back().id << ", diminuir=" << pares.back().diminuir << endl;
        }
    } catch (const exception& e) {
        cout << " [ERRO] " << e.what() << endl;
    }
}

int main() {
    cout << "==================================================" << endl;
    cout << "       TESTE DE CARREGAMENTO DOS ARQUIVOS        " << endl;
    cout << "==================================================" << endl;

    int N = 1000000; 
    string file_crescente   = "data/crescente.dat";
    string file_decrescente = "data/decrescente.dat";
    string file_aleatorio   = "data/aleatorio.dat";
    string file_decrease    = "data/decrease_key.dat";

    testarArquivoValores("Conjunto Crescente", file_crescente, N);
    testarArquivoValores("Conjunto Decrescente", file_decrescente, N);
    testarArquivoValores("Conjunto Aleatorio", file_aleatorio, N);
    testarArquivoDecreaseKey(file_decrease, N);

    cout << "\n==================================================" << endl;
    cout << "            TESTE CONCLUIDO COM SUCESSO           " << endl;
    cout << "==================================================" << endl;

    return 0;
}