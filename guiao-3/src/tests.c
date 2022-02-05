#include "tests.h"
#include "fileUtils.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

/**
 * Retorna a taxa de acerto do meu output com o output esperado.
 */
float hit_rate(FILE *my_result, FILE *expected_result){
    unsigned int correct_lines = 0, tested_lines = 0;
    correct_lines = compare_files(my_result,expected_result,&tested_lines);
    return (float)correct_lines / tested_lines;
}

/**
 * Calcula a média de um array de floats
 */
float average_f(float *values, unsigned int length){
    float sum = 0;
    unsigned int i;
    for(i = 1; i <= length; i++){
        sum += values[i-1];
    }
    return sum / i;
}

/**
 * Calcula a média de um array de doubles
*/
double average_d(double *values, unsigned int length){
    double sum = 0;
    unsigned int i;
    for(i = 1; i <= length; i++){
        sum += values[i-1];
    }
    return sum / i;
}

/**
 *  Calcula o desvio padrão de um array de floats.
 */
float std_deviation_f(float *values, unsigned int length){
    float sum = 0.0, mean, SD = 0.0;
    unsigned int i;
    for (i = 0; i < length; ++i) {
        sum += values[i];
    }
    mean = sum / length;
    for (i = 0; i < length; ++i) {
        SD += pow(values[i] - mean, 2);
    }
    return sqrt(SD / (length-1));
}

/**
 *  Calcula o desvio padrão de um array de doubles.
 */
double std_deviation_d(double *values, unsigned int length){
    double sum = 0.0, mean, SD = 0.0;
    unsigned int i;
    for (i = 0; i < length; ++i) {
        sum += values[i];
    }
    mean = sum / length;
    for (i = 0; i < length; ++i) {
        SD += pow(values[i] - mean, 2);
    }
    return sqrt(SD / (length-1));
}


/**
 * Efetua os testes à querie 1.
 * Recebe a função de execução da querie1 como argumento.
*/
void test_querie_1(int users, int orgs, int bots, void (*querie1)(int,int,int,int)){
    unsigned int i;
    clock_t start, end;
    FILE *q1_report = fopen("./reports/q1_report.csv","w"),
         *expected_result = fopen("./expected/q1_exp_res.txt","r");
    fprintf(q1_report,"#Execution;Time taken;Hit rate\n");
    for(i = 1; i <= NUMBER_OF_TESTS; i++){
        start = clock();
        querie1(users,orgs,bots,1);
        end = clock();
        FILE *result = fopen("./saida/command1_output.txt","r");
        fprintf(q1_report,"%u;%lf;%f\n",i,(double)(end-start) / CLOCKS_PER_SEC,hit_rate(result,expected_result));
        fclose(result);
    }
    fclose(expected_result);
    fclose(q1_report);
    printf("Tests querie1 done!\n");
}

/**
 * Efetua os testes à querie 2.
 * Recebe a função de execução da querie1 como argumento.
*/
void test_querie_2(int colabs, int repos, void (*querie2)(int,int,int)){
    unsigned int i;
    clock_t start, end;
    FILE *q2_report = fopen("./reports/q2_report.csv","w"),
         *expected_result = fopen("./expected/q2_exp_res.txt","r");
    fprintf(q2_report,"#Execution;Time taken;Hit rate\n");
    for(i = 1; i <= NUMBER_OF_TESTS; i++){
        start = clock();
        querie2(colabs,repos,2);
        end = clock();
        FILE *result = fopen("./saida/command2_output.txt","r");
        fprintf(q2_report,"%u;%lf;%f\n",i,(double)(end-start) / CLOCKS_PER_SEC,hit_rate(result,expected_result));
        fclose(result);
    }
    fclose(expected_result);
    fclose(q2_report);
    printf("Tests querie2 done!\n");
}

/**
 * Efetua os testes à querie 3.
 * Recebe a função de execução da querie1 como argumento.
*/
void test_querie_3(int collabBots, void (*querie3)(int,int)){
    unsigned int i;
    clock_t start, end;
    FILE *q3_report = fopen("./reports/q3_report.csv","w"),
         *expected_result = fopen("./expected/q3_exp_res.txt","r");
    fprintf(q3_report,"#Execution;Time taken;Hit rate\n");
    for(i = 1; i <= NUMBER_OF_TESTS; i++){
        start = clock();
        querie3(collabBots,3);
        end = clock();
        FILE *result = fopen("./saida/command3_output.txt","r");
        fprintf(q3_report,"%u;%lf;%f\n",i,(double)(end-start) / CLOCKS_PER_SEC,hit_rate(result,expected_result));
        fclose(result);
    }
    fclose(expected_result);
    fclose(q3_report);
    printf("Tests querie3 done!\n");
}

