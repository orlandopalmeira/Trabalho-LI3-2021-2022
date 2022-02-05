#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readWrite.h"
#include "parsing.h"
#include "linkedLists.h"
#include "fileUtils.h"
#include "tests.h"

#define MB 1048576 // 1 MB para os arrays de caracteres

/* Função para executar o exercício 1 do guião 1.*/
void g1_ex1(){
    FILE *users = fopen("./entrada/users-g3.csv","r"),
         *usersok = fopen("./entrada/users-ok.csv","w"),
         *commits = fopen("./entrada/commits-g3.csv","r"),
         *commitsok = fopen("./entrada/commits-ok.csv","w"),
         *repos   = fopen("./entrada/repos-g3.csv","r"), 
         *reposok = fopen("./entrada/repos-ok.csv","w");
    usersOK(users,usersok); //printf("Users ok done!!!\n");
    commitsOK(commits,commitsok); //printf("Commits ok done!!!\n");
    reposOK(repos,reposok); //printf("Repos ok done!!!\n");
    fclose(users); fclose(usersok);
    fclose(commits); fclose(commitsok);
    fclose(repos); fclose(reposok);
}

/* Função para executar o exercício 2 do guião 1.*/
void g1_ex2(){
    BSTreeINT usersTree = usersIDsTree(), reposTree = reposIDsTree(), reposWithCommits = NULL; // libertar a memória no fim da função
    FILE *commitsok = fopen("./entrada/commits-ok.csv","r"), 
         *commitsfinal = fopen("./entrada/commits-final.csv","w"),
         *reposok = fopen("./entrada/repos-ok.csv","r"),
         *reposfinal = fopen("./entrada/repos-final.csv","w");
    //Exercicios guiao 2
    commitsFinal(commitsok,commitsfinal,usersTree,reposTree,&reposWithCommits);
    //printf("Commits final done!!!\n");
    reposFinal(reposok,reposfinal,usersTree,reposWithCommits);
    //printf("Repos final done!!!\n");
    // fim dos exercicios do guiao 2

    remove("./entrada/repos-ok.csv"); remove("./entrada/commits-ok.csv"); // remoção de ficheiros desnecessários
    rename("./entrada/users-ok.csv","./entrada/users-final.csv"); // como os users nao sao alterados no exercicio 2, so precisamos de alterar o nome do ficheiro
    //printf("Users final done!!!\n");
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
    fprintf(output,"%d\n",collabBots);
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

void querie7Aux(CatUsers cusers, CatRepos crepos, RepoTree *reposBuffer, unsigned int *date){
    if(cusers){
        Repo associatedRepo = NULL;
        CatCommits usrcommits = getCommitsWithUser(getRoot(cusers));
        unsigned int *commit_at = getBiggestDate(usrcommits,crepos,&associatedRepo);
        if(usrcommits && dateLower(commit_at[0],commit_at[1],commit_at[2],date[0],date[1],date[2])){ 
            insertRepoTree(reposBuffer,associatedRepo,1);
        }else if(usrcommits){
            insertRepoTree(reposBuffer,associatedRepo,0);
        }
        querie7Aux(leftBranch(cusers),crepos,reposBuffer,date);
        querie7Aux(rightBranch(cusers),crepos,reposBuffer,date);
    }
}

unsigned int querie7(char *arguments,CatUsers cusers, CatRepos crepos, int cmdIndex){
    char filePath[100];
    sprintf(filePath,"./saida/command%d_output.txt",cmdIndex);
    FILE *output = fopen(filePath,"w");
    unsigned int date[3], length = 0;
    sscanf(arguments,"%u-%u-%u",&date[2],&date[1],&date[0]);
    RepoTree reposBuffer = NULL;
    querie7Aux(cusers,crepos,&reposBuffer,date);
    querie7WriteFile(output,reposBuffer);
    length = lenRepoTree(reposBuffer);
    deleteRepoTree(reposBuffer);
    fclose(output);
    return length;
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

// FUNÇÃO DE EXECUÇÃO DO GUIÃO 2
void g2(char *filename){
    char commandsFPath[100];
    sprintf(commandsFPath,"./entrada/%s",filename);
    FILE *commands = fopen(commandsFPath,"r"),
         *usersF = fopen("./entrada/users-final.csv","r"),
         *commitsF = fopen("./entrada/commits-final.csv","r"),
         *reposF = fopen("./entrada/repos-final.csv","r");
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

// GUIAO 3

int get_option(){
    char opt_s[32] = "\0";
    printf("Insira opção:\n");
    fgets(opt_s,31,stdin);
    return atoi(opt_s);
}

int menu(){
    printf("------------------------------------------------------------------------------\n");
    printf("| 1 | Quantidade de bots, organizações e utilizadores                        |\n");
    printf("------------------------------------------------------------------------------\n");
    printf("| 2 | Número médio de colaboradores por repositório                          |\n");
    printf("------------------------------------------------------------------------------\n");
    printf("| 3 | Número de repositórios contendo bots como colaboradores                |\n");
    printf("------------------------------------------------------------------------------\n");
    printf("| 4 | Quantidade média de commits por utilizador                             |\n");
    printf("------------------------------------------------------------------------------\n");
    printf("| 5 | Top N users mais ativos num determinado intervalo de datas             |\n");
    printf("------------------------------------------------------------------------------\n");
    printf("| 6 | Top N users com mais commits em repositórios de uma certa linguagem    |\n");
    printf("------------------------------------------------------------------------------\n");
    printf("| 7 | Repositórios inativos a partir de uma determinada data                 |\n");
    printf("------------------------------------------------------------------------------\n");
    printf("| 8 | Top N linguagens mais utilizadas a partir de uma determinada data      |\n");
    printf("------------------------------------------------------------------------------\n");
    printf("| 9 | Top N users com mais commits em repositórios cujo owner é um amigo seu |\n");
    printf("------------------------------------------------------------------------------\n");
    printf("| 10 | Top N users com as maiores mensagens de commit por repositório        |\n");
    printf("------------------------------------------------------------------------------\n");
    printf("| 11 | Sair                                                                  |\n");
    printf("------------------------------------------------------------------------------\n");
    return get_option();
}

void querie1_g3(int users, int orgs, int bots){
    printf("Bot: %d\nOrganization: %d\nUser: %d\n",bots,orgs,users);
}

void querie2_g3(int colabs, int repos){
    printf("%.2lf\n",(float)colabs/repos);
}

void querie3_g3(int collabBots){
    printf("%d\n",collabBots);
}

void querie4_g3(int commits, int users){
    printf("%.2lf\n",(float)commits/users);
}

// FUNÇÃO DE EXECUÇÃO DO GUIÃO 3
void g3(int flag_tests){
    // Variables and catalogs
    int opt = 0, validation = 0, build_catalogs = 0;
    int users = 0, bots = 0, orgs = 0, repos = 0, colabs = 0, commits = 0, collabBots = 0;
    char buffer[MB] = "\0";
    CatUsers usersCatalog = NULL;
    CatRepos reposCatalog = NULL;
    FILE *usersF = fopen("./entrada/users-final.csv","r"),
         *commitsF = fopen("./entrada/commits-final.csv","r"),
         *reposF = fopen("./entrada/repos-final.csv","r");
    // end of variables and catalogs
    // interaction with user
    ins_option:{
        if(!flag_tests){
            opt = menu();
            if(opt == 11) goto out; // sair do programa
            goto continue_;
        }
    }
    temp:{
        if(!flag_tests){
            opt = get_option();
        }
    }
    continue_:
    if(!validation){ // para evitar repetições de validação de dados
        g1_ex1(); 
        g1_ex2();
        validation = 1;
    }
    if(!build_catalogs){ // para nao reconstruir os catálogos
        jumpLine(usersF); 
        while(fgets(buffer,MB,usersF)){ 
            readUserLine(buffer,&usersCatalog,&users,&orgs,&bots);
        }
        jumpLine(commitsF);
        while(fgets(buffer,MB,commitsF)){
            readCommitLine(buffer,&usersCatalog,&colabs,&commits,&collabBots);
        }
        jumpLine(reposF);
        while(fgets(buffer,MB,reposF)){
            readRepoLine(buffer,&reposCatalog,&repos);
        }
        build_catalogs = 1;
    }
    if(flag_tests){ // execução dos testes
        test_querie_1(users,orgs,bots,querie1);
        test_querie_2(colabs,repos,querie2);
        test_querie_3(collabBots,querie3);
        test_querie_4(commits,users,querie4);
        test_querie_5(usersCatalog,querie5);
        test_querie_6(usersCatalog,reposCatalog,querie6);
        test_querie_7(usersCatalog,reposCatalog,querie7);
        test_querie_8(reposCatalog,querie8);
        test_querie_9(usersCatalog,reposCatalog,querie9);
        test_querie_10(usersCatalog,querie10);
        goto out;
    }
    switch (opt){
        case 1:{
            querie1_g3(users,orgs,bots);
            printf("Pressione ENTER para voltar ao menu\n");
            getchar();
            goto ins_option;
            break;
        }
        case 2:{
            querie2_g3(colabs,repos);
            printf("Pressione ENTER para voltar ao menu\n");
            getchar();
            goto ins_option;
            break;
        }
        case 3:{
            querie3_g3(collabBots);
            printf("Pressione ENTER para voltar ao menu\n");
            getchar();
            goto ins_option;
            break;
        }
        case 4:{
            querie4_g3(commits,users+orgs+bots);
            printf("Pressione ENTER para voltar ao menu\n");
            getchar();
            goto ins_option;
            break;
        }
        case 5:{
            char buffer[32] = "\0"; // guarda os argumentos da querie 5
            unsigned int top_n, date_i[3], date_f[3];
            int page_index = 1; 
            get_topN_q5:{
                printf("Insira o número de utilizadores: ");
                if(scanf("%u",&top_n) != 1){
                    printf("Valor inválido, tente de novo!\n"); goto get_topN_q5;
                }
            }
            get_date_i_q5:{
                printf("Data de início (formato AAAA-MM-DD): ");
                if(scanf("%u-%u-%u", &date_i[2],&date_i[1],&date_i[0]) != 3){
                    printf("Valor inválido, tente de novo!\n"); goto get_date_i_q5;
                }
            }
            get_date_f_q5:{
                printf("Data de fim (formato AAAA-MM-DD): ");
                if(scanf("%u-%u-%u", &date_f[2],&date_f[1],&date_f[0]) != 3){
                    printf("Valor inválido, tente de novo!\n"); goto get_date_f_q5;
                }
            }
            sprintf(buffer,"%u %u-%u-%u %u-%u-%u",top_n,date_i[2],date_i[1],date_i[0],date_f[2],date_f[1],date_f[0]);
            querie5(buffer,usersCatalog,5); // execucao querie 5
            FILE *result = fopen("./saida/command5_output.txt","r"); // output querie5
            while(1){ // o ciclo é terminado conforme as opções do utilizador, não existe uma condição que o faça terminar
                if(page_index <= 0) page_index = num_of_pages(top_n);
                else if(page_index > num_of_pages(top_n)) page_index = 1;
                if(page_index > 0 && page_index <= num_of_pages(top_n)){
                    printf("-----------------------------------------------------------------------\n");
                    printf("ID | LOGIN | Quantidade de commits\n");
                    print_page(result,page_index);
                    printf("--------------------------- Página %d de %d ---------------------------\n",page_index,num_of_pages(top_n));
                }
                printf("P        -> Próxima página\nA        -> Página anterior\nS <n>    -> Saltar para a página n\nM        -> Voltar ao menu principal\n");
                printf("Insira opção:\n");
                fgets(buffer,31,stdin);
                switch(buffer[0]){
                    case 'P': page_index++; break;
                    case 'A': page_index--; break;
                    case 'M': goto out_q5;
                    case 'S': page_index = atoi(buffer+1); break;
                    default: continue; break;
                }
            }
            out_q5:{
                fclose(result);
                goto ins_option;
            }
            break;
        }
        case 6:{
            char buffer[128], language[32];
            unsigned int top_n;int page_index = 1;
            get_topN_q6:{
                printf("Insira o número de utilizadores: ");
                fgets(buffer,127,stdin); top_n = atoi(buffer);
                if(top_n == 0){
                    printf("Valor inválido, tente de novo!\n"); goto get_topN_q6;
                }
            }
            printf("Insira a linguagem: ");
            fgets(language,31,stdin);
            sprintf(buffer,"%d %s",top_n,language);
            querie6(buffer,usersCatalog,reposCatalog,6);
            FILE *result = fopen("./saida/command6_output.txt","r");
            while (1){
                if(page_index <= 0) page_index = num_of_pages(top_n);
                else if(page_index > num_of_pages(top_n)) page_index = 1;
                if(page_index > 0 && page_index <= num_of_pages(top_n)){
                    printf("-----------------------------------------------------------------------\n");
                    printf("ID | LOGIN | Quantidade de commits\n");
                    print_page(result,page_index);
                    printf("--------------------------- Página %d de %d ---------------------------\n",page_index,num_of_pages(top_n));
                }
                printf("P        -> Próxima página\nA        -> Página anterior\nS <n>    -> Saltar para a página n\nM        -> Voltar ao menu principal\n");
                printf("Insira opção:\n");
                fgets(buffer,127,stdin);
                switch(buffer[0]){
                    case 'P': page_index++; break;
                    case 'A': page_index--; break;
                    case 'M': goto out_q6;
                    case 'S': page_index = atoi(buffer+1); break;
                    default: continue; break;
                }
            }
            out_q6:{
                fclose(result);
                goto ins_option;
            }
            break;
        }
        case 7:{
            char buffer[128], *buff_addr = buffer;
            unsigned int len = 0;int page_index = 1;
            get_date_q7:{
                printf("Data (formato AAAA-MM-DD): ");
                fgets(buffer,127,stdin);
                if(atoi(strsep(&buff_addr,"-")) <= 0 || atoi(strsep(&buff_addr,"-")) <= 0 || atoi(strsep(&buff_addr,"-")) <= 0){
                    printf("Data inválida, tente de novo\n"); goto get_date_q7;
                }else{
                    buffer[4] = buffer[7] = '-';
                }
            }
            len = querie7(buffer,usersCatalog,reposCatalog,7);
            FILE *result = fopen("./saida/command7_output.txt","r");
            while(1){
                if(page_index <= 0) page_index = num_of_pages(len);
                else if(page_index > num_of_pages(len)) page_index = 1;
                if(page_index > 0 && page_index <= num_of_pages(len)){
                    printf("-----------------------------------------------------------------------\n");
                    printf("REPO ID | DESCRIÇÃO\n");
                    print_page(result,page_index);
                    printf("--------------------------- Página %d de %d ---------------------------\n",page_index,num_of_pages(len));
                }
                printf("P        -> Próxima página\nA        -> Página anterior\nS <n>    -> Saltar para a página n\nM        -> Voltar ao menu principal\n");
                printf("Insira opção:\n");
                fgets(buffer,127,stdin);
                switch(buffer[0]){
                    case 'P': page_index++; break;
                    case 'A': page_index--; break;
                    case 'M': goto out_q7;
                    case 'S': page_index = atoi(buffer+1); break;
                    default: continue; break;
                }
            }
            out_q7:{
                fclose(result);
                goto ins_option;
            }
            goto temp;
            break;
        }
        case 8:{
            unsigned int top_n = 0, date[3]; int page_index = 1;
            char buffer[128], *buff_addr = buffer;
            get_topN_q8:{
                printf("Insira o número de linguagens: ");
                fgets(buffer, 127, stdin);
                top_n = atoi(buffer);
                if(top_n <= 0){
                    printf("Valor inválido, tente de novo!\n"); goto get_topN_q8;
                }
            }
            get_date_q8:{
                printf("Data (formato AAAA-MM-DD): ");
                fgets(buffer,127,stdin);
                date[2] = atoi(strsep(&buff_addr,"-"));
                date[1] = atoi(strsep(&buff_addr,"-"));
                date[0] = atoi(strsep(&buff_addr,"-"));
                if (date[0] <= 0 || date[1] <= 0 || date[2] <= 0){
                    printf("Data com formato inválido, tente de novo\n"); goto get_date_q8;
                }
            }
            sprintf(buffer,"%u %u-%u-%u",top_n,date[2],date[1],date[0]);
            querie8(buffer,reposCatalog,8);
            FILE *result = fopen("./saida/command8_output.txt","r");
            while (1){
                if(page_index <= 0) page_index = num_of_pages(top_n);
                else if(page_index > num_of_pages(top_n)) page_index = 1;
                if(page_index > 0 && page_index <= num_of_pages(top_n)){
                    printf("-----------------------------------------------------------------------\n");
                    printf("LINGUAGEM\n");
                    print_page(result,page_index);
                    printf("--------------------------- Página %d de %d ---------------------------\n",page_index,num_of_pages(top_n));
                }
                printf("P        -> Próxima página\nA        -> Página anterior\nS <n>    -> Saltar para a página n\nM        -> Voltar ao menu principal\n");
                printf("Insira opção:\n");
                fgets(buffer,127,stdin);
                switch(buffer[0]){
                    case 'P': page_index++; break;
                    case 'A': page_index--; break;
                    case 'M': goto out_q8;
                    case 'S': page_index = atoi(buffer+1); break;
                    default: continue; break;
                }
            }
            out_q8:{
                fclose(result);
                goto ins_option;
            }
            break;
        }
        case 9:{
            char buffer[128];
            unsigned int top_n = 0;int page_index = 1;
            get_topN_q9:{
                printf("Insira o número de utilizadores: ");
                fgets(buffer,127, stdin);
                top_n = atoi(buffer);
                if(top_n <= 0){
                    printf("Valor inválido, tente de novo!\n"); goto get_topN_q9;
                }
            }
            querie9(buffer,usersCatalog,reposCatalog,9);
            FILE *result = fopen("./saida/command9_output.txt","r");
            while(1){
                if(page_index <= 0) page_index = num_of_pages(top_n);
                else if(page_index > num_of_pages(top_n)) page_index = 1;
                if(page_index > 0 && page_index <= num_of_pages(top_n)){
                    printf("-----------------------------------------------------------------------\n");
                    printf("ID | LOGIN\n");
                    print_page(result,page_index);
                    printf("--------------------------- Página %d de %d ---------------------------\n",page_index,num_of_pages(top_n));
                }
                printf("P        -> Próxima página\nA        -> Página anterior\nS <n>    -> Saltar para a página n\nM        -> Voltar ao menu principal\n");
                printf("Insira opção:\n");
                fgets(buffer,127,stdin);
                switch(buffer[0]){
                    case 'P': page_index++; break;
                    case 'A': page_index--; break;
                    case 'M': goto out_q9;
                    case 'S': page_index = atoi(buffer+1); break;
                    default: continue; break;
                }
            }
            out_q9:{
                fclose(result);
                goto ins_option;
            }
            break;
        }
        case 10:{
            char buffer[128];
            unsigned int top_n = 0; int page_index = 1;
            get_topN_q10:{
                printf("Insira o número de utilizadores: ");
                fgets(buffer,127, stdin);
                top_n = atoi(buffer);
                if(top_n <= 0){
                    printf("Valor inválido, tente de novo!\n"); goto get_topN_q10;
                }
            }
            querie10(buffer,usersCatalog,10);
            FILE *result = fopen("./saida/command10_output.txt","r");
            while(1){
                if(page_index <= 0) page_index = num_of_pages(top_n);
                else if(page_index > num_of_pages(top_n)) page_index = 1;
                if(page_index > 0 && page_index <= num_of_pages(top_n)){
                    printf("-----------------------------------------------------------------------\n");
                    printf("ID | LOGIN | TAMANHO DA MENSAGEM\n");
                    print_page(result,page_index);
                    printf("--------------------------- Página %d de %d ---------------------------\n",page_index,num_of_pages(top_n));
                }
                printf("P        -> Próxima página\nA        -> Página anterior\nS <n>    -> Saltar para a página n\nM        -> Voltar ao menu principal\n");
                printf("Insira opção:\n");
                fgets(buffer,127,stdin);
                switch(buffer[0]){
                    case 'P': page_index++; break;
                    case 'A': page_index--; break;
                    case 'M': goto out_q10;
                    case 'S': page_index = atoi(buffer+1); break;
                    default: continue; break;
                }
            }
            out_q10:{
                fclose(result);
                goto ins_option;
            }
            break;
        }
        case 11: break;
        default:{
            printf("Opção inválida!\nTente novamente\n");
            goto temp;
            break;
        }
    }
    out:{
        printf("A sair...\n");
        fclose(usersF); fclose(commitsF); fclose(reposF);
        if(build_catalogs){
            deleteCatUsers(usersCatalog); deleteCatRepos(reposCatalog);
        }
    }
}

int main(int argc, char *argv[]){
    if(argc <= 1){
        //g3(1);
        if(strstr(argv[0],"tests")){
            printf("Running tests...\n");
            g3(1);
            printf("Done!\n");
        }
        else g3(0);
    }
    else if(argc == 2) g2(argv[1]);
    return 0;
} 