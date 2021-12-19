#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fileUtils.h"
#include "structs.h"

#define MB 1048576

/* Verifica se a primeira parte da string é um número inteiro até ao 
   caracter "endchar" e, caso isso aconteça, 
   devolve o nº de algarismos que esse numero tem. Se não acontecer, retorna 0*/
int positiveIntStr(char *str, char endchar,int *number){ // funciona
    if(*str && *str != endchar){
        int num;
        char *oldstr = str;
        num = strtol(str,&str,10);
        if(number) *number = num;
        return (*str == endchar || *str == '\n') && num >= 0 ? str-oldstr : 0;
    }
    return 0;
}

// Diz se a string contém uma data válida.
int validDateStr(char *str, char endchar){ // funciona
    if(*str == '2' && str[19] == endchar){
        int year, month, day, hours, minutes,seconds, args = 0;
        args = sscanf(str,"%d-%d-%d %d:%d:%d",&year,&month,&day,&hours,&minutes,&seconds);
        if(args != 6) return 0; // o sscanf só pode ler 6 argumentos 
        if(!dateLower(day,month,year,7,4,2005) && todayOrLess(day,month,year) && hours < 24 && minutes < 60 && seconds < 60){
            if(month>=1 && month<=12){
                if((day>=1 && day<=31) && (month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12))
                    return 1;
                else if((day>=1 && day<=30) && (month==4 || month==6 || month==9 || month==11))
                    return 1;
                else if((day>=1 && day<=28) && month==2)
                    return 1;
                else if(day==29 && month==2 && (year%400==0 ||(year%4==0 && year%100!=0))) // leap year
                    return 1;
            }
        }
    }
    return 0;
}

//Indica se o array presente na string é válido e coloca o seu tamanho na variável length se for solicitado
int validArrayStr(char *str, char endchar,int *length){ // Em princípio, funciona
    if(*str == '['){
        if(str[1] == ']'){
            if(length) *length = 0; return 2;
        }else{
            int count = 0; char *aux = str;
            *str = ' ';
            while(1){
                if(*str != ' ') return 0;
                strtol(str,&str,10);
                if(*str == ']') break;
                if(*str != ',') return 0;
                str++; count++;
            }
            *aux = '[';
            if(str[1] != endchar) return 0; 
            if(length) *length = ++count;
            return str-aux+1;
        }
    }
    return 0;
}

int validTypeStr(char *str, char endchar){
    int r = 0;
    switch (*str){
        case 'U': if(!strcmpV2(str,"User;",';')) r = 4; break;
        case 'O': if(!strcmpV2(str,"Organization;",';')) r = 12; break;
        case 'B': if(!strcmpV2(str,"Bot;",';')) r = 3; break;
        default: break;
    }
    return str[r] == endchar ? r : 0;
}

int validBoolStr(char *str, char endchar){
    int r = 0;
    switch(*str){
        case 'T': if(!strcmpV2(str,"True;",';')) r = 4; break;
        case 'F': if(!strcmpV2(str,"False;",';')) r = 5; break;
        default: break;
    }
    return str[r] == endchar ? r : 0;
}

/*esta pode ser a função que causa erros nos repos pois nesse ficheiro há repos
cuja descrição tem outras linhas csv entre aspas */
int validString(char *str, short flag, char endchar, int *length){
    if(flag && *str == endchar) {// Se a flag for diferente de 0, a função considera a string vazia válida.
        if(length) *length = 0;
        return 1; 
    }
    else if(*str && *str != endchar){
        char *old_str = str;
        while (*(str++) != endchar);
        if(length) *length = str-old_str-1;
        return str-old_str-1;
    }
    return 0;
}

