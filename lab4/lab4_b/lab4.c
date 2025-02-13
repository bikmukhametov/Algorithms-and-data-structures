#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "lab4.h"
#include "/home/students/b/bikmukhametov.aa/lib/input.c"

enum {
    BLACK = 0,
    RED = 1,
};

void Draw_recursion(FILE* file, Tree* tree, Node* root){
    if(root == tree->Elist){
	fprintf(file, "Elist [color=blue]; \n");
    }else{
        if(root->info != NULL){
            fprintf(file, "\"%d\" [shape=\"record\", label = \"%d|{", root->key, root->key);
            Info* element = root->info;
            while(element != NULL){
                fprintf(file, "release: %d\\ninfo: %s", element->release, element->info);
                if(element->next != NULL){
                    fprintf(file, "|", element->release, element->info);
                }
                element = element->next;
            }
            fprintf(file, "}\"]; \n");
	    if(root->parent == tree->Elist){
		fprintf(file, "Elist -- \"%d\" [style=dashed]; \n", root->key);
	    }else{
        	fprintf(file, "\"%d\" -- \"%d\"; \n", root->parent->key, root->key);
            }
	    if(root->color == BLACK){
  	        fprintf(file, "\"%d\" [color=black]; \n", root->key);
	    }else if(root->color == RED){
		fprintf(file, "\"%d\" [color=red]; \n", root->key);
            }
	    if((root->left == tree->Elist) && (root->right == tree->Elist)){
		fprintf(file, "\"%d\" -- Elist [style=dashed]; \n", root->key);
	    }
        }
        Draw_recursion(file, tree, root->left);
        Draw_recursion(file, tree, root->right);
    }
}

void Draw(Tree *tree){
    FILE *file = fopen("result.dot", "w");
    fprintf(file, "graph G{\n");
    Draw_recursion(file, tree, tree->root);
    fprintf(file, "}\n");
    fclose(file);
    system("dot -Tpng result.dot -o result.png");
}

Node* Left(Tree* tree, Node* node){
    Node *r = node->right;
    Node *rl = r->left;

    node->right = rl;
    if(rl != tree->Elist){
        rl->parent = node;
    }

    r->left = node;
    r->parent = node->parent;
    node->parent = r;

    if(r->parent == tree->Elist){
        tree->root = r;
    }else{
        Node* g = r->parent;
        if(g->left == node){
            g->left = r;
        }else{
            g->right = r;
        }
    }
    //printf("l\n");
    return r;
} // правый поворот
Node* Right(Tree* tree, Node* node){
    Node *l = node->left;
    Node *lr = l->right;

    node->left = lr;
    if(lr != tree->Elist) {
        lr->parent = node;
    }

    l->right = node;
    l->parent = node->parent;
    node->parent = l;

    if(l->parent == tree->Elist){
        tree->root = l;
    }else{
        Node* g = l->parent;
        if(g->left == node){
            g->left = l;
        }else{
            g->right = l;
        }
    }
    //printf("r\n");
    return l;
} //левый поворот

