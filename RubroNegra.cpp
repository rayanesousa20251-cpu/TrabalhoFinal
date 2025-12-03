#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

// Cores usadas pela Árvore Rubro-Negra
enum Color { RED, BLACK };


// Estrutura de cada nó da árvore
struct Node {
    float value;        // valor da temperatura
    Color color;        // cor do nó (vermelho ou preto)
    Node *left;         // filho esquerdo
    Node *right;        // filho direito
    Node *parent;       // pai do nó

    Node(float v) : value(v), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};


// Classe da Árvore Rubro-Negra
class RedBlackTree {
private:
    Node* root;     // raiz da árvore

    // Rotação à esquerda (reorganiza a árvore)
    void rotateLeft(Node* x) {
        Node* y = x->right;

        x->right = y->left;
        if (y->left != nullptr) 
            y->left->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr) 
            root = y;
        else if (x == x->parent->left) 
            x->parent->left = y;
        else 
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    // Rotação à direita
    void rotateRight(Node* y) {
        Node* x = y->left;

        y->left = x->right;
        if (x->right != nullptr) 
            x->right->parent = y;

        x->parent = y->parent;

        if (y->parent == nullptr) 
            root = x;
        else if (y == y->parent->right) 
            y->parent->right = x;
        else 
            y->parent->left = x;

        x->right = y;
        y->parent = x;
    }

    // Ajusta as cores e rotações após a inserção
    void fixInsert(Node* pt) {
        Node *parent, *grandparent;

        // Enquanto o nó e seu pai forem vermelhos, precisa corrigir
        while (pt != root && pt->color == RED && pt->parent->color == RED) {
            parent = pt->parent;
            grandparent = parent->parent;

            // Pai está à esquerda do avô
            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;

                // Caso 1: tio vermelho → recolorir
                if (uncle && uncle->color == RED) {
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;
                    pt = grandparent;
                } 
                // Caso 2/3: tio preto → rotações
                else {
                    if (pt == parent->right) {
                        rotateLeft(parent);
                        pt = parent;
                    }
                    rotateRight(grandparent);
                    swap(parent->color, grandparent->color);
                }
            }
            // Pai está à direita do avô
            else {
                Node* uncle = grandparent->left;

                if (uncle && uncle->color == RED) {
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;
                    pt = grandparent;
                } else {
                    if (pt == parent->left) {
                        rotateRight(parent);
                        pt = parent->parent;
                    }
                    rotateLeft(grandparent);
                    swap(parent->color, grandparent->color);
                }
            }
        }

        // A raiz sempre deve ser preta
        root->color = BLACK;
    }

    // Percurso in-order (gera os valores ordenados)
    void inorder(Node* node, vector<float>& arr) {
        if (!node) return;
        inorder(node->left, arr);
        arr.push_back(node->value);
        inorder(node->right, arr);
    }

    // Remove toda a árvore (usado para esvaziar)
    void deleteAll(Node* node) {
        if (!node) return;
        deleteAll(node->left);
        deleteAll(node->right);
        delete node;
    }

public:

    RedBlackTree() : root(nullptr) {}

    // Inserir um valor na árvore
    void insert(float value) {
        Node* pt = new Node(value);
        Node* parent = nullptr;
        Node* current = root;

        // Procura onde inserir o novo valor
        while (current != nullptr) {
            parent = current;
            current = (value < current->value) ? current->left : current->right;
        }

        // Conecta o novo nó ao seu pai
        pt->parent = parent;
        if (parent == nullptr) 
            root = pt;
        else if (value < parent->value) 
            parent->left = pt;
        else 
            parent->right = pt;

        // Ajusta cores e rotações
        fixInsert(pt);
    }

    // Limpa toda a árvore (esvazia tudo)
    void clear() {
        deleteAll(root);
        root = nullptr;
        cout << "\n>> Todos os dados foram removidos! Arvore vazia.\n";
    }

    // Retorna um vetor ordenado com todos os valores
    vector<float> getSorted() {
        vector<float> arr;
        inorder(root, arr);
        sort(arr.begin(), arr.end());
        return arr;
    }

    // Mostra as temperaturas em ordem crescente
    void printSorted() {
        auto arr = getSorted();

        if (arr.empty()) {
            cout << "\nNenhuma temperatura cadastrada.\n";
            return;
        }

        cout << "\nTemperaturas ordenadas:\n";
        for (float v : arr) cout << v << " ";
        cout << "\n";
    }

    // n menores temperaturas
    void minK(int k) {
        auto arr = getSorted();
        if (arr.empty()) { cout << "\nSem dados.\n"; return; }

        cout << k << " menores temperaturas: ";
        for (int i = 0; i < k && i < arr.size(); i++)
            cout << arr[i] << " ";
        cout << "\n";
    }

    // n maiores temperaturas
    void maxK(int k) {
        auto arr = getSorted();
        if (arr.empty()) { cout << "\nSem dados.\n"; return; }

        cout << k << " maiores temperaturas: ";
        for (int i = arr.size() - k; i < arr.size() && i >= 0; i++)
            cout << arr[i] << " ";
        cout << "\n";
    }

    // Buscar valores dentro do intervalo [a, b]
    void rangeQuery(float a, float b) {
        auto arr = getSorted();
        cout << "Temperaturas no intervalo [" << a << ", " << b << "]: ";
        for (float v : arr)
            if (v >= a && v <= b)
                cout << v << " ";
        cout << "\n";
    }

    // Calcular mediana
    void median() {
        auto arr = getSorted();
        if (arr.empty()) { cout << "Sem dados.\n"; return; }

        if (arr.size() % 2 == 1)
            cout << "Mediana: " << arr[arr.size()/2] << " °C\n";
        else
            cout << "Mediana: " 
                 << (arr[arr.size()/2 - 1] + arr[arr.size()/2]) / 2 
                 << " °C\n";
    }
};


//Main


int main() {
    RedBlackTree tree;
    cout << fixed << setprecision(2); // mostrar números com 2 casas decimais

    int op;
    do {
        // Menu para escolher ações
        cout << "\n----- MENU -----\n"
             << "1 - Inserir temperatura\n"
             << "2 - Mostrar todas ordenadas\n"
             << "3 - Mostrar n menores\n"
             << "4 - Mostrar n maiores\n"
             << "5 - Buscar intervalo [x, y]\n"
             << "6 - Mostrar mediana\n"
             << "7 - Esvaziar todas as temperaturas\n"
             << "0 - Sair\n"
             << "Opcao: ";
        cin >> op;

        switch (op) {

        case 1: {
            float t;
            cout << "Digite a temperatura: ";
            cin >> t;
            tree.insert(t);
            break;
        }

        case 2:
            tree.printSorted();
            break;

        case 3: {
            int n;
            cout << "n = ";
            cin >> n;
            tree.minK(n);
            break;
        }

        case 4: {
            int n;
            cout << "n = ";
            cin >> n;
            tree.maxK(n);
            break;
        }

        case 5: {
            float x, y;
            cout << "Digite x e y: ";
            cin >> x >> y;
            tree.rangeQuery(x, y);
            break;
        }

        case 6:
            tree.median();
            break;

        case 7:
            tree.clear();
            break;

        case 0:
            cout << "Encerrando...\n";
            break;

        default:
            cout << "Opcao invalida!\n";
        }

    } while (op != 0);

    return 0;
}


