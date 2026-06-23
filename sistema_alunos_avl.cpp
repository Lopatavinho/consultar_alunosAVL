#include <iostream>
#include <string>
using namespace std;

// ESTRUTURA DO ALUNO
// guarda todos os dados de um aluno
struct Aluno {
    int matricula;
    string nome;
    string curso;
    double nota1;
    double nota2;
    double media; // calculada automaticamente na hora de inserir
};

// ESTRUTURA DO NO DA ARVORE
// cada no guarda um aluno e aponta para
// os filhos da esquerda e da direita
// a altura e usada para o balanceamento da AVL
struct No {
    Aluno aluno;
    No* esquerda;
    No* direita;
    int altura;
};

// FUNCOES AUXILIARES

// retorna a altura de um no
// se o no for NULL retorna 0
int altura(No* n) {
    if (n == NULL) return 0;
    return n->altura;
}

// retorna o maior entre dois numeros
int maximo(int a, int b) {
    if (a > b) return a;
    return b;
}

// recalcula a altura de um no com base nos seus filhos
void atualizarAltura(No* n) {
    n->altura = 1 + maximo(altura(n->esquerda), altura(n->direita));
}

// calcula o fator de equilibrio de um no
// fator = altura da esquerda - altura da direita
// se o resultado for maior que 1 ou menor que -1, o no esta desbalanceado
int fatorEquilibrio(No* n) {
    if (n == NULL) return 0;
    return altura(n->esquerda) - altura(n->direita);
}

// cria um novo no com o aluno recebido
No* criarNo(Aluno a) {
    No* novo = new No();
    novo->aluno = a;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->altura = 1; // todo no novo comeca com altura 1
    return novo;
}

// ROTACOES DA AVL
// as rotacoes sao usadas para rebalancear
// a arvore depois de uma insercao

// rotacao simples para a direita
// usada quando o lado esquerdo esta pesado (caso esq-esq)
No* rotacaoDireita(No* y) {
    No* x = y->esquerda;
    No* T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    // y desceu entao atualiza primeiro
    atualizarAltura(y);
    atualizarAltura(x);

    return x; // x vira a nova raiz desse trecho
}

// rotacao simples para a esquerda
// usada quando o lado direito esta pesado (caso dir-dir)
No* rotacaoEsquerda(No* x) {
    No* y = x->direita;
    No* T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    // x desceu entao atualiza primeiro
    atualizarAltura(x);
    atualizarAltura(y);

    return y; // y vira a nova raiz desse trecho
}

// INSERCAO NA AVL
// funciona igual a uma BST normal,
// mas depois de inserir verifica se a arvore
// ficou desbalanceada e corrige com rotacoes
No* inserir(No* raiz, Aluno a) {

    // se chegou em um espaco vazio, cria o no aqui
    if (raiz == NULL) return criarNo(a);

    // decide para qual lado descer
    if (a.matricula < raiz->aluno.matricula)
        raiz->esquerda = inserir(raiz->esquerda, a);
    else if (a.matricula > raiz->aluno.matricula)
        raiz->direita = inserir(raiz->direita, a);
    else {
        // matricula duplicada: nao insere
        cout << "Matricula ja existe!" << endl;
        return raiz;
    }

    // atualiza a altura do no atual apos a insercao
    atualizarAltura(raiz);

    // verifica o fator de equilibrio
    int fe = fatorEquilibrio(raiz);

    // caso esquerda-esquerda: rotacao simples direita
    if (fe > 1 && a.matricula < raiz->esquerda->aluno.matricula)
        return rotacaoDireita(raiz);

    // caso direita-direita: rotacao simples esquerda
    if (fe < -1 && a.matricula > raiz->direita->aluno.matricula)
        return rotacaoEsquerda(raiz);

    // caso esquerda-direita: rotacao dupla (esquerda depois direita)
    if (fe > 1 && a.matricula > raiz->esquerda->aluno.matricula) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }

    // caso direita-esquerda: rotacao dupla (direita depois esquerda)
    if (fe < -1 && a.matricula < raiz->direita->aluno.matricula) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }

    // se nao precisou de rotacao, retorna o no sem alteracao
    return raiz;
}

