#ifndef COMMITS_H
#define COMMITS_H
#include "repos.h"

typedef struct commit *Commit;

typedef struct commits *CatCommits;

CatCommits insereCommit(CatCommits catcommits, Commit comm, int *cresceu);

Commit newCommit();

void setCommit(Commit *comm, unsigned int repo_id, unsigned int author_id, unsigned int commiter_id, unsigned int *commit_at);

int howManyCommitsBetween(CatCommits ccommits, unsigned int *dateL, unsigned int *dateB);

int howManyCommitsInLang(char *language, CatCommits ccommits, CatRepos crepos);

int commitsInRepoFriend(CatCommits ccommits, CatRepos crepos, unsigned int *followers_list, unsigned int followers, unsigned int *following_list, unsigned int following);

CatCommits leftBranchCcommits(CatCommits ccommits);

CatCommits rightBranchCcommits(CatCommits ccommits);

unsigned int getRepoIdFromCommitRoot(CatCommits ccommits);

unsigned int *getCommitAtRoot(CatCommits ccommits);

unsigned int *getBiggestDate(CatCommits ccommits, CatRepos crepos, Repo *associatedRepo);

void deleteCatCommits(CatCommits catcommits);

#endif