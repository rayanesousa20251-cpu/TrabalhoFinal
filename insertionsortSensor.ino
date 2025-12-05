#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

float lista[200];       // até 200 leituras
int tamanho = 0;

// --------------------------------
// INSERIR NA LISTA ORDENADA
// --------------------------------
void insertValue(float value) {
  if (tamanho >= 200) return;

  int pos = 0;
  while (pos < tamanho && lista[pos] < value) pos++;

  for (int i = tamanho; i > pos; i--) {
    lista[i] = lista[i - 1];
  }

  lista[pos] = value;
  tamanho++;
}

// --------------------------------
// REMOVER UM VALOR
// --------------------------------
void removeValue(float value) {
  int pos = -1;

  for (int i = 0; i < tamanho; i++) {
    if (lista[i] == value) {
      pos = i;
      break;
    }
  }

  if (pos == -1) {
    Serial.println("Valor não encontrado.");
    return;
  }

  for (int i = pos; i < tamanho - 1; i++) {
    lista[i] = lista[i + 1];
  }

  tamanho--;
  Serial.println("Valor removido.");
}

// --------------------------------
// IMPRIMIR LISTA ORDENADA
// --------------------------------
void printSorted() {
  Serial.println("Lista ordenada:");
  for (int i = 0; i < tamanho; i++) {
    Serial.println(lista[i]);
  }
  Serial.println("----------------------");
}

// --------------------------------
// 3 MENORES E 3 MAIORES
// --------------------------------
void printMinMax() {
  Serial.println("3 menores:");
  for (int i = 0; i < 3 && i < tamanho; i++) {
    Serial.println(lista[i]);
  }

  Serial.println("3 maiores:");
  for (int i = tamanho - 1; i >= 0 && i >= tamanho - 3; i--) {
    Serial.println(lista[i]);
  }
}

// --------------------------------
// RANGE QUERY
// --------------------------------
void rangeQuery(float x, float y) {
  Serial.print("Valores entre ");
  Serial.print(x);
  Serial.print(" e ");
  Serial.println(y);

  for (int i = 0; i < tamanho; i++) {
    if (lista[i] >= x && lista[i] <= y) {
      Serial.println(lista[i]);
    }
  }
}

// --------------------------------
// MEDIANA
// --------------------------------
float median() {
  if (tamanho == 0) return NAN;

  if (tamanho % 2 == 1) {
    return lista[tamanho / 2];
  } else {
    return (lista[tamanho/2 - 1] + lista[tamanho/2]) / 2.0;
  }
}

// --------------------------------
// MENU EXPLICATIVO
// --------------------------------
void mostrarMenu() {
  Serial.println();
  Serial.println("======== MENU DE COMANDOS ========");
  Serial.println(" print         -> listar valores ordenados");
  Serial.println(" remove X      -> remover valor X da lista");
  Serial.println(" minmax        -> mostrar 3 menores e 3 maiores valores");
  Serial.println(" median        -> calcular e mostrar a mediana");
  Serial.println(" range X Y     -> listar valores entre X e Y");
  Serial.println(" menu          -> mostrar o menu novamente");
  Serial.println("===================================");
  Serial.println();
}

// --------------------------------
// PROCESSAR COMANDOS DIGITADOS
// --------------------------------
void processCommand(String cmd) {
  cmd.trim();

  if (cmd == "menu") {
    mostrarMenu();
    return;
  }

  if (cmd == "print") {
    printSorted();
    return;
  }

  if (cmd == "minmax") {
    printMinMax();
    return;
  }

  if (cmd == "median") {
    Serial.print("Mediana: ");
    Serial.println(median());
    return;
  }

  if (cmd.startsWith("remove ")) {
    float v = cmd.substring(7).toFloat();
    removeValue(v);
    return;
  }

  if (cmd.startsWith("range ")) {
    int s1 = cmd.indexOf(' ');
    int s2 = cmd.indexOf(' ', s1 + 1);

    float x = cmd.substring(s1 + 1, s2).toFloat();
    float y = cmd.substring(s2 + 1).toFloat();

    rangeQuery(x, y);
    return;
  }

  Serial.println("Comando inválido. Digite 'menu' para ajuda.");
}

// --------------------------------
// SETUP
// --------------------------------
void setup() {
  Serial.begin(9600);
  dht.begin();

  Serial.println("Sistema iniciado!");
  mostrarMenu();
}

unsigned long lastRead = 0;

// --------------------------------
// LOOP PRINCIPAL
// --------------------------------
void loop() {
  // Leitura a cada 2s
  if (millis() - lastRead >= 2000) {
    float t = dht.readTemperature();

    if (!isnan(t)) {
      insertValue(t);
      Serial.print("Temperatura lida: ");
      Serial.println(t);
    } else {
      Serial.println("Erro ao ler o DHT22");
    }

    lastRead = millis();
  }

  // Comandos via Serial
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    processCommand(cmd);
  }
}

