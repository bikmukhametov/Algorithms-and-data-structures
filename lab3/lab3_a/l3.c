#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab3.h"


int main(){
        int i = 0;
        Table table;
        table.ks = NULL;
        table.msize = 0;
        table.csize = 0;
        i = dialog(&table);
        if(table.ks != NULL){
                delet_table(&table);
        }
        return 0;
}
