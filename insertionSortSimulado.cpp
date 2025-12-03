#include <iostream>     // Entrada e saída padrão
#include <vector>       // Vetor dinâmico para armazenar os valores
#include <algorithm>    // lower_bound (busca binária)
#include <random>       // Gerador de números aleatórios
#include <ctime>        // time() para seed do RNG
#include <iomanip>      // Formatação de casas decimais
using namespace std;

// =====================================================================
// Classe SortedList — mantém todos os valores sempre ordenados
// =====================================================================
class SortedList {
private:
    vector<double> data;  // Armazena os valores em ordem crescente

public:

    // --------------------------------------------------------
    // Insere um valor usando busca binária para manter ordem
    // --------------------------------------------------------
    void insertValue(double value) {
        // lower_bound retorna a primeira posição onde 'value'
        // pode ser inserido mantendo o vetor ordenado
        auto pos = lower_bound(data.begin(), data.end(), value);
        data.insert(pos, value);
    }

    // --------------------------------------------------------
    // Remove um valor considerando tolerância para números reais
    // --------------------------------------------------------
    void removeValue(double value) {
        const double EPS = 0.01; // tolerância de 0.01 para comparação

        // Percorre procurando um valor "aproximadamente igual"
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (fabs(*it - value) <= EPS) {  // diferença menor que EPS?
                data.erase(it);              // remove o elemento
                cout << "Valor removido com sucesso!\n";
                return;
            }
        }

        // Se nenhum valor foi encontrado dentro da tolerância
        cout << "Valor nao encontrado (diferenca muito grande).\n";
    }

    // --------------------------------------------------------
    // Exibe todos os valores ordenados
    // --------------------------------------------------------
    void printSorted() const {
        if (data.empty()) {
            cout << "Lista vazia.\n";
            return;
        }

        for (double v : data) cout << v << " ";
        cout << "\n";
    }

    // --------------------------------------------------------
    // Exibe as k menores temperaturas
    // --------------------------------------------------------
    void minValues(int k) const {
        if (data.empty()) {
            cout << "Lista vazia.\n";
            return;
        }

        // Garante que k não ultrapasse o tamanho da lista
        k = min(k, (int)data.size());

        for (int i = 0; i < k; i++)
            cout << data[i] << " ";
        cout << "\n";
    }

    // --------------------------------------------------------
    // Exibe as k maiores temperaturas
    // --------------------------------------------------------
    void maxValues(int k) const {
        if (data.empty()) {
            cout << "Lista vazia.\n";
            return;
        }

        k = min(k, (int)data.size());

        // Começa do fim e exibe k elementos
        for (int i = data.size() - k; i < (int)data.size(); i++)
            cout << data[i] << " ";
        cout << "\n";
    }

    // --------------------------------------------------------
    // Busca valores dentro do intervalo [x, y]
    // --------------------------------------------------------
    void rangeQuery(double x, double y) const {
        if (data.empty()) {
            cout << "Lista vazia.\n";
            return;
        }

        // lower_bound encontra o primeiro valor >= x
        auto it = lower_bound(data.begin(), data.end(), x);

        // percorre até ultrapassar o limite y
        while (it != data.end() && *it <= y) {
            cout << *it << " ";
            it++;
        }
        cout << "\n";
    }

    // --------------------------------------------------------
    // Calcula a mediana dos valores
    // --------------------------------------------------------
    double median() const {
        if (data.empty()) return 0; // evita erro se lista estiver vazia

        int n = data.size();

        // Caso ímpar: valor do meio
        if (n % 2 == 1)
            return data[n/2];

        // Caso par: média dos dois do meio
        return (data[n/2 - 1] + data[n/2]) / 2.0;
    }
};

// =====================================================================
// Função principal
// =====================================================================
int main() {
    SortedList lista;                       // objeto da lista ordenada
    cout << fixed << setprecision(2);       // imprime sempre 2 casas decimais

    // -----------------------------------------------------------------
    // Gerando 100 temperaturas aleatórias entre -10 e 50 graus
    // -----------------------------------------------------------------
    mt19937 rng(time(nullptr));             // inicializa RNG com seed atual
    uniform_real_distribution<double> dist(-10.0, 50.0);

    // Insere 100 valores simulados na lista
    for (int i = 0; i < 100; i++) {
        double temp = dist(rng);
        lista.insertValue(temp);
    }

    cout << "Foram geradas 100 leituras de temperatura!\n";

    int opcao;
    do {
        // ------------------- MENU ----------------------
        cout << "\n===== MENU DE CONSULTA =====\n"
             << "1 - Mostrar todas as temperaturas ordenadas\n"
             << "2 - Mostrar n menores temperaturas\n"
             << "3 - Mostrar n maiores temperaturas\n"
             << "4 - Buscar temperaturas no intervalo [x, y]\n"
             << "5 - Mostrar mediana atual\n"
             << "6 - Inserir valor manualmente\n"
             << "7 - Remover valor manualmente\n"
             << "0 - Sair\n"
             << "Escolha uma opcao: ";

        cin >> opcao;

        // Se o usuário digitar algo inválido
        if (!cin) {
            cin.clear();           // limpa o erro
            cin.ignore(999, '\n'); // descarta entrada inválida
            continue;
        }

        // ------------------- TRATAMENTO DAS OPÇÕES ----------------------
        switch (opcao) {

            case 1:
                cout << "\nTemperaturas ordenadas:\n";
                lista.printSorted();
                break;

            case 2: {
                int k;
                cout << "Quantas temperaturas menores deseja ver? ";
                cin >> k;
                lista.minValues(k);
                break;
            }

            case 3: {
                int k;
                cout << "Quantas temperaturas maiores deseja ver? ";
                cin >> k;
                lista.maxValues(k);
                break;
            }

            case 4: {
                double x, y;
                cout << "Digite o intervalo [x y]: ";
                cin >> x >> y;
                lista.rangeQuery(x, y);
                break;
            }

            case 5:
                cout << "Mediana atual: " << lista.median() << " \n";
                break;

            case 6: {
                double valor;
                cout << "Digite o valor para inserir: ";
                cin >> valor;
                lista.insertValue(valor);
                cout << "Valor inserido!\n";
                break;
            }

            case 7: {
                double valor;
                cout << "Digite o valor para remover: ";
                cin >> valor;
                lista.removeValue(valor);
                break;
            }

            case 0:
                cout << "Encerrando...\n";
                break;

            default:
                cout << "Opcao invalida!\n";
        }

    } while (opcao != 0); // Repete até o usuário escolher sair

    return 0;
}

