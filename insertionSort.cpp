#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <iomanip>
#include <cmath>
using namespace std;

class SortedList {
private:
    vector<double> data;

public:
    void insertValue(double value) {
        auto pos = lower_bound(data.begin(), data.end(), value);
        data.insert(pos, value);
    }

    void removeValue(double value) {
        const double EPS = 0.01;

        for (auto it = data.begin(); it != data.end(); ++it) {
            if (fabs(*it - value) <= EPS) {
                data.erase(it);
                cout << "Valor removido com sucesso!\n";
                return;
            }
        }

        cout << "Valor nao encontrado.\n";
    }

    void printSorted() const {
        if (data.empty()) {
            cout << "Lista vazia.\n";
            return;
        }
        for (double v : data) cout << v << " ";
        cout << "\n";
    }

    void minValues(int k) const {
        if (data.empty()) {
            cout << "Lista vazia.\n";
            return;
        }
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
        for (int i = data.size() - k; i < (int)data.size(); i++)
            cout << data[i] << " ";
        cout << "\n";
    }

    void rangeQuery(double x, double y) const {
        if (data.empty()) {
            cout << "Lista vazia.\n";
            return;
        }
        auto it = lower_bound(data.begin(), data.end(), x);
        while (it != data.end() && *it <= y) {
            cout << *it << " ";
            it++;
        }
        cout << "\n";
    }

    double median() const {
        if (data.empty()) return 0;
        int n = data.size();
        if (n % 2 == 1) return data[n/2];
        return (data[n/2 - 1] + data[n/2]) / 2.0;
    }
};

int main() {
    SortedList lista;
    cout << fixed << setprecision(2);

    cout << "A lista inicia vazia. Insira valores manualmente.\n";

    int opcao;
    do {
        cout << "\n===== MENU DE CONSULTA =====\n"
             << "1 - Mostrar todos os valores ordenados\n"
             << "2 - Mostrar menores k valores\n"
             << "3 - Mostrar maiores k valores\n"
             << "4 - Buscar valores no intervalo [x, y]\n"
             << "5 - Mostrar mediana atual\n"
             << "6 - Inserir valor manualmente\n"
             << "7 - Remover valor manualmente\n"
             << "0 - Sair\n"
             << "Escolha uma opcao: ";
        cin >> opcao;

        if (!cin) { cin.clear(); cin.ignore(999, '\n'); continue; }

        switch (opcao) {
            case 1:
                cout << "\nValores ordenados:\n";
                lista.printSorted();
                break;

            case 2: {
                int k;
                cout << "Quantos valores menores deseja ver? ";
                cin >> k;
                lista.minValues(k);
                break;
            }

            case 3: {
                int k;
                cout << "Quantos valores maiores deseja ver? ";
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
                cout << "Digite o valor a inserir: ";
                cin >> valor;
                lista.insertValue(valor);
                cout << "Valor inserido!\n";
                break;
            }

            case 7: {
                double valor;
                cout << "Digite o valor a remover: ";
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
