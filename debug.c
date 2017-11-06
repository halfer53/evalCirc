#include <stdio.h>
#include <stdlib.h>

void err_sys(char* str1, char* line, char* file){
    printf("%s in line %s in %s\n", str1, line, file);
    abort();
}