// Trabalho Pratico Programacao - LEI
// DEIS-ISEC 2020-2021
// André Lopes - 2019139754

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"


void initRandom(){
    srand(time(NULL));
}

int intUniformRnd(int a, int b){
    return a + rand()%(b-a+1);
}