// BUSCA NA AVL
// funciona igual a uma BST:
// se a matricula for menor vai para a esquerda,
// se for maior vai para a direita,
// se encontrar retorna o no
// como a arvore esta balanceada, a busca
// e sempre O(log n)
No* buscar(No* raiz, int matricula) {
    // se chegou em NULL, nao encontrou
    if (raiz == NULL) return NULL;

    // encontrou o aluno
    if (matricula == raiz->aluno.matricula) return raiz;

    // decide para qual lado continuar a busca
    if (matricula < raiz->aluno.matricula)
        return buscar(raiz->esquerda, matricula);
    else
        return buscar(raiz->direita, matricula);
}

// PERCURSO EM ORDEM (esquerda -> raiz -> direita)
// em uma AVL isso lista os nos em ordem
// crescente de matricula automaticamente,
// pois os menores ficam sempre a esquerda
void listar(No* raiz) {
    if (raiz == NULL) return;

    listar(raiz->esquerda);  // visita os menores primeiro

    // exibe o aluno atual
    cout << raiz->aluno.matricula << " | "
         << raiz->aluno.nome     << " | "
         << raiz->aluno.curso    << " | media: "
         << raiz->aluno.media    << endl;

    listar(raiz->direita);   // depois visita os maiores
}

// MEDIA GERAL DA TURMA
// percorre toda a arvore somando as medias
// e contando os alunos
void somarMedias(No* raiz, double& soma, int& cont) {
    if (raiz == NULL) return;
    somarMedias(raiz->esquerda, soma, cont);
    soma += raiz->aluno.media;
    cont++;
    somarMedias(raiz->direita, soma, cont);
}

// MAIN — menu interativo
int main() {

    No* raiz = NULL; // arvore começa vazia

    int opcao;

    do {
        cout << "\n1 - Inserir aluno"          << endl;
        cout << "2 - Buscar aluno"             << endl;
        cout << "3 - Listar todos em ordem"    << endl;
        cout << "4 - Media geral da turma"     << endl;
        cout << "5 - Sair"                     << endl;
        cout << "Opcao: ";
        cin >> opcao;

        if (opcao == 1) {
            Aluno novo;
            cout << "Matricula: "; cin >> novo.matricula;
            cin.ignore(); // limpa o enter que ficou no buffer
            cout << "Nome: ";  getline(cin, novo.nome);
            cout << "Curso: "; getline(cin, novo.curso);
            cout << "Nota 1: "; cin >> novo.nota1;
            cout << "Nota 2: "; cin >> novo.nota2;
            novo.media = (novo.nota1 + novo.nota2) / 2; // calcula a media automaticamente
            raiz = inserir(raiz, novo);
            cout << "Aluno inserido!" << endl;

        } else if (opcao == 2) {
            int mat;
            cout << "Matricula: "; cin >> mat;
            No* r = buscar(raiz, mat);
            if (r != NULL) {
                cout << "Nome: "   << r->aluno.nome   << endl;
                cout << "Curso: "  << r->aluno.curso  << endl;
                cout << "Nota 1: " << r->aluno.nota1  << endl;
                cout << "Nota 2: " << r->aluno.nota2  << endl;
                cout << "Media: "  << r->aluno.media  << endl;
            } else {
                cout << "Aluno nao encontrado." << endl;
            }

        } else if (opcao == 3) {
            cout << "\n--- Lista de alunos (ordem de matricula) ---" << endl;
            listar(raiz);

        } else if (opcao == 4) {
            double soma = 0;
            int cont = 0;
            somarMedias(raiz, soma, cont);
            if (cont > 0)
                cout << "Media geral da turma: " << soma / cont << endl;
            else
                cout << "Sem alunos cadastrados." << endl;

        } else if (opcao != 5) {
            cout << "Opcao invalida!" << endl;
        }

    } while (opcao != 5);

    cout << "Encerrando..." << endl;
    return 0;
}
