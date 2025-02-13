#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "/home/students/b/bikmukhametov.aa/lib/input.c"
#include "lab4.h"

void Draw_recursion(FILE* file, Node* root){
	if(root != NULL){
	Draw_recursion(file, root->left);
        if(root->info != NULL){
	    fprintf(file, "\"%s\" [shape=\"record\", label = \"%s|{", root->key, root->key);
            Info* element = root->info;
	    while(element != NULL){
		fprintf(file, "release: %d\\ninfo: %s", element->release, element->info);
		if(element->next != NULL){
		    fprintf(file, "|", element->release, element->info);
		}
                element = element->next;
            }
	    fprintf(file, "}\"]; \n");
	    if(root->parent != NULL){
		fprintf(file, "\"%s\" -- \"%s\"; \n", root->parent->key, root->key);
		fprintf(file, "\"%s\" [color=blue]; \n", root->key);
		fprintf(file, "\"%s\" [color=blue]; \n", root->parent->key);
	    }
        }
        Draw_recursion(file, root->right);
    }
}

void Draw(Tree *tree){
    FILE *file = fopen("result.dot", "w");
    fprintf(file, "graph G{\n");
    Draw_recursion(file, tree->root);
    fprintf(file, "}\n");
    fclose(file);
    system("dot -Tpng result.dot -o result.png");
}


int search(FILE* file, Tree* tree, Node* node, int *quantity){
        if(tree->root == NULL){
		return 1;
	}
	if(node != NULL){
		int count = 0;
        	Info* element = node->info;
        	while(element != NULL){
			count += 1;
                	element = element->next;
        	}
		*quantity += 1;
		fprintf(file, "%s - %d\n", node->key, count);
		count = 0;
        	search(file, tree, node->left, quantity);
        	search(file, tree, node->right, quantity);
	}
	return 0;
}

void correction(char** str){
    int len = strlen(*str);
    for (int i = 0; i < len; i++) {
        if (ispunct((*str)[i])) {
            for (int k = i; k < len; k++) {
                (*str)[k] = (*str)[k + 1];
            }
            len--;
            i--;
        }else if(((*str)[i] >= 'A') && ((*str)[i] <= 'Z')){
		(*str)[i] += 'a' - 'A';
	}
    }
}


int D_search(Tree*){
	Tree* tree = (Tree*) calloc (1, sizeof(Tree));
	tree->root = NULL;
	FILE* file;
	file = fopen("words.txt", "a+");
	while((file != NULL) && feof(file) == 0){
		int e;
		char* key = (char*) calloc (81, sizeof(char));
		e = fscanf(file, "%s", key);
		correction(&key);
		if(e == EOF){
			free(key);
		}else{
			int i = Add(tree, key, NULL);
			if(i == 2){
				free(key);
			}
		}
	}
	fclose(file);
	file = fopen("words_result.txt", "w");
	int quantity = 0;
	search(file, tree, tree->root, &quantity);
	fprintf(file, "\n");
	fprintf(file, "quantity - %d\n", quantity);
	fclose(file);
	Delete_tree(tree);
	printf("\nOK!\n");
	return 0;
}



int Add(Tree* tree, char* key, char *str){       //  функция добавления элемента
    Info* info = (Info *) calloc (1, sizeof(Info));
    Node* node = (Node *) calloc (1, sizeof(Node));
    node->info = info;
    node->info->info = str;
    node->info->release = 1;
    node->key = key;
    if(tree->root == NULL){
        tree->root = node;
        return 0;
    }
    Node* node_release = Find_key(tree->root, key);
    if(node_release != NULL){
        Info* info_release = node_release->info;
        while(info_release->next != NULL){
            info_release = info_release->next;
        }
        info_release->next = info;
        info->release = info_release->release + 1;
        node->info = info;
        free(node);
        return 2;
    }

    Node* ptr = tree->root;
    Node* par = NULL;
    while(ptr != NULL){
        par = ptr;
        if(strcmp(key, ptr->key) < 0){
            ptr = ptr->left;
        }else if(strcmp(key, ptr->key) > 0){
            ptr = ptr->right;
        }
    }
    if((strcmp(key, par->key)) < 0){
        par->left = node;
    }else{
        par->right = node;
    }
    node->parent = par;
    return 0;
} // добавление элемента
int D_Add(Tree* tree){
    char* key = NULL;
    char* info = NULL;
    printf("Enter key: ");
    int i = my_readline(&key);
    if(i == 1) {
        free(key);
        return 1;
    }
    printf("Enter info: ");
    i = my_readline(&info);
    if (i == 1) {
        free(info);
        free(key);
        return 1;
    }
    if((info[0] == '\0') || (key[0] == '\0')){
        printf("Ошибка! Пустое значение\n");
        free(key);
        free(info);
        return 0;
    }else{
        i = Add(tree, key, info);
    }
    if(i == 2){
        free(key);
    }
    return 0;
} // добавление элемента

