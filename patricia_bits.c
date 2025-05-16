#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define um nó da árvore PATRICIA
typedef struct PatriciaNode {
    const char *key;  // Chave armazenada (apenas em folhas)
    int bitIndex;     // Índice do bit de decisão
    struct PatriciaNode *left, *right;
} PatriciaNode;

// Função para obter um bit específico de uma string
/*
    @params
    key: string de onde o bit será lido
    bitIndex: índice do bit a ser lido (0-127)
    @return
    valor do bit (0 ou 1)
*/
int getBit(const char *key, int bitIndex) {
    int byteIndex = bitIndex / 8;        // Índice do byte na string
    int bitOffset = 7 - (bitIndex % 8);  // Offset do bit dentro do byte

    if (key[byteIndex] == '\0') return 0;  // Evita leitura fora da string

    return (key[byteIndex] >> bitOffset) & 1;  // Retorna o bit desejado (0 ou 1)
}

// Compara duas strings
bool isEqual(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

// Cria um novo nó folha com uma chave
/*
    @params
    key: chave a ser armazenada no nó
    @return
    ponteiro para o novo nó folha
*/
PatriciaNode *createLeaf(const char *key) {
    PatriciaNode *node = (PatriciaNode *)malloc(sizeof(PatriciaNode));
    node->key = key;
    node->bitIndex = -1;  // Nó folha tem bitIndex -1
    node->left = node->right = NULL;
    return node;
}

// Função para inserir uma chave na árvore PATRICIA
/*
    @params
    root: raiz da árvore
    key: chave a ser inserida
    @return
    nova raiz da árvore (pode ser diferente se a raiz for alterada)
*/
PatriciaNode *insert(PatriciaNode *root, const char *key) {
    if (!root) return createLeaf(key);  // Caso base: árvore vazia

    PatriciaNode *p = root, *parent = NULL;

    // Percorre a árvore até encontrar um nó folha
    while (p->bitIndex > (parent ? parent->bitIndex : -1)) {
        parent = p;
        p = getBit(key, p->bitIndex) ? p->right : p->left;
    }

    if (isEqual(p->key, key)) return root;  // Já existe

    // Encontra o primeiro bit diferente entre as duas chaves
    int i = 0;
    while (getBit(p->key, i) == getBit(key, i)) i++;

    // Encontra local para inserir novo nó
    PatriciaNode *x = root, *prev = NULL;
    while (x->bitIndex > (prev ? prev->bitIndex : -1) && x->bitIndex < i) {
        prev = x;
        x = getBit(key, x->bitIndex) ? x->right : x->left;
    }

    // Cria novo nó interno
    PatriciaNode *newNode = (PatriciaNode *)malloc(sizeof(PatriciaNode));
    newNode->key = NULL;  // Nó interno não guarda chave
    newNode->bitIndex = i;

    // Cria novo nó folha para a chave
    PatriciaNode *newLeaf = createLeaf(key);

    // Decide para onde cada filho vai com base no bit da chave
    if (getBit(key, i)) {
        newNode->left = x;
        newNode->right = newLeaf;
    } else {
        newNode->left = newLeaf;
        newNode->right = x;
    }

    // Conecta novo nó ao pai anterior
    if (!prev) return newNode;
    if (getBit(key, prev->bitIndex))
        prev->right = newNode;
    else
        prev->left = newNode;

    return root;
}

// Busca uma chave na árvore
/*
    @params
    root: raiz da árvore
    key: chave a ser buscada
    @return
    true se a chave for encontrada, false caso contrário
*/
bool search(PatriciaNode *root, const char *key) {
    if (!root) return false;

    PatriciaNode *p = root, *prev = NULL;

    // Percorre enquanto bitIndex aumenta (garante que não entra em loop)
    while (p->bitIndex > (prev ? prev->bitIndex : -1)) {
        prev = p;
        p = getBit(key, p->bitIndex) ? p->right : p->left;
    }

    // Verifica se a chave encontrada é igual
    return isEqual(p->key, key);
}

// Função para imprimir a árvore (recursiva)
/*
    @params
    node: nó atual da árvore
    depth: profundidade atual na árvore (para formatação)
*/
void printTree(PatriciaNode *node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; i++) printf("  ");

    if (node->key) {
        printf("Leaf: %s (bit %d)\n", node->key, node->bitIndex);
    } else {
        printf("Node (bit %d)\n", node->bitIndex);
    }

    printTree(node->left, depth + 1);
    printTree(node->right, depth + 1);
}

// Função principal de teste
int main() {
    PatriciaNode *root = NULL;

    root = insert(root, "cachorro");
    root = insert(root, "cacto");
    root = insert(root, "carro");

    printTree(root, 0);

    printf("\nBusca por 'cacto': %s\n", search(root, "cacto") ? "Encontrado" : "Não encontrado");
    printf("Busca por 'casa': %s\n", search(root, "casa") ? "Encontrado" : "Não encontrado");

    return 0;
}
