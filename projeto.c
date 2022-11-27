#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRAPH_NAME "newGraph.txt"
#define NIL -1
#define INFINITY 1000 //nos testes, 1000 eh um numero suficientemente grande para ser maior que todos os pesos
#define BRANCA 0
#define CINZA 1
#define PRETA 2

typedef struct s_header {
    int V;
    int E;
    char tipo;
    int peso;
}HEADER;

typedef struct s_adj {
    int vertice;
    int peso;
    struct s_adj *next;
}ADJ;

typedef struct vertex {
    ADJ* head;
}VERTEX;

typedef struct s_graph {
    HEADER cabecalho;
    int** matrizAdj;
    VERTEX* listaAdj; //"vetor" de vertices; cada vertice possui uma lista associada
}GRAFO;

typedef struct s_fila {
    int* queue;
    int top;
    int base;
}FILA;

void printaMenu();
void gerarGrafo();
void initialize(GRAFO* g);
void readMatrix(FILE** fp, GRAFO* g);
void printMatrix(GRAFO* g);
ADJ* newAdj(int vertice, int peso);
void newEdge(GRAFO* g, int i, int j, int p);
void imprimeLista(GRAFO* g);
void matrixToList(FILE** fp);
void conversaoMatrizLista();
void readList(FILE** fp, GRAFO* g);
void listToMatrix(FILE** fp);
void conversaoListaMatriz();
void calcDegrees(FILE** fp);
void calcularGrauVertices();
void algoritmoDePrim(GRAFO* g, int r);
void arvoreGeradoraMinima();
void initializeSingleSource(GRAFO* g, int s, int* d, int* pi);
int relax(int u, int v, int w, int* d, int* pi);
void dijkstra(GRAFO* g, int s);
void caminhoMaisCurto();
void createQueue(FILA* q);
void enqueue(FILA* q, int s);
int dequeue(FILA* q);
int isEmpty(FILA *q);
void printPath(GRAFO* g, int s, int v, int* pi);
void breadthFirstSearch(GRAFO* g, int s);
void buscaEmLargura();
void dfsVisit(GRAFO* g, int u, int* cor, int* d, int* f, int* pi);
void buscaEmProfundidade();


int main() {
    int opcao;

    do {
        printaMenu();
        
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            gerarGrafo();
            break;

        case 2:
            conversaoMatrizLista();
            break;

        case 3:
            conversaoListaMatriz();
            break;

        case 4:
            calcularGrauVertices();
            break;

        case 5:
            arvoreGeradoraMinima();
            break;

        case 6:
            caminhoMaisCurto();
            break;

        case 7:
            buscaEmLargura();
            break;

        case 8:
            buscaEmProfundidade();
            break;

        case 0:
            printf("Encerrando programa...");
            break;
        
        default:
            printf("Opcao invalida!\n");
            break;
        }

    }while(opcao != 0);
}

void printaMenu() {
    printf("\n***********************************************\n");
    printf("* 1. Gerar grafo *\n");
    printf("* 2. Converter matriz de adjacencia para lista *\n");
    printf("* 3. Converter lista de adjacencias para matriz *\n");
    printf("* 4. Calcular grau dos vertices do grafo *\n");
    printf("* 5. Arvore geradora minima (Prim) *\n");
    printf("* 6. Caminho mais curto (Dijkstra) *\n");
    printf("* 7. Busca em largura (BFS) *\n");
    printf("* 8. Busca em profundidade (DFS) *\n");
    printf("*************************************************\n");
}

void gerarGrafo() {
    FILE* fp = fopen(GRAPH_NAME, "w+");
    int vertices, arestas, peso;
    char tipo;
    
    printf("Numero de vertices: ");
    scanf("%d", &vertices);

    fprintf(fp, "%d ", vertices);

    printf("Numero de arestas: ");
    scanf("%d", &arestas);

    fprintf(fp, "%d ", arestas);

    do {
        printf("Tipo (G ou D): ");
        scanf(" %c", &tipo);
    }while((tipo != 'G') && (tipo != 'D'));

    fprintf(fp, "%c ", tipo);

    do {
        printf("Peso (0 ou 1): ");
        scanf("%d", &peso);
    }while((peso != 0) && (peso != 1));
    
    fprintf(fp, "%d\n", peso);

    int a, b, w;
    int i;

    switch (peso)
    {
    case 0:
        for(i = 0; i < arestas; i++) {
            printf("No A: ");
            scanf("%d", &a);

            if (a == -1) {
                break;
            }

            printf("No B: ");
            scanf("%d", &b);

            fprintf(fp, "%d %d", a, b);
            if(i < arestas - 1) {
                fprintf(fp, "\n");
            }
        }
        
        break;

    case 1:
        for(i = 0; i < arestas; i++) {
            printf("No A: ");
            scanf("%d", &a);

            if (a == -1) {
                break;
            }

            printf("No B: ");
            scanf("%d", &b);

            printf("Peso: ");
            scanf("%d", &w);

            fprintf(fp, "%d %d %d", a, b, w);
            if(i < arestas - 1) {
                fprintf(fp, "\n");
            }
        }

        break;
    
    default:
        break;
    }

    fclose(fp);

    return;
}

