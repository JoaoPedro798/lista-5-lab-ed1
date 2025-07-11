#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX 1000000

// ---------- Algoritmos de ordenação ----------

// Bubble Sort: compara pares adjacentes e troca se estiverem fora de ordem
void bubble_sort(int arr[], int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (arr[j] > arr[j+1]) {
                int tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
}

// Insertion Sort: insere cada elemento na posição correta do subarray à esquerda
void insertion_sort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int chave = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > chave) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = chave;
    }
}

// Função auxiliar para o Merge Sort: mescla dois subarrays ordenados
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));
    if (!L || !R) {
        printf("Erro de memória em merge\n");
        exit(1);
    }
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L);
    free(R);
}

// Merge Sort: divide o array e usa merge para ordenar
void merge_sort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort(arr, l, m);
        merge_sort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

// Quick Sort: escolhe um pivô e particiona o array em menores e maiores que o pivô
void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = arr[high], i = (low - 1);
        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                i++;
                int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
            }
        }
        int tmp = arr[i+1]; arr[i+1] = arr[high]; arr[high] = tmp;
        int pi = i + 1;

        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

// ---------- Utilitários ----------

// Preenche um array de inteiros com valores aleatórios
void gerar_array_int(int arr[], int n) {
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 10000;
}

// Preenche um array de caracteres com letras aleatórias
void gerar_array_char(char arr[], int n) {
    for (int i = 0; i < n; i++)
        arr[i] = 'A' + rand() % 26;
}

// ---------- Teste de tempo ----------

// Testa e mede o tempo de execução de um algoritmo de ordenação (função genérica)
void testar_algoritmo(const char* nome, void (*sort)(int[], int), int arr[], int n) {
    int* copia = (int*)malloc(n * sizeof(int));
    if (!copia) {
        printf("Erro de memória em testar_algoritmo\n");
        exit(1);
    }
    memcpy(copia, arr, n * sizeof(int));
    
    clock_t inicio = clock();
    sort(copia, n);
    clock_t fim = clock();

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("%s: %.4f segundos\n", nome, tempo);
    free(copia);
}

// Testa e mede o tempo do Merge Sort
void testar_merge(int arr[], int n) {
    int* copia = malloc(n * sizeof(int));
    memcpy(copia, arr, n * sizeof(int));

    clock_t inicio = clock();
    merge_sort(copia, 0, n - 1);
    clock_t fim = clock();

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Merge Sort: %.4f segundos\n", tempo);
    free(copia);
}

// Testa e mede o tempo do Quick Sort
void testar_quick(int arr[], int n) {
    int* copia = malloc(n * sizeof(int));
    memcpy(copia, arr, n * sizeof(int));

    clock_t inicio = clock();
    quick_sort(copia, 0, n - 1);
    clock_t fim = clock();

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Quick Sort: %.4f segundos\n", tempo);
    free(copia);
}

// ---------- Main ----------

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    int tamanhos[] = {1000, 100000, 1000000}; // Tamanhos dos arrays para teste

    // Testa os algoritmos de ordenação com arrays de inteiros de diferentes tamanhos
    for (int i = 0; i < 3; i++) {
        int n = tamanhos[i];
        printf("\n--- Testando com %d elementos inteiros ---\n", n);
        int* original = malloc(n * sizeof(int));
        gerar_array_int(original, n);

        // Bubble e Insertion só são testados para arrays pequenos
        if (n <= 10000) testar_algoritmo("Bubble Sort", bubble_sort, original, n);
        if (n <= 10000) testar_algoritmo("Insertion Sort", insertion_sort, original, n);

        testar_merge(original, n);
        testar_quick(original, n);
        free(original);
    }

    // Teste com array de caracteres
    int n = 1000;
    char* arr_chars = malloc(n * sizeof(char));
    gerar_array_char(arr_chars, n);

    printf("\n--- Testando com array de caracteres ---\n");

    int arr_int[n];
    // Converte cada caractere para inteiro (usando o valor ASCII)
    for (int i = 0; i < n; i++) arr_int[i] = (int)arr_chars[i];

    // Testa todos os algoritmos com o array convertido
    testar_algoritmo("Bubble Sort", bubble_sort, arr_int, n);
    testar_algoritmo("Insertion Sort", insertion_sort, arr_int, n);
    testar_merge(arr_int, n);
    testar_quick(arr_int, n);

    free(arr_chars);
    return 0;
}