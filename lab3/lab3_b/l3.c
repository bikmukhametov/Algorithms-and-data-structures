#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab3.h"

int main(){
    int i = 0;
    Table table;
    table.first = NULL;
    table.fname = NULL;
    table.fd = NULL;
    i = dialog(&table);
    if(table.first != NULL){
        delet_table(&table);
    }
    return 0;
}
