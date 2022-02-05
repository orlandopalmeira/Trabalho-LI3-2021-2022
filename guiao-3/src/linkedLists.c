#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linkedLists.h"
#include "users.h"

struct userList{
    int id, value;
    char *login;
    struct userList *next;
};

struct repoTree{
    unsigned int repo_id;
    char value;
    char *description;
    char balance;
    struct repoTree *left, *right;
};

struct repoList{
    unsigned int repo_id;
    char value;
    char *description;
    char balance;
    struct repoList *next;
};

struct languageList{
    char *language;
    int value;
    struct languageList *next;
};

/* USER LIST FUNCTIONS */

UserList createUserList(int length){
    if(length <= 0) return NULL;
    else{
        UserList newList = malloc(sizeof(struct userList));
        newList->id = -1;
        newList->login = NULL;
        newList->value = -1;
        newList->next = createUserList(length-1);
        return newList;
    }
}

UserList minimumUserList(UserList list){
    if(list != NULL){
        UserList curr, result = list;
        for(curr = list->next; curr != NULL; curr = curr->next){
            if(curr->value < result->value) result = curr;
        }
        return result;
    }
    return NULL;
}

void insertUserList(UserList *list, User usr, int value){
    if(*list){
        UserList min = minimumUserList(*list);
        if(min->value < value){
            free(min->login);
            min->id = getIDUser(usr);
            min->login = strdup(getLoginUser(usr));
            min->value = value;   
        }
    }
}

void querie5WriteFile(FILE *toWrite, UserList list){
    for(;list != NULL;list = list->next){
        fprintf(toWrite,"%d;%s;%d\n",list->id,list->login,list->value);
    }
}

void querie6WriteFile(FILE *toWrite, UserList list){
    for(;list != NULL;list = list->next){
        fprintf(toWrite,"%d;%s;%d\n",list->id,list->login,list->value);
    }
}

void querie9WriteFile(FILE *toWrite, UserList list){
    for(;list != NULL;list = list->next){
        fprintf(toWrite,"%d;%s\n",list->id,list->login);
    }
}

void querie10WriteFile(FILE *toWrite,UserList list){
    for(;list != NULL; list = list->next){
        fprintf(toWrite,"%d;%s;%d\n",list->id,list->login,list->value);
    }
}

void deleteUserList(UserList list){
    if(list){
        deleteUserList(list->next);
        free(list->login);
        free(list);
    }
}

/* REPOS TREE FUNCTIONS */

RepoTree rotateLeftRT(RepoTree repo_tree) {
    RepoTree aux;
    if(repo_tree!=NULL && repo_tree->right!=NULL) {
        aux = repo_tree->right;
        repo_tree->right = aux->left;
        aux->left = repo_tree;
        repo_tree = aux;
    }
    return repo_tree;
}

RepoTree rotateRightRT(RepoTree repo_tree) {
    RepoTree aux;
    if(repo_tree!=NULL && repo_tree->left!=NULL) {
        aux = repo_tree->left;
        repo_tree->left = aux->right;
        aux->right = repo_tree;
        repo_tree = aux;
    }
    return repo_tree;
}

RepoTree balanceLeftRT(RepoTree repo_tree) {
    if(repo_tree->left->balance==-1) {
        /* Rotacao simples a direita */
        repo_tree = rotateRightRT(repo_tree);
        repo_tree->balance=0;
        repo_tree->right->balance=0;
    }
    else {
    /* Dupla rotacao */
        repo_tree->left=rotateLeftRT(repo_tree->left);
        repo_tree = rotateRightRT(repo_tree);
        switch (repo_tree->balance) {
            case 0:
            if(repo_tree->left) repo_tree->left->balance=0;
            if(repo_tree->right) repo_tree->right->balance=0;
            break;
            case 1:
            if(repo_tree->left)repo_tree->left->balance=-1;
            if(repo_tree->right)repo_tree->right->balance=0;
            break;
            case -1:
            if(repo_tree->left)repo_tree->left->balance=0;
            if(repo_tree->right)repo_tree->right->balance=1;
        }
        repo_tree->balance=0;
    }
    return repo_tree;
}

RepoTree balanceRightRT(RepoTree repo_tree) {
if(repo_tree->right->balance==1) {
    /* Rotacao simples a esquerda */
    repo_tree = rotateLeftRT(repo_tree);
    repo_tree->balance=0;
    repo_tree->left->balance=0;
}
else {
    /* Dupla rotacao */
    repo_tree->right = rotateRightRT(repo_tree->right);
    repo_tree = rotateLeftRT(repo_tree);
    switch (repo_tree->balance) {
        case 0:
            if(repo_tree->left)repo_tree->left->balance=0;
            if(repo_tree->right)repo_tree->right->balance=0;
            break;
        case -1:
            if(repo_tree->left)repo_tree->left->balance=0;
            if(repo_tree->right)repo_tree->right->balance=1;
            break;
        case 1:
            if(repo_tree->left)repo_tree->left->balance=-1;
            if(repo_tree->right)repo_tree->right->balance=0;
    }
    repo_tree->balance=0;
    }
    return repo_tree;
}
RepoTree insereRepoTreeAux(RepoTree repo_tree, Repo repo, char value, int *cresceu);

