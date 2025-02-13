#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "graph_function.h"

enum error{
    OK = 0,
    ERROR = 1,
    VALUE_ERROR = 2,
}error;

enum room{
    ENTRANCE = 1,
    TRANSITION = 2,
    EXIT = 3,
}room;


int Draw(Graph *graph){
    FILE *file = fopen("result.dot", "w");
    fprintf(file, "graph G{\n");
    for (int i = 0; i < graph->vertex_count; i++){
	if(graph->vertices[i].type == ENTRANCE){
	    fprintf(file, "%s [color=green]; \n", graph->vertices[i].id);
	}else if(graph->vertices[i].type == TRANSITION){
	    fprintf(file, "%s [color=orange]; \n", graph->vertices[i].id);
	}else if(graph->vertices[i].type == EXIT){
	    fprintf(file, "%s [color=red]; \n", graph->vertices[i].id);
	}
	if(graph->vertices[i].edge_count == 0){
            fprintf(file, "%s; \n", graph->vertices[i].id);
        }
        for(int j = 0; j < graph->vertices[i].edge_count; j++){
            int index = graph->vertices[i].edges[j];
            if(index > i){
                fprintf(file, "%s -- %s; \n", graph->vertices[i].id, graph->vertices[index].id);
            }
        }
    }
    fprintf(file, "}\n");
    fclose(file);
    system("dot -Tpng result.dot -o result.png");
}

int Func_add_vertex(Graph* graph, char* id, int type){
    if(graph == NULL){
	free(id);
	return ERROR;
    }
    int count = graph->vertex_count;
    if(count != 0){
        for(int i = 0; i < count; i++){
            if(strcmp(graph->vertices[i].id, id) == 0){
                free(id);
                return VALUE_ERROR;
            }
        }
    }
    graph->vertex_count += 1;
    graph->vertices = (Vertex *) realloc (graph->vertices, graph->vertex_count * sizeof(Vertex));
    graph->vertices[count].id = id;
    graph->vertices[count].type = type;
    graph->vertices[count].edge_count = 0;
    graph->vertices[count].edges = NULL;
    return OK;
}


int Func_add_edge(Graph* graph, char* vertex_first, char* vertex_second){
    if((graph == NULL) || (graph->vertices == NULL)){
	return ERROR;
    }
    int index_first = INT_MAX;
    int index_second = INT_MAX;
    for(int i = 0; i < graph->vertex_count; i++){
        if(strcmp(vertex_first, graph->vertices[i].id) == 0){
            index_first = i;
        }else if(strcmp(vertex_second, graph->vertices[i].id) == 0){
            index_second = i;
        }
    }
    if((index_first == INT_MAX) || (index_second == INT_MAX)){
        return VALUE_ERROR;
    }
    for(int i = 0; i < graph->vertices[index_first].edge_count; i++){
        if(graph->vertices[index_first].edges[i] == index_second){
            return ERROR;
        }
    }

    int edge_count = graph->vertices[index_first].edge_count;
    graph->vertices[index_first].edges = (int*) realloc (graph->vertices[index_first].edges, (edge_count+1) * sizeof(int));
    graph->vertices[index_first].edges[edge_count] = index_second;
    graph->vertices[index_first].edge_count += 1;

    edge_count = graph->vertices[index_second].edge_count;
    graph->vertices[index_second].edges = (int*) realloc (graph->vertices[index_second].edges, (edge_count+1) * sizeof(int));
    graph->vertices[index_second].edges[edge_count] = index_first;
    graph->vertices[index_second].edge_count += 1;

    return OK;
}


