#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>

using namespace std;

void DFS(int v, vector<bool>& visited, const vector<vector<int>>& adj) {
    visited[v] = true;
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
        int start_vertex = -1;
        for (int i = 0; i < numVertices; ++i) {
            if (!adj[i].empty()) {
                start_vertex = i;
                break;
            }
        }

        if (start_vertex == -1) {
            cout << "O grafo não é conexo (não há arestas)." << endl;
            return;
        }

        DFS(start_vertex, visited, adj);

        for (int i = 0; i < numVertices; ++i) {
            if (!visited[i] && !adj[i].empty()) {
                cout << "O grafo não é conexo." << endl;
                return;
            }
        }

        fill(visited.begin(), visited.end(), false);
        DFS(start_vertex, visited, adjReverso);

        for (int i = 0; i < numVertices; ++i) {
            if (!visited[i] && !adj[i].empty()) {
                cout << "O grafo tem conectividade fraca." << endl;
                return;
            }
        }

        cout << "O grafo é conexo." << endl;
    }

    void verificarBipartido() {
        vector<int> cor(numVertices, -1); // -1 indica que o vértice ainda não foi colorido
        bool ehBipartido = true;

        for (int i = 0; i < numVertices; ++i) {
            if (cor[i] == -1) { // Se o vértice ainda não foi colorido
                if (!bipartidoDFS(i, cor)) {
                    ehBipartido = false;
                    break;
                }
            }
        }

        if (ehBipartido) {
            cout << "1" << endl; // Grafo é bipartido
        } else {
            cout << "0" << endl; // Grafo não é bipartido
        }
    }

    bool bipartidoDFS(int v, vector<int>& cor) {
        if (cor[v] == -1) {
            cor[v] = 0; // Comece colorindo o vértice com a cor 0
        }

        for (const Aresta& aresta : arestas) {
            if (aresta.origem == v || aresta.destino == v) {
                int u = (aresta.origem == v) ? aresta.destino : aresta.origem;
                if (cor[u] == -1) { // Se o vértice ainda não foi colorido
                    cor[u] = 1 - cor[v]; // Pinte com a cor oposta
                    if (!bipartidoDFS(u, cor)) {
                        return false;
                    }
                } else if (cor[u] == cor[v]) {
                    return false; // Encontrou dois vértices adjacentes com a mesma cor
                }
            }
        }

        return true;
    }

    void verificarEuleriano() {
        cout << "Verificando se o grafo é Euleriano..." << endl;
    }

    void verificarCiclo() {
        cout << "Verificando se o grafo possui ciclo..." << endl;
    }

    void calcularComponentesConexas() {
        vector<vector<int>> adj(numVertices);
        for (const auto& aresta : arestas) {
            adj[aresta.origem].push_back(aresta.destino);
            if (!direcionado) {
                adj[aresta.destino].push_back(aresta.origem);
            }
        }

        vector<bool> visitado(numVertices, false);
        int componentes = 0;

        for (int i = 0; i < numVertices; ++i) {
            if (!visitado[i]) {
                DFS(i, visitado, adj);
                componentes++;
            }
        }

        cout << componentes << endl;
    }

    void calcularComponentesFortementeConexas() {
        cout << "Calculando a quantidade de componentes fortemente conexas..." << endl;
    }

    void imprimirVerticesArticulacao() {
        cout << "Imprimindo os vértices de articulação..." << endl;
    }

    void calcularArestasPonte() {
        vector<vector<int>> adj(numVertices);
        for (const auto& aresta : arestas) {
            adj[aresta.origem].push_back(aresta.destino);
            if (!direcionado) {
                adj[aresta.destino].push_back(aresta.origem);
            }
        }

        vector<int> disc(numVertices, -1), low(numVertices, -1);
        vector<int> pai(numVertices, -1);
        vector<pair<int, int>> bridges;
        int time = 0;

        function<void(int)> bridgeDFS = [&](int u) {
            disc[u] = low[u] = ++time;
            for (int v : adj[u]) {
                if (disc[v] == -1) { // Se v não foi visitado
                    pai[v] = u;
                    bridgeDFS(v);

                    low[u] = min(low[u], low[v]);

                    // Se o vértice mais baixo alcançável a partir de v está abaixo de u na árvore DFS
                    if (low[v] > disc[u]) {
                        bridges.emplace_back(u, v);
                    }
                } else if (v != pai[u]) {
                    // Atualizar low[u] considerando os vértices alcançados através de v
                    low[u] = min(low[u], disc[v]);
                }
            }

            // Debug: Impressão dos valores de disc[] e low[]
            cout << "Vértice: " << u << ", disc: " << disc[u] << ", low: " << low[u] << endl;
        };

        for (int i = 0; i < numVertices; ++i) {
            if (disc[i] == -1) {
                bridgeDFS(i);
            }
        }

        cout << "Número de arestas ponte: " << bridges.size() << endl;
        for (const auto& bridge : bridges) {
            cout << "Aresta ponte: " << bridge.first << " - " << bridge.second << endl;
        }
    }

    void imprimirArvoreProfundidade() {
        cout << "Imprimindo a árvore em profundidade..." << endl;
    }

    void imprimirArvoreLargura() {
        cout << "Imprimindo a árvore em largura..." << endl;
    }

    void calcularArvoreGeradoraMinima() {
        sort(arestas.begin(), arestas.end(), [](Aresta a, Aresta b) {
            return a.peso < b.peso;
        });

        vector<int> pai(numVertices);
        for (int i = 0; i < numVertices; ++i) {
            pai[i] = i;
        }

        function<int(int)> encontrar = [&](int u) {
            return pai[u] == u ? u : pai[u] = encontrar(pai[u]);
        };

        vector<Aresta> mst;
        for (const auto& aresta : arestas) {
            int u = encontrar(aresta.origem);
            int v = encontrar(aresta.destino);
            if (u != v) {
                mst.push_back(aresta);
                pai[u] = v;
            }
        }

        cout << "Arestas da Árvore Geradora Mínima:" << endl;
        for (const auto& aresta : mst) {
            cout << aresta.origem << " - " << aresta.destino << " : " << aresta.peso << endl;
        }
    }

    void ordenarTopologicamente() {
        cout << "Imprimindo a ordenação topológica..." << endl;
    }

    void calcularCaminhoMinimo() {
        cout << "Calculando o caminho mínimo..." << endl;
    }

    void calcularFluxoMaximo() {
        vector<vector<int>> capacidade(numVertices, vector<int>(numVertices, 0));
        vector<vector<int>> adj(numVertices);
        for (const auto& aresta : arestas) {
            capacidade[aresta.origem][aresta.destino] = aresta.peso;
            adj[aresta.origem].push_back(aresta.destino);
            adj[aresta.destino].push_back(aresta.origem);
        }

        auto bfs = [&](int s, int t, vector<int>& parent) {
            fill(parent.begin(), parent.end(), -1);
            queue<pair<int, int>> q;
            q.push({s, INT_MAX});
            while (!q.empty()) {
                int cur = q.front().first;
                int flow = q.front().second;
                q.pop();

                for (int next : adj[cur]) {
                    if (parent[next] == -1 && capacidade[cur][next]) {
                        parent[next] = cur;
                        int new_flow = min(flow, capacidade[cur][next]);
                        if (next == t) {
                            return new_flow;
                        }
                        q.push({next, new_flow});
                    }
                }
            }
            return 0;
        };

        int s = 0; // Fonte
        int t = numVertices - 1; // Sumidouro
        int fluxo_maximo = 0;
        vector<int> parent(numVertices);

        int new_flow;
        while ((new_flow = bfs(s, t, parent)) > 0) {
            fluxo_maximo += new_flow;
            int cur = t;
            while (cur != s) {
                int prev = parent[cur];
                capacidade[prev][cur] -= new_flow;
                capacidade[cur][prev] += new_flow;
                cur = prev;
            }
        }

        cout << "Fluxo máximo: " << fluxo_maximo << endl;
    }

    void calcularFechoTransitivo() {
        cout << "Calculando o fecho transitivo..." << endl;
    }
};

int main() {
    string entrada;
    do {
        cout << "\nMenu de opcoes:" << endl;
        cout << "1. Verificar se o grafo e conexo" << endl;
        cout << "2. Verificar se o grafo e bipartido" << endl;
        cout << "3. Verificar se o grafo e Euleriano" << endl;
        cout << "4. Verificar se o grafo possui ciclo" << endl;
        cout << "5. Calcular a quantidade de componentes conexas" << endl;
        cout << "6. Calcular a quantidade de componentes fortemente conexas" << endl;
        cout << "7. Imprimir os vertices de articulacao" << endl;
        cout << "8. Calcular quantas arestas ponte possui o grafo" << endl;
        cout << "9. Imprimir a arvore em profundidade" << endl;
        cout << "10. Imprimir a arvore em largura" << endl;
        cout << "11. Calcular o valor final de uma arvore geradora minima" << endl;
        cout << "12. Imprimir a ordenacao topologica" << endl;
        cout << "13. Calcular o caminho minimo entre dois vertices" << endl;
        cout << "14. Calcular o valor do fluxo maximo" << endl;
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
