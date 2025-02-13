#ifndef GRAPH_FUCTION_H
#define GRAPH_FUCTION_H

typedef struct Vertex {
    int edge_count;
    int* edges;
    char* id;
    int type;
} Vertex;

typedef struct Graph {
    int vertex_count;
    Vertex* vertices;
} Graph;

int Func_add_vertex(Graph* graph, char* id, int type);
int Func_add_edge(Graph* graph, char* vertex_first, char* vertex_second);
int Func_delete_vertex(Graph* graph, char* id);
int Func_delete_edge(Graph* graph, char* vertex_first, char* vertex_second);
int Func_change_vertex(Graph* graph, char* id, int type);

void DFS_visit(Graph* graph, int index_entrance, int** color);
int Func_depth_first_search(Graph* graph, char* id, int** color);

int Extract_min(int** color, int* distance, int* count);
void print_path(Graph* graph, int index_first, int index_second,int* pred);
int Func_Dijkstra(Graph* graph, char* first, char* second);

void print_distance_pred(int** distance,int** pred, int count);
int find_min(Graph* graph, int** distance, int index, int count);
int Func_Floyd_Warshall(Graph* graph, char* vertex);
void delete_graph(Graph* graph);

int Draw(Graph *graph);
void input_file(Graph* graph, FILE** file);
void output_file(Graph* graph);

#endif
