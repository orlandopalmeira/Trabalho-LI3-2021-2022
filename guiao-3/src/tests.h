#ifndef TESTS_H
#define TESTS_H
#include "users.h"
#include "commits.h"

#define NUMBER_OF_TESTS 10
#define Q5_ARGS1 "50 2000-01-01 2022-02-05"
#define Q5_ARGS2 "10000 2000-01-01 2022-02-05"
#define Q6_ARGS1 "100 JavaScript\0"
#define Q6_ARGS2 "10000 Java\0"
#define Q7_ARGS1 "2015-06-05"
#define Q7_ARGS2 "2017-11-03"
#define Q8_ARGS1 "30 2015-01-01"
#define Q8_ARGS2 "500 2016-01-01"
#define Q9_ARGS1 "100"
#define Q9_ARGS2 "10000"

void test_querie_1(int users, int orgs, int bots, void (*querie1)(int,int,int,int));

void test_querie_2(int colabs, int repos, void (*querie2)(int,int,int));

void test_querie_3(int collabBots, void (*querie3)(int,int));

void test_querie_4(int commits, int users, void (*querie4)(int,int,int));

void test_querie_5(CatUsers cusers, void (*querie5)(char*,CatUsers,int));

void test_querie_6(CatUsers cusers, CatRepos crepos, void (*querie6)(char*,CatUsers,CatRepos,int));

void test_querie_8(CatRepos crepos, void (*querie8)(char*,CatRepos,int));

void test_querie_9(CatUsers cusers, CatRepos crepos,void (*querie9)(char*,CatUsers,CatRepos,int));

#endif