#ifndef LAB4_H
#define LAB4_H

typedef struct Info{                                    //информация
    char* info;
    int release;
    struct Info* next;
} Info;
typedef struct Node{
    int color;  // 0 - Black; 1 - Red.
    int key;
    Info* info;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;
typedef struct Tree{
    Node* root;
    Node* Elist;
} Tree;

int scan(int *x);
int my_readline(char **stroka);
Node* Right(Tree* tree, Node* node);
Node* Left(Tree* tree, Node* node);
int Red(Tree* tree, Node *node);
void balancing(Tree* tree, Node* par);
int Add(Tree* tree, int key, char *str);
int D_Add(Tree* tree);
void Show(Tree* tree, Node* root, int* n);
int D_Show(Tree* tree);
int D_Bypass(Tree* tree);
int Delete(Tree* tree, int key);
int D_Delete(Tree* tree);
Node* Find_key(Tree* tree, int key);
Node* Find_min(Tree* tree, Node* root);
Node* Find_next(Tree* tree, int x);
int D_Find_next(Tree* tree);
int D_Find_Key(Tree* tree);
Node* Find_next_key(Tree* tree, Node* x);
void Delete_recursion(Tree* tree, Node* root);
void Delete_tree(Tree* tree);
void input_file(Tree* tree, FILE** file);
void Draw_recursion(FILE* file, Tree* tree, Node* root);
void Draw(Tree *tree);
void output_file_recursion(Tree* tree, Node* root, FILE** file);
void output_file(Tree* tree);
int D_Timing(Tree*);
int dialog(const char *msgs[], int N);
int bypass(Tree* tree, Node* node, int x);

#endif
