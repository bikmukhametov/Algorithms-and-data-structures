#ifndef GRAPH_DIALOG_H
#define GRAPH_DIALOG_H

#include "graph_function.h"

int D_add_vertex(Graph* graph);
int D_add_edge(Graph* graph);
int D_delete_vertex(Graph* graph);
int D_delete_edge(Graph* graph);
int D_change_vertex(Graph* graph);
int D_depth_first_search(Graph* graph);
int D_Dijkstra(Graph* graph);
int D_Floyd_Warshall(Graph* graph);
int D_Show(Graph* graph);
int dialog(const char *msgs[], int N);

#endif
