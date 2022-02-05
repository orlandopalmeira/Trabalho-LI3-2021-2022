#include "users.h"
#include "repos.h"
#ifndef LINKEDLISTS_H
#define LINKEDLISTS_H
#include <stdio.h>

typedef struct userList *UserList;

typedef struct repoList *RepoList;

typedef struct repoTree *RepoTree;

typedef struct languageList *LangList;

/* USERS LIST FUNCTIONS */

UserList createUserList(int length);

UserList minimumUserList(UserList list);

void insertUserList(UserList *list, User usr, int value);

void querie5WriteFile(FILE *toWrite, UserList list);

void querie6WriteFile(FILE *toWrite, UserList list);

void querie9WriteFile(FILE *toWrite, UserList list);

void querie10WriteFile(FILE *toWrite,UserList list);

void deleteUserList(UserList list);

/* REPOS TREE FUNCTIONS */

void insertRepoTree(RepoTree *repo_tree, Repo repo, char value);

unsigned int lenRepoTree(RepoTree repo_tree);

void deleteRepoTree(RepoTree repo_tree);

void querie7WriteFile(FILE* output, RepoTree repo_tree);

/* LANGUAGE LIST FUNCTIONS */

LangList createLangList(int length);

LangList minimumLangList(LangList list);

void insertLangList(LangList *list, char *lang);

void querie8WriteFile(FILE *toWrite, LangList *list, int topN);

void deleteLangList(LangList list);

#endif