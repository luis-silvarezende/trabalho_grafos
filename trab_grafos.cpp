#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <set>
#include <functional>
#include <algorithm>
#include <queue>

using namespace std;

void DFS(int v, vector<bool> &visited, const vector<vector<int>> &adj)
{
    // Marca o vértice atual como visitado
    visited[v] = true;

    // Visita todos os vértices adjacentes não visitados
    for (int u : adj[v])
    {
        if (!visited[u])
        {
            DFS(u, visited, adj);
        }
    }
}

struct Aresta
{
    int id;
    int origem;
    int destino;
    int peso;
};

class Grafo
{
public:
    int numVertices;
    int numArestas;
    bool direcionado;
    vector<Aresta> arestas;
    vector<vector<int>> adj; // Adiciona a lista de adjacência como membro da classe

    Grafo(int v, int a, bool dir) : numVertices(v), numArestas(a), direcionado(dir), adj(v) {}

    void adicionarAresta(int id, int origem, int destino, int peso)
    {
        Aresta aresta = {id, origem, destino, peso};
        arestas.push_back(aresta);
        adj[origem].push_back(destino);
        if (!direcionado)
        {
            adj[destino].push_back(origem);
        }
    }

    void verificarConexo()
    {
        // Cria a lista de adjacência
        vector<vector<int>> adj(numVertices);
        vector<vector<int>> adjReverso(numVertices);

        for (const auto &aresta : arestas)
        {
            adj[aresta.origem].push_back(aresta.destino);
            adjReverso[aresta.destino].push_back(aresta.origem);
            if (!direcionado)
            {
                adj[aresta.destino].push_back(aresta.origem);
                adjReverso[aresta.origem].push_back(aresta.destino);
            }
        }

        vector<bool> visited(numVertices, false);

        // Encontra o primeiro vértice com arestas
        int start_vertex = -1;
        for (int i = 0; i < numVertices; ++i)
        {
            if (!adj[i].empty())
            {
                start_vertex = i;
                break;
            }
        }

        // Se não houver arestas, o grafo é considerado desconexo
        if (start_vertex == -1)
        {
            cout << "O grafo não é conexo (não há arestas)." << endl;
            return;
        }

        // Realiza DFS a partir do primeiro vértice encontrado
        DFS(start_vertex, visited, adj);

        // Verifica se todos os vértices com arestas foram visitados na DFS original
        for (int i = 0; i < numVertices; ++i)
        {
            if (!visited[i] && !adj[i].empty())
            {
                cout << "O grafo não é conexo." << endl;
                return;
            }
        }

        // Reseta o vetor de visitados e realiza DFS no grafo reverso
        fill(visited.begin(), visited.end(), false);
        DFS(start_vertex, visited, adjReverso);

        // Verifica se todos os vértices com arestas foram visitados na DFS reversa
        for (int i = 0; i < numVertices; ++i)
        {
            if (!visited[i] && !adj[i].empty())
            {
                cout << "O grafo tem conectividade fraca." << endl;
                return;
            }
        }

        cout << "O grafo é conexo." << endl;
    }

    void verificarBipartido()
    {
        // Implementação da verificação se o grafo é bipartido
        cout << "Verificando se o grafo é bipartido..." << endl;
    }

