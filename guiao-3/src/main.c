#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readWrite.h"
#include "parsing.h"
#include "linkedLists.h"
#include "fileUtils.h"

#define MB 1048576 // 1 MB para os arrays de caracteres

/* Função para executar o exercício 1 do guião 1.*/
void g1_ex1(){
    FILE *users = fopen("./entrada/users.csv","r"),
         *usersok = fopen("./saida/users-ok.csv","w"),
         *commits = fopen("./entrada/commits.csv","r"),
         *commitsok = fopen("./saida/commits-ok.csv","w"),
         *repos   = fopen("./entrada/repos.csv","r"), 
         *reposok = fopen("./saida/repos-ok.csv","w");
    usersOK(users,usersok); 
    commitsOK(commits,commitsok); 
    reposOK(repos,reposok); 
    fclose(users); fclose(usersok);
    fclose(commits); fclose(commitsok);
    fclose(repos); fclose(reposok);
}

/* Função para executar o exercício 2 do guião 1.*/
void g1_ex2(){
    BSTreeINT usersTree = usersIDsTree(), reposTree = reposIDsTree(), reposWithCommits = NULL; // libertar a memória no fim da função
    FILE *commitsok = fopen("./saida/commits-ok.csv","r"), 
         *commitsfinal = fopen("./saida/commits-final.csv","w"),
         *usersok = fopen("./saida/users-ok.csv","r"),
         *usersfinal = fopen("./saida/users-final.csv","w"),
         *reposok = fopen("./saida/repos-ok.csv","r"),
         *reposfinal = fopen("./saida/repos-final.csv","w");
    char buffer[MB] = "\0";
    fgets(buffer,MB,usersok); fputs(buffer,usersfinal);
    while (fgets(buffer,MB,usersok)){
        fputs(buffer,usersfinal);
    }
    commitsFinal(commitsok,commitsfinal,usersTree,reposTree,&reposWithCommits);
    reposFinal(reposok,reposfinal,usersTree,reposWithCommits);
    fclose(usersok);fclose(usersfinal);
    fclose(commitsok); fclose(commitsfinal);
    fclose(reposok); fclose(reposfinal);
    destroyBSTreeInt(usersTree); 
    destroyBSTreeInt(reposTree); 
    destroyBSTreeInt(reposWithCommits); 
}

// GUIAO 2
void querie1(int users, int orgs, int bots, int cmdIndex){
    char filePath[100];
    sprintf(filePath,"./saida/command%d_output.txt",cmdIndex);
    FILE *output = fopen(filePath,"w"); // o nome do ficheiro tem de ser diferente
    fprintf(output,"Bot: %d\nOrganization: %d\nUser: %d\n",bots,orgs,users);
    fclose(output);
} 

void querie2(int colabs, int repos, int cmdIndex){
    char filePath[100];
    sprintf(filePath,"./saida/command%d_output.txt",cmdIndex);
    FILE *output = fopen(filePath,"w");
    fprintf(output,"%.2lf\n",(float)colabs/repos);
    fclose(output);
} 

void querie3(int collabBots, int cmdIndex){
    char filePath[100];
    sprintf(filePath,"./saida/command%d_output.txt",cmdIndex);
    FILE *output = fopen(filePath,"w");
    fprintf(output,"%d",collabBots);
    fclose(output);
} 

void querie4(int commits, int users, int cmdIndex){
    char filePath[100];
    sprintf(filePath,"./saida/command%d_output.txt",cmdIndex);
    FILE *output = fopen(filePath,"w");
    fprintf(output,"%.2lf\n",(float)commits/users);
    fclose(output);
}

void querie5Aux(CatUsers cusers, UserList *usersBuffer, unsigned int *dateL, unsigned int *dateB){
    if(cusers){
        CatUsers left = leftBranch(cusers), right = rightBranch(cusers);
        querie5Aux(left,usersBuffer,dateL,dateB);
        User root = getRoot(cusers); CatCommits commits = getCommitsWithUser(root);
        int n_commits = howManyCommitsBetween(commits,dateL,dateB);
        insertUserList(usersBuffer,root,n_commits);
        querie5Aux(right,usersBuffer,dateL,dateB);
    }
}