void conversaoMatrizLista() {
    int opcao;
    FILE* fp;

    printf("Escolha um grafo para gerar a matriz: \n\n");
    printf("*1. Grafo gerado *\n");
    printf("*2. Grafo nao orientado *\n");
    printf("*3. Grafo orientado *\n\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
        fp = fopen(GRAPH_NAME, "r+");
        if (fp == NULL) {
            printf("Nenhum grafo gerado!\n");
            return;
        }
        break;

    case 2:
        fp = fopen("grafoAdjG.txt", "r+");
        break;

    case 3:
        fp = fopen("grafoAdjD.txt", "r+");
        break;
    
    default:
        printf("Opcao invalida!\n");
        return;
    }
    matrixToList(&fp);

    fclose(fp);

    return;
}

void initialize(GRAFO* g) {
    int i, j;
    //aloco espaço para a matriz
    g->matrizAdj = malloc(g->cabecalho.V * sizeof(int*));
    for (i = 0; i < g->cabecalho.V; i++) {
        g->matrizAdj[i] = malloc(g->cabecalho.V * sizeof(int));
    }

    //inicializo minha matriz com 0s, ou seja, nao existe nenhuma aresta AINDA
    for (i = 0; i < g->cabecalho.V; i++) {
        for (j = 0; j < g->cabecalho.V; j++) {
            g->matrizAdj[i][j] = 0;
        }
    }

    //inicializo e aloco espaco para a lista de adjacencias
    g->listaAdj = malloc(g->cabecalho.V * sizeof(VERTEX));
    for (i = 0; i < g->cabecalho.V; i++) {
        g->listaAdj[i].head = NULL;
    }
}

void readMatrix(FILE** fp, GRAFO* g) {
    int i;
    int a, b, w;

    //vejo se eh ponderado ou nao, pra saber como devo ler o arquivo
    switch(g->cabecalho.peso)
    {
    case 0:
        for (i = 0; i < g->cabecalho.E; i++) {
            fscanf(*fp, "%d %d", &a, &b);
            g->matrizAdj[a][b]++;
            if (g->cabecalho.tipo == 'G') {
                g->matrizAdj[b][a]++;
            }
        }
        break;

    case 1:
        for (i = 0; i < g->cabecalho.E; i++) {
            fscanf(*fp, "%d %d %d", &a, &b, &w);
            g->matrizAdj[a][b] = w;
            if (g->cabecalho.tipo == 'G') {
                g->matrizAdj[b][a] = w;
            }
        }
        break;
    
    default:
        printf("Peso invalido!\n");
        return;
    }
}

void printMatrix(GRAFO* g) {
    int i, j;
    for (i = 0; i < g->cabecalho.V; i++) {
        printf("|");
        for (j = 0; j < g->cabecalho.V; j++) {
            printf("%d", g->matrizAdj[i][j]);
            if (j < g->cabecalho.V - 1) {
                printf("\t");
            }
        }
        printf("|\n");
    }
}

//cria uma nova adjacencia na lista
ADJ* newAdj(int vertice, int peso) {
    ADJ* aux = malloc(sizeof(ADJ));
    aux->vertice = vertice;
    aux->peso = peso;
    aux->next = NULL;

    return aux;
}

//cria uma nova aresta
void newEdge(GRAFO* g, int i, int j, int p) {
    ADJ* new = newAdj(j, p);
    new->next = g->listaAdj[i].head;
    g->listaAdj[i].head = new;
}

void matrixToList(FILE** fp) {
    GRAFO g;
    fscanf(*fp, "%d %d %c %d", &g.cabecalho.V, &g.cabecalho.E, &g.cabecalho.tipo, &g.cabecalho.peso);

    initialize(&g);

    readMatrix(fp, &g);

    printMatrix(&g);

    //le cada posicao da matriz, se for diferente de 0,
    //existe uma aresta e adiciona-a a lista
    int i, j;
    for (i = 0; i < g.cabecalho.V; i++) {
        for (j = 0; j < g.cabecalho.V; j++) {
            if (g.matrizAdj[i][j] != 0) {
                newEdge(&g, i, j, g.matrizAdj[i][j]);   
            }
        }
    }

    printf("\n\n");
    imprimeLista(&g);
}