int Func_delete_vertex(Graph* graph, char* id){
    if((graph == NULL) || (graph->vertices == NULL)){
	return ERROR;
    }
    int index = INT_MAX;
    for(int i = 0; i < graph->vertex_count; i++){
        if(strcmp(id, graph->vertices[i].id) == 0){
            index = i;
        }
    }
    if(index == INT_MAX){
        return VALUE_ERROR;
    }
    free(graph->vertices[index].edges);
    free(graph->vertices[index].id);
    graph->vertex_count -= 1;
    for(int i = index; i < graph->vertex_count; i++){
        graph->vertices[i] = graph->vertices[i + 1];
     }
     graph->vertices = (Vertex *) realloc(graph->vertices, graph->vertex_count * sizeof(Vertex));

    int count = 0;
    for(int i = 0; i < graph->vertex_count; i++){
        for(int j = 0; j < graph->vertices[i].edge_count; j++){
            if(graph->vertices[i].edges[j] == index){
                graph->vertices[i].edge_count -= 1;
                for(int k = j; k < graph->vertices[i].edge_count; k++){
                    graph->vertices[i].edges[k] = graph->vertices[i].edges[k + 1];
                }
                count = graph->vertices[i].edge_count;
                graph->vertices[i].edges = (int *) realloc(graph->vertices[i].edges, count * sizeof(int));
            }
            if(j < count){
                if(graph->vertices[i].edges[j] > index){
                    graph->vertices[i].edges[j] -= 1;
                }
            }
        }
    }
    return OK;
}


int Func_delete_edge(Graph* graph, char* vertex_first, char* vertex_second){
    if((graph != NULL) && (graph->vertices != NULL)){
        int index_first = INT_MAX;
        int index_second = INT_MAX;
        for(int i = 0; i < graph->vertex_count; i++){
            if(strcmp(vertex_first, graph->vertices[i].id) == 0){
                index_first = i;
            }else if(strcmp(vertex_second, graph->vertices[i].id) == 0){
                index_second = i;
            }
        }
        if((index_first == INT_MAX) || (index_second == INT_MAX)){
            return 2;
        }

        int edge_count = 0;
        for(int i = 0; i < graph->vertices[index_first].edge_count; i++){
            if(graph->vertices[index_first].edges[i] == index_second){
                edge_count ++;
            }
        }
        if(edge_count != 1){
            return 2;
        }

        for(int j = 0; j < graph->vertices[index_first].edge_count; j++){
            if(graph->vertices[index_first].edges[j] == index_second){
                graph->vertices[index_first].edge_count -= 1;
                for(int k = j; k < graph->vertices[index_first].edge_count; k++){
                    graph->vertices[index_first].edges[k] = graph->vertices[index_first].edges[k + 1];
                }
                int count = graph->vertices[index_first].edge_count;
                graph->vertices[index_first].edges = (int *) realloc(graph->vertices[index_first].edges, count * sizeof(int)) ;
            }
        }

        for(int j = 0; j < graph->vertices[index_second].edge_count; j++){
            if(graph->vertices[index_second].edges[j] == index_first){
                graph->vertices[index_second].edge_count -= 1;
                for(int k = j; k < graph->vertices[index_second].edge_count; k++){
                    graph->vertices[index_second].edges[k] = graph->vertices[index_second].edges[k + 1];
                }
                int count = graph->vertices[index_second].edge_count;
                graph->vertices[index_second].edges = (int *) realloc(graph->vertices[index_second].edges, count * sizeof(int)) ;
            }
        }
        return 0;
    }else{
        return 1;
    }
}


int Func_change_vertex(Graph* graph, char* id, int type){
    if((graph == NULL) || (graph->vertices == NULL)){
	return ERROR;
    }
    for(int i = 0; i < graph->vertex_count; i++){
        if(strcmp(id, graph->vertices[i].id) == 0){
            graph->vertices[i].type = type;
            return OK;
        }
    }
    return VALUE_ERROR;
}