    void verificarEuleriano()
{
    if (!direcionado)
    {
        // Verificação para grafos não direcionados
        vector<int> grau(numVertices, 0);

        for (const auto &aresta : arestas)
        {
            grau[aresta.origem]++;
            grau[aresta.destino]++;
        }

        vector<bool> visited(numVertices, false);
        int start_vertex = -1;

        for (int i = 0; i < numVertices; ++i)
        {
            if (grau[i] > 0)
            {
                start_vertex = i;
                break;
            }
        }

        if (start_vertex == -1)
        {
            cout << "O grafo não é conexo (não há arestas)." << endl;
            return;
        }

        DFS(start_vertex, visited, adj);

        for (int i = 0; i < numVertices; ++i)
        {
            if (grau[i] > 0 && !visited[i])
            {
                cout << "O grafo não é conexo." << endl;
                return;
            }
        }

        int oddDegreeCount = 0;
        for (int i = 0; i < numVertices; ++i)
        {
            if (grau[i] % 2 != 0)
            {
                oddDegreeCount++;
            }
        }

        if (oddDegreeCount == 0)
        {
            cout << "O grafo tem um ciclo euleriano." << endl;
        }
        else if (oddDegreeCount == 2)
        {
            cout << "O grafo tem um caminho euleriano." << endl;
        }
        else
        {
            cout << "O grafo não é euleriano." << endl;
        }
    }
    else
    {
        // Verificação para grafos direcionados
        vector<int> grauEntrada(numVertices, 0);
        vector<int> grauSaida(numVertices, 0);

        for (const auto &aresta : arestas)
        {
            grauSaida[aresta.origem]++;
            grauEntrada[aresta.destino]++;
        }

        vector<bool> visited(numVertices, false);
        int start_vertex = -1;

        for (int i = 0; i < numVertices; ++i)
        {
            if (grauSaida[i] > 0 || grauEntrada[i] > 0)
            {
                start_vertex = i;
                break;
            }
        }

        if (start_vertex == -1)
        {
            cout << "O grafo não é conexo (não há arestas)." << endl;
            return;
        }

        DFS(start_vertex, visited, adj);

        for (int i = 0; i < numVertices; ++i)
        {
            if ((grauSaida[i] > 0 || grauEntrada[i] > 0) && !visited[i])
            {
                cout << "O grafo não é fortemente conexo." << endl;
                return;
            }
        }

        bool hasCycleEulerian = true;
        bool hasPathEulerian = true;
        int countEntradaMaiorSaida = 0;
        int countSaidaMaiorEntrada = 0;

        for (int i = 0; i < numVertices; ++i)
        {
            if (grauSaida[i] != grauEntrada[i])
            {
                if (grauSaida[i] == grauEntrada[i] + 1)
                {
                    countSaidaMaiorEntrada++;
                }
                else if (grauEntrada[i] == grauSaida[i] + 1)
                {
                    countEntradaMaiorSaida++;
                }
                else
                {
                    hasCycleEulerian = false;
                    hasPathEulerian = false;
                    break;
                }
            }
        }

        if (hasCycleEulerian && countEntradaMaiorSaida == 0 && countSaidaMaiorEntrada == 0)
        {
            cout << "O grafo tem um ciclo euleriano." << endl;
        }
        else if (hasPathEulerian && countEntradaMaiorSaida == 1 && countSaidaMaiorEntrada == 1)
        {
            cout << "O grafo tem um caminho euleriano." << endl;
        }
        else
        {
            cout << "O grafo não é euleriano." << endl;
        }
    }
}

    void verificarCiclo()
    {
        // Implementação da verificação se o grafo possui ciclo
        cout << "Verificando se o grafo possui ciclo..." << endl;
    }

    void calcularComponentesConexas()
    {
        // Implementação do cálculo de componentes conexas
        cout << "Calculando a quantidade de componentes conexas..." << endl;
    }

