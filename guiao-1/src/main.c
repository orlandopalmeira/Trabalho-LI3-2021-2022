#include <stdio.h>
#include <string.h>
#include "readWrite.h"

#define MB 1048576 // 1 MB para os arrays de caracteres

/* Função para executar o exercício 1 do guião 1.*/
void g1_ex1(){
    FILE *users = fopen("./entrada/users.csv","r"),
         *usersok = fopen("./saida/users-ok.csv","w"),
         *commits = fopen("./entrada/commits.csv","r"),
         *commitsok = fopen("./saida/commits-ok.csv","w"),
         *repos   = fopen("./entrada/repos.csv","r"), 
         *reposok = fopen("./saida/repos-ok.csv","w");
    int x = usersOK(users,usersok); // funciona
    int y = commitsOK(commits,commitsok); // funciona
    int z = reposOK(repos,reposok); // funciona
    fclose(users); fclose(usersok);
    fclose(commits); fclose(commitsok);
    fclose(repos); fclose(reposok);
    //printf("Users: %d lines deleted\nCommits: %d lines deleted\nRepos: %d lines deleted\n", x,y,z);
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
    // cópia do users-ok para o users-final
    char buffer[MB] = "\0";
    fgets(buffer,MB,usersok); fputs(buffer,usersfinal);
    while (fgets(buffer,MB,usersok)){
        fputs(buffer,usersfinal);
    }
    //printf("Number of users: %d\nNumber of repos: %d\n", treeSize(usersTree), treeSize(reposTree));
    // fim da cópia do users-ok para o users-final
    int x = commitsFinal(commitsok,commitsfinal,usersTree,reposTree,&reposWithCommits);
    //printf("Number of reposWithCommits: %d\n",treeSize(reposWithCommits));
    int y = reposFinal(reposok,reposfinal,usersTree,reposWithCommits);
    fclose(usersok);fclose(usersfinal);
    fclose(commitsok); fclose(commitsfinal);
    fclose(reposok); fclose(reposfinal);
    destroyBSTreeInt(usersTree); // memória libertada
    destroyBSTreeInt(reposTree); // memória libertada
    destroyBSTreeInt(reposWithCommits); // memória libertada
    //printf("UsersOK: 0 lines deleted\nCommitsOK: %d lines deleted\nReposOK: %d lines deleted\n",x,y); 
}

int main(int argc, char *argv[]){
    if(!strcmp(argv[1],"exercicio-1")){
        g1_ex1();
    }else if(!strcmp(argv[1],"exercicio-2")){
        g1_ex2();
    }else{     
        g1_ex1();
        g1_ex2();
    } 
    return 0;
}