/**
 * Efetua os testes à querie 4.
 * Recebe a função de execução da querie1 como argumento.
*/
void test_querie_4(int commits, int users, void (*querie4)(int,int,int)){
    unsigned int i;
    clock_t start, end;
    FILE *q4_report = fopen("./reports/q4_report.csv","w"),
         *expected_result = fopen("./expected/q4_exp_res.txt","r");
    fprintf(q4_report,"#Execution;Time taken;Hit rate\n");
    for(i = 1; i <= NUMBER_OF_TESTS; i++){
        start = clock();
        querie4(commits,users,4);
        end = clock();
        FILE *result = fopen("./saida/command4_output.txt","r");
        fprintf(q4_report,"%u;%lf;%f\n",i,(double)(end-start) / CLOCKS_PER_SEC,hit_rate(result,expected_result));
        fclose(result);
    }
    fclose(expected_result);
    fclose(q4_report);
    printf("Tests querie4 done!\n");
}

void test_querie_5(CatUsers cusers, void (*querie5)(char*,CatUsers,int)){
    unsigned int i;
    clock_t start, end;
    FILE *q5_report = fopen("./reports/q5_report.csv","w"),
         *expected_result_1 = fopen("./expected/q5_exp_res_1.txt","r"),
         *expected_result_2 = fopen("./expected/q5_exp_res_2.txt","r");
    fprintf(q5_report,"Command: %s\n",Q5_ARGS1);
    fprintf(q5_report,"#Execution;Time taken;Hit rate\n");
    for(i = 1; i <= NUMBER_OF_TESTS; i++){
        start = clock();
        querie5(Q5_ARGS1,cusers,5);
        end = clock();
        FILE *result = fopen("./saida/command5_output.txt","r");
        fprintf(q5_report,"%u;%lf;%f\n",i,(double)(end-start) / CLOCKS_PER_SEC,hit_rate(result,expected_result_1));
        fclose(result);
    }

    fprintf(q5_report,"\nCommand: %s\n",Q5_ARGS2);
    fprintf(q5_report,"#Execution;Time taken;Hit rate\n");
    for(i = 1; i <= NUMBER_OF_TESTS; i++){
        start = clock();
        querie5(Q5_ARGS2,cusers,5);
        end = clock();
        FILE *result = fopen("./saida/command5_output.txt","r");
        fprintf(q5_report,"%u;%lf;%f\n",i,(double)(end-start) / CLOCKS_PER_SEC,hit_rate(result,expected_result_2));
        fclose(result);
    }

    fclose(q5_report); fclose(expected_result_1); fclose(expected_result_2);
    printf("Tests querie5 done!\n");
}

void test_querie_6(CatUsers cusers, CatRepos crepos, void (*querie6)(char*,CatUsers,CatRepos,int)){
    unsigned int i;
    char aux[100];
    clock_t start, end;
    FILE *q6_report = fopen("./reports/q6_report.csv","w"),
         *expected_result_1 = fopen("./expected/q6_exp_res_1.txt","r"),
         *expected_result_2 = fopen("./expected/q6_exp_res_2.txt","r");
    strncpy(aux,Q6_ARGS1,99);
    fprintf(q6_report,"Command: %s\n",Q6_ARGS1);
    fprintf(q6_report,"#Execution;Time taken;Hit rate\n");
    for(i = 1; i <= NUMBER_OF_TESTS; i++){
        start = clock();
        querie6(aux,cusers,crepos,6);
        end = clock();
        strncpy(aux,Q6_ARGS1,99);
        FILE *result = fopen("./saida/command6_output.txt","r");
        fprintf(q6_report,"%u;%lf;%f\n",i,(double)(end-start) / CLOCKS_PER_SEC,hit_rate(result,expected_result_1));
        fclose(result);
    }

    strncpy(aux,Q6_ARGS2,99);
    fprintf(q6_report,"\nCommand: %s\n",Q6_ARGS2);
    fprintf(q6_report,"#Execution;Time taken;Hit rate\n");
    for(i = 1; i <= NUMBER_OF_TESTS; i++){
        start = clock();
        querie6(aux,cusers,crepos,6);
        end = clock();
        strncpy(aux,Q6_ARGS2,99);
        FILE *result = fopen("./saida/command6_output.txt","r");
        fprintf(q6_report,"%u;%lf;%f\n",i,(double)(end-start) / CLOCKS_PER_SEC,hit_rate(result,expected_result_2));
        fclose(result);
    }

    fclose(q6_report); fclose(expected_result_1); fclose(expected_result_2);
    printf("Tests querie6 done!\n");
}