void querie5(char *arguments,CatUsers cusers,int cmdIndex){
    char filePath[100];
    sprintf(filePath,"./saida/command%d_output.txt",cmdIndex);
    FILE *output = fopen(filePath,"w");
    int topN; unsigned int dateL[3], dateB[3];
    sscanf(arguments,"%d %u-%u-%u %u-%u-%u", &topN,&dateL[2],&dateL[1],&dateL[0],&dateB[2],&dateB[1],&dateB[0]);
    UserList usersBuffer = createUserList(topN);
    querie5Aux(cusers,&usersBuffer,dateL,dateB);
    querie5WriteFile(output,usersBuffer);
    deleteUserList(usersBuffer);
    fclose(output);
}

void querie6Aux(CatUsers cusers, CatRepos crepos, UserList *usersBuffer,char *language){
    if(cusers){
        CatUsers left = leftBranch(cusers), right = rightBranch(cusers);
        querie6Aux(left,crepos,usersBuffer,language);
        User root = getRoot(cusers);
        int n_commits = howManyCommitsInLang(language,getCommitsWithUser(root),crepos);
        insertUserList(usersBuffer,root,n_commits);
        querie6Aux(right,crepos,usersBuffer,language);
    }
}

void querie6(char *arguments, CatUsers cusers, CatRepos crepos, int cmdIndex){
    char filePath[100], language[50];
    sprintf(filePath,"./saida/command%d_output.txt",cmdIndex);
    FILE *output = fopen(filePath,"w");
    int topN = atoi(strsep(&arguments," \n\r\0"));
    strncpy(language,strsep(&arguments," \n\r\0"),49);
    UserList usersBuffer = createUserList(topN);
    querie6Aux(cusers,crepos,&usersBuffer,language);
    querie6WriteFile(output,usersBuffer);
    deleteUserList(usersBuffer);
    fclose(output);
}

void querie7Aux(CatUsers cusers, CatRepos crepos, RepoList *reposBuffer, unsigned int *date){
    if(cusers){
        Repo associatedRepo = NULL;
        CatCommits usrcommits = getCommitsWithUser(getRoot(cusers));
        unsigned int *commit_at = getBiggestDate(usrcommits,crepos,&associatedRepo);
        if(usrcommits && dateLower(commit_at[0],commit_at[1],commit_at[2],date[0],date[1],date[2])){
            insertRepoList(reposBuffer,associatedRepo,1);
        }else if(usrcommits){
            insertRepoList(reposBuffer,associatedRepo,0);
        }
        querie7Aux(leftBranch(cusers),crepos,reposBuffer,date);
        querie7Aux(rightBranch(cusers),crepos,reposBuffer,date);
    }
}

void querie7(char *arguments,CatUsers cusers, CatRepos crepos, int cmdIndex){
    char filePath[100];
    sprintf(filePath,"./saida/command%d_output.txt",cmdIndex);
    FILE *output = fopen(filePath,"w");
    unsigned int date[3];
    sscanf(arguments,"%u-%u-%u",&date[2],&date[1],&date[0]);
    RepoList reposBuffer = NULL;
    querie7Aux(cusers,crepos,&reposBuffer,date);
    querie7WriteFile(output,reposBuffer);
    deleteRepoList(reposBuffer);
    fclose(output);
}

void querie8Aux(CatRepos crepos, LangList *langBuffer, unsigned int *sinceDate){
    if(crepos){
        unsigned int *created_at = repoCreatedAtRoot(crepos);
        if(!dateLower(created_at[0],created_at[1],created_at[2],sinceDate[0],sinceDate[1],sinceDate[2])){
            insertLangList(langBuffer,getRepoLangRoot(crepos));
        }
        querie8Aux(leftBranchRepos(crepos),langBuffer,sinceDate);
        querie8Aux(rightBranchRepos(crepos),langBuffer,sinceDate);
    }
}

void querie8(char *arguments, CatRepos crepos, int cmdIndex){
    char filePath[100];
    sprintf(filePath,"./saida/command%d_output.txt",cmdIndex);
    FILE *output = fopen(filePath,"w");
    int topN; unsigned int date[3];
    sscanf(arguments,"%d %u-%u-%u",&topN,&date[2],&date[1],&date[0]);
    LangList langsBuffer = NULL;
    querie8Aux(crepos,&langsBuffer,date);
    querie8WriteFile(output,&langsBuffer,topN);
    deleteLangList(langsBuffer);
    fclose(output);
}

void querie9Aux(CatUsers cusers, CatRepos crepos, UserList *usersBuffer){
    if(cusers){
        CatUsers left = leftBranch(cusers), right = rightBranch(cusers);
        querie9Aux(left,crepos,usersBuffer);
        User root = getRoot(cusers);
        int n_commits = commitsInRepoFriend(getCommitsWithUser(root),crepos,getFollowersList(root),getNfollowers(root),getFollowingList(root),getNfollowing(root));
        insertUserList(usersBuffer,root,n_commits);
        querie9Aux(right,crepos,usersBuffer);
    }
}