   void calcularComponentesFortementeConexas()
{
    if (!direcionado)
    {
        cout << "O cálculo de componentes fortemente conexas só é aplicável a grafos direcionados." << endl;
        return;
    }

    vector<vector<int>> adj(numVertices);
    for (const auto &aresta : arestas)
    {
        adj[aresta.origem].push_back(aresta.destino);
    }

    vector<int> indices(numVertices, -1);
    vector<int> lowlink(numVertices, -1);
    stack<int> pilha;
    vector<bool> emPilha(numVertices, false);
    vector<vector<int>> scc;
    int index = 0;

    function<void(int)> tarjanDFS = [&](int v) {
        indices[v] = lowlink[v] = index++;
        pilha.push(v);
        emPilha[v] = true;

        for (int u : adj[v])
        {
            if (indices[u] == -1)
            {
                tarjanDFS(u);
                lowlink[v] = min(lowlink[v], lowlink[u]);
            }
            else if (emPilha[u])
            {
                lowlink[v] = min(lowlink[v], indices[u]);
            }
        }

        if (lowlink[v] == indices[v])
        {
            vector<int> componente;
            int w;
            do
            {
                w = pilha.top();
                pilha.pop();
                emPilha[w] = false;
                componente.push_back(w);
            } while (w != v);
            scc.push_back(componente);
        }
    };

    for (int i = 0; i < numVertices; ++i)
    {
        if (indices[i] == -1)
        {
            tarjanDFS(i);
        }
    }

    cout << "O número de componentes fortemente conexos é: " << scc.size() << endl;
}


    void imprimirVerticesArticulacao()
    {
        // Implementação da impressão dos vértices de articulação
        cout << "Imprimindo os vértices de articulação..." << endl;
    }



    void calcularArestasPonte()
    {
        // Implementação do cálculo das arestas ponte
        cout << "Calculando as arestas ponte..." << endl;
    }

     void imprimirArvoreProfundidade() {
    // Ordena os vizinhos de cada vértice na ordem lexicográfica
    for (int v = 0; v < numVertices; ++v) {
        sort(adj[v].begin(), adj[v].end());
    }

    // Vetores para manter controle dos vértices visitados e as arestas utilizadas
    vector<bool> visited(numVertices, false);
    vector<int> parent(numVertices, -1);
    vector<int> arestasUsadas;

    function<void(int)> dfs = [&](int v) {
        visited[v] = true;
        for (int u : adj[v]) {
            // Se o vizinho não foi visitado
            if (!visited[u]) {
                // Adiciona a aresta ao vetor de arestas usadas
                for (const auto& aresta : arestas) {
                    if ((aresta.origem == v && aresta.destino == u) ||
                        (!direcionado && aresta.origem == u && aresta.destino == v)) {
                        arestasUsadas.push_back(aresta.id);
                        break;
                    }
                }
                parent[u] = v; // Marca o pai do vértice
                dfs(u); // Continua a DFS
            }
        }
    };

    // Inicia a DFS a partir do vértice 0
    dfs(0);

    // Imprime as arestas utilizadas na DFS, ordenadas de acordo com o resultado
    cout << "Arestas da árvore em profundidade:" << endl;
    for (int id : arestasUsadas) {
        cout << id << " ";
    }
    cout << endl;
}





    void imprimirArvoreLargura()
    {
        // Implementação da impressão da árvore em largura
        cout << "Imprimindo a árvore em largura..." << endl;
    }

    void calcularArvoreGeradoraMinima()
    {
        // Implementação do cálculo da árvore geradora mínima
        cout << "Calculando a árvore geradora mínima..." << endl;
    }

   void ordenarTopologicamente()
{
    if (!direcionado)
    {
        cout << "Ordenação topológica só é aplicável a grafos direcionados." << endl;
        return;
    }

    vector<int> grauEntrada(numVertices, 0);

    // Calcula o grau de entrada de cada vértice
    for (int v = 0; v < numVertices; ++v)
    {
        for (int u : adj[v])
        {
            grauEntrada[u]++;
        }
    }

    queue<int> fila;

    // Adiciona todos os vértices com grau de entrada zero à fila
    for (int v = 0; v < numVertices; ++v)
    {
        if (grauEntrada[v] == 0)
        {
            fila.push(v);
        }
    }

    vector<int> ordenacao;

    while (!fila.empty())
    {
        int v = fila.front();
        fila.pop();
        ordenacao.push_back(v);

        // Para cada vizinho do vértice removido, diminui o grau de entrada e
        // adiciona à fila se o grau de entrada se tornar zero
        for (int u : adj[v])
        {
            grauEntrada[u]--;
            if (grauEntrada[u] == 0)
            {
                fila.push(u);
            }
        }
    }

    // Verifica se a ordenação topológica inclui todos os vértices
    if (ordenacao.size() == static_cast<size_t>(numVertices))
    {
        cout << "Ordenação topológica:" << endl;
        for (int v : ordenacao)
        {
            cout << v << " ";
        }
        cout << endl;
    }
    else
    {
        cout << "O grafo tem um ciclo e, portanto, não pode ser ordenado topologicamente." << endl;
    }
}