void DFS_visit(Graph* graph, int index_entrance, int** color){
    (*color)[index_entrance] = 1;
    for(int j = 0; j < graph->vertices[index_entrance].edge_count; j++){
        int index = graph->vertices[index_entrance].edges[j];
        if((*color)[index] == 0){
            DFS_visit(graph, index, color);
        }
    }
    (*color)[index_entrance] = 2;
}
int Func_depth_first_search(Graph* graph, char* id, int** color){
    if((graph == NULL) || (graph->vertices == NULL)){
	return ERROR;
    }
    int index_entrance = INT_MAX;
    for(int i = 0; i < graph->vertex_count; i++){
        if(strcmp(id, graph->vertices[i].id) == 0){
            index_entrance = i;
            if(graph->vertices[i].type != ENTRANCE){
                return VALUE_ERROR;
            }
        }
    }
    if(index_entrance == INT_MAX){
        return VALUE_ERROR;
    }

    DFS_visit(graph, index_entrance, color);
    return OK;
}



int Extract_min(int** color, int* distance, int* count){
    int min_dist = INT_MAX;
    int index = INT_MAX;
    for(int i = 0; i < *count; i++){
        if(((*color)[i] == 0) && (distance[i] < min_dist)){
            min_dist = distance[i];
            index = i;
        }
    }
    if(index != INT_MAX){
        (*color)[index] = 1;
        return index;
    }else{
        return INT_MAX;
    }
}
void print_path(Graph* graph, int index_first, int index_second,int* pred){
    if(index_first == index_second){
        printf("---> { %s } ", graph->vertices[index_second].id);
    }else{
        if(pred[index_second] == INT_MAX){
            printf("\n-- there is no way\n");
        }else{
            print_path(graph, index_first, pred[index_second], pred);
            printf("---> { %s } ", graph->vertices[index_second].id);
        }
    }
}
int Func_Dijkstra(Graph* graph, char* first, char* second){
    if((graph != NULL) && (graph->vertices != NULL)){
        int* distance = calloc(graph->vertex_count, sizeof(int));
        int* pred = calloc(graph->vertex_count, sizeof(int));
        int* color = calloc(graph->vertex_count, sizeof(int));
        for(int i = 0; i < graph->vertex_count; i++){
            distance[i] = INT_MAX - 1;
            pred[i] = INT_MAX;
            color[i] = 0;
        }
        int index_first = INT_MAX;
        int index_second = INT_MAX;
        for(int i = 0; i < graph->vertex_count; i++){
            if(strcmp(first, graph->vertices[i].id) == 0){
                index_first = i;
            }else if(strcmp(second, graph->vertices[i].id) == 0){
                index_second = i;
            }
        }
        if((index_first == INT_MAX) || (index_second == INT_MAX)
           || (graph->vertices[index_first].type != 1)
           || (graph->vertices[index_second].type != 3)){
            free(distance);
            free(pred);
            free(color);
            return 2;
        }

        distance[index_first] = 0;
        int count = graph->vertex_count;
        int min = INT_MAX;
        for(int j = 0; j < count; j++){
            min = Extract_min(&color, distance, &count);
            if(min != INT_MAX) {
                for (int i = 0; i < graph->vertices[min].edge_count; i++) {
                    int index = graph->vertices[min].edges[i];
                    if (distance[index] > distance[min] + 1) {
                        distance[index] = distance[min] + 1;
                        pred[index] = min;
                    }
                }
            }
        }
        print_path(graph, index_first, index_second, pred);
        free(distance);
        free(pred);
        free(color);
        printf("\n");
        return 0;
    }else{
        return 1;
    }
}


