
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20
#define MAX_NOME 30
#define MAX_TIPO 20

// Definição do struct Componente
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int prioridade;
} Componente;

// Funções auxiliares
void removerQuebraLinha(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

void mostrarComponentes(Componente v[], int n) {
    printf("\n--- Lista de Componentes ---\n");
    for (int i = 0; i < n; i++) {
        printf("Nome: %-20s | Tipo: %-15s | Prioridade: %d\n",
               v[i].nome, v[i].tipo, v[i].prioridade);
    }
    printf("----------------------------\n");
}

// Bubble Sort por nome
int bubbleSortNome(Componente v[], int n) {
    int comp = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comp++;
            if (strcmp(v[j].nome, v[j+1].nome) > 0) {
                Componente temp = v[j];
                v[j] = v[j+1];
                v[j+1] = temp;
            }
        }
    }
    return comp;
}

// Insertion Sort por tipo
int insertionSortTipo(Componente v[], int n) {
    int comp = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = v[i];
        int j = i - 1;
        while (j >= 0 && (comp++, strcmp(v[j].tipo, chave.tipo) > 0)) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;
    }
    return comp;
}

// Selection Sort por prioridade
int selectionSortPrioridade(Componente v[], int n) {
    int comp = 0;
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            comp++;
            if (v[j].prioridade < v[minIdx].prioridade) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            Componente temp = v[i];
            v[i] = v[minIdx];
            v[minIdx] = temp;
        }
    }
    return comp;
}

// Busca binária por nome (apenas após bubble sort)
int buscaBinariaPorNome(Componente v[], int n, char chave[]) {
    int inicio = 0, fim = n - 1;
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        int cmp = strcmp(v[meio].nome, chave);
        if (cmp == 0) return meio;
        else if (cmp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

// Medir tempo de execução
void medirTempo(int (*algoritmo)(Componente[], int),
                Componente v[], int n, const char *nomeAlg) {
    Componente copia[MAX_COMPONENTES];
    memcpy(copia, v, n * sizeof(Componente));

    clock_t inicio = clock();
    int comparacoes = algoritmo(copia, n);
    clock_t fim = clock();

    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\n>>> %s executado.\n", nomeAlg);
    printf("Comparações: %d\n", comparacoes);
    printf("Tempo: %.6f segundos\n", tempo);

    mostrarComponentes(copia, n);
}

// Programa principal
int main() {
    Componente componentes[MAX_COMPONENTES];
    int n = 0, opcao;
    char chaveBusca[MAX_NOME];
    int ordenadoPorNome = 0;

    do {
        printf("\n===== MENU =====\n");
        printf("1. Cadastrar componente\n");
        printf("2. Ordenar por nome (Bubble Sort)\n");
        printf("3. Ordenar por tipo (Insertion Sort)\n");
        printf("4. Ordenar por prioridade (Selection Sort)\n");
        printf("5. Busca binária por nome\n");
        printf("6. Mostrar todos os componentes\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                if (n < MAX_COMPONENTES) {
                    printf("Nome: ");
                    fgets(componentes[n].nome, MAX_NOME, stdin);
                    removerQuebraLinha(componentes[n].nome);

                    printf("Tipo: ");
                    fgets(componentes[n].tipo, MAX_TIPO, stdin);
                    removerQuebraLinha(componentes[n].tipo);

                    printf("Prioridade (1-10): ");
                    scanf("%d", &componentes[n].prioridade);
                    getchar();
                    n++;
                } else {
                    printf("Limite de componentes atingido!\n");
                }
                break;
            case 2:
                medirTempo(bubbleSortNome, componentes, n, "Bubble Sort (Nome)");
                ordenadoPorNome = 1;
                break;
            case 3:
                medirTempo(insertionSortTipo, componentes, n, "Insertion Sort (Tipo)");
                ordenadoPorNome = 0;
                break;
            case 4:
                medirTempo(selectionSortPrioridade, componentes, n, "Selection Sort (Prioridade)");
                ordenadoPorNome = 0;
                break;
            case 5:
                if (!ordenadoPorNome) {
                    printf("Primeiro ordene os componentes por nome (opção 2).\n");
                } else {
                    printf("Digite o nome para buscar: ");
                    fgets(chaveBusca, MAX_NOME, stdin);
                    removerQuebraLinha(chaveBusca);
                    int idx = buscaBinariaPorNome(componentes, n, chaveBusca);
                    if (idx != -1) {
                        printf("Componente encontrado:\n");
                        printf("Nome: %s | Tipo: %s | Prioridade: %d\n",
                               componentes[idx].nome,
                               componentes[idx].tipo,
                               componentes[idx].prioridade);
                    } else {
                        printf("Componente NÃO encontrado!\n");
                    }
                }
                break;
            case 6:
                mostrarComponentes(componentes, n);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}