void conversaoListaMatriz() {
    int opcao;
    int i;
    FILE* fp;

    printf("Escolha um grafo para gerar a lista: \n\n");
    printf("*1. Grafo gerado *\n");
    printf("*2. Grafo nao orientado *\n");
    printf("*3. Grafo orientado *\n\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
        fp = fopen(GRAPH_NAME, "r+");
        if (fp == NULL) {
            printf("Nenhum grafo gerado!\n");
            return;
        }
        break;

    case 2:
        fp = fopen("grafoAdjG.txt", "r+");
        break;

    case 3:
        fp = fopen("grafoAdjD.txt", "r+");
        break;
    
    default:
        printf("Opcao invalida!\n");
        return;
    }

    listToMatrix(&fp);

    fclose(fp);

    return;
}

void listToMatrix(FILE** fp) {
    GRAFO g;
    fscanf(*fp, "%d %d %c %d", &g.cabecalho.V, &g.cabecalho.E, &g.cabecalho.tipo, &g.cabecalho.peso);

    initialize(&g);

    readList(fp, &g);

    imprimeLista(&g);

    //le cada posicao da matriz, se for diferente de 0,
    //existe uma aresta e adiciona-a a lista
    int i, j;
    for (i = 0; i < g.cabecalho.V; i++) {
        ADJ* list = g.listaAdj[i].head;
        while(list) {
            g.matrizAdj[i][list->vertice] = list->peso;
            list = list->next;
        }
    }

    printf("\n\n");
    printMatrix(&g);
}

void imprimeLista(GRAFO* g) {
    int i;
    for (i = 0; i < g->cabecalho.V; i++) {
        printf("%d -> ", i);
        ADJ* list = g->listaAdj[i].head;
        while(list) {
            printf("%d(%d) -> ", list->vertice, list->peso);
            list = list->next;
        }
        printf("NULL\n");
    }
}

void readList(FILE** fp, GRAFO* g) {
    int i;
    int a, b, w;

    //vejo se eh ponderado ou nao, pra saber como devo ler o arquivo
    switch(g->cabecalho.peso)
    {
    case 0:
        for (i = 0; i < g->cabecalho.E; i++) {
            fscanf(*fp, "%d %d", &a, &b);
            newEdge(g, a, b, 1);
            if (g->cabecalho.tipo == 'G') {
                newEdge(g, b, a, 1);
            }
        }
        break;

    case 1:
        for (i = 0; i < g->cabecalho.E; i++) {
            fscanf(*fp, "%d %d %d", &a, &b, &w);
            newEdge(g, a, b, w);
            if (g->cabecalho.tipo == 'G') {
                newEdge(g, b, a, w);
            }
        }
        break;
    
    default:
        printf("Peso invalido!\n");
        return;
    }
}