void print_distance_pred(int** distance,int** pred, int count){
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            if (distance[i][j] == INT_MAX)
                printf("INF\t");
            else
                printf("%d\t", distance[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            if (pred[i][j] == INT_MAX)
                printf("INF\t");
            else
                printf("%d\t", pred[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}
int find_min(Graph* graph, int** distance, int index, int count){
    int index_min = INT_MAX;
    int min = INT_MAX;
    for(int i = 0; i < count; i++){
        if((distance[index][i] < min) && (distance[index][i] != 0) && (graph->vertices[i].type == 3)){
            min = distance[index][i];
            index_min = i;
        }
    }
    return index_min;
}
int Func_Floyd_Warshall(Graph* graph, char* vertex){
    if((graph == NULL) || (graph->vertices == NULL)){
	return ERROR;
    }
    int count = graph->vertex_count;
    int index = INT_MAX;
    int **distance = (int **) calloc(count, sizeof(int*));
    int **pred = (int **) calloc(count, sizeof(int*));
    for(int i = 0; i < count; i++){
        pred[i] = (int*) calloc(count, sizeof(int));
        distance[i] = (int*) calloc(count, sizeof(int));
        if(strcmp(vertex, graph->vertices[i].id) == 0) {
            index = i;
        }
        for(int j = 0; j < count; j++){
            pred[i][j] = INT_MAX;
            if(i == j){
                distance[i][j] = 0;
            }else{
                distance[i][j] = INT_MAX;
            }
        }
        for(int j = 0; j < graph->vertices[i].edge_count; j++){
            int index_vertex = graph->vertices[i].edges[j];
            distance[i][index_vertex] = 1;
            pred[i][index_vertex] = i;
        }
    }
    int index_exit = INT_MAX;
    if((index != INT_MAX) && (graph->vertices[index].type == 1)){
        for(int i = 0; i < count; i++){
            for(int j = 0; j < count; j++){
                for(int k = 0; k < count; k++){
                    if((distance[j][i] != INT_MAX) && (distance[i][k] != INT_MAX)){
                        if (distance[j][k] > distance[j][i] + distance[i][k]) {
                            distance[j][k] = distance[j][i] + distance[i][k];
                            pred[j][k] = i;
                        }
                    }
                }
            }
        }
        index_exit = find_min(graph, distance, index, count);
        if(index_exit != INT_MAX) {
            print_distance_pred(distance, pred, count);
            print_path(graph, index, index_exit, pred[index]);
            printf("\ndistance: %d\n", distance[index][index_exit]);
        }
    }

    for(int i = 0; i < count; i++){
        free(pred[i]);
        free(distance[i]);
    }
    free(distance);
    free(pred);
    if((index == INT_MAX) || (graph->vertices[index].type != 1)){
        return VALUE_ERROR;
    }else if(index_exit == INT_MAX){
        return 3;
    }
    return OK;
}


void delete_graph(Graph* graph){
    for(int i = 0; i < graph->vertex_count; i++){
        free(graph->vertices[i].edges);
        free(graph->vertices[i].id);
    }
    free(graph->vertices);
    free(graph);
}

void input_file(Graph* graph, FILE** file){
    if((*file != NULL) && feof(*file) == 0){
        int count = 0;
        int type = 0;
        int e = fscanf(*file, "%d", &count);
        if (e != EOF) {
            graph->vertex_count = count;
            graph->vertices = (Vertex *) calloc(count, sizeof(Vertex));
        }
        for(int k = 0; k < count; k++){
            int count_vertex = 0;
            char* id = (char*) calloc (81, sizeof(char));
            fscanf(*file, "%d %d %s", &count_vertex, &type, id);
            graph->vertices[k].edges = calloc(count_vertex, sizeof(int));
            graph->vertices[k].edge_count = count_vertex;
            graph->vertices[k].type = type;
            graph->vertices[k].id = id;
            int vertex = 0;
            for (int i = 0; i < count_vertex; i++) {
                fscanf(*file, "%d", &vertex);
                graph->vertices[k].edges[i] = vertex;
            }
        }
    }
}
void output_file(Graph* graph){
    if((graph != NULL) && (graph->vertices != NULL)){
        FILE* file;
        file = fopen("file.txt", "w");
        fprintf(file, "%d\n", graph->vertex_count);
        for(int i = 0; i < graph->vertex_count; i++){
            fprintf(file, "%d %d %s\n", graph->vertices[i].edge_count, graph->vertices[i].type, graph->vertices[i].id);
            for(int j = 0; j < graph->vertices[i].edge_count; j++){
                fprintf(file, "%d ", graph->vertices[i].edges[j]);
            }
            fprintf(file, "\n");
        }
        fclose(file);
    }
}
