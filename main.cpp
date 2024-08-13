#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

void DFS(int v, vector<bool>& visited, const vector<vector<int>>& adj) {
    // Marca o vértice atual como visitado
    visited[v] = true;

    // Visita todos os vértices adjacentes não visitados
    for (int u : adj[v]) {
        if (!visited[u]) {
            DFS(u, visited, adj);
        }
    }
}

struct Aresta {
    int id;
    int origem;
    int destino;
    int peso;
};

class Grafo {
public:
    int numVertices;
    int numArestas;
    bool direcionado;
    vector<Aresta> arestas;

    Grafo(int v, int a, bool dir) : numVertices(v), numArestas(a), direcionado(dir) {}

    void adicionarAresta(int id, int origem, int destino, int peso) {
        Aresta aresta = {id, origem, destino, peso};
        arestas.push_back(aresta);
    }

    void verificarConexo() {
    // Cria a lista de adjacência
    vector<vector<int>> adj(numVertices);
    vector<vector<int>> adjReverso(numVertices);

    for (const auto& aresta : arestas) {
        adj[aresta.origem].push_back(aresta.destino);
        adjReverso[aresta.destino].push_back(aresta.origem);
        if (!direcionado) {
            adj[aresta.destino].push_back(aresta.origem);
            adjReverso[aresta.origem].push_back(aresta.destino);
        }
    }

    vector<bool> visited(numVertices, false);

    // Encontra o primeiro vértice com arestas
    int start_vertex = -1;
    for (int i = 0; i < numVertices; ++i) {
        if (!adj[i].empty()) {
            start_vertex = i;
            break;
        }
    }

    // Se não houver arestas, o grafo é considerado desconexo
    if (start_vertex == -1) {
        cout << "O grafo não é conexo (não há arestas)." << endl;
        return;
    }

    // Realiza DFS a partir do primeiro vértice encontrado
    DFS(start_vertex, visited, adj);

    // Verifica se todos os vértices com arestas foram visitados na DFS original
    for (int i = 0; i < numVertices; ++i) {
        if (!visited[i] && !adj[i].empty()) {
            cout << "O grafo não é conexo." << endl;
            return;
        }
    }

    // Reseta o vetor de visitados e realiza DFS no grafo reverso
    fill(visited.begin(), visited.end(), false);
    DFS(start_vertex, visited, adjReverso);

    // Verifica se todos os vértices com arestas foram visitados na DFS reversa
    for (int i = 0; i < numVertices; ++i) {
        if (!visited[i] && !adj[i].empty()) {
            cout << "O grafo tem conectividade fraca." << endl;
            return;
        }
    }

    cout << "O grafo é conexo." << endl;

    }

    void verificarBipartido() {
        // Implementação da verificação se o grafo é bipartido
        cout << "Verificando se o grafo é bipartido..." << endl;
    }

    void verificarEuleriano() {
        // Implementação da verificação se o grafo é Euleriano
        cout << "Verificando se o grafo é Euleriano..." << endl;
    }

    void verificarCiclo() {
        // Implementação da verificação se o grafo possui ciclo
        cout << "Verificando se o grafo possui ciclo..." << endl;
    }

    void calcularComponentesConexas() {
        // Implementação do cálculo de componentes conexas
        cout << "Calculando a quantidade de componentes conexas..." << endl;
    }

    void calcularComponentesFortementeConexas() {
        // Implementação do cálculo de componentes fortemente conexas
        cout << "Calculando a quantidade de componentes fortemente conexas..." << endl;
    }

    void imprimirVerticesArticulacao() {
        // Implementação da impressão dos vértices de articulação
        cout << "Imprimindo os vértices de articulação..." << endl;
    }

    void calcularArestasPonte() {
        // Implementação do cálculo das arestas ponte
        cout << "Calculando as arestas ponte..." << endl;
    }

    void imprimirArvoreProfundidade() {
        // Implementação da impressão da árvore em profundidade
        cout << "Imprimindo a árvore em profundidade..." << endl;
    }

