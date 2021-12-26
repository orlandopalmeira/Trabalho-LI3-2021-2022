#ifndef FILEUTILS_H
#define FILEUTILS_H

short dateLower(int d1, int m1, int y1, int d2, int m2, int y2);

short dateHigher(int d1, int m1, int y1, int d2, int m2, int y2);

short dateEqual(int d1,int m1, int y1,int d2,int m2, int y2);

short dateBetween(int d1, int m1, int y1, int db, int mb, int yb, int d2, int m2, int y2);

short todayOrLess(int d, int m, int y);

int strcmpV2(char s1[], char s2[], char endchar);

int strlenV2(char str[], char endchar);

short belongsToArrInt(unsigned int val, unsigned int *arr, int N);

#endif