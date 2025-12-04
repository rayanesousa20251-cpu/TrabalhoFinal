#include <iostream>
#include <vector>
#include <algorithm>
#include <random> // Adicionado para geração aleatória
#include <ctime>
#include <iomanip>
#include <cmath>
using namespace std;

// Classe que mantém uma lista ordenada
class SortedList {
private:
    vector<double> data; // Vetor onde os valores serão armazenados ordenadamente

public:

    // Insere um valor mantendo a ordem crescente
    void insertValue(double value) {
        // lower_bound encontra a posição correta para inserir mantendo a ordem
        auto pos = lower_bound(data.begin(), data.end(), value);
        data.insert(pos, value); // insere na posição certa
    }

    // Remove um valor aproximado (tolerância de +-0.01)
    void removeValue(double value) {
        const double EPS = 0.01; // margem de erro permitida

        // percorre todo o vetor procurando um valor próximo
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (fabs(*it - value) <= EPS) { // se for próximo o suficiente
                data.erase(it); // remove o valor
                cout << "Valor removido com sucesso!\n";
                return;
            }
        }
        cout << "Valor nao encontrado.\n";
    }

    // Imprime todos os valores já ordenados
    void printSorted() const {
        if (data.empty()) {
            cout << "Lista vazia.\n";
            return;
        }
        for (double v : data) cout << v << " "; // percorre e imprime
        cout << "\n";
    }

    // Mostra os menores k valores
    void minValues(int k) const {
        if (data.empty()) {
            cout << "Lista vazia.\n";
            return;
        }
        k = min(k, (int)data.size()); // evita pedir mais valores que o tamanho
        for (int i = 0; i < k; i++) cout << data[i] << " ";
        cout << "\n";
    }

    // Mostra os maiores k valores
    void maxValues(int k) const {
        if (data.empty()) {
            cout << "Lista vazia.\n";
            return;
        }
        k = min(k, (int)data.size());
        // começa no final menos k e vai até o final
        for (int i = data.size() - k; i < (int)data.size(); i++)
            cout << data[i] << " ";
        cout << "\n";
    }

    // Procura valores dentro de um intervalo [x, y]
    void rangeQuery(double x, double y) const {
        if (data.empty()) {
            cout << "Lista vazia.\n";
            return;
        }

        // encontra o primeiro valor >= x
        auto it = lower_bound(data.begin(), data.end(), x);

        // imprime enquanto o valor estiver dentro do intervalo
        while (it != data.end() && *it <= y) {
            cout << *it << " ";
            it++;
        }
        cout << "\n";
    }

    // Calcula a mediana dos valores
    double median() const {
        if (data.empty()) return 0;
        int n = data.size();

        if (n % 2 == 1)        // se for quantidade ímpar
            return data[n/2]; // mediana é o valor central

        // se for quantidade par: média dos dois do meio
        return (data[n/2 - 1] + data[n/2]) / 2.0;
    }

    // Esvazia toda a lista, deixando ela zerada
    void clearList() {
        data.clear(); // remove todos os elementos
        cout << "A lista foi esvaziada com sucesso!\n";
    }
};

int main() {
    SortedList lista; // cria a lista ordenada
    cout << fixed << setprecision(2); // formata saída para 2 casas decimais

    cout << "A lista inicia vazia. Insira valores manualmente ou simule sensores.\n";

    int opcao;
    do {
        // Menu de opções
        cout << "\n----- MENU DE CONSULTA -----\n"
             << "1 - Mostrar todos os valores ordenados\n"
             << "2 - Mostrar menores k valores\n"
             << "3 - Mostrar maiores k valores\n"
             << "4 - Buscar valores no intervalo [x, y]\n"
             << "5 - Mostrar mediana atual\n"
             << "6 - Inserir valor manualmente\n"
             << "7 - Remover valor manualmente\n"
             << "8 - Esvaziar lista completamente\n"
             << "9 - Simular Dados (Gerar Aleatorio)\n"
             << "0 - Sair\n"
             << "Escolha uma opcao: ";
        cin >> opcao;

        // Verifica erro de entrada
        if (!cin) {
            cin.clear();       // limpa erro
            cin.ignore(999, '\n'); // limpa buffer
            continue;
        }

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

            case 8:
                lista.clearList();
                break;
            
            case 9: { // NOVA OPÇÃO: GERAÇÃO ALEATÓRIA
                int qtd;
                cout << "Quantos valores deseja simular? ";
                cin >> qtd;
                
                // Configura gerador de numeros aleatorios
                random_device rd;
                mt19937 gen(rd());
                uniform_real_distribution<> dis(20.0, 45.0); // Gera entre 20 e 45 graus

                for(int i = 0; i < qtd; i++){
                    lista.insertValue(dis(gen));
                }
                cout << qtd << " valores simulados inseridos com sucesso!\n";
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
}



