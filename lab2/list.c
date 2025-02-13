#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Item {
    int data;
    struct Item* next;
} Item;

typedef struct Stack {
    Item* head;
} Stack;

void m_push(Stack* stack, int i){
    Item* elem = (Item *) calloc(1, sizeof(Item));
    elem->data = i;
    elem->next = stack->head;
    stack->head = elem;
}

void m_pop(Stack* stack, int j){
    if(j == 1){
        Item* soh_1 = stack->head;
        Item* soh_2 = stack->head->next;
        soh_2->data = soh_2->data + soh_1->data;
        stack->head = soh_2;
        free(soh_1);
    } else if(j == 2){
        Item* soh_1 = stack->head;
        Item* soh_2 = stack->head->next;
        soh_2->data = soh_2->data - soh_1->data;
        stack->head = soh_2;
        free(soh_1);
    } else if(j == 3){
        Item* soh_1 = stack->head;
        Item* soh_2 = stack->head->next;
        soh_2->data = soh_2->data * soh_1->data;
        stack->head = soh_2;
        free(soh_1);
    } else if(j == 4) {
	if(stack->head->data != 0){
	        Item* soh_1 = stack->head;
        	Item* soh_2 = stack->head->next;
        	soh_2->data = soh_2->data / soh_1->data;
        	stack->head = soh_2;
        	free(soh_1);
	}
    }
}

void delet(Stack* stack){
    Item *ptr = stack->head;
    Item *ptr_prev = NULL;
    while (ptr != NULL) {
        ptr_prev = ptr;
        ptr = ptr->next;
        free(ptr_prev);
    }
    //free(stack);
}

void input(Stack* stack){
	stack->head = NULL;
}

void print_result(Stack* stack){
    if(stack->head != NULL){
        printf("The result of an algebraic expression  --> %d\n", stack->head->data);
    }
}

char *in_str(){
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n;
    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            return NULL;
        } else if (n > 0) {
            int chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        } else {
            scanf("%*c"); }
    } while (n > 0);
    if (len > 0) {
        res[len] = '\0';
    } else {
        res = calloc(1, sizeof(char));
    }
    for(int i = 0; i < strlen(res); i++){
        if(res[i] == EOF){
            free(res);
            return NULL;
        }
    }
    return res;
}

void func(Stack* stack, char** stroka){
    int res = 0;
    int sign = 0;
    int number = 0;
    int check = 0;
    for(int i = 0; i < strlen(*stroka); i++){
        int j = 0;
        if(((*stroka)[i] == ' ') || ((*stroka)[i] == '\t')){
            m_push(stack, res);
            res = 0;
        } else{
            if((*stroka)[i] == '+'){
                sign++;
                if(number <= sign){
                    i = strlen(*stroka);
                    check = 1;
                } else {
                    j = 1;
                    m_pop(stack, j);
                    i++;
                }
            } else if((*stroka)[i] == '-'){
                sign++;
                if(number <= sign){
                    i = strlen(*stroka);
                    check = 1;
                } else {
                    j = 2;
                    m_pop(stack, j);
                    i++;
                }
            } else if((*stroka)[i] == '*'){
                sign++;
                if(number <= sign){
                    i = strlen(*stroka);
                    check = 1;
                } else {
                    j = 3;
                    m_pop(stack, j);
                    i++;
                }
            } else if((*stroka)[i] == '/'){
		if((*stroka)[i - 2] != '0'){
                        sign++;
                }
                if(number <= sign){
                    i = strlen(*stroka);
                    check = 1;
                } else {
                    j = 4;
                    m_pop(stack, j);
                    i++;
                }
            } else if(((*stroka)[i] <= '9') && ((*stroka)[i] >= '0')){
                number++;
                int k = (*stroka)[i] - '0';
                res = (10 * res) + k;
                if(res != k){
                    number--;
                }
            } else{
                check = 1;
                i = strlen(*stroka);
            }
        }
    }
    if((number == sign + 1) && (check == 0) && (sign > 0)){
        print_result(stack);
    } else{
        printf("error!\n");
    }
}
