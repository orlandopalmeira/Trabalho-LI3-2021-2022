#include <stdio.h>
#include <time.h>

int dateLower(int d1, int m1, int y1, int d2, int m2, int y2){
    if(y1 < y2) return 1;
    else if(y1 == y2 && m1 < m2) return 1;
    else if(y1 == y2 && m1 == m2 && d1 < d2) return 1;
    return 0;
}

int todayOrLess(int d, int m, int y){
    time_t seconds = time(NULL);
    struct tm* current_time = localtime(&seconds);
    int year = current_time->tm_year + 1900, month = current_time->tm_mon +1, day = current_time->tm_mday;
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