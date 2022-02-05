#include <stdio.h>
#include <time.h>
#include <string.h>

#define LINES_PER_PAGE 15
#define MB 1048576

short dateLower(unsigned int d1, unsigned int m1, unsigned int y1, unsigned int d2, unsigned int m2, unsigned int y2){
    // verifica d1/m1/y1 < d2/m2/l2
    if(y1 < y2) return 1;
    else if(y1 == y2 && m1 < m2) return 1;
    else if(y1 == y2 && m1 == m2 && d1 < d2) return 1;
    return 0;
} //

short dateHigher(unsigned int d1, unsigned int m1, unsigned int y1, unsigned int d2, unsigned int m2, unsigned int y2){
    // verifica d1/m1/y1 > d2/m2/l2
    if(y1 > y2) return 1;
    else if(y1 == y2 && m1 > m2) return 1;
    else if(y1 == y2 && m1 == m2 && d1 > d2) return 1;
    return 0;
}

short dateEqual(unsigned int d1,unsigned int m1, unsigned int y1,unsigned int d2,unsigned int m2, unsigned int y2){
    // verifica d1/m1/y1 == d2/m2/y2
    return d1 == d2 && m1 == m2 && y1 == y2;
} 

short dateBetween(unsigned int d1, unsigned int m1, unsigned int y1, unsigned int db, unsigned int mb, unsigned int yb, unsigned int d2, unsigned int m2, unsigned int y2){
    // verifica d1/m1/y1 <= db/mb/yb <= d2/m2/y2
    return (dateLower(d1,m1,y1,db,mb,yb) || dateEqual(d1,m1,y1,db,mb,yb)) && 
           (dateLower(db,mb,yb,d2,m2,y2) || dateEqual(db,mb,yb,d2,m2,y2));
}


short todayOrLess(unsigned int d, unsigned int m, unsigned int y){
    time_t seconds = time(NULL);
    struct tm* current_time = localtime(&seconds);
    unsigned int year = current_time->tm_year + 1900, month = current_time->tm_mon +1, day = current_time->tm_mday;
    if(y > year) return 0;
    else if(y == year && m > month) return 0;
    else if(y == year && m == month && d > day) return 0;
    else return 1;
}

int strcmpV2(char s1[], char s2[], char endchar){
    if(*s1 == endchar || *s2 == endchar) return (*s1)-(*s2);
    else if(*s1 == *s2) return strcmpV2(s1+1,s2+1,endchar);
    else return (*s1) - (*s2);
}

int strlenV2(char str[], char endchar){
    return *str == endchar || !(*str) ? 0 : 1 + strlenV2(str+1,endchar);
}

short belongsToArrInt(unsigned int val, unsigned int *arr, int N){
    int i;
    for(i = 0; i < N && arr[i] != val; i++) i = i;
    return i < N;
}

void replace(char *s, char old, char new){
    while(*s != '\n' && *s){
        if(*s == old) *s = new;
        s++;
    }
}

void jumpLine(FILE *file){
    while(fgetc(file) != '\n');
}

void jumpLines(FILE *file, unsigned int n_lines){
    if(n_lines > 0){
        jumpLine(file);
        jumpLines(file,n_lines-1);
    }
}

int num_of_pages(int len){
    if(len % LINES_PER_PAGE == 0) return len / LINES_PER_PAGE;
    else return (len / LINES_PER_PAGE) +1;
}

void printNLines(FILE *file, unsigned int n_lines){
    char buffer[MB] = "\0",*aux = buffer;
    unsigned int i;
    for(i = 0; i < n_lines && aux; i++){
        aux = fgets(buffer,MB-1,file);
        if(aux){
            replace(buffer,';','|');
            printf("%s",buffer);
        }
    }
    rewind(file);
}

void print_page(FILE *file, unsigned int page_index){
    jumpLines(file,LINES_PER_PAGE*(page_index-1));
    printNLines(file,LINES_PER_PAGE);
}

unsigned short line_belongs_to_file(char *line, FILE *file){
    char buffer[MB] = "\0"; // para guardar as linhas do ficheiro
    replace(line,'\r','\n'); // evitar a ocorrência de \r
    while(fgets(buffer,MB-1,file)){ // leitura da linha do ficheiro
        replace(buffer,'\r','\n'); // evitar a ocorrência de \r
        if(!strcmp(line,buffer)){ // encontra a linha, então pára e retorna 1
            rewind(file);
            return 1;
        }
    }
    rewind(file);
    return 0;
}

/*
Devolve o número de linhas que os ficheiros têm em comum
*/
unsigned int compare_files(FILE *to_test, FILE *to_compare, unsigned int *lines_tested){
    unsigned int lines_in_both_files = 0, lines_tested_ = 0;
    char buffer[MB] = "\0";
    while(fgets(buffer,MB-1,to_test)){
        if(line_belongs_to_file(buffer,to_compare)){
            lines_in_both_files++;
        }
        lines_tested_++;
    }
    if(lines_tested) *lines_tested = lines_tested_;
    return lines_in_both_files;
}
