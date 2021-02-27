#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

struct matriz {
    int numero_elementos;
    int** elementos;
};

struct nodo {
    int indice;
    int valor;
};

//---------------------------------------------------------------------------------------//

void solucao_inicial_random(struct matriz m, int *caminho) {
    int *inseridos = malloc(m.numero_elementos * sizeof(int));

    for(int i = 0; i < m.numero_elementos; i++) {
        inseridos[i] = FALSE;
        caminho[i] = 0;
    }
    caminho[0] = 5;
    inseridos[5] = TRUE;
    int vet[5];
    int achou;
    for(int i=1; i<=5; i++){
        do{
            vet[i] = rand()%5;
            achou = 0;

            for(int j=0; j<i; j++){
                if(vet[i]==vet[j]){
                    achou=1;
                    break;
                }
            }
        }while(achou);
    }
    for(int i=1; i<=5; i++){
        for(int a=1; a<=5;a++)
        caminho[i] = vet[i];
        inseridos[vet[i]] = TRUE;
        }
    caminho[m.numero_elementos] = 5;
    free(inseridos);
}



void ler_arquivo(struct matriz* m, char* arquivo) {
    FILE* fp = fopen(arquivo, "r");

    fscanf(fp, "%d\n", &m->numero_elementos);

    m->elementos = malloc(m->numero_elementos * sizeof(int*));

    for(int i = 0; i < m->numero_elementos; i++) {
        m->elementos[i] = malloc(m->numero_elementos * sizeof(int));
        for(int j = 0; j < m->numero_elementos; j++) {
            fscanf(fp, "%d ", &m->elementos[i][j]);
        }
    }
    fclose(fp);
}
//-----------------------------------------------------------------------------

void construir_caminho(struct matriz m, int* caminho) {
    int *inseridos = malloc(m.numero_elementos * sizeof(int));

    for(int i = 0; i < m.numero_elementos; i++) {
        inseridos[i] = FALSE;
    }

    caminho[0] = 5;
    inseridos[5] = TRUE;
    int vizinho_selecionado = 5;

    for(int i = 0; i < m.numero_elementos; i++) {
        int linha = vizinho_selecionado;
        int valor_referencia = INT_MAX;
        for(int j = 0; j < m.numero_elementos; j++) {
            if(!inseridos[j] && valor_referencia > m.elementos[linha][j] && m.elementos[linha][j]!=0) {
                vizinho_selecionado = j;
                valor_referencia = m.elementos[linha][j];
            }
        }

        caminho[i + 1] = vizinho_selecionado;
        inseridos[linha] = TRUE;
    }

    caminho[m.numero_elementos] = 5;

    free(inseridos);
}



//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

void imprimir_matriz(struct matriz m) {
    linha();
    printf("Matriz\n\n");

    for(int i = 0; i < m.numero_elementos; i++) {
        for(int j = 0; j < m.numero_elementos; j++) {
            printf("%d ", m.elementos[i][j]);
        }

        printf("\n");
    }

    linha();
}

//-----------------------------------------------------------------------------

void imprimir_caminho(int n, int* caminho) {
    int i;

    for(i = 0; i < n; i++) {
        printf("%d ", caminho[i]);
    }
    printf("\n");
}

int calcular_custo2(struct matriz m, int caminho[]) {
    int custo = 0;

    for(int i = 0; i < m.numero_elementos; i++) {
        custo = custo + m.elementos[caminho[i]][caminho[i + 1]];
    }

    return custo;
}

//-----------------------------------------------------------------------------