int validLine_Users(char *str){
    // aux armazena valores de verdade, o value armazena números que podem ser o número de seguidores e o length armazena o tamanho dos arrays.
    int aux = positiveIntStr(str,';',NULL), value = 0, length = 0; 
    if(!aux) return 0; str+= aux+1; // id
    aux = validString(str,0,';',NULL);
    if(!aux) return 0; str += aux+1; // login
    aux = validTypeStr(str,';');
    if(!aux) return 0; str += aux+1; // type
    aux = validDateStr(str,';');
    if(!aux) return 0; str += 20; // created_at
    aux = positiveIntStr(str,';',&value);
    if(!aux) return 0; str += aux+1; // followers;
    aux = validArrayStr(str,';',&length);
    if(!aux || length != value) return 0; str += aux+1; // follower_list;
    aux = positiveIntStr(str,';',&value);
    if(!aux) return 0; str += aux+1; // following;
    aux = validArrayStr(str,';',&length);
    if(!aux || length != value) return 0; str += aux+1; // following_list;
    aux = positiveIntStr(str,';',NULL);
    if(!aux) return 0; str += aux +1; // public_gists;
    aux = positiveIntStr(str,'\r',NULL);
    if(!aux) return 0; // public_repos
    return 1;
}

int validLine_Commits(char *str){
    int aux = positiveIntStr(str,';',NULL); // repo_id;
    if(!aux) return 0; str += aux+1;
    aux = positiveIntStr(str,';',NULL); // author_id;
    if(!aux) return 0; str += aux+1;
    aux = positiveIntStr(str,';',NULL); // committer_id;
    if(!aux) return 0; str += aux+1;
    aux = validDateStr(str,';'); // commit_at;
    if(!aux) return 0; str += 20;
    aux = validString(str,1,'\r',NULL); // message
    if(!aux) return 0;
    return 1;
}

int validLine_Repos(char *str){
    int aux = positiveIntStr(str,';',NULL), length = 0; // id
    if(!aux) return 0; str += aux+1;
    aux = positiveIntStr(str,';',NULL); // owner_id
    if(!aux) return 0; str += aux+1;
    aux = validString(str,0,';',NULL); // full_name
    if(!aux) return 0; str += aux+1;
    aux = validString(str,0,';',NULL); // license
    if(!aux) return 0; str += aux+1;
    aux = validBoolStr(str,';'); //has_wiki
    if(!aux) return 0; str += aux +1;
    aux = validString(str,1,';',&length); // description
    if(!aux) return 0; str += length+1;
    aux = validString(str,0,';',NULL); // language
    if(!aux) return 0; str += aux+1;
    aux = validString(str,0,';',NULL); // default_branch
    if(!aux) return 0; str += aux+1;
    aux = validDateStr(str,';'); // created_at
    if(!aux) return 0; str += 20;
    aux = validDateStr(str,';'); // updated_at
    if(!aux) return 0; str += 20;
    aux = positiveIntStr(str,';',NULL); // forks_count
    if(!aux) return 0; str += aux+1;
    aux = positiveIntStr(str,';',NULL); // open_issues
    if(!aux) return 0; str += aux+1;
    aux = positiveIntStr(str,';',NULL); // stargazers_count
    if(!aux) return 0; str += aux+1;
    aux = positiveIntStr(str,'\r',NULL); // size
    if(!aux) return 0; 
    return 1;
}

// Esta funcção foi substituída pela sscanf, mas vou mantê-la caso seja necessário
void getIDsFromCommit(char *str, int *repoID, int *authorID, int *committerID){
    int rID, cID, aID; // repo_id, committer_id e repo_id, respetivamente
    rID = strtol(str,&str,10); str++;
    aID = strtol(str,&str,10); str++;
    cID = strtol(str,&str,10);
    if(repoID) *repoID = rID;
    if(committerID) *committerID = cID;
    if(authorID) *authorID = aID;
}

int validLineCommits_Final(char *str, BSTreeINT usersTree, BSTreeINT reposTree, BSTreeINT *reposWithCommits){
    int rID, cID, aID; //repo_id ,commiter_id e author_id, respetivamente
    sscanf(str,"%d;%d;%d;",&rID,&cID,&aID);
    if(search(usersTree,cID) && search(usersTree,aID) && search(reposTree,rID)){
        insert(reposWithCommits,rID);
        return 1;
    }else return 0;
}

int validLineRepos_Final(char *str, BSTreeINT usersTree, BSTreeINT reposWithCommits){
    int rID,oID;
    sscanf(str,"%d;%d;",&rID,&oID);
    return search(usersTree, oID) && search(reposWithCommits,rID);
}