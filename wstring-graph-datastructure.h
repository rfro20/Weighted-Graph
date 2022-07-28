#if !defined(WSTRING_GRAPH_DATASTRUCTURE_H)
#define WSTRING_GRAPH_DATASTRUCTURE_H

struct vertex;

/* Struct representing edge in a graph */
typedef struct edge {
  struct vertex *destination;
  int weight;
  struct edge *next;
} Edge;

/* Struct representing a vertex in a graph */
typedef struct vertex {
  Edge *edge_list;
  char *name;
  /* Linked List Implementation */
  struct vertex *next;
} Vertex;


typedef struct {
  /* graph needs to have a starting point */
  Vertex *head; 
} WString_graph;

#endif
