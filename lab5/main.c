#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph_dialog.h"

int main() {
    const char *msgs[] = {"0. Quit", "1. Add a vertex", "2. Add an edge",
                          "3. Delete a vertex","4. Delete an edge","5. Change the vertex",
                          "6. Show", "7. Depth-first search", "8. Dijkstra", "9. Floyd-Warshall"};
    const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
    int (*fptr[])(Graph *) = {NULL, D_add_vertex, D_add_edge, D_delete_vertex,
                              D_delete_edge, D_change_vertex, D_Show, D_depth_first_search, D_Dijkstra, D_Floyd_Warshall};
    int rc;
    Graph* graph = (Graph*) calloc (1, sizeof(Graph));
    graph->vertex_count = 0;
    FILE* file;
    file = fopen("file.txt", "a+");
    input_file(graph, &file);
    fclose(file);
    while ((rc = dialog(msgs, NMsgs))){
        if (fptr[rc](graph)){
            break;
        }
    }
    printf("That's all. Bye!\n");
    output_file(graph);
    Draw(graph);
    delete_graph(graph);
    return 0;
}