    void imprimirArvoreLargura() {
        // Implementação da impressão da árvore em largura
        cout << "Imprimindo a árvore em largura..." << endl;
    }

    void calcularArvoreGeradoraMinima() {
        // Implementação do cálculo da árvore geradora mínima
        cout << "Calculando a árvore geradora mínima..." << endl;
    }

    void ordenarTopologicamente() {
        // Implementação da ordenação topológica
        cout << "Imprimindo a ordenação topológica..." << endl;
    }

    void calcularCaminhoMinimo() {
        // Implementação do cálculo do caminho mínimo
        cout << "Calculando o caminho mínimo..." << endl;
    }

    void calcularFluxoMaximo() {
        // Implementação do cálculo do fluxo máximo
        cout << "Calculando o fluxo máximo..." << endl;
    }

    void calcularFechoTransitivo() {
        // Implementação do cálculo do fecho transitivo
        cout << "Calculando o fecho transitivo..." << endl;
    }
};

int main() {
    string entrada;
    do {
        cout << "\nMenu de opções:" << endl;
        cout << "1. Verificar se o grafo é conexo" << endl;
        cout << "2. Verificar se o grafo é bipartido" << endl;
        cout << "3. Verificar se o grafo é Euleriano" << endl;
        cout << "4. Verificar se o grafo possui ciclo" << endl;
        cout << "5. Calcular a quantidade de componentes conexas" << endl;
        cout << "6. Calcular a quantidade de componentes fortemente conexas" << endl;
        cout << "7. Imprimir os vértices de articulação" << endl;
        cout << "8. Calcular quantas arestas ponte possui o grafo" << endl;
        cout << "9. Imprimir a árvore em profundidade" << endl;
        cout << "10. Imprimir a árvore em largura" << endl;
        cout << "11. Calcular o valor final de uma árvore geradora mínima" << endl;
        cout << "12. Imprimir a ordenação topológica" << endl;
        cout << "13. Calcular o caminho mínimo entre dois vértices" << endl;
        cout << "14. Calcular o valor do fluxo máximo" << endl;
        cout << "15. Calcular o fecho transitivo" << endl;
        cout << "0. Sair" << endl;

        getline(cin, entrada);

        stringstream ss(entrada);
        int opcao;

        if (!entrada.empty() && entrada != "0") {
            int v, a;
            string direcionado;

            getline(cin, entrada);
            stringstream(entrada) >> v >> a;

            getline(cin, direcionado);

            bool isDirecionado = (direcionado == "direcionado");

            Grafo grafo(v, a, isDirecionado);

            for (int i = 0; i < a; ++i) {
                int id, origem, destino, peso;
                getline(cin, entrada);
                stringstream(entrada) >> id >> origem >> destino >> peso;
                grafo.adicionarAresta(id, origem, destino, peso);
            }

            while (ss >> opcao) {
                switch (opcao) {
                    case 1: grafo.verificarConexo(); break;
                    case 2: grafo.verificarBipartido(); break;
                    case 3: grafo.verificarEuleriano(); break;
                    case 4: grafo.verificarCiclo(); break;
                    case 5: grafo.calcularComponentesConexas(); break;
                    case 6: grafo.calcularComponentesFortementeConexas(); break;
                    case 7: grafo.imprimirVerticesArticulacao(); break;
                    case 8: grafo.calcularArestasPonte(); break;
                    case 9: grafo.imprimirArvoreProfundidade(); break;
                    case 10: grafo.imprimirArvoreLargura(); break;
                    case 11: grafo.calcularArvoreGeradoraMinima(); break;
                    case 12: grafo.ordenarTopologicamente(); break;
                    case 13: grafo.calcularCaminhoMinimo(); break;
                    case 14: grafo.calcularFluxoMaximo(); break;
                    case 15: grafo.calcularFechoTransitivo(); break;
                    case 0: cout << "Saindo..." << endl; break;
                    default: cout << "Opção inválida!" << endl; break;
                }
            }
        }

    } while (entrada != "0");

    return 0;
}

