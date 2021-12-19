#ifndef BSTREES_H
#define BSTREES_H

/**
 * Árvore binária de procura para números inteiros. 
 */
typedef struct nodeInt *BSTreeINT;

/**
 * Insere um elemento numa árvore de procura se este não existir na árvore.
 */
short insert(BSTreeINT *tree, int val);

/**
 * Verifica se um dado elemento pertence a uma árvore de procura. 
 */
short search(BSTreeINT tree, int key);

/**
 *  Esta função liberta toda a memória alocada numa árvore.
 */
void destroyBSTreeInt(BSTreeINT tree);

#endif