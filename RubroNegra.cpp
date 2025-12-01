#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

// ==================== NÓ DA ÁRVORE ====================
enum Color { RED, BLACK };

struct Node {
    float value;
    Color color;
    Node *left, *right, *parent;

    Node(float v) : value(v), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

// ==================== ÁRVORE RUBRO-NEGRA ====================
class RedBlackTree {
private:
    Node* root;

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

    void fixInsert(Node* pt) {
        Node *parent, *grandparent;
        while (pt != root && pt->color == RED && pt->parent->color == RED) {
            parent = pt->parent;
            grandparent = parent->parent;
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
            } else {
                Node* uncle = grandparent->left;
                if (uncle && uncle->color == RED) {
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;
                    pt = grandparent;
                } else {
                    if (pt == parent->left) {
                        rotateRight(parent);
                        pt = parent;
                        parent = pt->parent;
                    }
                    rotateLeft(grandparent);
                    swap(parent->color, grandparent->color);
                    pt = parent;
                }
            }
        }
        root->color = BLACK;
    }

    void inorder(Node* node, vector<float>& arr) {
        if (!node) return;
        inorder(node->left, arr);
        arr.push_back(node->value);
        inorder(node->right, arr);
    }

public:
    RedBlackTree() : root(nullptr) {}

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

    vector<float> getSorted() {
        vector<float> arr;
        inorder(root, arr);
        sort(arr.begin(), arr.end());
        return arr;
    }

    void printSorted() {
        auto arr = getSorted();
        cout << "\nTemperaturas ordenadas:\n";
        for (float v : arr) cout << v << " ";
        cout << "\n";
    }

    void minK(int k) {
        auto arr = getSorted();
        cout << k << " menores temperaturas: ";
        for (int i = 0; i < k && i < arr.size(); i++) cout << arr[i] << " ";
        cout << "\n";
    }

    void maxK(int k) {
        auto arr = getSorted();
        cout << k << " maiores temperaturas: ";
        for (int i = arr.size() - k; i < arr.size() && i >= 0; i++) cout << arr[i] << " ";
        cout << "\n";
    }

    void rangeQuery(float a, float b) {
        auto arr = getSorted();
        cout << "Temperaturas no intervalo [" << a << ", " << b << "]: ";
        for (float v : arr) if (v >= a && v <= b) cout << v << " ";
        cout << "\n";
    }

    void median() {
        auto arr = getSorted();
        if (arr.empty()) { cout << "Sem dados.\n"; return; }
        if (arr.size() % 2 == 1)
            cout << "Mediana: " << arr[arr.size()/2] << " °C\n";
        else
            cout << "Mediana: " << (arr[arr.size()/2 - 1] + arr[arr.size()/2]) / 2 << " \n";
    }
};

// ==================== MAIN ====================
int main() {
    RedBlackTree tree;
    srand(time(nullptr));
    cout << fixed << setprecision(2);

    cout << "Simulando 100 leituras de temperatura...\n";
    for (int i = 0; i < 100; i++) {
        float temp = (rand() % 600) / 10.0; // 0.0 a 60.0 ºC
        tree.insert(temp);
    }

    int op;
    do {
        cout << "\n===== MENU DE CONSULTA  =====\n"
             << "1 - Mostrar todas as temperaturas ordenadas\n"
             << "2 - Mostrar menores k temperaturas\n"
             << "3 - Mostrar maiores k temperaturas\n"
             << "4 - Buscar temperaturas no intervalo [x, y]\n"
             << "5 - Mostrar mediana\n"
             << "0 - Sair\n"
             << "Escolha uma opcao: ";
        cin >> op;

        switch (op) {
            case 1:
                tree.printSorted();
                break;
            case 2: {
                int n;
                cout << "n = ";
                cin >> n;
                tree.minK(n);
                break;
            }
            case 3: {
                int n;
                cout << "n = ";
                cin >> n;
                tree.maxK(n);
                break;
            }
            case 4: {
                float x, y;
                cout << "Digite x e y: ";
                cin >> x >> y;
                tree.rangeQuery(x, y);
                break;
            }
            case 5:
                tree.median();
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