void calcularGrauVertices() {
    int opcao;
    FILE* fp;

    printf("Escolha um grafo para analisar: \n\n");
    printf("*1. Grafo gerado *\n");
    printf("*2. Grafo nao orientado *\n");
    printf("*3. Grafo orientado *\n");
    printf("*4. Grafo da Arvore Geradora Minima *\n");
    printf("*5. Grafo da BFS *\n");
    printf("*6. Grafo da DFS *\n");
    printf("*7. Grafo do Caminho Mais Curto *\n\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    
    switch (opcao)
    {
    case 1:
        fp = fopen(GRAPH_NAME, "r+");
        if (fp == NULL) {
            printf("Nenhum grafo gerado!\n");
            return;
        }
        break;

    case 2:
        fp = fopen("grafoAdjG.txt", "r+");
        break;

    case 3:
        fp = fopen("grafoAdjD.txt", "r+");
        break;

    case 4:
        fp = fopen("grafoArvoreGeradoraMinima.txt", "r+"); 
        break;

    case 5:
        fp = fopen("grafoBFS.txt", "r+");
        break;

    case 6:
        fp = fopen("grafoDFS.txt", "r+");
        break;

    case 7:
        fp = fopen("grafoDijkstra.txt", "r+");
        break;
    
    default:
        printf("Opcao invalida!\n");
        break;
    }

    calcDegrees(&fp);

    fclose(fp);

    return;
}

void calcDegrees(FILE** fp) {
    GRAFO g;
    fscanf(*fp, "%d %d %c %d", &g.cabecalho.V, &g.cabecalho.E, &g.cabecalho.tipo, &g.cabecalho.peso);

    initialize(&g);

    readMatrix(fp, &g);

    printMatrix(&g);

    int grauEntrada[g.cabecalho.V];
    int grauSaida[g.cabecalho.V];

    int i, j;
    for (i = 0; i < g.cabecalho.V; i++) {
        grauEntrada[i] = 0;
        grauSaida[i] = 0;
    }

    for (i = 0; i < g.cabecalho.V; i++) {
        for (j = 0; j < g.cabecalho.V; j++) {
            if(g.matrizAdj[i][j] != 0) {
                switch (g.cabecalho.tipo)
                {
                case 'G':
                    grauEntrada[i]++;
                    break;

                case 'D':
                    grauSaida[i]++;
                    grauEntrada[j]++;
                    break;
                
                default:
                    break;
                }
            }
        }
    }

    if (g.cabecalho.tipo == 'G') {
        for (i = 0; i < g.cabecalho.V; i++) {
            printf("d(v%d): %d\n", i, grauEntrada[i]);
        }
    } else {
        for (i = 0; i < g.cabecalho.V; i++) {
            printf("d-(v%d): %d\td+(v%d): %d\n", i, grauEntrada[i], i, grauSaida[i]);
        }
    }
}

void arvoreGeradoraMinima() {
    FILE* fp = fopen("grafoArvoreGeradoraMinima.txt", "r+");

    GRAFO g;
    fscanf(fp, "%d %d %c %d", &g.cabecalho.V, &g.cabecalho.E, &g.cabecalho.tipo, &g.cabecalho.peso);

    initialize(&g);

    readList(&fp, &g);

    imprimeLista(&g);

    algoritmoDePrim(&g, 0); //0 eh o vertice raiz

    fclose(fp);

    return;
}

void algoritmoDePrim(GRAFO* g, int r) {
    int key[g->cabecalho.V]; //vetor de pesos 
    int pi[g->cabecalho.V]; //vetor de pais

    int i;
    for(i = 0; i < g->cabecalho.V; i++) {
        key[i] = INFINITY;
        pi[i] = NIL;
    }

    printf("\n");

    key[r] = 0;
    int contador = 0;
    while(contador < g->cabecalho.V) {
        int u = contador;
        ADJ* list = g->listaAdj[u].head;
        while(list) {
            if (list->vertice > u && list->peso < key[list->vertice]) {
                pi[list->vertice] = u;
                key[list->vertice] = list->peso;
            }
            list = list->next;
        }
        contador++;
    }

    for (i = 0; i < g->cabecalho.V; i++) {
        printf("pi[%d]: %d\n", i, pi[i]);
    }
}

void caminhoMaisCurto() {
    FILE* fp = fopen("grafoDijkstra.txt", "r+");

    GRAFO g;
    fscanf(fp, "%d %d %c %d", &g.cabecalho.V, &g.cabecalho.E, &g.cabecalho.tipo, &g.cabecalho.peso);

    initialize(&g);

    readList(&fp, &g);

    imprimeLista(&g);

    dijkstra(&g, 0);

    fclose(fp);

    return;
}

void dijkstra(GRAFO* g, int s) { //s eh o vertice fonte
    int d[g->cabecalho.V];
    int pi[g->cabecalho.V];

    initializeSingleSource(g, s, d, pi);

    int contador = 0;
    int aux;
    while(contador < g->cabecalho.V) {
        int u = contador;
        ADJ* list = g->listaAdj[u].head;
        while(list) {
            aux = relax(u, list->vertice, list->peso, d, pi);
            list = list->next;
            if (aux == 1) {
                contador = 0;
            }
        }
        contador++;
    }

    int i;
    int pai;

    printf("\n\nFonte: %d\n", s);
    printf("DISTANCIA\tCAMINHO\n");
    for (i = 0; i < g->cabecalho.V; i++) {
        if (i != s) {
            printf("%d\t\t", d[i]);
            printf("%d <- ", i);
            pai = pi[i];
            while(pai != s) {
                printf("%d <- ", pai);
                pai = pi[pai];
            }
            printf("%d\n", s);
        }
    }
}

int relax(int u, int v, int w, int* d, int* pi) {
    if (d[v] > d[u] + w) {
        //caminho mais curto encontrado; atualizando...
        d[v] = d[u] + w;
        pi[v] = u;
        return 1;
    }
    return 0;
}

void initializeSingleSource(GRAFO* g, int s, int* d, int* pi) {
    int i;
    for (i = 0; i < g->cabecalho.V; i++) {
        d[i] = INFINITY;
        pi[i] = NIL;
    }
    d[s] = 0;
}

void buscaEmLargura() {
    FILE* fp = fopen("grafoBFS.txt", "r+");

    GRAFO g;
    fscanf(fp, "%d %d %c %d", &g.cabecalho.V, &g.cabecalho.E, &g.cabecalho.tipo, &g.cabecalho.peso);

    initialize(&g);

    readList(&fp, &g);

    imprimeLista(&g);

    breadthFirstSearch(&g, 1);

    fclose(fp);

    return;
}

void breadthFirstSearch(GRAFO* g, int s) {
    int cor[g->cabecalho.V];
    int d[g->cabecalho.V];
    int pi[g->cabecalho.V];

    //inicializacao
    int i;
    for (i = 0; i < g->cabecalho.V; i++) {
        cor[i] = BRANCA;
        d[i] = INFINITY;
        pi[i] = NIL;
    }

    cor[s] = CINZA;
    d[s] = 0;
    pi[s] = NIL;

    FILA q;
    q.queue = malloc(g->cabecalho.V * sizeof(int));
    createQueue(&q);
    enqueue(&q, s);

    int u;
    while(!isEmpty(&q)) {
        u = dequeue(&q);
        ADJ* list = g->listaAdj[u].head;
        while(list) {
            if(cor[list->vertice] == BRANCA) {
                cor[list->vertice] = CINZA;
                d[list->vertice] = d[u] + 1;
                pi[list->vertice] = u;
                enqueue(&q, list->vertice);
            }
            list = list->next;
        }
        cor[u] = PRETA;
    }

    printf("\n\nFonte: %d\n", s);
    printf("VERTICE\t\tCAMINHO\n");
    for (i = 0; i < g->cabecalho.V; i++) {
        if (i != s) {
            printf("%d\t\t", i);
            printPath(g, s, i, pi);
            printf("NULL");
            printf("\n");
        }
    }
    
}

void printPath(GRAFO* g, int s, int v, int* pi) {
    if (v == s) {
        printf("%d -> ", s);
    } else {
        if (pi[v] == NIL) {
            printf("Nao existe caminho de s a v\n");
            return;
        } else {
            printPath(g, s, pi[v], pi);
            printf("%d -> ", v);
        }
    }
}

void createQueue(FILA* q) {
    q->top = NIL;
    q->base = NIL;
}

void enqueue(FILA* q, int s) {
    if (q->top == NIL) {
        q->top = 0;
    }
    q->base++;
    q->queue[q->base] = s;
}

int dequeue(FILA* q) {
    int aux = q->queue[q->top];
    q->top++;
    if(q->top > q->base) {
        q->top = q->base;
        q->base = NIL;
    }
    return aux;
}

int isEmpty(FILA *q) {
    if (q->base == -1) {
        return 1;
    } else return 0;
}

void buscaEmProfundidade() {
    FILE* fp = fopen("grafoDFS.txt", "r+");

    GRAFO g;
    fscanf(fp, "%d %d %c %d", &g.cabecalho.V, &g.cabecalho.E, &g.cabecalho.tipo, &g.cabecalho.peso);

    initialize(&g);

    readList(&fp, &g);

    imprimeLista(&g);

    depthFirstSearch(&g);

    fclose(fp);

    return;
}

int tempo;
void depthFirstSearch(GRAFO* g) {
    int cor[g->cabecalho.V];
    int d[g->cabecalho.V]; //instante da descoberta
    int f[g->cabecalho.V]; //instante da finalizacao
    int pi[g->cabecalho.V];

    //inicializacao
    int i;
    for (i = 0; i < g->cabecalho.V; i++) {
        cor[i] = BRANCA;
        pi[i] = NIL;
    }

    tempo = 0;
    for (i = 0; i < g->cabecalho.V; i++) {
        if (cor[i] == BRANCA) {
            dfsVisit(g, i, cor, d, f, pi);
        }
    }

    printf("\n");
    printf("VERTICE\t\t\tDESCOBERTA\t\tFINALIZACAO\n");
    for(i = 0; i < g->cabecalho.V; i++) {
        printf("%d\t\t\t%d\t\t\t%d\n", i, d[i], f[i]);
    }
}

void dfsVisit(GRAFO* g, int u, int* cor, int* d, int* f, int* pi) {
    cor[u] = CINZA;
    tempo = tempo + 1;
    d[u] = tempo;
    
    ADJ* list = g->listaAdj[u].head;
    while(list) {
        if (cor[list->vertice] == BRANCA) {
            pi[list->vertice] == u;
            dfsVisit(g, list->vertice, cor, d, f, pi);
        }
        list = list->next;
    }

    cor[u] = PRETA;
    tempo = tempo + 1;
    f[u] = tempo;
}