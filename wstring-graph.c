/* Name: Ryan Frohman
   UID: 116470254
   Section: 0106
   terpconnect ID: rfrohman
*/

/* This file represents a weighted directed graph in which each vertex
   carries a dynamically allocated string as its data. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wstring-graph.h"

static Vertex *get_vertex(const WString_graph * const graph, const char name[]);
static int edge_exists(const WString_graph *const graph, const char from[],
		       const char to[]);

/* This function initializes a WString_graph variable to be used. The
   parameter represents a pointer to a weighted string graph */
void init_graph(WString_graph *const graph) {
  /* Check to see if pointer is NULL */
  if(graph == NULL)
    return;
  
  /* Initialize head of the graph to null */
  graph->head = NULL;
  
}

/* Adds a new vertex to the graph if the parameters are valid
   The parameters are a pointer to a graph, and the name of the
   vertex to be added. Returns 1 if successful, 0 otherwise */
int new_vertex_add(WString_graph *const graph, const char new_vertex[]) {
  Vertex *new, *curr, *prev;
  
  /* Check to see if arguments are invalid */
  if(graph == NULL || new_vertex == NULL)
    return 0;
  
  /* Check to see if vertex with that name already exists in the graph */
  if(is_existing_vertex(graph, new_vertex))
    return 0;
  
  /* Initialize new vertex */
  
  new = malloc(sizeof(*new));
  /* Make sure allocation worked */
  if(new == NULL)
    return 0;
  
  new-> edge_list = NULL;
  
  /* Allocate memory for name, then add one for null character */
  new->name = malloc((sizeof(char) * strlen(new_vertex)) + 1);
  strncpy(new->name, new_vertex, strlen(new_vertex) + 1);
  
  new->next = NULL;
  
  /* Case where graph is empty */
  if(num_vertices(graph) == 0) {
    graph->head = new;
  } else {
    /* Iterate through graph to add the vertex in, in order */
    curr = graph->head;
    prev = NULL;
    while(curr != NULL && strcmp(curr->name, new->name) < 0) {
      prev = curr;
      curr = curr->next;
    }

    /* Case 1: Add the vertex into the beginning of the list */
    if(prev == NULL) {
      
      new->next = graph->head;
      graph->head = new;
    } else if(curr == NULL) {
      
      /* Case 2: Add the vertex into the end of the list */
      prev->next = new;
    } else {
      
      /* Regular case: Add the vertex into the middle of the list */
      prev->next = new;
      new->next = curr;
    }
  }

  return 1;
}

/* This function returns the number of vertices in the given graph 
   Parameter represents pointer to the graph to be analyzed */
int num_vertices(const WString_graph *const graph) {
  Vertex *curr;
  int size = 0;
  /*Check to see if graph is invalid */
  if(graph == NULL)
    return 0;
  
  /* Iterate through graph, adding one to the size for each vertex */
  curr = graph->head;
  while(curr != NULL) {
    curr = curr->next;
    size++;
  }
  
  return size;
}

/* This function states whether or not the inputted graph contains a vertex
   with a name that matches the inputted name. Returns 1 if it exists, 0 if not.
   Parameters represent a pointer to the graph, and a string representing
   the name */
int is_existing_vertex(const WString_graph *const graph, const char name[]) {
  Vertex * curr;
  /*Check to see if arguments are invalid */
  if(graph == NULL || name == NULL)
    return 0;

  /* Iterate through the graph, and check each vertex's name */
  curr = graph->head;
  while(curr != NULL) {
    if(strcmp(curr->name, name) == 0)
      return 1;
    curr = curr->next;
  }
  
  /* The name wasn't found, so it doesn't exist and 0 is returned */
  return 0;
}

/* This function returns a pointer to pointers of characters, where each 
   pointer to characters is a pointer to the  name of one of the vertices. 
   The parameter represents the graph in which the vertex names will be 
   taken from */
char **get_vertices(const WString_graph *const graph) {
  char **names;
  Vertex *curr;
  /* Represents the array index for names, matching where curr is */
  int currIndex = 0;
  
  /* Check to see if the argument is invalid */
  if(graph == NULL)
    return NULL;

  /* Allocate memory for the array and make sure it worked */
  /* Size of one character pointer multiplied by number of names
     plus one for the last element to be NULL */
  names = malloc(sizeof(*names) * num_vertices(graph) + 1);
  names[num_vertices(graph)] = NULL;
  
  if(names == NULL)
    return NULL;
  
  /* Iterate through the graph and copy each name into the array */
  curr = graph->head;
  while(curr != NULL) {
    /* Allocate memory for the name that curr is on
       Allocate size of a character multiplied by length of name, plus
       1 for the null character at the end  */
    names[currIndex] = malloc(sizeof(*(names[currIndex])) *
			      strlen(curr->name) + 1);
    
    /* Check to see if allocation worked */
    if(names[currIndex] == NULL)
      return NULL;

    /* Make copy of current vertex's name */
    strncpy(names[currIndex], curr->name, strlen(curr->name) + 1);
    
    currIndex++;
    curr = curr->next;
  }
  
  /* Array is already sorted since the vertices are traversed in order, 
     so return it now */
  return names;

}

