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

struct repoList{
    unsigned int repo_id;
    int value;
    char *description;
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

/* REPOS LIST FUNCTIONS */

RepoList createRepoList(int length){
    if(length <= 0) return NULL;
    else{
        RepoList newList = malloc(sizeof(struct repoList));
        newList->repo_id = -1;
        newList->description = NULL;
        newList->value = -1;
        newList->next = createRepoList(length-1);
        return newList;
    }
}

RepoList minimumRepoList(RepoList list){
    if(list != NULL){
        RepoList curr, result = list;
        for(curr = list->next; curr != NULL; curr = curr->next){
            if(curr->value < result->value) result = curr;
        }
        return result;
    }
    return NULL;
}

RepoList searchRepoList(unsigned int repo_id, RepoList list){
    if(list){
        if(list->repo_id == repo_id) return list;
        else return searchRepoList(repo_id,list->next);
    }
    return NULL;
}

void insertRepoList(RepoList *list, Repo repo, int value){
    if(*list){
        RepoList found = searchRepoList(getRepoID(repo),*list);
        if(found){
            if(found->value == 1 && value == 0) found->value = value;
        }else{
            RepoList newNode = malloc(sizeof(struct repoList));
            newNode->repo_id = getRepoID(repo);
            newNode->description = strdup(getRepoDesc(repo));
            newNode->value = value;
            newNode->next = *list;
            *list = newNode;
        }
    }else{
        (*list) = malloc(sizeof(struct repoList));
        (*list)->repo_id = getRepoID(repo);
        (*list)->description = strdup(getRepoDesc(repo));
        (*list)->value = value;
        (*list)->next = NULL;
    }
}

void querie7WriteFile(FILE *output,RepoList list){
    if(list){
        if(list->value != 0){
            fprintf(output,"%d;%s\n",list->repo_id,list->description);
        }
        querie7WriteFile(output,list->next);
    }
}

void deleteRepoList(RepoList list){
    if(list){
        deleteRepoList(list->next);
        free(list->description);
        free(list);
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
    if(list != NULL){
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

void testingFunc(LangList list){
    if(list){
        printf("%s;%d\n",list->language,list->value);
        testingFunc(list->next);
    }
}

void querie8WriteFile(FILE *toWrite, LangList *list, int topN){
    if(topN > 0){
        LangList max = maximumLangList(list);
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