#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para armazenar uma palavra e sua frequência
struct PalavraFrequencia {
    char* palavra;                      // Armazena a palavra
    int frequencia;                     // Armazena a frequência da palavra
    struct PalavraFrequencia* proximo;  // Aponta para o próximo nó na lista encadeada (em caso de colisão)
};

// Estrutura para a tabela de dispersão
struct TabelaDispersao {
    int tamanho;                        // Armazena o tamanho da tabela
    struct PalavraFrequencia** tabela;  // Aponta para a tabela propriamente dita (array de listas encadeadas)
};

// Função de hash simples: soma dos códigos ASCII dos caracteres da palavra
unsigned int hash(char* palavra, int tamanho) {
    unsigned int hash_valor = 0;
    while (*palavra) {
        hash_valor += *palavra;
        palavra++;
    }
    return hash_valor % tamanho;  // Módulo do hash pelo tamanho da tabela
}

// Inicializa a tabela de dispersão
struct TabelaDispersao* inicializar_tabela(int tamanho) {
    struct TabelaDispersao* tabela = (struct TabelaDispersao*)malloc(sizeof(struct TabelaDispersao));
    tabela->tamanho = tamanho;
    tabela->tabela = (struct PalavraFrequencia**)malloc(tamanho * sizeof(struct PalavraFrequencia*));

    // Inicializa todas as entradas da tabela como NULL
    int i = 0;
    while (i < tamanho) {
        tabela->tabela[i] = NULL;
        i++;
    }

    return tabela;
}

// Insere uma palavra na tabela de dispersão usando encadeamento externo para lidar com colisões
void inserir_palavra(struct TabelaDispersao* tabela, char* palavra) {
    int indice = hash(palavra, tabela->tamanho);

    struct PalavraFrequencia* atual = tabela->tabela[indice];
    while (atual != NULL) {
        if (strcmp(atual->palavra, palavra) == 0) {
            // Se a palavra já existe, incrementa a frequência
            atual->frequencia++;
            return;
        }
        atual = atual->proximo;
    }

    // Se a palavra não existe na lista, adiciona ao início da lista
    struct PalavraFrequencia* nova_palavra = (struct PalavraFrequencia*)malloc(sizeof(struct PalavraFrequencia));
    nova_palavra->palavra = strdup(palavra);
    nova_palavra->frequencia = 1;
    nova_palavra->proximo = tabela->tabela[indice];
    tabela->tabela[indice] = nova_palavra;
}

// Encontra a palavra mais frequente na tabela de dispersão
void palavra_mais_frequente(struct TabelaDispersao* tabela) {
    int max_frequencia = 0;
    char* palavra_mais_frequente = NULL;

    // Itera sobre a tabela de dispersão
    int i = 0;
    while (i < tabela->tamanho) {
        struct PalavraFrequencia* atual = tabela->tabela[i];
        // Itera sobre a lista encadeada no índice atual
        while (atual != NULL) {
            // Atualiza a palavra mais frequente se encontrar uma frequência maior
            if (atual->frequencia > max_frequencia) {
                max_frequencia = atual->frequencia;
                palavra_mais_frequente = atual->palavra;
            }
            atual = atual->proximo;
        }
        i++;
    }

    // Imprime a palavra mais frequente e sua frequência
    printf("%d %s\n", max_frequencia, palavra_mais_frequente);
}

void liberar_tabela(struct TabelaDispersao* tabela) {
    int i = 0;
    while (i < tabela->tamanho) {
        struct PalavraFrequencia* atual = tabela->tabela[i];
        while (atual != NULL) {
            struct PalavraFrequencia* proximo = atual->proximo;
            free(atual->palavra);
            free(atual);
            atual = proximo;
        }
        i++;
    }
    free(tabela->tabela);
    free(tabela);
}

int main() {
    // Inicializa uma tabela de dispersão com tamanho 10
    struct TabelaDispersao* tabela = inicializar_tabela(10);

    char palavra[100];

    // Loop de leitura interativo: lê palavras da entrada padrão até o final do arquivo (Ctrl+D)
    while (scanf("%s", palavra) != EOF) {
        // Insere cada palavra na tabela de dispersão
        inserir_palavra(tabela, palavra);
    }

    // Encontra e imprime a palavra mais frequente
    palavra_mais_frequente(tabela);

    // Libera a memória alocada para a tabela de dispersão
    liberar_tabela(tabela);

    return 0;
}
