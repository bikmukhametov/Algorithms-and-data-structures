#ifndef LAB4_H
#define LAB4_H

typedef struct Info{
    char* info;
    int release;
    struct Info* next;
} Info;

typedef struct Node{
    char* key;
    Info* info;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;

typedef struct Tree{
    Node* root;
} Tree;

void correction(char** str);
int search(FILE* file, Tree* tree, Node* node, int *quantity);
int D_search(Tree*);
int Add(Tree* tree, char* key, char *str);
int D_Add(Tree* tree);
void Show(Node* root, int* n);
int D_Show(Tree* tree);
int D_Bypass(Tree* tree);
int Delete(Tree* tree, char* key);
int D_Delete(Tree* tree);
Node* Find_key(Node* root, char* key);
Node* Find_min(Node* root);
Node* Find_next(Node* x);
int D_Find_Key(Tree* tree);
Node* Find_Max(Node* root);
int D_Find_Max(Tree* tree);
void Delete_recursion(Node* root);
void Delete_tree(Tree* tree);
void input_file(Tree* tree, FILE** file);
void output_file_recursion(Node* root, FILE** file);
void Draw(Tree* tree);
void Draw_recursion(FILE* file, Node* root);
void output_file(Tree* tree);
int D_Timing(Tree*);
int dialog(const char *msgs[], int N);

#endif
