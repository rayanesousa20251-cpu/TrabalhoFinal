#include "DHT.h"

// Define o pino e o tipo do sensor DHT22
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

float lista[200];       // Vetor que armazena até 200 leituras
int tamanho = 0;        // Quantidade atual de valores armazenados

// INSERIR UM VALOR MANTENDO A LISTA ORDENADA
void insertValue(float value) {
  if (tamanho >= 200) return;   // Evita inserção se a lista estiver cheia

  int pos = 0;

  // Encontra a posição correta onde o valor deve ser inserido
  while (pos < tamanho && lista[pos] < value) pos++;

  // Desloca os elementos para abrir espaço no vetor
  for (int i = tamanho; i > pos; i--) {
    lista[i] = lista[i - 1];
  }

  // Insere o novo valor
  lista[pos] = value;
  tamanho++;
}

// REMOVER UM VALOR ESPECÍFICO DA LISTA
void removeValue(float value) {
  int pos = -1;

  // Procura o valor na lista
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

  // Desloca os elementos para fechar o espaço removido
  for (int i = pos; i < tamanho - 1; i++) {
    lista[i] = lista[i + 1];
  }

  tamanho--;
  Serial.println("Valor removido.");
}

// IMPRIMIR A LISTA TOTALMENTE ORDENADA
void printSorted() {
  Serial.println("Lista ordenada:");
  for (int i = 0; i < tamanho; i++) {
    Serial.println(lista[i]);
  }
  Serial.println("----------------------");
}

// EXIBIR OS 3 MENORES E 3 MAIORES VALORES SALVOS
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

// LISTAR VALORES ENTRE X E Y
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

// CALCULAR A MEDIANA DA LISTA
float median() {
  if (tamanho == 0) return NAN;   // Lista vazia não tem mediana

  // Se a quantidade for ímpar, retorna o elemento do meio
  if (tamanho % 2 == 1) {
    return lista[tamanho / 2];
  } 
  
  // Se for par, retorna a média dos dois elementos centrais
  return (lista[tamanho/2 - 1] + lista[tamanho/2]) / 2.0;
}

// MOSTRAR MENU DE COMANDOS DISPONÍVEIS
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

// INTERPRETAR E EXECUTAR COMANDOS DIGITADOS NO SERIAL MONITOR
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
    float v = cmd.substring(7).toFloat();   // Extrai o número após "remove "
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

// CONFIGURAÇÃO INICIAL DO SISTEMA
void setup() {
  Serial.begin(9600);
  dht.begin();

  Serial.println("Sistema iniciado!");
  mostrarMenu();
}

unsigned long lastRead = 0;

// LOOP PRINCIPAL: LER SENSOR E TRATAR COMANDOS
void loop() {

  // Lê a temperatura a cada 2 segundos
  if (millis() - lastRead >= 2000) {
    float t = dht.readTemperature();

    if (!isnan(t)) {
      insertValue(t);   // Salva a leitura na lista ordenada
      Serial.print("Temperatura lida: ");
      Serial.println(t);
    } else {
      Serial.println("Erro ao ler o DHT22");
    }

    lastRead = millis();
  }

  // Processa comandos digitados no Serial Monitor
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    processCommand(cmd);
  }
}