/*
void querie7(){

}
*/

void test_querie_8(CatRepos crepos, void (*querie8)(char*,CatRepos,int)){
    unsigned int i;
    clock_t start, end;
    FILE *q8_report = fopen("./reports/q8_report.csv","w"),
         *expected_result_1 = fopen("./expected/q8_exp_res_1.txt","r"),
         *expected_result_2 = fopen("./expected/q8_exp_res_2.txt","r");
    fprintf(q8_report,"Command: %s\n",Q8_ARGS1);
    fprintf(q8_report,"#Execution;Time taken;Hit rate\n");
    for(i = 1; i <= NUMBER_OF_TESTS; i++){
        start = clock();
        querie8(Q8_ARGS1,crepos,8);
        end = clock();
        FILE *result = fopen("./saida/command8_output.txt","r");
        fprintf(q8_report,"%u;%lf;%f\n",i,(double)(end-start) / CLOCKS_PER_SEC,hit_rate(result,expected_result_1));
        fclose(result);
    }

    fprintf(q8_report,"\nCommand: %s\n",Q8_ARGS2);
    fprintf(q8_report,"#Execution;Time taken;Hit rate\n");
    for(i = 1; i <= NUMBER_OF_TESTS; i++){
        start = clock();
        querie8(Q8_ARGS2,crepos,8);
        end = clock();
        FILE *result = fopen("./saida/command8_output.txt","r");
        fprintf(q8_report,"%u;%lf;%f\n",i,(double)(end-start) / CLOCKS_PER_SEC,hit_rate(result,expected_result_2));
        fclose(result);
    }

    fclose(q8_report); fclose(expected_result_1); fclose(expected_result_2);
    printf("Tests querie8 done!\n");
}

void test_querie_9(CatUsers cusers, CatRepos crepos,void (*querie9)(char*,CatUsers,CatRepos,int)){
    unsigned int i;
    char aux[100];
    clock_t start, end;
    FILE *q9_report = fopen("./reports/q9_report.csv","w"),
         *expected_result_1 = fopen("./expected/q9_exp_res_1.txt","r"),
         *expected_result_2 = fopen("./expected/q9_exp_res_2.txt","r");
    strncpy(aux,Q9_ARGS1,99);
    fprintf(q9_report,"Command: %s\n",Q9_ARGS1);
    fprintf(q9_report,"#Execution;Time taken;Hit rate\n");
    for(i = 1; i <= NUMBER_OF_TESTS; i++){
        start = clock();
        querie9(Q9_ARGS1,cusers,crepos,9);
        end = clock();
        strncpy(aux,Q9_ARGS1,99);
        FILE *result = fopen("./saida/command9_output.txt","r");
        fprintf(q9_report,"%u;%lf;%f\n",i,(double)(end-start) / CLOCKS_PER_SEC,hit_rate(result,expected_result_1));
        fclose(result);
    }

    strncpy(aux,Q9_ARGS2,99);
    fprintf(q9_report,"\nCommand: %s\n",Q9_ARGS2);
    fprintf(q9_report,"#Execution;Time taken;Hit rate\n");
    for(i = 1; i <= NUMBER_OF_TESTS; i++){
        start = clock();
        querie9(Q9_ARGS2,cusers,crepos,9);
        end = clock();
        strncpy(aux,Q9_ARGS2,99);
        FILE *result = fopen("./saida/command9_output.txt","r");
        fprintf(q9_report,"%u;%lf;%f\n",i,(double)(end-start) / CLOCKS_PER_SEC,hit_rate(result,expected_result_2));
        fclose(result);
    }

    fclose(q9_report); fclose(expected_result_1); fclose(expected_result_2);
    printf("Tests querie9 done!\n");
}

/*
void test_querie_10(...){
    ...
}
*/
