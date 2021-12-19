#include <stdio.h>
#include <stdlib.h>
#include "parsing.h"

#define MB 1048576 // 1 MB para os arrays

int usersOK(FILE *toRead,FILE *toWrite){
    char buffer[MB] = "\0"; int count = 0;
    fgets(buffer,MB,toRead); fputs(buffer,toWrite);
    while(fgets(buffer,MB,toRead)){
        if(validLine_Users(buffer)){
            fputs(buffer,toWrite);
        }else{
            //printf("%s",buffer); 
            count++;
        }
    }
    return count;
}

int commitsOK(FILE *toRead,FILE *toWrite){
    char buffer[MB] = "\0";int count = 0;
    fgets(buffer,MB,toRead); fputs(buffer,toWrite);
    while(fgets(buffer,MB,toRead)){
        if(validLine_Commits(buffer)){
            fputs(buffer,toWrite); 
        }else {
            count++;
            //printf("%s",buffer);
        }
    }
    return count;
}

int reposOK(FILE *toRead,FILE *toWrite){
    char buffer[MB] = "\0";int count = 0, i = 2;
    fgets(buffer,MB,toRead); fputs(buffer,toWrite);
    while(fgets(buffer,MB,toRead)){
        if(validLine_Repos(buffer)){
            fputs(buffer,toWrite); 
        }else{
            count++;
            //printf("%s",buffer);
        } i++;
    }
    return count;
}


BSTreeINT usersIDsTree(){
    BSTreeINT tree = NULL;
    FILE *usersok = fopen("./saida/users-ok.csv","r"); // fazer fclose
    char buffer[MB], *buff = buffer;
    unsigned int aux;
    fgets(buffer,MB,usersok);
    while(fgets(buffer,MB,usersok)){
        aux = strtol(buffer,&buff,10);
        insert(&tree,aux);
    }
    fclose(usersok); // fclose feito
    return tree;
}

BSTreeINT reposIDsTree(){
    BSTreeINT tree = NULL;
    FILE *reposok = fopen("./saida/repos-ok.csv","r"); // fazer fclose
    char buffer[MB], *buff = buffer;
    unsigned int aux;
    fgets(buffer,MB,reposok);
    while(fgets(buffer,MB,reposok)){
        aux = strtol(buffer,&buff,10);
        insert(&tree,aux);
    }
    fclose(reposok); // fclose feito
    return tree;
}

int commitsFinal(FILE *toRead, FILE *toWrite, BSTreeINT usersTree, BSTreeINT reposTree, BSTreeINT *reposWithCommits){
    char buffer[MB] = "\0";int count = 0;
    fgets(buffer,MB,toRead); fputs(buffer,toWrite);
    while(fgets(buffer,MB,toRead)){
        if(validLineCommits_Final(buffer,usersTree,reposTree,reposWithCommits)){
            fputs(buffer,toWrite);
        }else {
            count++;
            //printf("%s",buffer);
        }
    }
    return count;
}

int reposFinal(FILE *toRead, FILE *toWrite, BSTreeINT usersTree, BSTreeINT reposWithCommits){
    char buffer[MB] = "\0"; int count = 0;
    fgets(buffer,MB,toRead); fputs(buffer,toWrite);
    while(fgets(buffer,MB,toRead)){
        if(validLineRepos_Final(buffer,usersTree,reposWithCommits)){
            fputs(buffer,toWrite);
        }else{
            count++;
            //printf("%s",buffer);
        }
    }
    return count;
}