void balancing(Tree* tree, Node* par){
    while(par != tree->Elist){
        //printf("ooo0\n");
        if((Red(tree, par->right) == 1) && (Red(tree, par->left) == 0)){
            par = Left(tree, par);
            int c = par->color;
            par->color = par->left->color;
            par->left->color = c;
            //printf("--1--\n");
        }
        if((Red(tree, par->left) == 1) && (Red(tree, par->left->left) == 1)){
            par = Right(tree, par);
            int c = par->color;
            par->color = par->right->color;
            par->right->color = c;
            //printf("--2--\n");
        }
        if((Red(tree, par->left) == 1) && (Red(tree, par->right) == 1)){
            par->color = (par->color + 1) % 2;
            par->left->color = 0;
            par->right->color = 0;
            //printf("--3--\n");
        }
        par = par->parent;
    }
} // балансировка
int Red(Tree* tree, Node *node){
    if ((node != tree->Elist) && (node != NULL)){   ///////
        return node->color;
    }else{
        return 0;
    }
} //красный/черный элемент
int Add(Tree* tree, int key, char *str){       //  функция добавления элемента
    if(key < 0){
        printf("error!\n");
        free(str);
        return 0;
    }
    Info* info = (Info *) calloc (1, sizeof(Info));
    Node* node = (Node *) calloc (1, sizeof(Node));
    node->info = info;
    node->info->info = str;
    node->info->release = 1;
    node->key = key;
    node->color = 1;
    if(tree->root == tree->Elist){
        tree->root = node;
        node->color = 0;
        node->parent = tree->Elist;
        node->left = tree->Elist;
        node->right = tree->Elist;
        return 0;
    }
    Node* node_release = Find_key(tree, key);
    if(node_release != tree->Elist){
        Info* info_release = node_release->info;
        while(info_release->next != NULL){
            info_release = info_release->next;
        }
        info_release->next = info;
        info->release = info_release->release + 1;
        //node->info = info;
        free(node);
        return 2;
    }

    Node* ptr = tree->root;
    Node* par = tree->Elist;
    while(ptr != tree->Elist){
        par = ptr;
        if(key < ptr->key){
            ptr = ptr->left;
        }else if(key > ptr->key){
            ptr = ptr->right;
        }
    }

    if(key < par->key){
        par->left = node;
    }else{
        par->right = node;
    }
    node->parent = par;
    node->left = tree->Elist;
    node->right = tree->Elist;

    balancing(tree, par);
    return 0;
} // добавление элемента
int D_Add(Tree* tree){
    int key = 0;
    char* info = NULL;
    printf("Enter key: ");
    int i = scan(&key);
    if(i == 1) {
        return 1;
    }
    printf("Enter info: ");
    i = my_readline(&info);
    if (i == 1) {
        free(info);
        return 1;
    }
    if(info[0] == '\0'){
        printf("Ошибка! Пустое значение\n");
        free(info);
        return 0;
    }else{
        Add(tree, key, info);
        tree->root->color = 0;
    }
    return 0;
} // добавление элемента
void Show(Tree* tree, Node* root, int* n){
    if(root != tree->Elist){
        if(root->info != NULL){
            Info* element = root->info;
            int i = 0;
            while(element != NULL){
                while(i < *n){
                    if(i + 9 > *n){
                        printf("-");
                    }else if(i + 10 > *n){
                        printf("|");
                    }else{
                        printf(" ");
                    }
                    i++;
                }
                int l_r = 0;
                if(root->parent != tree->Elist){
                    if(root == root->parent->left){
                        l_r = -1;
                    }else{
                        l_r = 1;
                    }
                }
                if((root->color == 1) && (l_r == - 1)){
                    printf(">l {(R) %d;%d;%s}", root->key, element->release, element->info);
                }else if((root->color == 1) && (l_r == 1)){
                    printf(">r {(R) %d;%d;%s}", root->key, element->release, element->info);
                }else if((root->color == 0) && (l_r == - 1)){
                    printf(">l {(B) %d;%d;%s}", root->key, element->release, element->info);
                }else if((root->color == 0) && (l_r == 1)){
                    printf(">r {(B) %d;%d;%s}", root->key, element->release, element->info);
                }else if(root->color == 1){
                    printf("> {(R) %d;%d;%s}", root->key, element->release, element->info);
                }else if(root->color == 0){
                    printf("> {(B) %d;%d;%s}", root->key, element->release, element->info);
                }
                element = element->next;
                if(element != NULL) {
                    i = *n;
                    printf(" --");
                }
            }
            printf("\n\n");
        }
        *n += 14;
        Show(tree, root->right, n);
        Show(tree, root->left, n);
        *n -= 14;
    }
} // полный обход
int D_Show(Tree* tree){
    int n = 2;
    Show(tree, tree->root, &n);
    return 0;
} // полный обход

int bypass(Tree* tree, Node* node, int x){
    if(node != tree->Elist){
        while((node->key >= x)){
            node = node->left;
            if(node == tree->Elist){
                return 0;
            }
        }
        if(node->info != NULL){
            Info* element = node->info;
            while(element != NULL){
                if(node->color == 1){
                    printf("---> {(R) %d;%d;%s}", node->key, element->release, element->info);
                }else if(node->color == 0){
                    printf("---> {(B) %d;%d;%s}", node->key, element->release, element->info);
                }
                element = element->next;
                printf("\n");
            }
        }
        bypass(tree, node->left, x);
        bypass(tree, node->right, x);
    }
    return 0;
} // обход
int D_Bypass(Tree* tree){
    if(tree->root != NULL){
        int x = INT_MAX;
        printf("before: ");
        int i = scan(&x);
        if(i == 1){
            return 1;
        }
        bypass(tree, tree->root, x);
    }
    return 0;
} // обход

