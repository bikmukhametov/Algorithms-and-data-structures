#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "graph_dialog.h"
#include "graph_function.h"
#include "/home/students/b/bikmukhametov.aa/lib/input.c"

int D_add_vertex(Graph* graph){
    int type = 0;
    char* id = NULL;
    printf("Enter id: ");
    int i = my_readline(&id);
    if(i == 1) {
        free(id);
        return 1;
    }
    while((type != 1) && (type != 2) && (type != 3)){
        printf("\n");
        printf("1. entrance\n");    //вход
        printf("2. exit\n");        //выход
        printf("3. transition\n\n");  //переход
        printf("Enter type: ");
        i = scan(&type);
        if (i == 1) {
            free(id);
            return 1;
        }
        if((type != 1) && (type != 2) && (type != 3)){
            printf("\n-- error!\n");
        }
    }
    i = Func_add_vertex(graph, id, type);
    if(i == 1){
        printf("\n-- error!\n");
    }else if(i == 2){
        printf("\n-- the ID is repeated!\n");
    }
    return 0;
}

int D_add_edge(Graph* graph){
    char* vertex_first = NULL;
    char* vertex_second = NULL;
    printf("Enter the first vertex: ");
    int i = my_readline(&vertex_first);
    if(i == 1) {
        free(vertex_first);
        return 1;
    }
    printf("Enter the second vertex: ");
    i = my_readline(&vertex_second);
    if(i == 1) {
        free(vertex_first);
        free(vertex_second);
        return 1;
    }
    i = Func_add_edge(graph, vertex_first, vertex_second);
    if(i == 1){
        printf("\n-- error!\n");
    }else if(i == 2){
        printf("\n-- value error!\n");
    }
    free(vertex_first);
    free(vertex_second);
    return 0;
}

int D_delete_vertex(Graph* graph){
    char* id = NULL;
    printf("Enter id: ");
    int i = my_readline(&id);
    if(i == 1) {
        free(id);
        return 1;
    }

    i = Func_delete_vertex(graph, id);
    if(i == 1){
        printf("\n-- error!\n");
    }else if(i == 2){
        printf("\n-- vertex not found!\n");
    }
    free(id);
    return 0;
}

int D_delete_edge(Graph* graph){
    char* vertex_first = NULL;
    char* vertex_second = NULL;
    printf("Enter the first vertex: ");
    int i = my_readline(&vertex_first);
    if(i == 1) {
        free(vertex_first);
        return 1;
    }
    printf("Enter the second vertex: ");
    i = my_readline(&vertex_second);
    if(i == 1) {
        free(vertex_first);
        free(vertex_second);
        return 1;
    }
    i = Func_delete_edge(graph, vertex_first, vertex_second);
    if(i == 1){
        printf("\n-- error!\n");
    }else if(i == 2){
        printf("\n-- edge/vertex not found!\n");
    }
    free(vertex_first);
    free(vertex_second);
    return 0;
}

int D_change_vertex(Graph* graph){
    char* id = NULL;
    printf("Enter id: ");
    int i = my_readline(&id);
    if(i == 1) {
        free(id);
        return 1;
    }
    int type = 0;
    while((type != 1) && (type != 2) && (type != 3)){
        printf("\n");
        printf("1. entrance\n");      //вход
        printf("2. exit\n");          //выход
        printf("3. transition\n\n");  //переход
        printf("Enter type: ");
        i = scan(&type);
        if (i == 1) {
            free(id);
            return 1;
        }
        if((type != 1) && (type != 2) && (type != 3)){
            printf("\n-- error!\n");
        }
    }
    i = Func_change_vertex(graph, id, type);
    if(i == 1){
        printf("\n-- error!\n");
    }else if(i == 2){
        printf("\n-- vertex not found!\n");
    }
    free(id);
    return 0;
}

int D_depth_first_search(Graph* graph){
    char* id = NULL;
    printf("enter the id entrance: ");
    int i = my_readline(&id);
    if(i == 1) {
        free(id);
        return 1;
    }
    int* color = calloc(graph->vertex_count, sizeof(int));
    i = Func_depth_first_search(graph, id, &color);
    int k = 0;
    for(int j = 0; j < graph->vertex_count; j++){
        if((color[j] == 2) && (graph->vertices[j].type == 3)){
            k = 1;
	    if(graph->vertices[j].type == 1){
                printf("\n{%s; entrance}\n", graph->vertices[j].id);
            }else if(graph->vertices[j].type == 2){
                printf("\n{%s; transition}\n", graph->vertices[j].id);
            }else if(graph->vertices[j].type == 3){
                printf("\n{%s; exit}\n", graph->vertices[j].id);
            }
        }
    }
    if(k == 0){
        printf("\n-- the top is unattainable!\n");
    }
    if(i == 1){
        printf("\n-- error!\n");
    }else if(i == 2){
        printf("\n-- value error!\n");
    }
    free(id);
    free(color);
    return 0;
}

int D_Dijkstra(Graph* graph){
    char* vertex_first = NULL;
    char* vertex_second = NULL;
    printf("Enter the id entrance: ");
    int i = my_readline(&vertex_first);
    if(i == 1) {
        free(vertex_first);
        return 1;
    }
    printf("Enter the id exit: ");
    i = my_readline(&vertex_second);
    if(i == 1) {
        free(vertex_first);
        free(vertex_second);
        return 1;
    }
    printf("\n");
    i = Func_Dijkstra(graph, vertex_first, vertex_second);
    if(i == 1){
        printf("\n-- error!\n");
    }else if(i == 2){
        printf("\n-- value error!\n");
    }
    free(vertex_first);
    free(vertex_second);
    return 0;
}

int D_Floyd_Warshall(Graph* graph){
    char* entrance = NULL;
    printf("Enter the id entrance: ");
    int i = my_readline(&entrance);
    if(i == 1) {
        free(entrance);
        return 1;
    }
    printf("\n");
    i = Func_Floyd_Warshall(graph, entrance);
    if(i == 1){
        printf("\n-- error!\n");
    }else if(i == 2){
        printf("\n-- value error!\n");
    }else if(i == 3){
        printf("\n-- path not found\n");
    }
    free(entrance);
    return 0;
}

int D_Show(Graph* graph){
    if((graph != NULL) && (graph->vertices != NULL)){
        printf("\n");
        for(int i = 0; i < graph->vertex_count; i++){
            if(graph->vertices[i].type == 1){
                printf("{%s; entrance} ", graph->vertices[i].id);
            }else if(graph->vertices[i].type == 2){
                printf("{%s; transition} ", graph->vertices[i].id);
            }else if(graph->vertices[i].type == 3){
                printf("{%s; exit} ", graph->vertices[i].id);
            }
            for(int j = 0; j < graph->vertices[i].edge_count; j++){
                int index = graph->vertices[i].edges[j];
                printf("---> { %s } ", graph->vertices[index].id);
            }
            printf("\n");
        }
    }
    return 0;
}

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
}