void Show(Node* root, int* n){
    if(root != NULL){
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
		if((element != NULL) && (root != NULL)){
	                printf("> {%s;%d;%s}", root->key, element->release, element->info);
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
        Show(root->left, n);
        Show(root->right, n);
        *n -= 14;
    }
} // полный обход
int D_Show(Tree* tree){
    int n = 2;
    Show(tree->root, &n);
    return 0;
} // полный обход
int D_Bypass(Tree* tree){
    if(tree->root != NULL){
        char* x = NULL;
        char* y = NULL;
        printf("interval:\n");
        printf("--->");
        int i = my_readline(&x);
        if(i == 1){
            return 1;
        }
        printf("--->");
        i = my_readline(&y);
        if(i == 1){
            free(x);
            return 1;
        }
        printf("[ %s ; %s ]\n", x, y);
        Node* root = Find_min(tree->root);
        while(root != NULL){
            if((strcmp(root->key, x) < 0) || (strcmp(root->key, y) > 0)){
                Info* element = root->info;
                while(element != NULL){
                    printf("%s    ", root->key);
                    printf("%d    ", element->release);
                    printf("%s    ", element->info);
                    printf("\n");
                    element = element->next;
                }
            }
            root = Find_next(root);
        }
        free(x);
        free(y);
    }
    return 0;
} // обход не входящий в интервал

int Delete(Tree* tree, char* key){
    Node* root = tree->root;
    Node* x = Find_key(root, key);

    if(x == NULL){
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
    if((x->right == NULL) || (x->left == NULL)) {
        y = x;
    }else{
        y = Find_next(x);
    }

    Node *p;
    if(y->left != NULL){
        p = y->left;
    }else{
        p = y->right;
    }
    Node *par = y->parent;
    if(p != NULL){
        p->parent = par;
    }
    if(par == NULL){
        tree->root = p;
    }else if(par->left == y){
        par->left = p;
    }else{
        par->right = p;
    }

    if(y != x){ //переносим, затем удаляем нижний
        free(x->key); x->key = NULL;
        free(x->info->info); x->info->info = NULL;
        free(x->info);
        x->info = y->info;
        x->key = y->key;
        free(y);

    }else{ //весь элемент удаляем
        free(y->key);
        free(y->info->info);
        free(y->info);
        free(y);
    }
    return 0;
} // удаление элемента
int D_Delete(Tree* tree){
    if(tree->root == NULL){
        printf("Not tree!\n");
        return 0;
    }
    printf("Enter the key: ");
    char* key = NULL;
    int i = my_readline(&key);
    if(i == 1){
        free(key);
        return 0;
    }
    Node* node = Find_key(tree->root, key);
    if(node == NULL){
        free(key);
        printf("Not key!\n");
        return 0;
    }
    i = Delete(tree, key);
    if(i == 1){
        printf("Not key!\n");
    }else{
        printf("Ok!\n");
    }
    free(key);
    return 0;
} // удаление элемента

Node* Find_key(Node* root, char* key){
    if(root != NULL){
        Node* ptr = root;
        while(ptr != NULL){
            if((strcmp(key, ptr->key)) == 0){
                return ptr;
            }else if((strcmp(key, ptr->key)) > 0){
                ptr = ptr->right;
            }else if((strcmp(key, ptr->key)) < 0){
                ptr = ptr->left;
            }
        }
    }
    return NULL;
} // поиск по ключу в дереве

Node* Find_min(Node* root){
    if(root == NULL){
        return NULL;
    }else{
        while(root->left != NULL){
            root = root->left;
        }
        return root;
    }
} // поиск минимального в поддереве

Node* Find_next(Node* x){
    Node* y;
    if(x->right){
        y = Find_min(x->right);
        return y;
    }else{
        Node* ptr = x->parent;
        while((ptr != NULL) && (ptr->right == x)){
            x = ptr;
            ptr = x->parent;
        }
        return ptr;
    }
} // поиск следующего в дереве

int D_Find_Key(Tree* tree){
    printf("Enter the key: ");
    char* key = NULL;
    int i = my_readline(&key);
    printf("\n");
    if(i == 1){
        free(key);
        return 1;
    }
    Node* node = Find_key(tree->root, key);
    if(node == NULL){
        printf("Not key!\n");
        return 0;
    }
    Info* element = node->info;
    while(element != NULL){
        printf("%s    ", node->key);
        printf("%d    ", element->release);
        printf("%s    ", element->info);
        printf("\n");
        element = element->next;
    }
    free(key);
    return 0;
} // поиск по ключу в дереве

Node* Find_Max(Node* root){
    if(root == NULL){
        return NULL;
    }else{
        while(root->right != NULL){
            root = root->right;
        }
        return root;
    }
} // поиск максимального в поддереве

int D_Find_Max(Tree* tree){
    if(tree->root != NULL){
        Node* root = Find_Max(tree->root);
        if(root->key != NULL){
            Info* element = root->info;
            while(element != NULL){
                printf("%s    ", root->key);
                printf("%d    ", element->release);
                printf("%s    ", element->info);
                printf("\n");
                element = element->next;
            }
        }
    }
    return 0;
} // поиск максимального в поддереве

void Delete_recursion(Node* root){
    if(root != NULL){
        Delete_recursion(root->left);
        Delete_recursion(root->right);
        if(root->info != NULL){
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
            if(root->key != NULL) {
                free(root->key);
                root->key = NULL;
            }
            free(root);
        }
    }
} // удаление дерева

void Delete_tree(Tree* tree){
    if(tree->root != NULL) {
        Delete_recursion(tree->root);
    }
    free(tree);
}; // удаление дерева

void input_file(Tree* tree, FILE** file){
    while((*file != NULL) && feof(*file) == 0){
        int e;
        char* key = (char*) calloc (81, sizeof(char));
        char* info = (char*) calloc (81, sizeof(char));
        e = fscanf(*file, "%s%s", key, info);
        if(e == EOF){
            free(key);
            free(info);

        }else{
            int i = Add(tree, key, info);
            if(i == 2){
                free(key);
            }
        }
    }
} // заполнение дерева из файла
void output_file_recursion(Node* root, FILE** file){
    if(root != NULL){
        if(root->info != NULL){
            Info* element = root->info;
            while(element != NULL){
                fprintf(*file, "%s\n", root->key);
                fprintf(*file, "%s\n", element->info);
                element = element->next;
            }
        }
        output_file_recursion(root->left, file);
        output_file_recursion(root->right, file);
    }
} // запись дерева в файл
void output_file(Tree* tree){
    if(tree->root != NULL){
        FILE* file;
        file = fopen("file.txt", "w");
        output_file_recursion(tree->root, &file);
        fclose(file);
    }
} // запись дерева в файл

int D_Timing(Tree*){
    Tree* tree = (Tree*) calloc (1, sizeof(Tree));
    int n = 10;
    char* key = (char *) calloc (11, sizeof(char));
    int cnt = 1000;
    int i;
    clock_t first, last;
    double time = 0.0;
    while(n-- > 0) {
        for (i = 0; i < 10; i++) {
            key[i] = 'a' + rand() % 26;
        }
        key[10] = '\0';
        for (i = 0; i < cnt; i++) {
            Add(tree, key, NULL);
        }
        for (int t = 0; t < 10; t++) {
            first = clock();
            for (i = 0; i < 10; ++i) {
                //Add(tree, key, NULL);
                //Find_key(tree->root, key);
                Delete(tree, key);
            }
            last = clock();
            time += (double)(last - first) / CLOCKS_PER_SEC;
        }
        time /= 10;
        printf("test #%d, number of nodes = %d, time = %f\n", 10 - n, (10 - n) * cnt, time);
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