int Delete(Tree* tree, int key){ //с лекции
    Node* x = Find_key(tree, key);
    if(x == tree->Elist){
        return 1;
    }
    if(x->info->next != NULL){
        Info* element = x->info;
        Info* element2 = element->next;
        x->info = element2;
        free(element->info);
        free(element);
        return 0;
    }

    Node *y;
    if((x->right == tree->Elist) || (x->left == tree->Elist)) {
        y = x;
    }else{
        y = Find_next_key(tree, x);
    }
    /*if(y == tree->root){
        tree->root = tree->Elist;

        free(y->info->info);
        free(y->info);
        free(y);
        return 0;
    }*/

    Node *p; // единственное поддерево Y
    if(y->left != tree->Elist){
        p = y->left;
    }else{
        p = y->right;
    }

    Node *par = y->parent; //связать деда с внуком
    if(p != tree->Elist){
        p->parent = par;
    }
    if(par == tree->Elist){
        tree->root = p;
    }else if(par->left == y){
        par->left = p;
    }else{
        par->right = p;
    }


    if(y != x){ //переносим, затем удаляем нижний
        free(x->info->info); x->info->info = NULL;
        free(x->info);
        x->info = y->info;
        x->key = y->key;
        free(y);

    }else{ //весь элемент удаляем
        free(y->info->info);
        free(y->info);
        free(y);
    }

    balancing(tree, par);

    return 0;
} // удаление элемента
int D_Delete(Tree* tree){
    if(tree->root == tree->Elist){
        printf("Not tree!\n");
        return 0;
    }
    printf("Enter the key: ");
    int key = 0;
    int i = scan(&key);
    if(i == 1){
        return 0;
    }
    Node* node = Find_key(tree, key);
    if(node == tree->Elist){
        printf("Not key!\n");
        return 0;
    }
    i = Delete(tree, key);
    if(i == 1){
        printf("Not key!\n");
    }else{
        printf("Ok!\n");
    }
    return 0;
} // удаление элемента

Node* Find_key(Tree* tree, int key){
    Node* root = tree->root;
    if(root != tree->Elist){
        Node* ptr = root;
        while(ptr != tree->Elist){
            if(key == ptr->key){
                return ptr;
            }else if(key > ptr->key){
                ptr = ptr->right;
            }else if(key < ptr->key){
                ptr = ptr->left;
            }
        }
    }
    return tree->Elist;
} // поиск по ключу в дереве
int D_Find_Key(Tree* tree) {
    printf("Enter the key: ");
    int key = 0;
    int i = scan(&key);
    printf("\n");
    if (i == 1) {
        return 1;
    }
    Node *node = Find_key(tree, key);
    if (node == tree->Elist) {
        printf("Not key!\n");
        return 0;
    }
    Info *element = node->info;
    while (element != NULL) {
        if (node->color == 1) {
            printf("---> {(R) %d;%d;%s}", node->key, element->release, element->info);
        } else if (node->color == 0) {
            printf("---> {(B) %d;%d;%s}", node->key, element->release, element->info);
            printf("\n");
        }element = element->next;
    }
    return 0;
} // поиск по ключу
Node* Find_min(Tree* tree, Node* root){
    if(root == tree->Elist){
        return tree->Elist;
    }else{
        while(root->left != tree->Elist){
            root = root->left;
        }
        return root;
    }
} // поиск минимального в поддереве
Node* Find_next(Tree* tree, int x){
    Node* ptr = tree->root;
    Node* par = tree->root;
    Node* res;
    while(ptr != tree->Elist){
        par = ptr;
        if(x < ptr->key){
            ptr = ptr->left;
        }else if(x > ptr->key){
            ptr = ptr->right;
        }else{
            res = Find_next_key(tree, ptr);
            return res;
        }
    }
    if(x < par->key){
        return par;
    }else{
        res = Find_next_key(tree, par);
        return res;
    }
} // ключ, превышающий значение
int D_Find_next(Tree* tree){
    printf("Enter the key: ");
    int key = 0;
    int i = scan(&key);
    printf("\n");
    if(i == 1){
        return 1;
    }
    Node* node = Find_next(tree, key);
    Info* element = node->info;
    while(element != NULL){
        if(node->color == 1){
            printf("---> {(R) %d;%d;%s}", node->key, element->release, element->info);
        }else if(node->color == 0){
            printf("---> {(B) %d;%d;%s}", node->key, element->release, element->info);
        }
        element = element->next;
    }
    return 0;
} // ключ, превышающий значение
Node* Find_next_key(Tree* tree, Node* x){
    Node* y;
    if(x->right != tree->Elist){
        y = Find_min(tree, x->right);
        return y;
    }else{
        Node* ptr = x->parent;
        while((ptr != tree->Elist) && (ptr->right == x)){
            x = ptr;
            ptr = x->parent;
        }
        return ptr;
    }
} //поиск следующего ключа