void querie9(char *arguments, CatUsers cusers, CatRepos crepos, int cmdIndex){
    char filePath[100];
    sprintf(filePath,"./saida/command%d_output.txt",cmdIndex);
    FILE *output = fopen(filePath,"w");
    int topN = atoi(arguments);
    UserList usersBuffer = createUserList(topN);
    querie9Aux(cusers,crepos,&usersBuffer);
    querie9WriteFile(output,usersBuffer);
    deleteUserList(usersBuffer);
    fclose(output);
}

void querie10Aux(CatUsers cusers, UserList *usersBuffer){
    if(cusers != NULL){
        CatUsers left = leftBranch(cusers), right = rightBranch(cusers);
        querie10Aux(right,usersBuffer);
        int msgSize = getMsgSizeRoot(cusers);
        if(msgSize > 0){
            insertUserList(usersBuffer,getRoot(cusers),msgSize);
        }
        querie10Aux(left,usersBuffer);
    }
}

void querie10(char *arguments, CatUsers cusers, int cmdIndex){
    int topN = atoi(arguments);
    char filePath[100];
    sprintf(filePath,"./saida/command%d_output.txt",cmdIndex);
    FILE *output = fopen(filePath,"w");
    UserList usersBuffer = createUserList(topN);
    querie10Aux(cusers,&usersBuffer);
    querie10WriteFile(output,usersBuffer);
    deleteUserList(usersBuffer);
    fclose(output);
}

// FUNÇÃO DE EXECUÇÃO DO GUIÃO 3 
void g2(char *filename){
    char commandsFPath[100];
    sprintf(commandsFPath,"./entrada/%s",filename);
    FILE *commands = fopen(commandsFPath,"r"),
         *usersF = fopen("./entrada/users-g2.csv","r"),
         *commitsF = fopen("./entrada/commits-g2.csv","r"),
         *reposF = fopen("./entrada/repos-g2.csv","r");
    CatUsers usersCatalog = NULL;
    CatRepos reposCatalog = NULL;
    int users = 0, bots = 0, orgs = 0, repos = 0, colabs = 0, commits = 0, collabBots = 0,cmdIndex = 1, querieID;
    char buffer[MB] = "\0";

    //Criacao dos catalogos
    fseek(usersF,100,SEEK_SET); 
    while(fgets(buffer,MB,usersF)){ 
        readUserLine(buffer,&usersCatalog,&users,&orgs,&bots);
    }
    fseek(commitsF, 49,SEEK_SET);
    while(fgets(buffer,MB,commitsF)){
        readCommitLine(buffer,&usersCatalog,&colabs,&commits,&collabBots);
    }
    fseek(reposF,143,SEEK_SET);
    while(fgets(buffer,MB,reposF)){
        readRepoLine(buffer,&reposCatalog,&repos);
    }
    // fim da criacao dos catalogos
    
    //leitura dos comandos
    while(fgets(buffer,MB,commands)){
        querieID = atoi(buffer);
        switch(querieID){
            case 1: querie1(users,orgs,bots,cmdIndex);break;
            case 2: querie2(colabs,repos,cmdIndex); break;
            case 3: querie3(collabBots,cmdIndex); break;
            case 4: querie4(commits,users+bots+orgs,cmdIndex); break;
            case 5: querie5(buffer+2,usersCatalog,cmdIndex); break;
            case 6: querie6(buffer+2,usersCatalog,reposCatalog,cmdIndex); break;
            case 7: querie7(buffer+2,usersCatalog,reposCatalog,cmdIndex); break;
            case 8: querie8(buffer+2,reposCatalog,cmdIndex); break;
            case 9: querie9(buffer+2,usersCatalog,reposCatalog,cmdIndex); break;
            case 10: querie10(buffer+3,usersCatalog,cmdIndex); break;
            default: break;
        }
        cmdIndex++;
    }
    // fim da leitura dos comandos

    //libertacao da memoria
    deleteCatUsers(usersCatalog);
    deleteCatRepos(reposCatalog);
    fclose(usersF); fclose(commitsF); fclose(reposF); fclose(commands);
}

// FUNÇÃO DE EXECUÇÃO DO GUIÃO 3
void g3(){
    // code here ...
}

int main(int argc, char *argv[]){
    if(argc <= 1) printf("No arguments!\n");
    else if(argc == 2) g2(argv[1]);
    return 0;
} 