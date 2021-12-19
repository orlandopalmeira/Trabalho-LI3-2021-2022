#include <stdlib.h>
#include <string.h>
#include "commits.h"
#include "fileUtils.h"
#include "linkedLists.h"

struct commit{
    unsigned int repo_id, 
        		 author_id, 
        		 committer_id, 
        		 commit_at[3];
};

struct commits{
    Commit commt;
    struct commits *left, *right;
    char balance;
};

CatCommits rotateLeftC(CatCommits catcommits){
	CatCommits aux;
	if(catcommits!=NULL && catcommits->right!=NULL) {
		aux = catcommits->right;
		catcommits->right = aux->left;
		aux->left = catcommits;
		catcommits = aux;
	}
	return catcommits;
}

CatCommits rotateRightC(CatCommits catcommits){
	CatCommits aux;
	if(catcommits!=NULL && catcommits->left!=NULL) {
		aux = catcommits->left;
		catcommits->left = aux->right;
		aux->right = catcommits;
		catcommits = aux;
	}
	return catcommits;
}

CatCommits balanceLeftC(CatCommits catcommits){
	if(catcommits->left->balance==-1) {
		/* Rotacao simples a direita */
		catcommits = rotateRightC(catcommits);
		catcommits->balance=0;
		catcommits->right->balance=0;
	}
	else {
		/* Dupla rotacao */
		catcommits->left=rotateLeftC(catcommits->left);
		catcommits = rotateRightC(catcommits);
		switch (catcommits->balance) {
			case 0:
				catcommits->left->balance=0;
				catcommits->right->balance=0;
				break;
			case 1:
				catcommits->left->balance=-1;
				catcommits->right->balance=0;
				break;
			case -1:
				catcommits->left->balance=0;
				catcommits->right->balance=1;
		}
		catcommits->balance=0;
	}
	return catcommits;
}

CatCommits balanceRightC(CatCommits catcommits){
	if(catcommits->right->balance==1) {
		/* Rotacao simples a esquerda */
		catcommits = rotateLeftC(catcommits);
		catcommits->balance=0;
		catcommits->left->balance=0;
	}
	else {
		/* Dupla rotacao */
		catcommits->right = rotateRightC(catcommits->right);
		catcommits = rotateLeftC(catcommits);
		switch(catcommits->balance){
			case 0:
				catcommits->left->balance=0;
				catcommits->right->balance=0;
				break;
			case -1:
				catcommits->left->balance=0;
				catcommits->right->balance=1;
				break;
			case 1:
				catcommits->left->balance=-1;
				catcommits->right->balance=0;
		}
		catcommits->balance=0;
	}
	return catcommits;
}

CatCommits insereLeftC(CatCommits catcommits, Commit comm, int *cresceu){
	catcommits->left= insereCommit(catcommits->left, comm, cresceu);
	if(*cresceu) {
		switch (catcommits->balance) {
			case 1:
				catcommits->balance=0;
				*cresceu=0;
				break;
			case 0:
				catcommits->balance=-1;
				*cresceu=1;
				break;
			case -1:
				catcommits = balanceLeftC(catcommits);
				*cresceu=0;
		}
	}
	return catcommits;
}

CatCommits insereRightC(CatCommits catcommits, Commit comm, int *cresceu){
	catcommits->right = insereCommit(catcommits->right,comm,cresceu);
	if(*cresceu) {
		switch (catcommits->balance) {
			case -1:
				catcommits->balance=0;
				*cresceu=0;
				break;
			case 0:
				catcommits->balance=1;
				*cresceu=1;
				break;
			case 1:
				catcommits = balanceRightC(catcommits);
				*cresceu=0;
		}
	}
	return catcommits;
}

CatCommits insereCommit(CatCommits catcommits, Commit comm, int *cresceu){
	if(catcommits==NULL){
		catcommits = malloc(sizeof(struct commits));
		catcommits->commt = comm;
		catcommits->right=NULL;
		catcommits->left=NULL;
		catcommits->balance = 0;
		*cresceu=1;
	}
	else if(dateLower(comm->commit_at[0],comm->commit_at[1],comm->commit_at[2],catcommits->commt->commit_at[0],catcommits->commt->commit_at[1],catcommits->commt->commit_at[2])){
		catcommits = insereLeftC(catcommits,comm,cresceu);
	}
	else {
		catcommits = insereRightC(catcommits,comm,cresceu);
	}
	return catcommits;
}