    void calcularCaminhoMinimo()
    {
        // Implementação do cálculo do caminho mínimo
        cout << "Calculando o caminho mínimo..." << endl;
    }

    void calcularFluxoMaximo()
    {
        // Implementação do cálculo do fluxo máximo
        cout << "Calculando o fluxo máximo..." << endl;
    }

  void calcularFechoTransitivo()
    {
        if (!direcionado)
        {
            cout << "O fecho transitivo só é aplicável a grafos direcionados." << endl;
            return;
        }

        vector<vector<int>> fechoTransitivo(numVertices, vector<int>(numVertices, 0));

        for (int i = 0; i < numVertices; ++i)
        {
            for (int j : adj[i])
            {
                fechoTransitivo[i][j] = 1;
            }
        }

        for (int k = 0; k < numVertices; ++k)
        {
            for (int i = 0; i < numVertices; ++i)
            {
                for (int j = 0; j < numVertices; ++j)
                {
                    fechoTransitivo[i][j] = fechoTransitivo[i][j] || (fechoTransitivo[i][k] && fechoTransitivo[k][j]);
                }
            }
        }

        for (int j = 0; j < numVertices; ++j)
        {
            if (fechoTransitivo[0][j] == 1)
            {
                cout << j << " ";
            }
        }
        cout << endl;
    }
};

int main()
{
    string entrada;
    do
    {
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

        if (!entrada.empty() && entrada != "0")
        {
            int v, a;
            string direcionado;

            getline(cin, entrada);
            stringstream(entrada) >> v >> a;

            getline(cin, direcionado);

            bool isDirecionado = (direcionado == "direcionado");

            Grafo grafo(v, a, isDirecionado);

            for (int i = 0; i < a; ++i)
            {
                int id, origem, destino, peso;
                getline(cin, entrada);
                stringstream(entrada) >> id >> origem >> destino >> peso;
                grafo.adicionarAresta(id, origem, destino, peso);
            }

            while (ss >> opcao)
            {
                switch (opcao)
                {
                case 1:
                    grafo.verificarConexo();
                    break;
                case 2:
                    grafo.verificarBipartido();
                    break;
                case 3:
                    grafo.verificarEuleriano();
                    break;
                case 4:
                    grafo.verificarCiclo();
                    break;
                case 5:
                    grafo.calcularComponentesConexas();
                    break;
                case 6:
                    grafo.calcularComponentesFortementeConexas();
                    break;
                case 7:
                    grafo.imprimirVerticesArticulacao();
                    break;
                case 8:
                    grafo.calcularArestasPonte();
                    break;
                case 9:
                    grafo.imprimirArvoreProfundidade();
                    break;
                case 10:
                    grafo.imprimirArvoreLargura();
                    break;
                case 11:
                    grafo.calcularArvoreGeradoraMinima();
                    break;
                case 12:
                    grafo.ordenarTopologicamente();
                    break;
                case 13:
                    grafo.calcularCaminhoMinimo();
                    break;
                case 14:
                    grafo.calcularFluxoMaximo();
                    break;
                case 15:
                    grafo.calcularFechoTransitivo();
                    break;
                case 0:
                    cout << "Saindo..." << endl;
                    break;
                default:
                    cout << "Opção inválida!" << endl;
                    break;
                }
            }
        }

    } while (entrada != "0");

    return 0;
}
