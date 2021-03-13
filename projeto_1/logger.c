/* localtime example */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "logger.h"

FILE *fp;
int first_log_call = 0;

void build_datetime_string(char string[]){
    time_t rawtime;
    struct tm *timeinfo;

    char timestr[30];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strcpy(timestr, asctime(timeinfo));
    
    // montando string de data
    strncpy(string, &timestr[20], 4); // ano
    strcat(string, "-");
    strncat(string, &timestr[4], 3); // mes
    strcat(string, "-");
    strncat(string, &timestr[8], 2); // dia
    
    // montando string de hora
    strcat(string, " ");
    strncat(string, &timestr[11], 8); // hora
    strcat(string, "\0");
}

void create_log_file(){
    fp = fopen("log.txt", "w");

    if(fp == NULL){
        printf("Erro ao criar arquivo de log.\n");
        exit(1);
    }
}

void write_on_log_file(float TI, float TE, float TR){
    if(first_log_call == 0){
        fputs("data/hora,temperatura_interna,temperatura_externa,temperatura_de_referencia\n", fp);
        first_log_call++;
    }
    
    char *d = (char *) malloc(sizeof(char) * DATETIME_STRING_SIZE);
    build_datetime_string(d);
    fputs(d, fp);
    fputs(", ", fp);
    free(d);

    char ti_buffer[20];
    char te_buffer[20];
    char tr_buffer[20];

    gcvt(TI, 4, ti_buffer);
    gcvt(TE, 4, te_buffer);
    gcvt(TR, 4, tr_buffer);

    fputs(ti_buffer, fp);
    fputs(", ", fp);

    fputs(te_buffer, fp);
    fputs(", ", fp);

    fputs(tr_buffer, fp);
    fputs("\n", fp);
}

void close_log_file(){
    fclose(fp);
}
