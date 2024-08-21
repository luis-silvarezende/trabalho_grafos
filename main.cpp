/*
Luis Felipe Silva Rezende - 202120791
Miguel Chagas Figueiredo - 202220176
João Lucas Pereira Ramalho - 202220166
*/
#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <sstream>
#include <climits>
#include <set>
#include <queue>
#include <algorithm>
#include <limits>
#include <functional>

using namespace std;

/*
 Essa parte do código utiliza de vários algoritmos para o caminhamento em Grafos.
 Existe diferentes implementações para o mesmo algoritmo, porque ele é utilizado de
 forma expecifica para funções diferentes.
*/

// DFS apenas para as funções verificarConexo(), calcularComponentesConexas() e verificarEuleriano()
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

// DFS para a função imprimirArvoreProfundidade()
void dfs(int u, vector<bool>& visited, vector<vector<pair<int, int>>>& adj, vector<int>& arvoreProfundidade) {
    visited[u] = true;

    // Ordena os vizinhos para garantir a ordem lexicográfica
    sort(adj[u].begin(), adj[u].end());

    for (const auto& [v, id] : adj[u]) {
        if (!visited[v]) {
            arvoreProfundidade.push_back(id);
            dfs(v, visited, adj, arvoreProfundidade);
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
void encontrarVerticesArticulacaoUtil(int u, vector<bool>& visited, vector<int>& disc, vector<int>& low, vector<int>& parent, vector<bool>& articulationPoints, vector<vector<int>>& adj) {
    // Variável estática para manter o tempo de descoberta dos vértices
    static int time = 0;
    
    // Contador de filhos do vértice atual
    int children = 0;

    // Marcar o vértice atual como visitado
    visited[u] = true;
    
    // Inicializar o tempo de descoberta e o valor de low para o vértice atual
    disc[u] = low[u] = ++time;

    // Iterar sobre todos os vértices adjacentes ao vértice atual
    for (int v : adj[u]) {
        // Se o vértice v não foi visitado ainda
        if (!visited[v]) {
            // Incrementar o contador de filhos
            children++;
            
            // Definir o pai de v como u
            parent[v] = u;
            
            // Recursivamente aplicar a função para o vértice v
            encontrarVerticesArticulacaoUtil(v, visited, disc, low, parent, articulationPoints, adj);

            // Atualizar o valor de low[u] com o menor valor de low[v] encontrado
            low[u] = min(low[u], low[v]);

            // Verificar se o vértice u é um ponto de articulação
            // Caso u seja a raiz da árvore e tenha mais de um filho
            if (parent[u] == -1 && children > 1) {
                articulationPoints[u] = true;
            }
            
            // Caso u não seja a raiz e low[v] >= disc[u], então u é um ponto de articulação
            if (parent[u] != -1 && low[v] >= disc[u]) {
                articulationPoints[u] = true;
            }
        } 
        // Se o vértice v já foi visitado e não é o pai do vértice atual u
        else if (v != parent[u]) {
            // Atualizar o valor de low[u] com o valor de disc[v]
            low[u] = min(low[u], disc[v]);
        }
    }
}

/*
Função Utilitária encontrarArestasPontesUtil: Esta função realiza uma busca em profundidade (DFS)
para identificar as arestas que, se removidas, desconectariam o grafo.
*/

// DFS para as funções calcularComponentesFortementeConexas()
void DFSUtil(int v, vector<bool>& visited, const vector<vector<int>>& adj) {
    visited[v] = true;
    for (int u : adj[v]) {
        if (!visited[u]) {
            DFSUtil(u, visited, adj);
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
    vector<vector<int>> adj;

    Grafo(int v, int a, bool dir) : numVertices(v), numArestas(a), direcionado(dir) {
        adj.resize(numVertices);
    }

    // Cria uma aresta, e uma lista de adj "global".
    void adicionarAresta(int id, int origem, int destino, int peso) {
        Aresta aresta = {id, origem, destino, peso};
        arestas.push_back(aresta);
        adj[aresta.origem].push_back(aresta.destino);
            if (!direcionado) {
                adj[aresta.destino].push_back(aresta.origem);
            }
    }

    void verificarConexo() {
        vector<bool> visited(numVertices, false);

        // Para grafos direcionados, cria uma versão não direcionada do grafo.
        if (direcionado) {
            vector<vector<int>> undirectedAdj = adj;
            for (int u = 0; u < numVertices; ++u) {
                for (int v : adj[u]) {
                    undirectedAdj[v].push_back(u);  // Adiciona a aresta inversa para criar um grafo não direcionado
                }
            }
            DFS(0, visited, undirectedAdj);
        } else {
            DFS(0, visited, adj);
        }

        for (int i = 0; i < numVertices; ++i) {
            if (!visited[i]) {
                cout << "0";  // Grafo não é conexo
                return;
            }
        }
        cout << "1";  // Grafo é conexo ou fracamente conexo
    }

    void verificarBipartido() {
        cout << endl;
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

        cout << (ehBipartido ? "1" : "0");
    }

    bool bipartidoDFS(int v, vector<int>& cor) {
        cor[v] = 0; // Comece colorindo o vértice com a cor 0

        for (int u : adj[v]) {
            if (cor[u] == -1) {
                cor[u] == 1 - cor[v];
                if (!bipartidoDFS(u, cor)) return false; // Se o vértice ainda não foi colorido
                    cor[u] = 1 - cor[v]; // Pinte com a cor oposta
                } else if (cor[u] == cor[v]) {
                    return false; // Encontrou dois vértices adjacentes com a mesma cor
                }
        }

        return true;
    }

    void verificarEuleriano() {
        cout << endl;     
        // Verifica se o grafo é conexo
        vector<bool> visited(numVertices, false);
        DFS(0, visited, adj); // Realiza DFS a partir do vértice 0

        // Verifica se todos os vértices têm grau par
        for (int i = 0; i < numVertices; ++i) {
            if (!visited[i]) {
                cout << "0"; // O grafo não é conexo
                return;
            }
            if (adj[i].size() % 2 != 0) {
                cout << "0"; // Algum vértice tem grau ímpar
                return;
            }
        }

        // Se o grafo é conexo e todos os vértices têm grau par
        cout << "1";
    }

    void verificarCiclo() {
        cout << endl;
        // Vetor para marcar os vértices visitados
        vector<bool> visited(numVertices, false);
        // Vetor para rastrear o caminho atual na DFS para detectar ciclos
        vector<bool> recStack(numVertices, false);

        // Verifica todos os vértices do grafo
        for (int i = 0; i < numVertices; ++i) {
            // Se o vértice não foi visitado, realiza uma busca DFS
            if (!visited[i] && DFS_Ciclo(i, visited, recStack, adj)) {
                // Se a DFS encontrar um ciclo, imprime '1' e retorna
                    cout << '1';
                    return;
            }
        }
        
        // Se nenhum ciclo for encontrado, imprime '0'
        cout << '0';
    }

    void calcularComponentesConexas() {
        cout << endl;
        
        // Verifica se o grafo é direcionado
        if(direcionado){
            // Se o grafo é direcionado, imprime '-1' e retorna, pois o cálculo de componentes conexas
            // só é aplicável para grafos não direcionados
            cout << "-1";
            return;
        } 
        // Vetor para marcar os vértices visitados
        vector<bool> visitado(numVertices, false);
        int componentes = 0;

        // Verifica todos os vértices do grafo
        for (int i = 0; i < numVertices; ++i) {
            // Se o vértice não foi visitado, realiza uma busca DFS a partir desse vértice
            if (!visitado[i]) {
                DFS(i, visitado, adj);
                // Incrementa o contador de componentes conexas
                componentes++;
            }
        }

            // Imprime o número total de componentes conexas
            cout << componentes;
    }

    // Tarjan para calcular componentes fortemente conexas
    void tarjanDFS(int u, vector<int>& disc, vector<int>& low, vector<bool>& inStack, stack<int>& Stack, vector<vector<int>>& sccs) {
        static int time = 0;
        disc[u] = low[u] = ++time;
        Stack.push(u);
        inStack[u] = true;

        for (int v : adj[u]) {
            if (disc[v] == -1) {
                tarjanDFS(v, disc, low, inStack, Stack, sccs);
                low[u] = min(low[u], low[v]);
            } else if (inStack[v]) {
                low[u] = min(low[u], disc[v]);
            }
        }

        // Se u é um ponto de origem de uma SCC
        if (low[u] == disc[u]) {
            vector<int> component;
            while (Stack.top() != u) {
                int v = Stack.top();
                Stack.pop();
                inStack[v] = false;
                component.push_back(v);
            }
            Stack.pop();
            inStack[u] = false;
            component.push_back(u);
            sccs.push_back(component);
        }
    }

    void calcularComponentesFortementeConexas() {
        cout << endl;
        if(direcionado) {
            vector<int> disc(numVertices, -1);
            vector<int> low(numVertices, -1);
            vector<bool> inStack(numVertices, false);
            stack<int> Stack;
            vector<vector<int>> sccs;

            for (int i = 0; i < numVertices; ++i) {
                if (disc[i] == -1) {
                    tarjanDFS(i, disc, low, inStack, Stack, sccs);
                }
            }

            cout << sccs.size();
        }else{
            cout << "-1";
        }

    }

    void imprimirVerticesArticulacao() {
        cout << endl;
        
        // Verifica se o grafo é direcionado
        if (!direcionado) {
            // Cria uma lista de adjacência para o grafo não direcionado

            // Preenche a lista de adjacência com as arestas do grafo
            for (const auto& aresta : arestas) {
                adj[aresta.origem].push_back(aresta.destino);
                adj[aresta.destino].push_back(aresta.origem);
            }

            // Vetores necessários para a busca de vértices de articulação
            vector<bool> visited(numVertices, false);
            vector<int> disc(numVertices, -1);
            vector<int> low(numVertices, -1);
            vector<int> parent(numVertices, -1);
            vector<bool> articulationPoints(numVertices, false);

            // Realiza a busca DFS para encontrar vértices de articulação
            for (int i = 0; i < numVertices; i++) {
                if (!visited[i]) {
                    encontrarVerticesArticulacaoUtil(i, visited, disc, low, parent, articulationPoints, adj);
                }
            }

            // Verifica se existem vértices de articulação e os imprime
            bool hasArticulationPoint = false;
            for (int i = 0; i < numVertices; i++) {
                if (articulationPoints[i]) {
                    cout << i << " ";
                    hasArticulationPoint = true;
                }
            }

            // Se não houver vértices de articulação, imprime "0"
            if (!hasArticulationPoint) {
                cout << "0";
            }

        } else {
            // Se o grafo for direcionado, imprime "-1"
            cout << "-1";
        }
    }

    void dfsTarjan(int u, vector<int>& disc, vector<int>& low, vector<int>& parent, vector<pair<int, int>>& pontes, int& time) {
    disc[u] = low[u] = ++time;

        for (int v : adj[u]) {
            if (disc[v] == -1) { // Se o vértice v não foi visitado
                parent[v] = u;
                dfsTarjan(v, disc, low, parent, pontes, time);

                // Atualiza o low value do u
                low[u] = min(low[u], low[v]);

                // Verifica se a aresta (u, v) é uma aresta ponte
                if (low[v] > disc[u]) {
                    pontes.push_back({u, v});
                }
            } else if (v != parent[u]) { // Atualiza o low[u] com o valor de disc[v]
                low[u] = min(low[u], disc[v]);
            }
        }
    }

    void calcularArestasPonte() {
        cout << endl;

        if (!direcionado) {
            vector<int> disc(numVertices, -1);
            vector<int> low(numVertices, -1);
            vector<int> parent(numVertices, -1);
            vector<pair<int, int>> pontes;
            int time = 0;

            for (int i = 0; i < numVertices; i++) {
                if (disc[i] == -1) {
                    vector<bool> inStack(numVertices, false);
                    stack<int> Stack;
                    dfsTarjan(i, disc, low, parent, pontes, time);
                }
            }
            // Imprime a quantidade de arestas ponte
            cout << pontes.size();
        } else {
            cout << "-1";
        }
    }

    void imprimirArvoreProfundidade() {
        cout << endl;
        
        // Cria uma lista de adjacência para o grafo
        vector<vector<pair<int, int>>> adj(numVertices);

        // Monta a lista de adjacência com os identificadores das arestas
        for (const auto& aresta : arestas) {
            adj[aresta.origem].emplace_back(aresta.destino, aresta.id);
            if (!direcionado) {
                adj[aresta.destino].emplace_back(aresta.origem, aresta.id);
            }
        }

        // Vetores para marcar vértices visitados e armazenar a árvore de profundidade
        vector<bool> visited(numVertices, false);
        vector<int> arvoreProfundidade;

        // Realiza a DFS começando do vértice 0
        dfs(0, visited, adj, arvoreProfundidade);

        // Imprime os identificadores das arestas na ordem encontrada pela DFS
        for (int id : arvoreProfundidade) {
            cout << id << " ";
        }
    }

    void imprimirArvoreLargura() {
        cout << endl;

        // Cria uma lista de adjacência para o grafo, incluindo o identificador das arestas
        vector<vector<pair<int, int>>> adj(numVertices);
        for (const auto& aresta : arestas) {
            adj[aresta.origem].emplace_back(aresta.destino, aresta.id);
            if (!direcionado) {
                adj[aresta.destino].emplace_back(aresta.origem, aresta.id);
            }
        }

        // Vetor para marcar vértices visitados e fila para a BFS
        vector<bool> visited(numVertices, false);
        queue<int> q;
        vector<int> ordemArestas;

        // Inicializa a BFS a partir do vértice 0
        q.push(0);
        visited[0] = true;

        // Executa a BFS
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            // Ordena a lista de adjacência do vértice atual em ordem lexicográfica
            sort(adj[v].begin(), adj[v].end());

            // Explora todos os vizinhos não visitados do vértice atual
            for (const auto& [u, id] : adj[v]) {
                if (!visited[u]) {
                    visited[u] = true;
                    ordemArestas.push_back(id);
                    q.push(u);
                }
            }
        }

        // Imprime os identificadores das arestas na ordem encontrada pela BFS
        for (int id : ordemArestas) {
            cout << id << " ";
        }
    }

    void calcularArvoreGeradoraMinima() {
        cout << endl;

        // Verifica se o grafo é direcionado
        if (direcionado) {
            cout << "-1";
            return; // Retorna -1 para grafos direcionados, pois MST é aplicável apenas a grafos não direcionados
        }

        // Ordena as arestas pelo peso em ordem crescente
        sort(arestas.begin(), arestas.end(), [](const Aresta& a, const Aresta& b) {
            return a.peso < b.peso;
        });

        // Inicializa o vetor pai para o algoritmo de Kruskal
        vector<int> pai(numVertices);
        for (int i = 0; i < numVertices; ++i) {
            pai[i] = i;
        }

        // Função para encontrar o representante de um conjunto usando compressão de caminho
        function<int(int)> encontrar = [&](int u) {
            return pai[u] == u ? u : pai[u] = encontrar(pai[u]);
        };

        int valorMST = 0;

        // Itera sobre as arestas e constrói a MST
        for (const auto& aresta : arestas) {
            int u = encontrar(aresta.origem);
            int v = encontrar(aresta.destino);
            if (u != v) {
                valorMST += aresta.peso; // Adiciona o peso da aresta ao valor da MST
                pai[u] = v; // Une os dois conjuntos
            }
        }

        // Imprime o valor total da árvore geradora mínima
        cout << valorMST;
    }

    void imprimirOrdenacaoTopologica() {
        cout << endl;
        if (!direcionado) {
            cout << "-1";
            return; // Ordenação topológica não é aplicável a grafos não direcionados
        }

        // Matriz de adjacência

        // Preencher a lista de adjacência
        for (const auto& aresta : arestas) {
            adj[aresta.origem].push_back(aresta.destino);
        }

        // Ordenar a lista de adjacência para priorizar a ordem lexicográfica
        for (int i = 0; i < numVertices; ++i) {
            sort(adj[i].begin(), adj[i].end());
        }

        vector<bool> visited(numVertices, false);
        vector<int> ordenacaoTopologica;

        // Função auxiliar de DFS para realizar a ordenação topológica
        function<void(int)> dfs = [&](int v) {
            visited[v] = true;

            for (int u : adj[v]) {
                if (!visited[u]) {
                    dfs(u);
                }
            }

            // Adicionar o vértice ao vetor de ordenação topológica
            ordenacaoTopologica.push_back(v);
        };

        // Executar DFS a partir de todos os vértices que ainda não foram visitados
        for (int i = 0; i < numVertices; ++i) {
            if (!visited[i]) {
                dfs(i);
            }
        }

        // Como os vértices foram adicionados no final da recursão, a ordenação precisa ser invertida
        reverse(ordenacaoTopologica.begin(), ordenacaoTopologica.end());

        // Imprimir a ordenação topológica
        for (int v : ordenacaoTopologica) {
            cout << v << " ";
        }
    }

    void calcularCaminhoMinimo() {
        cout << endl;
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
        cout << dist[numVertices - 1];
    }

    void calcularFluxoMaximo() {
        cout << endl;

        // Verifica se o grafo é direcionado
        if (!direcionado) {
            cout << "-1";
            return; // Retorna -1 para grafos não direcionados, pois fluxo máximo é aplicável apenas a grafos direcionados
        }

        // Inicializa as matrizes de capacidade e adjacência
        vector<vector<int>> capacidade(numVertices, vector<int>(numVertices, 0));

        for (const auto& aresta : arestas) {
            capacidade[aresta.origem][aresta.destino] = aresta.peso;
            adj[aresta.origem].push_back(aresta.destino);
        }

        // Função BFS para encontrar o caminho de aumento
        auto bfs = [&](int s, int t, vector<int>& parent) {
            fill(parent.begin(), parent.end(), -1);
            parent[s] = s;
            queue<pair<int, int>> q;
            q.push({s, INT_MAX});
            while (!q.empty()) {
                int cur = q.front().first;
                int flow = q.front().second;
                q.pop();

                for (int next : adj[cur]) {
                    if (parent[next] == -1 && capacidade[cur][next] > 0) {
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
        // Enquanto houver um caminho de aumento, atualiza o fluxo máximo
        while ((new_flow = bfs(s, t, parent)) > 0) {
            fluxo_maximo += new_flow;
            int cur = t;
            // Atualiza as capacidades das arestas no caminho de aumento
            while (cur != s) {
                int prev = parent[cur];
                capacidade[prev][cur] -= new_flow;
                capacidade[cur][prev] += new_flow;
                cur = prev;
            }
        }

        // Imprime o fluxo máximo encontrado
        cout << fluxo_maximo;
    }

    void calcularFechoTransitivo() {
        cout << endl;
        if (!direcionado) {
            cout << "-1";
            return; // Se o grafo não for direcionado, retorna -1
        }

        // Matriz de Fecho Transitivo (inicialmente com 0)
        vector<vector<bool>> fecho(numVertices, vector<bool>(numVertices, false));

        // Preencher a matriz de adjacência
        for (const auto& aresta : arestas) {
            fecho[aresta.origem][aresta.destino] = true;
        }

        // Floyd-Warshall para calcular o Fecho Transitivo
        for (int k = 0; k < numVertices; ++k) {
            for (int i = 0; i < numVertices; ++i) {
                for (int j = 0; j < numVertices; ++j) {
                    fecho[i][j] = fecho[i][j] || (fecho[i][k] && fecho[k][j]);
                }
            }
        }

        // Imprimir o Fecho Transitivo a partir do vértice 0, priorizando a ordem lexicográfica
        int verticeEscolhido = 0;
        for (int j = 0; j < numVertices; ++j) {
            if (fecho[verticeEscolhido][j]) {
                cout << j << " ";
            }
        }
    }
};

int main() {
    string entrada;
    do {
        cout << "\nMenu de opções:" << endl;
        cout << "0. Verificar se o grafo é conexo" << endl;
        cout << "1. Verificar se o grafo é bipartido" << endl;
        cout << "2. Verificar se o grafo é Euleriano" << endl;
        cout << "3. Verificar se o grafo possui ciclo" << endl;
        cout << "4. Calcular a quantidade de componentes conexas" << endl;
        cout << "5. Calcular a quantidade de componentes fortemente conexas" << endl;
        cout << "6. Imprimir os vértices de articulação" << endl;
        cout << "7. Calcular quantas arestas ponte possui o grafo" << endl;
        cout << "8. Imprimir a árvore em profundidade" << endl;
        cout << "9. Imprimir a árvore em largura" << endl;
        cout << "10. Calcular o valor final de uma árvore geradora mínima" << endl;
        cout << "11. Imprimir a ordenação topológica" << endl;
        cout << "12. Calcular o caminho mínimo entre dois vértices" << endl;
        cout << "13. Calcular o valor do fluxo máximo" << endl;
        cout << "14. Calcular o fecho transitivo" << endl;
        cout << "15. Sair" << endl;

        getline(cin, entrada);

        stringstream ss(entrada);
        int opcao;

        if (!entrada.empty() && entrada != "15") {
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
                    case 0: grafo.verificarConexo(); break;
                    case 1: grafo.verificarBipartido(); break;
                    case 2: grafo.verificarEuleriano(); break;
                    case 3: grafo.verificarCiclo(); break;
                    case 4: grafo.calcularComponentesConexas(); break;
                    case 5: grafo.calcularComponentesFortementeConexas(); break;
                    case 6: grafo.imprimirVerticesArticulacao(); break;
                    case 7: grafo.calcularArestasPonte(); break;
                    case 8: grafo.imprimirArvoreProfundidade(); break;
                    case 9: grafo.imprimirArvoreLargura(); break;
                    case 10: grafo.calcularArvoreGeradoraMinima(); break;
                    case 11: grafo.imprimirOrdenacaoTopologica(); break;
                    case 12: grafo.calcularCaminhoMinimo(); break;
                    case 13: grafo.calcularFluxoMaximo(); break;
                    case 14: grafo.calcularFechoTransitivo(); break;
                    case 15: cout << "Saindo..." << endl; break;
                    default: cout << "Opção inválida!" << endl; break;
                }
            }
        }

    } while (entrada != "0");

    return 0;
}

