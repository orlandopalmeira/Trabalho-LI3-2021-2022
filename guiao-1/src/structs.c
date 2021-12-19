#include <stdlib.h>
#include "structs.h"

short insert(BSTreeINT *tree, int val){
    if(*tree){
        int aux = (*tree)->value;
        if(val == aux) return 1;
        else if(val < aux) return insert(&((*tree)->left),val);
        else return insert(&((*tree)->right),val);
    }else{
        *tree = malloc(sizeof(struct nodeInt));
        (*tree)->value = val;
        (*tree)->left = (*tree)->right = NULL;
        return 0;
    }
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