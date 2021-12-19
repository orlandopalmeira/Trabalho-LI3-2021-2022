#include "bstrees.h"
#ifndef READWRITE_H
#define READWRITE_H
#include <stdio.h>

int usersOK(FILE *toRead,FILE *toWrite);

int reposOK(FILE *toRead,FILE *toWrite);

int commitsOK(FILE *toRead,FILE *toWrite);

/**
 * Esta função devolve uma árvore binária de procura com os ID's dos utilizadores
 * provenientes do ficheiro users-ok.csv . 
 */
BSTreeINT usersIDsTree();

/**
 * Esta função devolve uma árvore binária de procura com os ID's dos repositórios
 * provenientes do ficheiro repos-ok.csv . 
 */
BSTreeINT reposIDsTree();

int commitsFinal(FILE *toRead, FILE *toWrite, BSTreeINT usersTree, BSTreeINT reposTree, BSTreeINT *reposWithCommits);

int reposFinal(FILE *toRead, FILE *toWrite, BSTreeINT usersTree, BSTreeINT reposWithCommits);

#endif