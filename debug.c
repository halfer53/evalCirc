#include <stdio.h>
#include <stdlib.h>

void err_sys(char* str1, ...){
    void* nextval = &str1;
    nextval = (char *)nextval + 1;
    printf(str1, *(int *)nextval, nextval+1);
    perror("errno: ");
    exit(EXIT_FAILURE);
}