void Delete_recursion(Tree* tree, Node* root){
    if(root != tree->Elist){
        Delete_recursion(tree, root->left);
        Delete_recursion(tree, root->right);
        if(root != tree->Elist){
            Info* element = root->info;
            Info* element2 = NULL;
            while(element->next != NULL){
                element2 = element->next;
                element->next = element2->next;
                if(element2->info != NULL){
                    free(element2->info);
                    element2->info = NULL;
                }
                free(element2);
            }
            if(element->info != NULL) {
                free(element->info);
                element->info = NULL;
            }
            free(element);
            free(root);
        }
    }
} // удаление дерева
void Delete_tree(Tree* tree){
    if(tree->root != NULL) {
        Delete_recursion(tree, tree->root);
    }
    free(tree->Elist);
    free(tree);
}; // удаление дерева

void input_file(Tree* tree, FILE** file){
    while((*file != NULL) && feof(*file) == 0){
        int key;
        char* info = (char*) calloc (81, sizeof(char));
        int e = fscanf(*file, "%d%s", &key, info);
        if(e == EOF){
            free(info);
        }else{
            Add(tree, key, info);
        }
    }
} // заполнение дерева из файла
void output_file_recursion(Tree* tree, Node* root, FILE** file){
    if(root != tree->Elist){
        if(root->info != NULL){
            Info* element = root->info;
            while(element != NULL){
                fprintf(*file, "%d\n", root->key);
                fprintf(*file, "%s\n", element->info);
                element = element->next;
            }
        }
        output_file_recursion(tree, root->left, file);
        output_file_recursion(tree, root->right, file);
    }
} // запись дерева в файл
void output_file(Tree* tree){
    if(tree->root != tree->Elist){
        FILE* file;
        file = fopen("file.txt", "w");
        output_file_recursion(tree, tree->root, &file);
        fclose(file);
    }
} // запись дерева в файл

int D_Timing(Tree*){
    Tree* tree = (Tree*) calloc (1, sizeof(Tree));
    tree->Elist = (Node *) calloc (1, sizeof(Node));
    tree->Elist->color = 0;
    tree->root = tree->Elist;
    int n = 0;
    int key;
    int cnt = 1000000;
    int i;
    double first, last;
    double time;
    while(n++ < 10) {
        time = 0;
        for (i = 0; i < cnt; i++) {
            key = rand();
            Add(tree, key, NULL);
            tree->root->color = 0;
        } ////
        first = clock();
        for (i = 0; i < 100000; ++i) {
            key = rand();
            //Add(tree, key, NULL);
            //tree->root->color = 0;
            Find_key(tree, key);
            //Delete(tree, key);
            //Find_Max(tree->root);
        }
        last = clock();
        time += (last - first) / CLOCKS_PER_SEC;
        //int k = 2;
        //Show(tree, tree->root, &k);
        printf("test #%d, number of nodes = %d, time = %.4f\n", n, n * cnt, time);
    }
    Delete_tree(tree);
    return 0;
} // таймирование
int dialog(const char *msgs[], int N){
    char *errmsg = "";
    int rc;
    int i, n;
    do{
        puts(errmsg);
        errmsg = "You are wrong. Repeate, please!";
        for(i = 0; i < N; ++i)
            puts(msgs[i]);
        puts("Make your choice: --> ");
        n = scan(&rc);
        if(n == 1)
            rc = 0;
    } while(rc < 0 || rc >= N);
    return rc;
} // диалоговая функция

