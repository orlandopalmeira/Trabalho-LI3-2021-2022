#include <stdlib.h>
#include "bstrees.h"

struct nodeInt{
    int value;
    char balance;
    struct nodeInt *left;
    struct nodeInt *right;
};

BSTreeINT insere(BSTreeINT tree, int value, int *cresceu);

BSTreeINT rotateLeft(BSTreeINT tree) {
	BSTreeINT aux;
	if(tree!=NULL && tree->right!=NULL) {
		aux = tree->right;
		tree->right = aux->left;
		aux->left = tree;
		tree = aux;
	}
	return tree;
}

BSTreeINT rotateRight(BSTreeINT tree) {
	BSTreeINT aux;
	if(tree!=NULL && tree->left!=NULL) {
		aux = tree->left;
		tree->left = aux->right;
		aux->right = tree;
		tree = aux;
	}
	return tree;
}

BSTreeINT balanceLeft(BSTreeINT tree) {
	if(tree->left->balance==-1) {
		/* Rotacao simples a direita */
		tree = rotateRight(tree);
		tree->balance=0;
		tree->right->balance=0;
	}
	else {
		/* Dupla rotacao */
		tree->left=rotateLeft(tree->left);
		tree = rotateRight(tree);
		switch (tree->balance) {
		case 0:
			if(tree->left)tree->left->balance=0;
			if(tree->right)tree->right->balance=0;
			break;
		case 1:
			if(tree->left)tree->left->balance=-1;
			if(tree->right)tree->right->balance=0;
			break;
		case -1:
			if(tree->left)tree->left->balance=0;
			if(tree->right)tree->right->balance=1;
		}
		tree->balance=0;
	}
	return tree;
}

BSTreeINT balanceRight(BSTreeINT tree) {
	if(tree->right->balance==1) {
		/* Rotacao simples a esquerda */
		tree = rotateLeft(tree);
		tree->balance=0;
		tree->left->balance=0;
	}
	else {
		/* Dupla rotacao */
		tree->right = rotateRight(tree->right);
		tree = rotateLeft(tree);
		switch (tree->balance) {
		case 0:
			if(tree->left) tree->left->balance=0;
			if(tree->right) tree->right->balance=0;
			break;
		case -1:
			if(tree->left) tree->left->balance=0;
			if(tree->right) tree->right->balance=1;
			break;
		case 1:
			if(tree->left) tree->left->balance=-1;
			if(tree->right) tree->right->balance=0;
		}
		tree->balance=0;
	}
	return tree;
}

BSTreeINT insereLeft(BSTreeINT tree, int value, int *cresceu) {
	tree->left= insere(tree->left, value, cresceu);
	if(*cresceu) {
		switch (tree->balance) {
		case 1:
			tree->balance=0;
			*cresceu=0;
			break;
		case 0:
			tree->balance=-1;
			*cresceu=1;
			break;
		case -1:
			tree = balanceLeft(tree);
			*cresceu=0;
		}
	}
	return tree;
}

BSTreeINT insereRight(BSTreeINT tree, int value, int *cresceu) {
  tree->right = insere(tree->right,value,cresceu);
  if(*cresceu) {
    switch (tree->balance) {
      case -1:
          tree->balance=0;
          *cresceu=0;
          break;
      case 0:
          tree->balance=1;
          *cresceu=1;
          break;
      case 1:
          tree = balanceRight(tree);
          *cresceu=0;
    }
  }
  return tree;
}

BSTreeINT insere(BSTreeINT tree, int value, int *cresceu){
	if(tree==NULL){
		tree = malloc(sizeof(struct nodeInt));
		tree->value = value;
		tree->right=NULL;
		tree->left=NULL;
		tree->balance = 0;
		*cresceu=1;
	}
	else if(tree->value == value) return tree;
	else if(value < tree->value) {
		tree = insereLeft(tree,value,cresceu);
	}
	else {
		tree = insereRight(tree,value,cresceu);
	}
	return(tree);
}

void insert(BSTreeINT *tree, int value){
	int cresceu = 1;
	*tree = insere(*tree,value,&cresceu);
}

short search(BSTreeINT tree, int key){
    if(tree){
        int aux = tree->value;
        if(key == aux) return 1;
        else if(key < aux) return search(tree->left,key);
        else return search(tree->right,key);
    }
    return 0;
}

void destroyBSTreeInt(BSTreeINT tree){
    if(tree){
        destroyBSTreeInt(tree->left);
        destroyBSTreeInt(tree->right);
        free(tree);
    }
}