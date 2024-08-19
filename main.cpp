#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <climits>
#include <set>
#include <queue>
#include <algorithm>
#include <limits>

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

// Essa DFS foi feita apenas para verificar se possui ciclos no grafo
bool DFS_Ciclo(int v, vector<bool>& visited, vector<bool>& recStack, const vector<vector<int>>& adj) {
    // Marca o vértice atual como visitado e o coloca na pilha de recursão
    visited[v] = true;
    recStack[v] = true;

    // Visita todos os vértices adjacentes
    for (int u : adj[v]) {
        if (!visited[u] && DFS_Ciclo(u, visited, recStack, adj)) {
            return true;
        } else if (recStack[u]) {
            return true;
        }
    }

    // Remove o vértice atual da pilha de recursão
    recStack[v] = false;
    return false;
}

// Algoritmo de Tarjan
void encontrarVerticesArticulacaoUtil(int v, vector<bool>& visited, vector<int>& disc, vector<int>& low,
                                      vector<int>& parent, vector<bool>& articulationPoints, vector<vector<int>>& adj) {
    static int time = 0;
    int children = 0;

    visited[v] = true;
    disc[v] = low[v] = ++time;

    for (int u : adj[v]) {
        if (!visited[u]) {
            children++;
            parent[u] = v;
            encontrarVerticesArticulacaoUtil(u, visited, disc, low, parent, articulationPoints, adj);

            low[v] = min(low[v], low[u]);

            // Se v não é a raiz e u não pode alcançar ancestrais de v, v é um ponto de articulação
            if (parent[v] != -1 && low[u] >= disc[v]) {
                articulationPoints[v] = true;
            }

            // Se v é a raiz e tem mais de um filho, é um ponto de articulação
            if (parent[v] == -1 && children > 1) {
                articulationPoints[v] = true;
            }
        } else if (u != parent[v]) {
            low[v] = min(low[v], disc[u]);
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
            cout << "0" << endl;
            return;
        }
    }

    // Reseta o vetor de visitados e realiza DFS no grafo reverso
    fill(visited.begin(), visited.end(), false);
    DFS(start_vertex, visited, adjReverso);

    // Verifica se todos os vértices com arestas foram visitados na DFS reversa
    for (int i = 0; i < numVertices; ++i) {
        if (!visited[i] && !adj[i].empty()) {
            cout << "1" << endl;
            return;
        }
    }

    cout << "1" << endl;

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
        vector<vector<int>> adj(numVertices);
        for(const auto& aresta : arestas) {
            adj[aresta.origem].push_back(aresta.destino);
            if (!direcionado) {
                adj[aresta.destino].push_back(aresta.origem);
            }
        }

        vector<bool> visited(numVertices, false);
        vector<bool> recStack(numVertices, false);

        for (int i = 0; i < numVertices; ++i) {
            if (!visited[i]) {
                if (DFS_Ciclo(i, visited, recStack, adj)) {
                    cout << '1' << endl;
                    return;
                }
            }
        }
        cout << '0' << endl;
    }

    void calcularComponentesConexas() {
       // implementar
    }


    void calcularComponentesFortementeConexas() {
        // implementar
    }


    void imprimirVerticesArticulacao() {
        if(!direcionado) {
            vector<vector<int>> adj(numVertices);
        
            for (const auto& aresta : arestas) {
                adj[aresta.origem].push_back(aresta.destino);
                if (!direcionado) {
                    adj[aresta.destino].push_back(aresta.origem);
                }
            }

            vector<bool> visited(numVertices, false);
            vector<int> disc(numVertices, -1);
            vector<int> low(numVertices, -1);
            vector<int> parent(numVertices, -1);
            vector<bool> articulationPoints(numVertices, false);

            for (int i = 0; i < numVertices; i++) {
                if (!visited[i]) {
                    encontrarVerticesArticulacaoUtil(i, visited, disc, low, parent, articulationPoints, adj);
                }
            }

            set<int> articulationVertices;
            for (int i = 0; i < numVertices; i++) {
                if (articulationPoints[i]) {
                    articulationVertices.insert(i);
                }
            }

            for (int vertex : articulationVertices) {
                cout << vertex << " ";
            }
            cout << endl;

        }else{
            cout << "-1" << endl;
        }
        
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
         vector<vector<pair<int, int>>> adj(numVertices); // Lista de adjacência com identificador de aresta

        for (const auto& aresta : arestas) {
            adj[aresta.origem].emplace_back(aresta.destino, aresta.id);
            if (!direcionado) {
                adj[aresta.destino].emplace_back(aresta.origem, aresta.id);
            }
        }

        vector<bool> visited(numVertices, false);
        queue<int> q;
        vector<int> ordemArestas;

        q.push(0);
        visited[0] = true;

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            // Ordena a lista de adjacência do vértice atual em ordem lexicográfica
            sort(adj[v].begin(), adj[v].end());

            for (const auto& [u, id] : adj[v]) {
                if (!visited[u]) {
                    visited[u] = true;
                    ordemArestas.push_back(id);
                    q.push(u);
                }
            }
        }

        for (int id : ordemArestas) {
            cout << id << " ";
        }
        cout << endl;

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
        vector<vector<pair<int, int>>> adj(numVertices); // Lista de adjacência com (destino, peso)

        for (const auto& aresta : arestas) {
            adj[aresta.origem].emplace_back(aresta.destino, aresta.peso);
            if (!direcionado) {
                adj[aresta.destino].emplace_back(aresta.origem, aresta.peso);
            }
        }

        // Vetor de distâncias, inicializado com infinito
        vector<int> dist(numVertices, numeric_limits<int>::max());
        dist[0] = 0;

        // Fila de prioridade (min-heap) para selecionar o vértice com a menor distância
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, 0}); // {distância, vértice}

        while (!pq.empty()) {
            int d = pq.top().first;
            int v = pq.top().second;
            pq.pop();

            if (d > dist[v]) continue;  

            for (const auto& [u, peso] : adj[v]) {
                if (dist[v] + peso < dist[u]) {
                    dist[u] = dist[v] + peso;
                    pq.push({dist[u], u});
                }
            }
        }
        cout << dist[numVertices - 1] << endl;
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

