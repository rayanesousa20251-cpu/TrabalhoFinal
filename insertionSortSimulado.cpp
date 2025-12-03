#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <iomanip>
using namespace std;

class SortedList {
private:
    vector<double> data;  // Armazena os valores ordenados

public:

    void insertValue(double value) {
        // Encontra a posição correta para manter a ordem
        auto pos = lower_bound(data.begin(), data.end(), value);
        data.insert(pos, value);
    }

    void removeValue(double value) {
        // Tolerância para comparar valores decimais
        const double EPS = 0.01;

        // Procura por um valor aproximado ao informado
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (fabs(*it - value) <= EPS) {
                data.erase(it);  // Remove o valor
                cout << "Valor removido com sucesso!\n";
                return;
            }
        }

        cout << "Valor nao encontrado (diferenca muito grande).\n";
    }

    void printSorted() const {
        // Verifica se a lista está vazia
        if (data.empty()) {
            cout << "Lista vazia.\n";
            return;
        }

        // Imprime todos os valores
        for (double v : data) cout << v << " ";
        cout << "\n";
    }

    void minValues(int k) const {
        if (data.empty()) {
            cout << "Lista vazia.\n";
            return;
        }

        // Impede que k seja maior que o tamanho
        k = min(k, (int)data.size());

        for (int i = 0; i < k; i++) cout << data[i] << " ";
        cout << "\n";
    }

    void maxValues(int k) const {
        if (data.empty()) {
            cout << "Lista vazia.\n";
            return;
        }

        k = min(k, (int)data.size());

        // Imprime os últimos k valores
        for (int i = data.size() - k; i < (int)data.size(); i++)
            cout << data[i] << " ";
        cout << "\n";
    }

    void rangeQuery(double x, double y) const {
        if (data.empty()) {
            cout << "Lista vazia.\n";
            return;
        }

        // Encontra o primeiro valor >= x
        auto it = lower_bound(data.begin(), data.end(), x);

        // Imprime valores até ultrapassar y
        while (it != data.end() && *it <= y) {
            cout << *it << " ";
            it++;
        }
        cout << "\n";
    }

    double median() const {
        if (data.empty()) return 0;

        int n = data.size();

        // Caso ímpar
        if (n % 2 == 1) return data[n/2];

        // Caso par
        return (data[n/2 - 1] + data[n/2]) / 2.0;
    }
};

int main() {
    SortedList lista;
    cout << fixed << setprecision(2);  // Define 2 casas decimais

    // Gerador de números aleatórios
    mt19937 rng(time(nullptr));
    uniform_real_distribution<double> dist(-10.0, 50.0);

    // Gera 100 temperaturas
    for (int i = 0; i < 100; i++) {
        double temp = dist(rng);
        lista.insertValue(temp);
    }

    cout << "Foram geradas 100 leituras de temperatura!\n";

    int opcao;
    do {
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

        // Limpa entrada inválida
        if (!cin) {
            cin.clear();
            cin.ignore(999, '\n');
            continue;
        }

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

    } while (opcao != 0);

    return 0;
}


