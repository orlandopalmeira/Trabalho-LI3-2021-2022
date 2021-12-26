#include "bstrees.h"
#include "users.h"
#ifndef PARSING_H
#define PARSING_H


int positiveIntStr(char *str, char endchar,int *number);

int validDateStr(char *str, char endchar);

int validArrayStr(char *str, char endchar,int *length);

int validTypeStr(char *str, char endchar);

int validBoolStr(char *str, char endchar);

int validString(char *str, short flag, char endchar, int *length);

int validLine_Users(char *str);

int validLine_Commits(char *str);

int validLine_Repos(char *str);

void getIDsFromCommit(char *str, int *repoID, int *commiterID, int *authorID);

int validLineCommits_Final(char *str, BSTreeINT usersTree, BSTreeINT reposTree, BSTreeINT *reposWithCommits);

int validLineRepos_Final(char *str, BSTreeINT usersTree, BSTreeINT reposWithCommits);

int readArray(char *arrStr, int *arrInt);

void readUserLine(char *line, CatUsers *cusers, int *users, int *orgs, int *bots);

void readCommitLine(char *line, CatUsers *cusers, int *collabs, int *commits, int *collabBots);

void readRepoLine(char *line, CatRepos *crepos, int *repos);

#endif