RepoTree insereLeftRT(RepoTree repo_tree, Repo rp, char value ,int *cresceu) {
    repo_tree->left = insereRepoTreeAux(repo_tree->left,rp,value,cresceu);
    if(*cresceu) {
        switch (repo_tree->balance) {
            case 1:
                repo_tree->balance=0;
                *cresceu=0;
                break;
            case 0:
                repo_tree->balance=-1;
                *cresceu=1;
                break;
            case -1:
                repo_tree = balanceLeftRT(repo_tree);
                *cresceu=0;
        }
    }
    return repo_tree;
}

RepoTree insereRightRT(RepoTree repo_tree, Repo rp, char value ,int *cresceu) {
    repo_tree->right = insereRepoTreeAux(repo_tree->right,rp,value,cresceu);
    if(*cresceu) {
        switch (repo_tree->balance) {
            case -1:
                repo_tree->balance=0;
                *cresceu=0;
                break;
            case 0:
                repo_tree->balance=1;
                *cresceu=1;
                break;
            case 1:
                repo_tree = balanceRightRT(repo_tree);
                *cresceu=0;
        }
    }
    return repo_tree;
}

RepoTree insereRepoTreeAux(RepoTree repo_tree, Repo repo, char value, int *cresceu){
    if(repo_tree==NULL){
        repo_tree = malloc(sizeof(struct repoTree));
        repo_tree->repo_id = getRepoID(repo);
        repo_tree->value = value;
        repo_tree->description = strdup(getRepoDesc(repo));
        repo_tree->balance = 0;
        repo_tree->right=NULL;
        repo_tree->left=NULL;
        *cresceu=1;
    }
    else if(repo_tree->repo_id == getRepoID(repo) && repo_tree->value == 1 && value == 0){ 
        repo_tree->value = value;
        return repo_tree;
    }
    else if(getRepoID(repo) < repo_tree->repo_id) {
        repo_tree = insereLeftRT(repo_tree,repo,value,cresceu);
    }
    else {
        repo_tree = insereRightRT(repo_tree,repo,value,cresceu);
    }
    return(repo_tree);
}

void querie7WriteFile(FILE* output, RepoTree repo_tree){
    if(repo_tree){
        querie7WriteFile(output,repo_tree->left);
        if(repo_tree->value != 0){
            fprintf(output,"%d;%s\n",repo_tree->repo_id,repo_tree->description);
        }
        querie7WriteFile(output,repo_tree->right);
    }
}

void insertRepoTree(RepoTree *repo_tree, Repo repo, char value){
    int cresceu = 1;
    *repo_tree = insereRepoTreeAux(*repo_tree,repo,value,&cresceu);
}

unsigned int lenRepoTree(RepoTree repo_tree){
    if(repo_tree){
        if(repo_tree->value != 0) return 1 + lenRepoTree(repo_tree->left) + lenRepoTree(repo_tree->right);
        else return lenRepoTree(repo_tree->left) + lenRepoTree(repo_tree->right);
    }else{
        return 0;
    }
}

void deleteRepoTree(RepoTree repo_tree){
    if(repo_tree){
        deleteRepoTree(repo_tree->left);
        deleteRepoTree(repo_tree->right);
        free(repo_tree->description);
        free(repo_tree);
    }
}

/* LANGUAGE LIST FUNCTIONS */
LangList createLangList(int length){
    if(length <= 0) return NULL;
    else{
        LangList newList = malloc(sizeof(struct languageList));
        newList->language = NULL;
        newList->value = 0;
        newList->next = createLangList(length-1);
        return newList;
    }
}

LangList minimumLangList(LangList list){
    if(list != NULL){
        LangList curr, result = list;
        for(curr = list->next; curr != NULL; curr = curr->next){
            if(curr->value < result->value) result = curr;
        }
        return result;
    }
    return NULL;
}

LangList maximumLangList(LangList *list){
    if(list != NULL && *list != NULL){
        LangList antCurr = *list, antRes = NULL, curr, result = *list;
        for(curr = (*list)->next; curr != NULL; curr = curr->next){
            if(curr->value > result->value){
                antRes = antCurr;
                result = curr;
            }
            antCurr = curr;
        }
        if(antRes == NULL){ // retira da cabeça
            (*list) = (*list)->next;
            result->next = NULL;
            return result;
        }else{ // retira do meio ou do fim da lista
            antRes->next = result->next;
            result->next = NULL;
            return result;
        }
    }
    return NULL;
}

LangList searchLang(LangList list, char *language){
    if(list){
        if(!strcmp(list->language,language)) return list;
        else return searchLang(list->next,language);
    }
    return NULL;
}

void insertLangList(LangList *list, char *lang){
    if(strcmp(lang,"None\0") == 0) return;
    if(*list){
        LangList found = searchLang(*list,lang);
        if(found){
            (found->value)++;
        }else{
            LangList newLang = malloc(sizeof(struct languageList));
            newLang->language = strdup(lang);
            newLang->value = 1;
            newLang->next = *list;
            *list = newLang;
        }
    }else{
        *list = malloc(sizeof(struct languageList));
        (*list)->language = strdup(lang);
        (*list)->value = 1;
        (*list)->next = NULL;
    }
}

void querie8WriteFile(FILE *toWrite, LangList *list, int topN){
    if(topN > 0){
        LangList max = maximumLangList(list);
        if(!max) return;
        if(strcmp(max->language,"None\0") != 0){
            fprintf(toWrite,"%s\n",max->language);
            free(max->language); free(max);
            querie8WriteFile(toWrite,list,topN-1);
        }else{
            free(max->language); free(max);
            querie8WriteFile(toWrite,list,topN);
        }
    }
}

void deleteLangList(LangList list){
    if(list){
        deleteLangList(list->next);
        free(list->language);
        free(list);
    }
}