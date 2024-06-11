# Trabalho de Implementação – Individual (T1)

**Disciplina:** Grafos e Aplicações  
**Professor:** Prof. Dr. Denis Henrique Pinheiro Salvadeo  
**Data de entrega:** 12/12/2022  

## Objetivo

O objetivo deste trabalho é implementar uma estrutura de dados para manipular Grafos usando linguagem C ou C++ e operações para manipular os grafos. Crie as structs ou classes que julgar necessário para manipular os grafos.

## Operações a Implementar

As seguintes operações devem ser implementadas:

### a) Ler um arquivo .txt contendo um grafo
O arquivo terá o seguinte formato:

**1ª linha:**  
`<num_vertices> <num_arestas ou arcos> <tipo> <valorado>`

**Linhas seguintes:** para cada aresta ou arco {vi, vj}, criar uma linha com o formato:  
`<vi> <vj> [<peso>]`

**OBS:**

- `<tipo>` será G para grafo não dirigido e D para grafo dirigido
- `<valorado>` será 1 para grafo valorado e 0 para grafo não valorado
- `<peso>` é opcional e só existirá para grafo ponderado
- Arestas ou arcos paralelos aparecerão mais de uma vez no arquivo

### b) Gerar um arquivo .txt contendo um grafo, com o formato definido em (a)

### c) Converter de matriz de adjacências para lista de adjacências (e vice-versa)

### d) Calcular e mostrar o grau de cada vértice

### e) Encontrar uma árvore geradora mínima

### f) Encontrar um caminho mais curto usando Dijkstra

### g) Realizar Busca em largura

### h) Realizar Busca em profundidade

## O que devo entregar

1. Código fonte produzido
2. Arquivos de exemplo utilizados
3. Pequeno vídeo (até 5 minutos) mostrando testes sendo executados

