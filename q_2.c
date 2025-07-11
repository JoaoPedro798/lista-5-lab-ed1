#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stddef.h> // Para ptrdiff_t

#define MAX 1000000
#define TEXTO_TAMANHO 100000

// ---------- BUSCA SEQUENCIAL ----------
int busca_sequencial(int arr[], int n, int chave) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == chave)
            return i;
    }
    return -1;
}

// ---------- BUSCA BINÁRIA ----------
int busca_binaria(int arr[], int chave, int inicio, int fim) {
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;  // Evita overflow
        if (arr[meio] == chave)
            return meio;
        else if (arr[meio] < chave)
            inicio = meio + 1;
        else
            fim = meio - 1;
    }
    return -1;
}

// ---------- GERA ARRAY ----------
void gerar_array(int arr[], int n) {
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 100000;
}

// ---------- ORDENAÇÃO NECESSÁRIA PARA BUSCA BINÁRIA ----------
int comparar(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// ---------- TESTE DE TEMPO ----------
void testar_tempos(int arr[], int n, int chave) {
    clock_t inicio, fim;
    double tempo;
    
    // Ajusta iterações baseado no tamanho do array
    int iteracoes;
    if (n <= 1000) {
        iteracoes = 1000000;
    } else if (n <= 100000) {
        iteracoes = 100000;
    } else {
        iteracoes = 10000;
    }

    // Fazer uma cópia do array original
    int* arr_copia = malloc(n * sizeof(int));
    if (!arr_copia) {
        printf("Erro ao alocar memória para arr_copia\n");
        return;
    }
    memcpy(arr_copia, arr, n * sizeof(int));

    // Sequencial
    inicio = clock();
    for(int i = 0; i < iteracoes; i++) {
        busca_sequencial(arr_copia, n, chave);
    }
    fim = clock();
    tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC / iteracoes;
    printf("Busca Sequencial (média de %d execuções): %.10f segundos\n", 
           iteracoes, tempo);

    // Ordenar para binária
    qsort(arr_copia, n, sizeof(int), comparar);

    // Binária
    inicio = clock();
    for(int i = 0; i < iteracoes; i++) {
        busca_binaria(arr_copia, chave, 0, n - 1);
    }
    fim = clock();
    tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC / iteracoes;
    printf("Busca Binária (média de %d execuções): %.10f segundos\n", 
           iteracoes, tempo);

    free(arr_copia);
}

// ---------- BUSCA EM TEXTO ----------
void gerar_texto(char texto[], int tamanho) {
    for (int i = 0; i < tamanho - 1; i++)
        texto[i] = 'a' + rand() % 26;
    texto[tamanho - 1] = '\0';
}

void buscar_palavra_em_texto(char* texto, char* palavra) {
    clock_t inicio, fim;
    double tempo;
    int iteracoes = 100000;

    inicio = clock();
    char* encontrado = NULL;
    for(int i = 0; i < iteracoes; i++) {
        encontrado = strstr(texto, palavra);
    }
    fim = clock();

    tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC / iteracoes;
    if (encontrado) {
        printf("Palavra encontrada em posição %td\n", (ptrdiff_t)(encontrado - texto));
    } else {
        printf("Palavra não encontrada\n");
    }
    printf("Tempo de busca no texto (média de %d execuções): %.10f segundos\n", 
           iteracoes, tempo);
}

// ---------- MAIN ----------
int main() {
    srand((unsigned int)time(NULL));

    int tamanhos[] = {1000, 100000, 1000000};
    for (int i = 0; i < 3; i++) {
        int n = tamanhos[i];
        printf("\n--- Cenário com %d elementos ---\n", n);
        int* arr = malloc(n * sizeof(int));
        if (!arr) {
            printf("Erro ao alocar memória para arr\n");
            return 1;
        }
        gerar_array(arr, n);
        int chave = arr[rand() % n]; // chave garantida para estar no array
        testar_tempos(arr, n, chave);
        free(arr);
    }

    // Cenário d: busca em texto
    printf("\n--- Buscando uma palavra em um texto ---\n");
    char* texto = malloc(TEXTO_TAMANHO * sizeof(char));
    if (!texto) {
        printf("Erro ao alocar memória para texto\n");
        return 1;
    }
    gerar_texto(texto, TEXTO_TAMANHO);
    strcpy(&texto[5000], "algoritmo"); // palavra inserida no meio

    buscar_palavra_em_texto(texto, "algoritmo");
    buscar_palavra_em_texto(texto, "naoexiste");

    free(texto);
    return 0;
}