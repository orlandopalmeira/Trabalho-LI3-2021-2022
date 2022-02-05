#include <stdlib.h>
#include <string.h>
#include "repos.h"

struct repo{
    unsigned int id,
                 owner_id,
                 created_at[3];
    char *language,
         *description;
};

struct repos{
    Repo repo;
    struct repos *left, *right;
    char balance;
};

CatRepos rotateLeftR(CatRepos catrepos) {
    CatRepos aux;
    if(catrepos!=NULL && catrepos->right!=NULL) {
        aux = catrepos->right;
        catrepos->right = aux->left;
        aux->left = catrepos;
        catrepos = aux;
    }
    return catrepos;
}

CatRepos rotateRightR(CatRepos catrepos) {
    CatRepos aux;
    if(catrepos!=NULL && catrepos->left!=NULL) {
        aux = catrepos->left;
        catrepos->left = aux->right;
        aux->right = catrepos;
        catrepos = aux;
    }
    return catrepos;
}

CatRepos balanceLeftR(CatRepos catrepos) {
    if(catrepos->left->balance==-1) {
        /* Rotacao simples a direita */
        catrepos = rotateRightR(catrepos);
        catrepos->balance=0;
        catrepos->right->balance=0;
    }
    else {
    /* Dupla rotacao */
        catrepos->left=rotateLeftR(catrepos->left);
        catrepos = rotateRightR(catrepos);
        switch (catrepos->balance) {
            case 0:
            catrepos->left->balance=0;
            catrepos->right->balance=0;
            break;
            case 1:
            catrepos->left->balance=-1;
            catrepos->right->balance=0;
            break;
            case -1:
            catrepos->left->balance=0;
            catrepos->right->balance=1;
        }
        catrepos->balance=0;
    }
    return catrepos;
}

CatRepos balanceRightR(CatRepos catrepos) {
if(catrepos->right->balance==1) {
    /* Rotacao simples a esquerda */
    catrepos = rotateLeftR(catrepos);
    catrepos->balance=0;
    catrepos->left->balance=0;
}
else {
    /* Dupla rotacao */
    catrepos->right = rotateRightR(catrepos->right);
    catrepos = rotateLeftR(catrepos);
    switch (catrepos->balance) {
        case 0:
            catrepos->left->balance=0;
            catrepos->right->balance=0;
            break;
        case -1:
            catrepos->left->balance=0;
            catrepos->right->balance=1;
            break;
        case 1:
            catrepos->left->balance=-1;
            catrepos->right->balance=0;
    }
    catrepos->balance=0;
    }
    return catrepos;
}

CatRepos insereLeftR(CatRepos catrepos, Repo rp, int *cresceu) {
    catrepos->left= insereRepo(catrepos->left, rp, cresceu);
    if(*cresceu) {
        switch (catrepos->balance) {
            case 1:
                catrepos->balance=0;
                *cresceu=0;
                break;
            case 0:
                catrepos->balance=-1;
                *cresceu=1;
                break;
            case -1:
                catrepos = balanceLeftR(catrepos);
                *cresceu=0;
        }
    }
    return catrepos;
}

CatRepos insereRightR(CatRepos catrepos, Repo rp, int *cresceu) {
    catrepos->right = insereRepo(catrepos->right,rp,cresceu);
    if(*cresceu) {
        switch (catrepos->balance) {
            case -1:
                catrepos->balance=0;
                *cresceu=0;
                break;
            case 0:
                catrepos->balance=1;
                *cresceu=1;
                break;
            case 1:
                catrepos = balanceRightR(catrepos);
                *cresceu=0;
        }
    }
    return catrepos;
}

CatRepos insereRepo(CatRepos catrepos, Repo repo, int *cresceu){
    if(catrepos==NULL){
        catrepos = malloc(sizeof(struct repos));
        catrepos->repo = repo;
        catrepos->right=NULL;
        catrepos->left=NULL;
        catrepos->balance = 0;
        *cresceu=1;
    }
    else if(catrepos->repo->id == repo->id) return catrepos;
    else if(repo->id < catrepos->repo->id) {
        catrepos = insereLeftR(catrepos,repo,cresceu);
    }
    else {
        catrepos = insereRightR(catrepos,repo,cresceu);
    }
    return(catrepos);
}

Repo searchRepo(unsigned int id, CatRepos crepos){
    if(crepos){
		unsigned int rootRepoId = crepos->repo->id;
		if(rootRepoId == id) return crepos->repo;
		else if(id < rootRepoId) return searchRepo(id,crepos->left);
		else return searchRepo(id,crepos->right);
	}
	return NULL;
}

Repo newRepo(){
    Repo new_repo = malloc(sizeof(struct repo));
    return new_repo;
}

void setRepo(Repo *repo, unsigned int id, unsigned int owner_id, unsigned int *created_at, char *description,char *language){
    (*repo)->id = id;
    (*repo)->owner_id = owner_id;
    (*repo)->created_at[0] = created_at[0];
    (*repo)->created_at[1] = created_at[1];
    (*repo)->created_at[2] = created_at[2];
    (*repo)->description = strdup(description);
    (*repo)->language = strdup(language);
}

unsigned int getRepoID(Repo repo){
    return repo ? repo->id : 0;
}

char *getRepoDesc(Repo repo){
    return repo ? repo->description : NULL;
}

char *getRepoLang(Repo repo){
    return repo ? repo->language : NULL;
}

char *getRepoLangWithID(unsigned int id, CatRepos crepos){
    if(crepos){
        unsigned int rootID = crepos->repo->id;
        if(rootID == id) return crepos->repo->language;
        else if (id < rootID) return getRepoLangWithID(id,crepos->left);
        else return getRepoLangWithID(id,crepos->right);
    }
    return NULL;
}

char *getRepoLangRoot(CatRepos crepos){
    return crepos ? crepos->repo->language : NULL;
}

unsigned int *repoCreatedAtRoot(CatRepos crepos){
    return crepos ? crepos->repo->created_at : NULL;
}

unsigned int getOwnerId(unsigned int repo_id,CatRepos crepos){
    if(!crepos) return 0;
    Repo repo = searchRepo(repo_id,crepos);
	return repo->owner_id;
}

CatRepos leftBranchRepos(CatRepos crepos){
    return crepos ? crepos->left : NULL;
}

CatRepos rightBranchRepos(CatRepos crepos){
    return crepos ? crepos->right : NULL;
}

void deleteCatRepos(CatRepos catrepos){
    if(catrepos){
        deleteCatRepos(catrepos->left);
        deleteCatRepos(catrepos->right);
        free(catrepos->repo->language);
        free(catrepos->repo->description);
        free(catrepos->repo);
        free(catrepos);
    }
}
