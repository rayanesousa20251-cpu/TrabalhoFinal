#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

enum Color { RED, BLACK };

// =====================
// Estrutura do nó
// =====================
struct Node {
    float value;
    Color color;
    Node *left, *right, *parent;

    Node(float v) : value(v), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

// =====================
// Árvore Rubro-Negra
// =====================
class RedBlackTree {
private:
    Node* root;

    // Rotação à esquerda
    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != nullptr) y->left->parent = x;
        y->parent = x->parent;

        if (x->parent == nullptr) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    // Rotação à direita
    void rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right != nullptr) x->right->parent = y;
        x->parent = y->parent;

        if (y->parent == nullptr) root = x;
        else if (y == y->parent->right) y->parent->right = x;
        else y->parent->left = x;

        x->right = y;
        y->parent = x;
    }

    // Correção após inserção
    void fixInsert(Node* pt) {
        Node *parent, *grandparent;

        while (pt != root && pt->color == RED && pt->parent->color == RED) {
            parent = pt->parent;
            grandparent = parent->parent;

            // Pai é filho esquerdo do avô
            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;

                if (uncle && uncle->color == RED) {
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;
                    pt = grandparent;
                } else {
                    if (pt == parent->right) {
                        rotateLeft(parent);
                        pt = parent;
                        parent = pt->parent;
                    }
                    rotateRight(grandparent);
                    swap(parent->color, grandparent->color);
                    pt = parent;
                }
            }
            // Pai é filho direito
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

        root->color = BLACK;
    }

    // Caminhamento in-order
    void inorder(Node* node, vector<float>& arr) {
        if (!node) return;
        inorder(node->left, arr);
        arr.push_back(node->value);
        inorder(node->right, arr);
    }

    // Remover recursivamente (usado para limpar tudo)
    void deleteAll(Node* node) {
        if (!node) return;
        deleteAll(node->left);
        deleteAll(node->right);
        delete node;
    }

public:

    RedBlackTree() : root(nullptr) {}

    // Inserção normal
    void insert(float value) {
        Node* pt = new Node(value);
        Node* parent = nullptr;
        Node* current = root;

        while (current != nullptr) {
            parent = current;
            current = (value < current->value) ? current->left : current->right;
        }

        pt->parent = parent;
        if (parent == nullptr) root = pt;
        else if (value < parent->value) parent->left = pt;
        else parent->right = pt;

        fixInsert(pt);
    }

    // Limpar árvore (esvaziar lista)
    void clear() {
        deleteAll(root);
        root = nullptr;
        cout << "\n>> Todos os dados foram removidos! Arvore vazia.\n";
    }

    // Vetor ordenado
    vector<float> getSorted() {
        vector<float> arr;
        inorder(root, arr);
        sort(arr.begin(), arr.end());
        return arr;
    }

    // Imprimir temperaturas ordenadas
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

    // n menores
    void minK(int k) {
        auto arr = getSorted();
        if (arr.empty()) {
            cout << "\nSem dados.\n"; return;
        }
        cout << k << " menores temperaturas: ";
        for (int i = 0; i < k && i < arr.size(); i++) cout << arr[i] << " ";
        cout << "\n";
    }

    // n maiores
    void maxK(int k) {
        auto arr = getSorted();
        if (arr.empty()) {
            cout << "\nSem dados.\n"; return;
        }
        cout << k << " maiores temperaturas: ";
        for (int i = arr.size() - k; i < arr.size() && i >= 0; i++) cout << arr[i] << " ";
        cout << "\n";
    }

    // Intervalo [a, b]
    void rangeQuery(float a, float b) {
        auto arr = getSorted();
        cout << "Temperaturas no intervalo [" << a << ", " << b << "]: ";
        for (float v : arr) if (v >= a && v <= b) cout << v << " ";
        cout << "\n";
    }

    // Mediana
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


// =====================
// Programa principal
// =====================
int main() {
    RedBlackTree tree;
    cout << fixed << setprecision(2);

    int op;
    do {
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
