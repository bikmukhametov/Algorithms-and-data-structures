#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab4.h"

int main() {
    const char *msgs[] = {"0. Quit", "1. Add", "2. Bypass", "3. Find_Key", "4. Find_next",
                          "5. Delete", "6. Show", "7. Timing"};
    const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
    int (*fptr[])(Tree *) = {NULL, D_Add, D_Bypass, D_Find_Key, D_Find_next, D_Delete, D_Show, D_Timing};
    int rc;
    Tree* tree = (Tree*) calloc (1, sizeof(Tree));
    tree->Elist = (Node *) calloc (1, sizeof(Node));
    tree->Elist->color = 0;
    tree->root = tree->Elist;
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
