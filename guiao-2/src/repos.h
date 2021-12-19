#ifndef REPOS_H
#define REPOS_H

typedef struct repo *Repo;

typedef struct repos *CatRepos;

CatRepos insereRepo(CatRepos catrepos, Repo repo, int *cresceu);

Repo searchRepo(unsigned int id, CatRepos crepos);

Repo newRepo();

void setRepo(Repo *repo, unsigned int id, unsigned int owner_id, unsigned int *created_at, char *description ,char *language);

unsigned int getRepoID(Repo repo);

char *getRepoDesc(Repo repo);

char *getRepoLang(Repo repo);

char *getRepoLangWithID(unsigned int id, CatRepos crepos);

char *getRepoLangRoot(CatRepos crepos);

unsigned int *repoCreatedAtRoot(CatRepos crepos);

unsigned int getOwnerId(unsigned int repo_id,CatRepos crepos);

CatRepos leftBranchRepos(CatRepos crepos);

CatRepos rightBranchRepos(CatRepos crepos);

void deleteCatRepos(CatRepos catrepos);

#endif