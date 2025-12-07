#include <DHT.h>

// Configuração do sensor DHT22
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Cores possíveis de um nó na árvore rubro-negra
enum Color { RED, BLACK };

// Estrutura básica do nó da árvore RB
struct Node {
  float value;
  Color color;
  Node *left, *right, *parent;

  Node(float v) {
    value = v;
    color = RED;        // Todo novo nó começa vermelho
    left = right = parent = NULL;
  }
};

// Classe da árvore rubro-negra
class RedBlackTree {
private:
  Node* root;

  // Retorna o menor valor da subárvore
  Node* minimum(Node* node) {
    while (node->left != NULL) node = node->left;
    return node;
  }

  // Retorna o maior valor da subárvore
  Node* maximum(Node* node) {
    while (node->right != NULL) node = node->right;
    return node;
  }

  // Rotação à esquerda
  void rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;

    if (y->left != NULL) y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;

    y->left = x;
    x->parent = y;
  }

  // Rotação à direita
  void rotateRight(Node* x) {
    Node* y = x->left;
    x->left = y->right;

    if (y->right != NULL) y->right->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL) root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;

    y->right = x;
    x->parent = y;
  }

  // Corrige propriedades após inserção
  void fixInsert(Node* k) {
    // Enquanto pai for vermelho, deve ser ajustado
    while (k->parent != NULL && k->parent->color == RED) {

      // Caso o pai esteja do lado esquerdo
      if (k->parent == k->parent->parent->left) {
        Node* u = k->parent->parent->right; // Tio

        // Caso tío seja vermelho: recoloração
        if (u != NULL && u->color == RED) {
          k->parent->color = BLACK;
          u->color = BLACK;
          k->parent->parent->color = RED;
          k = k->parent->parent;
        } else {
          // Ajuste de rotação
          if (k == k->parent->right) {
            k = k->parent;
            rotateLeft(k);
          }
          k->parent->color = BLACK;
          k->parent->parent->color = RED;
          rotateRight(k->parent->parent);
        }
      }

      // Caso pai esteja do lado direito
      else {
        Node* u = k->parent->parent->left;

        if (u != NULL && u->color == RED) {
          k->parent->color = BLACK;
          u->color = BLACK;
          k->parent->parent->color = RED;
          k = k->parent->parent;
        } else {
          if (k == k->parent->left) {
            k = k->parent;
            rotateRight(k);
          }
          k->parent->color = BLACK;
          k->parent->parent->color = RED;
          rotateLeft(k->parent->parent);
        }
      }
    }
    root->color = BLACK;  // A raiz sempre fica preta
  }

  // Corrige a árvore após remoção
  void fixDelete(Node* x) {
    while (x != root && (x == NULL || x->color == BLACK)) {
      if (x == x->parent->left) {
        Node* w = x->parent->right;

        if (w->color == RED) {
          w->color = BLACK;
          x->parent->color = RED;
          rotateLeft(x->parent);
          w = x->parent->right;
        }

        if ((w->left == NULL || w->left->color == BLACK) &&
            (w->right == NULL || w->right->color == BLACK)) {
          w->color = RED;
          x = x->parent;
        } else {
          if (w->right == NULL || w->right->color == BLACK) {
            if (w->left != NULL) w->left->color = BLACK;
            w->color = RED;
            rotateRight(w);
            w = x->parent->right;
          }
          w->color = x->parent->color;
          x->parent->color = BLACK;
          if (w->right != NULL) w->right->color = BLACK;
          rotateLeft(x->parent);
          x = root;
        }
      }

      else {
        Node* w = x->parent->left;

        if (w->color == RED) {
          w->color = BLACK;
          x->parent->color = RED;
          rotateRight(x->parent);
          w = x->parent->left;
        }

        if ((w->right == NULL || w->right->color == BLACK) &&
            (w->left == NULL || w->left->color == BLACK)) {
          w->color = RED;
          x = x->parent;
        } else {
          if (w->left == NULL || w->left->color == BLACK) {
            if (w->right != NULL) w->right->color = BLACK;
            w->color = RED;
            rotateLeft(w);
            w = x->parent->left;
          }
          w->color = x->parent->color;
          x->parent->color = BLACK;
          if (w->left != NULL) w->left->color = BLACK;
          rotateRight(x->parent);
          x = root;
        }
      }
    }

    if (x != NULL) x->color = BLACK;
  }

  // Substitui um nó por outro na árvore
  void transplant(Node* u, Node* v) {
    if (u->parent == NULL) root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;

    if (v != NULL) v->parent = u->parent;
  }

  // Busca um valor
  Node* search(Node* node, float value) {
    if (node == NULL || value == node->value) return node;
    if (value < node->value) return search(node->left, value);
    return search(node->right, value);
  }

  // Impressão em ordem (ordenado)
  void inorder(Node* node) {
    if (node == NULL) return;
    inorder(node->left);
    Serial.println(node->value, 2);
    inorder(node->right);
  }

  // Busca valores dentro do intervalo [a, b]
  void rangeQuery(Node* node, float a, float b) {
    if (node == NULL) return;
    if (a < node->value) rangeQuery(node->left, a, b);
    if (node->value >= a && node->value <= b) Serial.println(node->value, 2);
    if (b > node->value) rangeQuery(node->right, a, b);
  }

  // Conta total de nós
  int countNodes(Node* node) {
    if (node == NULL) return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
  }

  // Preenche um array com valores ordenados
  void fillArray(Node* node, float arr[], int &i) {
    if (node == NULL) return;
    fillArray(node->left, arr, i);
    arr[i++] = node->value;
    fillArray(node->right, arr, i);
  }