Commit newCommit(){
	Commit new_commit = malloc(sizeof(struct commit));
	return new_commit;
}

void setCommit(Commit *comm, unsigned int repo_id, unsigned int author_id, unsigned int commiter_id, unsigned int *commit_at){
	(*comm)->repo_id = repo_id;
	(*comm)->author_id = author_id;
	(*comm)->committer_id = commiter_id;
	(*comm)->commit_at[0] = commit_at[0];
	(*comm)->commit_at[1] = commit_at[1];
	(*comm)->commit_at[2] = commit_at[2];
}

int howManyCommitsBetween(CatCommits ccommits, unsigned int *dateL, unsigned int *dateB){
	if(ccommits != NULL){
		if(dateBetween(dateL[0],dateL[1],dateL[2],ccommits->commt->commit_at[0],ccommits->commt->commit_at[1],ccommits->commt->commit_at[2],dateB[0],dateB[1],dateB[2])){
			return 1 + howManyCommitsBetween(ccommits->left,dateL,dateB) + howManyCommitsBetween(ccommits->right,dateL,dateB);
		}
		else{
			return howManyCommitsBetween(ccommits->left,dateL,dateB) + howManyCommitsBetween(ccommits->right,dateL,dateB);
		}
	}
	return 0;
}

int howManyCommitsInLang(char *language, CatCommits ccommits, CatRepos crepos){
	if(ccommits){
		Repo repo = searchRepo(ccommits->commt->repo_id,crepos);
		if(strcmp(language,getRepoLang(repo)) == 0) return 1 + howManyCommitsInLang(language,ccommits->left,crepos) + howManyCommitsInLang(language,ccommits->right,crepos);
		else return howManyCommitsInLang(language,ccommits->left,crepos) + howManyCommitsInLang(language,ccommits->right,crepos);
	}
	return 0;
}

int commitsInRepoFriend(CatCommits ccommits, CatRepos crepos,unsigned int *followers_list, unsigned int followers, unsigned int *following_list, unsigned int following){
	if(ccommits){
		int owner_id = getOwnerId(ccommits->commt->repo_id,crepos);
		if(belongsToArrInt(owner_id,followers_list,followers) && belongsToArrInt(owner_id,following_list,following))
			return 1 + commitsInRepoFriend(ccommits->left,crepos,followers_list,followers,following_list,following) + commitsInRepoFriend(ccommits->right,crepos,followers_list,followers,following_list,following);
		else return commitsInRepoFriend(ccommits->left,crepos,followers_list,followers,following_list,following) + commitsInRepoFriend(ccommits->right,crepos,followers_list,followers,following_list,following);
	}
	return 0;
}

CatCommits leftBranchCcommits(CatCommits ccommits){
	return ccommits->left;
}

CatCommits rightBranchCcommits(CatCommits ccommits){
	return ccommits->right;
}

unsigned int getRepoIdFromCommitRoot(CatCommits ccommits){
	return ccommits->commt->repo_id;
}

unsigned int *getCommitAtRoot(CatCommits ccommits){
	return ccommits->commt->commit_at;
}

unsigned int *getBiggestDate(CatCommits ccommits, CatRepos crepos, Repo *associatedRepo){
	if(ccommits && ccommits->right){
		return getBiggestDate(ccommits->right,crepos,associatedRepo);
	}else if(ccommits && ccommits->right == NULL){
		Repo found = searchRepo(ccommits->commt->repo_id,crepos);
		*associatedRepo = found;
		return ccommits->commt->commit_at;
	}else{
		return NULL;
	}
}

void deleteCatCommits(CatCommits catcommits){
    if(catcommits){
        deleteCatCommits(catcommits->left);
        deleteCatCommits(catcommits->right);
        free(catcommits->commt);
        free(catcommits);
    }
}