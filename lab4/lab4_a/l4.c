#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab4.h"

int main() {
    const char *msgs[] = {"0. Quit", "1. Add", "2. Bypass", "3. Find_Key", "4. Find_Max",
                          "5. Delete", "6. Show", "7. Timing", "8. Search in the file"};
    const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
    int (*fptr[])(Tree *) = {NULL, D_Add, D_Bypass, D_Find_Key, D_Find_Max, D_Delete, D_Show, D_Timing, D_search};
    int rc;
    Tree* tree = (Tree*) calloc (1, sizeof(Tree));
    tree->root = NULL;
    FILE* file;
    file = fopen("file.txt", "a+");
    input_file(tree, &file);
    fclose(file);
    while ((rc = dialog(msgs, NMsgs))){
        if (fptr[rc](tree)){
            break;
        }
    }
    printf("That's all. Bye!\n");
    output_file(tree);
    Draw(tree);
    Delete_tree(tree);
    return 0;
}