void linha() {
    int i;
    printf("\n");
    for(i = 0; i < 80; i++) printf("_");
    printf("\n");
}
//------------------------------------------------------------------------------
void best_improvement(int n, int* caminho, struct matriz m){
    int *melhor_local = malloc((m.numero_elementos + 1) * sizeof(int));
    int tamanho = n;
    int vet_inicial[tamanho];
    int vet_modificado[tamanho];

    for(int i = 0; i < n; i++) {
        vet_inicial[i] = caminho[i];
    }
    for(int i = 0; i < n; i++) {
    vet_modificado[i] = caminho[i];
    }

    int custo1 = calcular_custo2(m, vet_inicial);
    int custo2 = 0;

    for(int i=1; i<=5; i++){
        int aux;
        if(i==5){
            aux = vet_modificado[i];
            vet_modificado[i] = vet_modificado[1];
            vet_modificado[1] = aux;
        }
        else{
            aux = vet_modificado[i];
            vet_modificado[i] = vet_modificado[i+1];
            vet_modificado[i+1] = aux;
        }
        custo1 = calcular_custo2(m, vet_inicial);
        custo2 = calcular_custo2(m, vet_modificado);
        printf("\nCusto Inicial: %d", custo1);
        printf("\nImprimir mudancas no vetor: ");
            for(int i = 0; i<tamanho; i++){
            caminho[i] = vet_inicial[i];
            printf("%d ", vet_inicial[i]);
            }
        if(custo2 < custo1){
            for(int j = 0; j<=6; j++){
                vet_inicial[j] = vet_modificado[j];
            }
        }
        else{
            if(i==5){
                aux = vet_modificado[i];
                vet_modificado[i] = vet_modificado[1];
                vet_modificado[1] = aux;
            }
            else{
                aux = vet_modificado[i+1];
                vet_modificado[i+1] = vet_modificado[i];
                vet_modificado[i] = aux;
            }
        }
        printf("\nCusto Refinado: %d", custo2);
        printf("\n");
    }

    printf("\n");

    int custo = 0;
    for(int i = 0; i < m.numero_elementos; i++) {
        custo = custo + m.elementos[caminho[i]][caminho[i + 1]];
    }
    printf("Custo bestImprovement: %d", custo);
    printf("\n");
}
//----------------------------------------------------------------


void multi_start(int* caminho_melhor, struct matriz m,int n){
    int parada = 10;
    int solucao_local[7];
    int solucao_global[7];
    int custo1 = 0;
    int custo2 = 0;
    for(int i = 0; i<=6; i++){
        solucao_global[i] = caminho_melhor[i];
        solucao_local[i] = solucao_global[i];
    }
        custo2 = calcular_custo2(m, solucao_global);
        printf("\nSolucao melhor custo: %d", custo2);
        for(int j = 1; j<=parada; j++){
            printf("\n\n\nGerar solucao local aleatoria\n");
            solucao_inicial_random(m, solucao_local);
            imprimir_caminho(n, solucao_local);
            best_improvement(n, solucao_local, m);
            custo1 = calcular_custo2(m, solucao_local);
            if(custo1 < custo2){
                for(int x = 0; x<=6; x++){
                    solucao_global[x] = solucao_local[x];
                }
                custo2 = calcular_custo2(m, solucao_global);
                j=1;
            }
            printf("\nCusto da melhor solucao ate o momento: %d", custo2);
            linha();
            linha();
        }
        printf("Custo da solucao final: %d\n", custo2);
        printf("Solucao: ");
        for(int i=0;i<=6; i++){
            printf("%d ", solucao_global[i]);
        }
        linha();
        linha();
}
//-----------------------------------------------------------------------------


int main(int argc, char *argv[]) {
    printf("HEURISTICA VIZINHO MAIS PROXIMO\n\n");
    linha();
    struct matriz m;
    ler_arquivo(&m, "06_01_16.txt");
    imprimir_matriz(m);

    int *solucao_inicial = malloc((m.numero_elementos + 1) * sizeof(int));
    construir_caminho(m, solucao_inicial);
    printf("Solucao Gulosa: ");
    imprimir_caminho(m.numero_elementos + 1, solucao_inicial);
    int custo_solucao_inicial = calcular_custo2(m, solucao_inicial);
    printf("Custo Solucao Gulosa: %d\n", custo_solucao_inicial);
    linha();

    printf("\n\nMETAHEURISTICA MULTI-START\n\n");
    linha();
    linha();
    int *solucao_aleatoria_inicial = malloc((m.numero_elementos + 1) * sizeof(int));
    srand(time(NULL));
    solucao_inicial_random(m, solucao_aleatoria_inicial);
    printf("Solucao inicial aleatoria: ");
    imprimir_caminho(m.numero_elementos + 1, solucao_aleatoria_inicial);
    custo_solucao_inicial = calcular_custo2(m, solucao_aleatoria_inicial);
    printf("Custo da solucao inicial aleatoria: %d", custo_solucao_inicial);
    multi_start(solucao_aleatoria_inicial, m, m.numero_elementos+1);
}
