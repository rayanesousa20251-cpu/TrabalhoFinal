# Trabalho Final — Estruturas de Dados Aplicadas a Sensores de Temperatura

Este repositório contém o código e o relatório final do projeto de **Estruturas de Dados Aplicadas a Sensores de Temperatura**, realizado na **Universidade Federal do Rio de Janeiro (UFRJ)**, pelos alunos **Alex Liu Li, Caio Guilherme de Oliveira Freire, Gabriel Linhares Motta e Rayane Santos**, sob a orientação do **Prof. Cláudio Miceli**.

O projeto compara o desempenho de duas estruturas de dados — **Lista Ordenada** e **Árvore Rubro-Negra** — no armazenamento e processamento de leituras reais e simuladas do sensor **DHT22**, em um cenário que simula o monitoramento em tempo real em ambientes industriais.

---

## 1. Resumo e Motivação

Sistemas de monitoramento industrial exigem o registro e a consulta rápida de dados de sensores. Estruturas simples, como a **Lista Ordenada**, sofrem com a degradação de desempenho em grandes volumes de leituras (custo `O(n)`), limitando sua escalabilidade.

**Comparação:**

- **Versão Básica: Lista Ordenada (InsertionSort):** Simples, mas com eficiência linear para inserção e remoção.  
- **Versão Aprimorada: Árvore Rubro-Negra:** Estrutura auto-balanceada que garante operações em tempo logarítmico (`O(log n)`), ideal para sistemas em tempo real e escaláveis.

**Objetivo:** Avaliar o custo-benefício entre simplicidade e eficiência para o processamento contínuo de dados de temperatura e umidade.

---

## 2. Descrição do Problema e da Escolha da Solução

O objetivo principal é armazenar leituras do sensor **DHT22** e executar operações essenciais para o monitoramento (**inserção, remoção, consulta por intervalo, mediana, min/max**) de forma eficiente.

### Versão Básica: Lista Ordenada (InsertionSort)

- Mantém um vetor sempre ordenado.  
- Operações de inserção e remoção têm custo linear (`O(n)`) no pior caso.  
- Limitações em cenários de alto volume de dados.

### Versão Aprimorada: Árvore Rubro-Negra

- Estrutura de busca balanceada que garante custo logarítmico (`O(log n)`) para inserção, remoção e busca.  
- Ideal para processamento contínuo de dados em tempo real.  

---

## 3. Descrição Técnica das Estruturas de Dados

### 3.1 Lista Ordenada

- **Conceito:** Mantém elementos em ordem crescente.  
- **Complexidade de Tempo (Pior Caso):**  
  - Inserção e Remoção: `O(n)`  
  - Mediana: `O(1)` (acesso direto)

### 3.2 Árvore Rubro-Negra

- **Conceito:** Árvore Binária de Busca Auto-Balanceável com cores nos nós (vermelho ou preto) e operações de rotação/recoloração.  
- **Complexidade de Tempo (Pior Caso):**  
  - Inserção, Remoção e Busca: `O(log n)`  
- **Regras Estruturais:**  
  - Raiz sempre preta  
  - Não pode haver dois nós vermelhos consecutivos  
  - Todos os caminhos da raiz até folhas nulas contêm o mesmo número de nós pretos

---

## 4. Metodologia de Comparação

Experimentos realizados em **Desktop (C++)** e **Arduino**, com leituras simuladas e reais do **DHT22**.

| Operação        | Lista Ordenada (Pior Caso) | Árvore Rubro-Negra (Pior Caso) |
|-----------------|----------------------------|--------------------------------|
| Inserção        | O(n)                       | O(log n)                       |
| Remoção         | O(n)                       | O(log n)                       |
| Range Query     | O(log n) + O(k)            | O(log n) + O(k)                |
| Mediana         | O(1)                       | O(log n)                        |


> k = número de elementos no intervalo de consulta

**Resultados Chave:**

- Inserção: Árvore Rubro-Negra escalável, Lista Ordenada cresce linearmente, chegando a 920ms para 100.000 inserções.  
- Range Query: Árvore Rubro-Negra mais rápida e estável em grandes volumes.

| N       | Lista Ordenada (ms) | Árvore Rubro-Negra (ms) |
|---------|-------------------|-------------------------|
| 100.000 | 920               | 1.4                     |

**Conclusão:** Árvore Rubro-Negra recomendada para monitoramento contínuo escalável.

---

## 5. Resultados Experimentais

### 5.1 Tempo de Inserção

| N       | Lista Ordenada (ms) | Árvore Rubro-Negra (ms) |
|---------|-------------------|-------------------------|
| 100     | 0.3               | 0.4                     |
| 1.000   | 4.8               | 0.8                     |
| 10.000  | 67                | 1.0                     |
| 100.000 | 920               | 1.4                     |

**Análise:** Lista Ordenada degrada linearmente; Árvore Rubro-Negra mantém desempenho quase constante.

### 5.2 Tempo de Consulta (Range Query)

| N       | Lista Ordenada (ms) | Árvore Rubro-Negra (ms) |
|---------|-------------------|-------------------------|
| 100     | 0.1               | 0.2                     |
| 1.000   | 0.7               | 0.2                     |
| 10.000  | 9.3               | 0.3                     |
| 100.000 | 112               | 0.5                     |

**Análise:** Árvore Rubro-Negra significativamente mais rápida em grandes volumes de dados.

## Resultados Experimentais

### Tempo de Inserção
![Gráfico de tempo de inserção](grafico_insercao.png)

### Tempo de Consulta (Range Query)
![Gráfico de Range Query](grafico_range.png)


---

## 6. Organização do Código e Funções Principais

### Arquivos

| Nome do Arquivo         | Plataforma  | Estrutura de Dados      | Descrição                               |
|-------------------------|------------|------------------------|-----------------------------------------|
| insertionSort.cpp       | C++        | Lista Ordenada         | Implementação e testes de desempenho    |
| insertionSortSensor.ino | Arduino    | Lista Ordenada         | Arduino + DHT22 usando Lista Ordenada   |
| rubroNegra.cpp          | C++        | Árvore Rubro-Negra     | Implementação e testes de desempenho    |
| rubroNegraSensor.ino    | Arduino    | Árvore Rubro-Negra     | Arduino + DHT22, menu interativo       |

### Funções

- **Lista Ordenada:** `insertValue()`, `removeValue()`, `getMedian()`, `getMin()`, `getMax()`, `queryRange()`, `print()`, `clearList()`.
  
- **Árvore Rubro-Negra:** `insertValue()+fixInsert()`, `removeValue()+fixDelete()`, `inorder()`, `search()`, `minimum()`, `maximum()`, `getMedian()`.

### Execução

**PC (C++):**

```bash
g++ insertionSort.cpp -o insertionSort.exe

### Arduino

- Selecione a porta e faça upload do arquivo `.ino` (ex: `rubroNegraSensor.ino`).  
- Abra o monitor serial (115200 baud).

---

## 7. Uso de Ferramentas de IA Generativa

- Auxílio na revisão e padronização do relatório e comentários do código.  
- Ajuda na explicação de conceitos técnicos e formatação LaTeX (tabelas e gráficos).  
- Sugestões para estruturação e refactoring de código.

---

## 8. Referências

- Cormen, T. et al. *Introduction to Algorithms*.  
- Goodrich, M.; Tamassia, R. *Data Structures and Algorithms in C++*.  
- Datasheet do sensor DHT22.  
- Documentação oficial Arduino e STL C++.
.