public:
  RedBlackTree() { root = NULL; }

  // Inserção principal
  void insertValue(float value) {
    Node* newNode = new Node(value);
    Node* y = NULL;
    Node* x = root;

    // Busca posição de inserção
    while (x != NULL) {
      y = x;
      if (newNode->value < x->value) x = x->left;
      else x = x->right;
    }

    newNode->parent = y;

    if (y == NULL) root = newNode;
    else if (newNode->value < y->value) y->left = newNode;
    else y->right = newNode;

    fixInsert(newNode);   // Ajusta as propriedades da árvore RB
  }

  // Remoção principal
  void removeValue(float value) {
    Node* z = search(root, value);

    if (z == NULL) {
      Serial.println("Valor não encontrado.");
      return;
    }

    Node* y = z;
    Node* x;
    Color yOriginalColor = y->color;

    if (z->left == NULL) {
      x = z->right;
      transplant(z, z->right);
    } else if (z->right == NULL) {
      x = z->left;
      transplant(z, z->left);
    } else {
      y = minimum(z->right);
      yOriginalColor = y->color;
      x = y->right;

      if (y->parent == z) {
        if (x != NULL) x->parent = y;
      } else {
        transplant(y, y->right);
        y->right = z->right;
        y->right->parent = y;
      }

      transplant(z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
    }

    delete z;

    if (yOriginalColor == BLACK && x != NULL) {
      fixDelete(x);
    }
  }

  // Imprime todos os valores em ordem
  void print() {
    Serial.println("Lista ordenada:");
    inorder(root);
    Serial.println("----------------------");
  }

  float getMin() { return minimum(root)->value; }
  float getMax() { return maximum(root)->value; }

  // Calcula mediana usando array ordenado
  float getMedian() {
    int n = countNodes(root);
    if (n == 0) return NAN;

    float* arr = new float[n];
    int i = 0;

    fillArray(root, arr, i);

    float med = (n % 2 == 1)
                ? arr[n/2]
                : (arr[n/2 - 1] + arr[n/2]) / 2.0;

    delete[] arr;
    return med;
  }

  void queryRange(float a, float b) {
    Serial.println("Valores no intervalo:");
    rangeQuery(root, a, b);
  }
};

RedBlackTree tree;

// Mostra os comandos disponíveis
void mostrarMenu() {
  Serial.println();
  Serial.println("====== MENU DE COMANDOS ======");
  Serial.println(" print       -> listar valores ordenados");
  Serial.println(" remove X    -> remover valor");
  Serial.println(" minmax      -> menor e maior valor");
  Serial.println(" median      -> mediana");
  Serial.println(" range X Y   -> valores no intervalo");
  Serial.println(" menu        -> mostrar menu");
  Serial.println("==============================");
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  Serial.println("Sistema iniciado!");
  mostrarMenu();
}

void loop() {
  // Leitura do sensor DHT22
  float temp = dht.readTemperature();

  if (!isnan(temp)) {
    Serial.print("Temperatura: ");
    Serial.println(temp);
    tree.insertValue(temp);
  } else {
    Serial.println("Erro ao ler DHT22!");
  }

  // Processa comandos digitados
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "menu") mostrarMenu();
    else if (cmd == "print") tree.print();
    else if (cmd == "minmax") {
      Serial.print("Min: "); Serial.println(tree.getMin());
      Serial.print("Max: "); Serial.println(tree.getMax());
    }
    else if (cmd == "median") {
      Serial.print("Mediana: ");
      Serial.println(tree.getMedian());
    }
    else if (cmd.startsWith("remove ")) {
      float v = cmd.substring(7).toFloat();
      tree.removeValue(v);
    }
    else if (cmd.startsWith("range ")) {
      int space = cmd.indexOf(' ', 6);
      float a = cmd.substring(6, space).toFloat();
      float b = cmd.substring(space + 1).toFloat();
      tree.queryRange(a, b);
    }
  }

  delay(1500);  // Nova leitura a cada 1.5s
}
