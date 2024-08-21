# Projeto de Análise de Grafos

Este projeto implementa diversas funcionalidades para análise de grafos, utilizando algoritmos clássicos de teoria dos grafos. Ele foi desenvolvido por:

- **Luis Felipe Silva Rezende** - 202120791
- **Miguel Chagas Figueiredo** - 202220176
- **João Lucas Pereira Ramalho** - 202220166

## Funcionalidades

O código possui as seguintes funcionalidades principais:

### 1. Verificar Conectividade
- **Descrição:** Verifica se o grafo é conexo (ou fracamente conexo, no caso de grafos direcionados).
- **Método:** Busca em Profundidade (DFS).

### 2. Verificar Bipartição
- **Descrição:** Verifica se o grafo é bipartido.
- **Método:** DFS com coloração dos vértices.

### 3. Verificar Ciclos
- **Descrição:** Detecta se há ciclos no grafo.
- **Método:** DFS com rastreamento de pilha de recursão.

### 4. Verificar Propriedade Euleriana
- **Descrição:** Verifica se o grafo é Euleriano, ou seja, se todos os vértices têm grau par e o grafo é conexo.
- **Método:** DFS para conectividade e verificação de graus dos vértices.

### 5. Calcular Componentes Conexas
- **Descrição:** Calcula o número de componentes conexas do grafo (apenas para grafos não direcionados).
- **Método:** DFS para contar componentes.

### 6. Calcular Componentes Fortemente Conexas
- **Descrição:** Calcula o número de componentes fortemente conexas no grafo (apenas para grafos direcionados).
- **Método:** Algoritmo de Tarjan.

### 7. Encontrar Pontos de Articulação
- **Descrição:** Identifica os vértices que, se removidos, desconectariam o grafo.
- **Método:** Algoritmo de Tarjan.

### 8. Calcular Arestas-Ponte
- **Descrição:** Identifica as arestas que, se removidas, desconectariam o grafo.
- **Método:** Algoritmo de Tarjan.

### 9. Imprimir Árvores de Profundidade e Largura
- **Descrição:** Imprime as árvores de profundidade e largura do grafo, ordenando os vértices lexicograficamente.
- **Método:** DFS para a árvore de profundidade e BFS para a árvore de largura.

## Estrutura do Código

O código está estruturado em uma classe `Grafo`, que contém:

- **Atributos:** 
  - `numVertices` - Número de vértices no grafo.
  - `numArestas` - Número de arestas no grafo.
  - `direcionado` - Indica se o grafo é direcionado ou não.
  - `arestas` - Vetor de arestas do grafo.
  - `adj` - Lista de adjacências do grafo.

- **Métodos:**
  - `adicionarAresta()` - Adiciona uma aresta ao grafo.
  - `verificarConexo()` - Verifica a conectividade do grafo.
  - `verificarBipartido()` - Verifica se o grafo é bipartido.
  - `verificarCiclo()` - Verifica se o grafo possui ciclos.
  - `verificarEuleriano()` - Verifica se o grafo é Euleriano.
  - `calcularComponentesConexas()` - Calcula o número de componentes conexas.
  - `calcularComponentesFortementeConexas()` - Calcula o número de componentes fortemente conexas.
  - `imprimirVerticesArticulacao()` - Imprime os vértices de articulação.
  - `calcularArestasPonte()` - Calcula as arestas-pontes.
  - `imprimirArvoreProfundidade()` - Imprime a árvore de largura.

## Como Executar

1. **Clonar o repositório:**
   ```bash
   git clone https://github.com/seu-usuario/nome-do-repositorio.git
   cd nome-do-repositorio
   g++ -o grafos main.cpp
   ./grafos