#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Stack{
    int n;
    int *array;
}Stack;

void m_push(Stack* stack, int i){
    stack->n += 1;
    stack->array = (int *) realloc (stack->array, stack->n * sizeof(int));
    for(int j = stack->n - 1; j > 0; j --){
        stack->array[j] = stack->array[j - 1];
    }
    stack->array[0] = i;
}

void m_pop(Stack* stack, int j){
    if(j == 1){
        stack->array[0] = stack->array[1] + stack->array[0];
        for(int i = 1; i < stack->n - 1; i++){
            stack->array[i] = stack->array[i + 1];
        }
        stack->n--;
        stack->array = (int *) realloc(stack->array, stack->n * sizeof(int));
    } else if(j == 2){
        stack->array[0] = stack->array[1] - stack->array[0];
        for(int i = 1; i < stack->n - 1; i++){
            stack->array[i] = stack->array[i + 1];
        }
        stack->n--;
        stack->array = (int *) realloc(stack->array, stack->n * sizeof(int));
    } else if(j == 3){
        stack->array[0] = stack->array[1] * stack->array[0];
        for(int i = 1; i < stack->n - 1; i++){
            stack->array[i] = stack->array[i + 1];
        }
        stack->n--;
        stack->array = (int *) realloc(stack->array, stack->n * sizeof(int));
    } else if(j == 4) {
	if(stack->array[0] != 0){
		stack->array[0] = stack->array[1] / stack->array[0];
        		for(int i = 1; i < stack->n - 1; i++){
        			stack->array[i] = stack->array[i + 1];
        		}
        	stack->n--;
        	stack->array = (int *) realloc(stack->array, stack->n * sizeof(int));
	}
    }
}

void delet(Stack* stack){
    free(stack->array);
    stack->array = NULL;
    stack->n = 0;
}

void print_result(Stack* stack){
    if(stack->n != 0){
        printf("The result of an algebraic expression  --> %d\n", stack->array[0]);
    }
}

void input(Stack* stack){
    stack->array = (int *) calloc(1, sizeof(int));
    stack->n = 0;
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
