#include <iostream>
using namespace std;

// HEAPIFY (peneira para baixo)
// garante que o elemento na posicao i seja
// maior que seus filhos, descendo se necessario
// n = tamanho atual do heap
void heapify(int v[], int n, int i) {

    int maior = i;          // assume que o pai e o maior
    int esquerda = 2*i + 1; // indice do filho esquerdo
    int direita  = 2*i + 2; // indice do filho direito

    // verifica se o filho esquerdo e maior que o pai
    if (esquerda < n && v[esquerda] > v[maior])
        maior = esquerda;

    // verifica se o filho direito e maior que o atual maior
    if (direita < n && v[direita] > v[maior])
        maior = direita;

    // se o maior nao for o pai, troca e continua descendo
    if (maior != i) {
        int tmp = v[i];
        v[i] = v[maior];
        v[maior] = tmp;

        heapify(v, n, maior); // desce recursivamente
    }
}

// HEAPSORT
// passo 1: transforma o vetor em um max-heap
// passo 2: remove o maior elemento repetidamente colocando-o no final do vetor
// resultado: vetor ordenado em ordem crescente
void heapSort(int v[], int n) {

    // passo 1: build max-heap
    // começa do ultimo no interno e vai ate a raiz
    for (int i = n/2 - 1; i >= 0; i--)
        heapify(v, n, i);

    cout << "\nMax-Heap construido: ";
    for (int i = 0; i < n; i++)
        cout << v[i] << " ";
    cout << endl;

    // passo 2: extrai o maior elemento um por vez
    cout << "\n--- Passo a passo das remocoes ---" << endl;

    for (int i = n-1; i > 0; i--) {

        // troca a raiz (maior elemento) com o ultimo
        cout << "\nRemove " << v[0] << " (raiz)";
        cout << " -> troca com " << v[i] << " (posicao " << i << ")";

        int tmp = v[0];
        v[0] = v[i];
        v[i] = tmp;

        // reorganiza o heap sem o elemento ja ordenado
        heapify(v, i, 0);

        // mostra o estado atual
        cout << "\nHeap:     ";
        for (int j = 0; j < i; j++)   cout << v[j] << " ";
        cout << "\nOrdenado: ";
        for (int j = i; j < n; j++)   cout << v[j] << " ";
        cout << endl;
    }
}

// FUNCAO AUXILIAR: imprime o vetor
void imprimir(int v[], int n) {
    for (int i = 0; i < n; i++)
        cout << v[i] << " ";
    cout << endl;
}

// MAIN
int main() {

    int n;
    cout << "Quantos numeros voce quer ordenar? ";
    cin >> n;

    int v[100]; // suporta ate 100 numeros

    cout << "Digite os " << n << " numeros:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "Numero " << i+1 << ": ";
        cin >> v[i];
    }

    cout << "\nVetor antes da ordenacao:  ";
    imprimir(v, n);

    heapSort(v, n);

    cout << "\nVetor depois da ordenacao: ";
    imprimir(v, n);

    return 0;
}