#include <stdio.h>
#include <stdlib.h>
#include "EvalCircuit.h"
#include "evalCirc.h"
#include "getGate.h"

void err_sys(char* str1, ...){
    void* nextval = &str1;
    nextval = (char *)nextval + 1;
    printf(str1, *(int *)nextval, nextval+1);
    perror("errno: ");
    exit(EXIT_FAILURE);
}


void debug_pg8244gate(pgate8244_t g){
    printf("\ninput | ");
    for (int i = 0; i < 16; ++i) {
        printf("%x ", g->gid[i]);
        if(i % 2)
            printf("| ");
    }
    printf(" | type | ");
    for (int j = 0; j < 4; ++j) {
        printf("%x ", g->gtype[j]);
    }
    printf("\n");
}

void debug_vm(UByteP vm){
    printf("vm: ");
    for (int i = 0; i < pArgs.vmsize; ++i) {
        printf("%d %x | ",i, vm[i]);
    }
    printf("\n");
}