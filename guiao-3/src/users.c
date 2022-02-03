#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "commits.h"
#include "users.h"

struct user {
	unsigned int id;               /* ID User */
	char type;       		       /* Tipo de USER - existem constantes no users.h que dizem qual o tipo */
	unsigned int created_at[3],    /* Data de criação - [day,month,year] */
        	     followers,        /* Quantas pessoas seguem o USER */
        	     *followers_list,  /* Lista de seguidores */
        	     following,        /* Numero de USERS que está a seguir */
        	     *following_list,  /* Lista de USERS que segue */
        	     biggest_msg_size; /* Tamanho da sua maior mensagem */
  	char *login;        /* Nome de utilizador */
  	CatCommits commits; /* Commits do utilizador*/
};

struct users{
    User usr;
    struct users *left, *right;
    char balance;
};

CatUsers rotateLeftU(CatUsers catusers) {
	CatUsers aux;
	if(catusers!=NULL && catusers->right!=NULL) {
		aux = catusers->right;
		catusers->right = aux->left;
		aux->left = catusers;
		catusers = aux;
	}
	return catusers;
}

CatUsers rotateRightU(CatUsers catusers) {
	CatUsers aux;
	if(catusers!=NULL && catusers->left!=NULL) {
		aux = catusers->left;
		catusers->left = aux->right;
		aux->right = catusers;
		catusers = aux;
	}
	return catusers;
}

CatUsers balanceLeftU(CatUsers catusers) {
	if(catusers->left->balance==-1) {
		/* Rotacao simples a direita */
		catusers = rotateRightU(catusers);
		catusers->balance=0;
		catusers->right->balance=0;
	}
	else {
		/* Dupla rotacao */
		catusers->left=rotateLeftU(catusers->left);
		catusers = rotateRightU(catusers);
		switch (catusers->balance) {
		case 0:
			catusers->left->balance=0;
			catusers->right->balance=0;
			break;
		case 1:
			catusers->left->balance=-1;
			catusers->right->balance=0;
			break;
		case -1:
			catusers->left->balance=0;
			catusers->right->balance=1;
		}
		catusers->balance=0;
	}
	return catusers;
}

CatUsers balanceRightU(CatUsers catusers) {
	if(catusers->right->balance==1) {
		/* Rotacao simples a esquerda */
		catusers = rotateLeftU(catusers);
		catusers->balance=0;
		catusers->left->balance=0;
	}
	else {
		/* Dupla rotacao */
		catusers->right = rotateRightU(catusers->right);
		catusers = rotateLeftU(catusers);
		switch (catusers->balance) {
		case 0:
			catusers->left->balance=0;
			catusers->right->balance=0;
			break;
		case -1:
			catusers->left->balance=0;
			catusers->right->balance=1;
			break;
		case 1:
			catusers->left->balance=-1;
			catusers->right->balance=0;
		}
		catusers->balance=0;
	}
	return catusers;
}

CatUsers insereLeftU(CatUsers catusers, User us, int *cresceu) {
	catusers->left= insereUser(catusers->left, us, cresceu);
	if(*cresceu) {
		switch (catusers->balance) {
		case 1:
			catusers->balance=0;
			*cresceu=0;
			break;
		case 0:
			catusers->balance=-1;
			*cresceu=1;
			break;
		case -1:
			catusers = balanceLeftU(catusers);
			*cresceu=0;
		}
	}
	return catusers;
}

CatUsers insereRightU(CatUsers catusers, User us, int *cresceu) {
  catusers->right = insereUser(catusers->right,us,cresceu);
  if(*cresceu) {
    switch (catusers->balance) {
      case -1:
          catusers->balance=0;
          *cresceu=0;
          break;
      case 0:
          catusers->balance=1;
          *cresceu=1;
          break;
      case 1:
          catusers = balanceRightU(catusers);
          *cresceu=0;
    }
  }
  return catusers;
}

CatUsers insereUser(CatUsers catusers, User us, int *cresceu){
	if(catusers==NULL){
		catusers = malloc(sizeof(struct users));
		catusers->usr = us;
		catusers->right=NULL;
		catusers->left=NULL;
		catusers->balance = 0;
		*cresceu=1;
	}
	else if(catusers->usr->id == us->id) return catusers;
	else if(us->id < catusers->usr->id) {
		catusers = insereLeftU(catusers,us,cresceu);
	}
	else {
		catusers = insereRightU(catusers,us,cresceu);
	}
	return(catusers);
}

User newUser(){
	struct user *new_user = NULL;
	new_user = calloc(1,sizeof(struct user));
	return new_user;
}

User searchUser(CatUsers cusers, unsigned int id){
	if(cusers == NULL) return NULL;
	else if(cusers->usr->id == id) return cusers->usr;
	else if(id < cusers->usr->id) return searchUser(cusers->left,id);
	else return searchUser(cusers->right,id);
}

void setUser(User *usr,unsigned int id, char type, unsigned int *date, unsigned int followers, unsigned int *followers_list, unsigned int following, unsigned int *following_list, char *login){
	(*usr)->id = id;
	(*usr)->type = type;
	(*usr)->created_at[0] = date[0];
	(*usr)->created_at[1] = date[1];
	(*usr)->created_at[2] = date[2];
	(*usr)->followers = followers;
	(*usr)->followers_list = followers_list;
	(*usr)->following = following;
	(*usr)->following_list = following_list;
	(*usr)->biggest_msg_size = 0;
	(*usr)->login = strdup(login);
}

CatCommits getCommitsWithID(CatUsers cusers, unsigned int id){
	User found = searchUser(cusers,id);
	return found->commits;
}

CatCommits getCommitsWithUser(User usr){
  	return usr->commits;
}

void setBiggestMsgSize(User *usr, unsigned int newMsgSize){
	if((*usr)->biggest_msg_size < newMsgSize) 
		(*usr)->biggest_msg_size = newMsgSize;
}

void setCommits(User *usr, CatCommits comms){
  	(*usr)->commits = comms;
}

short isCollabBot(CatUsers cusers, unsigned int id){
	User found = searchUser(cusers,id);
	return found != NULL && found->commits != NULL && found->type == BOT;
}

CatUsers leftBranch(CatUsers cusers){
  	return cusers->left;
}

CatUsers rightBranch(CatUsers cusers){
  	return cusers->right;
}

User getRoot(CatUsers cusers){
 	return cusers->usr;
}

unsigned int getIDFromRoot(CatUsers cusers){
  return cusers ? cusers->usr->id : 0;
}

char *getLoginFromRoot(CatUsers cusers){
  	return cusers ? cusers->usr->login : NULL;
}

unsigned int getMsgSizeRoot(CatUsers cusers){
  	return cusers ? cusers->usr->biggest_msg_size : 0;
}

unsigned int getIDUser(User usr){
  	return usr->id;
}

char *getLoginUser(User usr){
  	return usr->login;
}

unsigned int *getFollowersList(User usr){
	return usr->followers_list;
}

unsigned int *getFollowingList(User usr){
	return usr->following_list;
}

unsigned int getNfollowers(User usr){
	return usr->followers;
}

unsigned int getNfollowing(User usr){
	return usr->following;
}

void deleteCatUsers(CatUsers catusers) {
	if(catusers!=NULL) {
		deleteCatUsers(catusers->left);
		deleteCatUsers(catusers->right);
		free(catusers->usr->followers_list);
		free(catusers->usr->following_list);
		free(catusers->usr->login);
		deleteCatCommits(catusers->usr->commits);
		free(catusers->usr);
		catusers->balance=0;
		free(catusers);
	}
}