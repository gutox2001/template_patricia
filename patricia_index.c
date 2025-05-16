#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Nó da árvore
typedef struct PatriciaNode {
    const char *key;  // Chave armazenada (apenas em folhas)
    int charIndex;    // Índice do caractere de decisão
    struct PatriciaNode *left, *right;
} PatriciaNode;

// Compara duas strings
bool isEqual(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

// Cria um novo nó folha
PatriciaNode *createLeaf(const char *key) {
    PatriciaNode *node = (PatriciaNode *)malloc(sizeof(PatriciaNode));
    node->key = key;
    node->charIndex = -1;  // Nó folha
    node->left = node->right = NULL;
    return node;
}

// Função para inserir uma chave na árvore
/*
    @params
    root: raiz da árvore
    key: chave(string) a ser inserida
    @return
    Ponteiro para nova raiz da árvore (pode ser diferente se a raiz for alterada)
*/
PatriciaNode *insert(PatriciaNode *root, const char *key) {
    // Se a árvore estiver vazia, cria uma nova folha com a chave
    if (!root) return createLeaf(key);

    // Se a árvore não estiver vazia, cria esses ponteiros para nós para percorrer a árvore
    PatriciaNode *p = root, *parent = NULL;

    // Percorre a árvore até encontrar um nó folha ou um nó com índice de caractere menor
    while (p->charIndex > (parent ? parent->charIndex : -1)) {
        parent = p;
        // Decide se vai para a subárvore esquerda ou direita com base no caractere atual
        p = key[p->charIndex] < p->key[p->charIndex] ? p->left : p->right;
    }

    // Se a chave já existir na árvore, não faz nada e retorna a raiz
    if (isEqual(p->key, key)) return root;

    // Encontra o primeiro índice onde as chaves diferem
    int i = 0;
    while (key[i] && p->key[i] && key[i] == p->key[i]) {
        i++;
    }

    PatriciaNode *x = root, *prev = NULL;

    // Percorre novamente a árvore para encontrar o ponto de inserção
    while (x->charIndex > (prev ? prev->charIndex : -1) && x->charIndex < i) {
        prev = x;
        // Decide se vai para a subárvore esquerda ou direita com base no caractere atual
        x = key[x->charIndex] < x->key[x->charIndex] ? x->left : x->right;
    }

    // Cria um novo nó interno para dividir a árvore no ponto de diferença
    PatriciaNode *newNode = (PatriciaNode *)malloc(sizeof(PatriciaNode));
    newNode->key = key;      // Temporariamente armazena a chave para comparação
    newNode->charIndex = i;  // Índice do caractere onde as chaves diferem

    // Cria uma nova folha para armazenar a chave
    PatriciaNode *newLeaf = createLeaf(key);

    // Define os filhos do novo nó interno com base na ordem dos caracteres
    if (key[i] < p->key[i]) {
        newNode->left = newLeaf;
        newNode->right = x;
    } else {
        newNode->left = x;
        newNode->right = newLeaf;
    }

    // Se o novo nó interno for a raiz, retorna ele
    if (!prev) return newNode;

    // Atualiza o ponteiro do pai para apontar para o novo nó interno
    if (key[prev->charIndex] < prev->key[prev->charIndex])
        prev->left = newNode;
    else
        prev->right = newNode;

    // Retorna a raiz da árvore
    return root;
}

// Função de busca
bool search(PatriciaNode *root, const char *key) {
    if (!root) return false;

    PatriciaNode *p = root, *prev = NULL;

    while (p->charIndex > (prev ? prev->charIndex : -1)) {
        prev = p;
        p = key[p->charIndex] < p->key[p->charIndex] ? p->left : p->right;
    }

    return isEqual(p->key, key);
}

// Impressão da árvore
void printTree(PatriciaNode *node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; i++) printf("  ");

    if (node->key && node->charIndex == -1)
        printf("Leaf: %s\n", node->key);
    else
        printf("Node at char[%d] = '%c'\n", node->charIndex, node->key[node->charIndex]);

    printTree(node->left, depth + 1);
    printTree(node->right, depth + 1);
}

// Teste
int main() {
    PatriciaNode *root = NULL;

    root = insert(root, "gato");
    root = insert(root, "galinha");
    root = insert(root, "golfinho");

    printTree(root, 0);

    printf("\nBusca por 'gato': %s\n", search(root, "gato") ? "Encontrado" : "Não encontrado");
    printf("Busca por 'galho': %s\n", search(root, "galho") ? "Encontrado" : "Não encontrado");

    return 0;
}