/* This function adds an edge between the given source and destination vertex.
   The first parameter represents the graph, the second represents the name of
   the source vertex, the third represents the name of the destination vertex,
   and the last represents the weight (cost) of the edge. */
   
int add_edge(WString_graph *const graph, const char source[], const char dest[],
	     int cost) {
  Edge *new, *curr;
  Vertex *from, *to;
  
  /* Check for invalid arguments */
  if(graph == NULL || source == NULL || dest == NULL || cost < 0)
    return 0;

  /* See if edge already exists. If so, then change weight of the edge
     to cost */
  if(edge_exists(graph, source, dest)) {
    
    /* Iterate through edge list until the correct edge is found */
    curr = get_vertex(graph, source)->edge_list;
    while(strcmp(curr->destination->name, dest) != 0)
      curr = curr->next;
    
    /* Change the weight, and end the function */
    curr->weight = cost;
    return 1;

  }
  
  /* Check to see if vertices are in the graph. If not, add them in */
  if(!(is_existing_vertex(graph, source)))
    new_vertex_add(graph, source);
  if(!(is_existing_vertex(graph, dest)))
    new_vertex_add(graph, dest);

  new = malloc(sizeof(*new));
  /* Make sure allocation worked */
  if(new == NULL)
    return 0;

  /* Iterate through and find source and destination vertex */
  from = get_vertex(graph, source);
  to = get_vertex(graph, dest);

  /*Initialize the rest accordingly */
  new->destination = to;
  new->weight = cost;
  new->next = NULL;
  
  /* Case where vertex has no edges */
  if(from->edge_list == NULL)
    from->edge_list = new;
  else { /* Case where you add an edge to the source vertex's edge list */

    /* Iterate through until reaching the end of the edge list, then add the
       new edge at the end */
    curr = from->edge_list;
    while(curr->next != NULL)
      curr = curr->next;
    curr->next = new;
  }

  return 1;
}

/* This function returns the weight of the edge between
 the two given names of vertices in the provided graph. If the vertice(s)
 don't exist or the edge doesn't exist, -1 is returned */
int get_weight_of_edge(const WString_graph *const graph, const char source[],
		       const char dest[]) {
  Vertex *from;
  Edge *curr;
  /* Check to see if arguments are invalid */
  if(graph == NULL || source == NULL || dest == NULL)
    return -1;

  /* Check to see if vertices are in the graph and the edge between 
     them exists */
  /* edge_exists() covers both cases */
  if(!edge_exists(graph, source, dest))
    return -1;

  /* Start at the source vertex and iterate through edges until the
     correct one is found */
  from = get_vertex(graph, source);
  curr = from->edge_list;
  /* Don't have to check for curr being NULL because the edge is
     guaranteed to exist */
  while(strcmp(curr->destination->name, dest) != 0)
    curr = curr->next;

  return curr->weight;
}

int num_neighbors(const WString_graph *const graph, const char vertex[]) {
  Vertex *vert;
  Edge *curr;
  int sum = 0;
  
  /* Make sure arguments are valid and vertex exists in the graph */
  if(graph == NULL || vertex == NULL || !(is_existing_vertex(graph, vertex)))
    return -1;

  /* Iterate through vertex's edge list, adding one
 to sum variable for each edge */
  vert = get_vertex(graph, vertex);
  curr = vert->edge_list;
  while(curr != NULL) {
    sum++;
    curr = curr->next;
  }
  
  return sum;
}

/* This function searches for a vertex in the given graph  with the
   name provided, and returns the vertex if found, null otherwise */
static Vertex *get_vertex(const WString_graph * const graph,
			  const char name[]) {
  Vertex *curr;
  
  if(graph == NULL || name == NULL)
    return NULL;
  /* Iterate through the graph and search until either the vertex
     with the given name is found or until the end is reached */
  curr = graph->head;
  while(strcmp(curr->name, name) != 0 && curr != NULL)
    curr = curr->next;
  return curr;

}

/* This function determines whether an edge exists between the vertex
 with the name of from and the vertex with the name of to. Returns 1
 if yes, 0 if not */
static int edge_exists(const WString_graph *const graph, const char from[],
		       const char to[]) {
  Vertex *source;
  Edge *curr;
  
  /* Check if both vertices are in the graph */
  if(!is_existing_vertex(graph, from) || !is_existing_vertex(graph, to))
    return 0;

  source = get_vertex(graph, from);

  /* Look for edge between vertices */
  curr = source->edge_list;
  while(curr != NULL) {
    if(strcmp(curr->destination->name, to) == 0)
      return 1;
    curr = curr->next;
  }

  /* Edge wasn't found, so return 0 */
  return 0;
  
  

}

