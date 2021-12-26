#include "commits.h"
#ifndef USERS_H
#define USERS_H

typedef struct user *User;

typedef struct users *CatUsers;

#define BOT 1
#define USER 2
#define ORG 3

CatUsers insereUser(CatUsers catusers, User us, int *cresceu);

User newUser();

User searchUser(CatUsers cusers, unsigned int id);
//void setUser(User *usr,unsigned int id, char type, unsigned int *date, unsigned int followers, unsigned int *followers_list, unsigned int following, unsigned int *following_list, char *login){
void setUser(User *usr, unsigned int id, char type, unsigned int *date, unsigned int followers, unsigned int *followers_list, unsigned int following, unsigned int *following_list, char *login);

CatCommits getCommitsWithID(CatUsers cusers, unsigned int id);

CatCommits getCommitsWithUser(User usr);

void setBiggestMsgSize(User *usr, unsigned int newMsgSize);

void setCommits(User *usr, CatCommits comms);

short isCollabBot(CatUsers cusers, unsigned int id);

CatUsers leftBranch(CatUsers cusers);

CatUsers rightBranch(CatUsers cusers);

unsigned int getMsgSizeRoot(CatUsers cusers);

User getRoot(CatUsers cusers);

unsigned int getIDFromRoot(CatUsers cusers);

char *getLoginFromRoot(CatUsers cusers);

unsigned int getIDUser(User usr);

char *getLoginUser(User usr);

unsigned int *getFollowersList(User usr);

unsigned int *getFollowingList(User usr);

unsigned int getNfollowers(User usr);

unsigned int getNfollowing(User usr);

void deleteCatUsers(CatUsers catusers);

#endif