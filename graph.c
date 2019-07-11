//  graph.c
//  OU5_SommarLocal
//  Created by Amine Balta and Matilda Nilsson on 2019-06-09.
//  Copyright © 2019 Amine Balta and Matilda Nilsson. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "array_2d.h"


struct node {
    bool visited;
    char *nodeName;
};
struct graph {
    array_2d *airArray;
};

// =================== NODE COMPARISON FUNCTION ======================

/**
 * nodes_are_equal() - Check whether two nodes are equal.
 * @n1: Pointer to node 1.
 * @n2: Pointer to node 2.
 *
 * Returns: true if the nodes are considered equal, otherwise false.
 *
 */
bool nodes_are_equal(const node *n1,const node *n2){
    if (n1 == n2){
        return true;
    }
    return false;
}

// =================== GRAPH STRUCTURE INTERFACE ======================

/**
 * graph_empty() - Create an empty graph.
 * @max_nodes: The maximum number of nodes the graph can hold.
 *
 * Returns: A pointer to the new graph.
 */
graph *graph_empty(int max_nodes){
    //Allocate memory
    graph *graph = malloc(sizeof(graph));
    //Create array
    graph->airArray = array_2d_create(0, max_nodes, 0, max_nodes, NULL);
    
    return graph;
}

/**
 * graph_is_empty() - Check if a graph is empty, i.e. has no nodes.
 * @g: Graph to check.
 *
 * Returns: True if graph is empty, otherwise false.
 */
bool graph_is_empty(const graph *g){
    //Control if the graph is empty
    for(int i = 0; i < array_2d_high(g->airArray, 1); i++){
        for(int j = 0; j < array_2d_high(g->airArray, 2); j++){
            if(array_2d_has_value(g->airArray, i, j)){
                return false;
            }
        }
    }
    return true;
}

/**
 * graph_has_edges() - Check if a graph has any edges.
 * @g: Graph to check.
 *
 * Returns: True if graph has any edges, otherwise false.
 */
//bool graph_has_edges(const graph *g);

/**
 * graph_insert_node() - Inserts a node with the given name into the graph.
 * @g: Graph to manipulate.
 * @s: Node name.
 *
 * Creates a new node with a copy of the given name and puts it into
 * the graph.
 *
 * Returns: The modified graph.
 */
graph *graph_insert_node(graph *g, const char *s){
    //A char string is limited to a maximum of 40 characters
    //The array will always end with '\0'
    int maxChar = 41;
    int i = 0;
    
    //Creates a node (size 41)
    node *node = malloc(sizeof(char)*maxChar);
    node->nodeName = (char*)s;
    node->visited = false;
    
    //Loop to find the index of the first free space
    while (array_2d_has_value(g->airArray, i, 0)){
        i++;
    }
    //Insert node
    array_2d_set_value(g->airArray, node, i, 0);
    
    return g;
}
/**
 * graph_find_node() - Find a node stored in the graph.
 * @g: Graph to manipulate.
 * @s: Node identifier, e.g. a char *.
 *
 * Returns: A pointer to the found node, or NULL.
 */

node *graph_find_node(const graph *g, const char *s){
    int i = 0;
    node *nodeFind = NULL;
    
    while (array_2d_has_value(g->airArray, i, 0)) {
        nodeFind = array_2d_inspect_value(g->airArray, i, 0);
        
        if (strcmp(nodeFind->nodeName, s) == 0) {
            return nodeFind;
        }
        i++;
    }
    return NULL;
}

/**
 * graph_node_is_seen() - Return the seen status for a node.
 * @g: Graph storing the node.
 * @n: Node in the graph to return seen status for.
 *
 * Returns: The seen status for the node.
 */
bool graph_node_is_seen(const graph *g, const node *n){
    
    node *nodeSeen = graph_find_node(g, n->nodeName);
   
    if (nodeSeen->visited) {
        return true;
    }
    else{
        return false;
    }
}

/**
 * graph_node_set_seen() - Set the seen status for a node.
 * @g: Graph storing the node.
 * @n: Node in the graph to set seen status for.
 * @s: Status to set.
 *
 * Returns: The modified graph.
 */
graph *graph_node_set_seen(graph *g, node *n, bool seen){
    
    node *nodeSeen = graph_find_node(g, n->nodeName);
    nodeSeen->visited = seen;
    
    return g;
}

/**
 * graph_reset_seen() - Reset the seen status on all nodes in the graph.
 * @g: Graph to modify.
 *
 * Returns: The modified graph.
 */
graph *graph_reset_seen(graph *g){
    
    int i = 0, j;
    node *nodeSeen;
    
    while(array_2d_has_value(g->airArray, i, 0)){
        j = 0;
        while (array_2d_has_value(g->airArray, i, j)) {
            nodeSeen = array_2d_inspect_value(g->airArray, i, j);
            nodeSeen->visited = false;
            j++;
        }
        i++;
    }
    return g;
}

/**
 * graph_insert_edge() - Insert an edge into the graph.
 * @g: Graph to manipulate.
 * @n1: Source node (pointer) for the edge.
 * @n2: Destination node (pointer) for the edge.
 *
 * NOTE: Undefined unless both nodes are already in the graph.
 *
 * Returns: The modified graph.
 */
graph *graph_insert_edge(graph *g, node *n1, node *n2){
    
    int i = 0;
    node *srcNode, *destNode;
    
    while(array_2d_has_value(g->airArray, i, 0)){
        
        srcNode = array_2d_inspect_value(g->airArray, i, 0);
        if(strcmp(srcNode->nodeName, n1->nodeName) == 0){
            int j = 0;
            
            while (array_2d_has_value(g->airArray, i, j)) {
                destNode = array_2d_inspect_value(g->airArray, i, j);
                
                if (strcmp(destNode->nodeName, n2->nodeName) == 0) {
                    break;
                };
                    j++;
                    if(!array_2d_has_value(g->airArray, i, j)){
                        array_2d_set_value(g->airArray, n2, i, j);
                        j++;
                    }
            }
        }
        i++;
    }
    return g;
}

/**
 * graph_delete_node() - Remove a node from the graph.
 * @g: Graph to manipulate.
 * @n: Node to remove from the graph.
 *
 * Returns: The modified graph.
 *
 * NOTE: Undefined if the node is not in the graph.
 */
//graph *graph_delete_node(graph *g, node *n);

/**
 * graph_delete_edge() - Remove an edge from the graph.
 * @g: Graph to manipulate.
 * @n1: Source node (pointer) for the edge.
 * @n2: Destination node (pointer) for the edge.
 *
 * Returns: The modified graph.
 *
 * NOTE: Undefined if the edge is not in the graph.
 */
//graph *graph_delete_edge(graph *g, node *n1, node *n2);

/**
 * graph_choose_node() - Return an arbitrary node from the graph.
 * @g: Graph to inspect.
 *
 * Returns: A pointer to an arbitrayry node.
 *
 * NOTE: The return value is undefined for an empty graph.
 */
//node *graph_choose_node(const graph *g);

/**
 * graph_neighbours() - Return a list of neighbour nodes.
 * @g: Graph to inspect.
 * @n: Node to get neighbours for.
 *
 * Returns: A pointer to a list of nodes. Note: The list must be
 * dlist_kill()-ed after use.
 */
dlist *graph_neighbours(const graph *g,const node *n){
    
    dlist *dlistNeigbours = dlist_empty(NULL);
    dlist_pos position = dlist_first(dlistNeigbours);
    node *nodeNeighbours;
    int i = 0, j =1;
    
    while(array_2d_has_value(g->airArray, i, 0)){
        if (nodes_are_equal(array_2d_inspect_value(g->airArray, i, 0), n)){
            break;
        }
            i++;
    }
    while (array_2d_has_value(g->airArray, i, j)) {
        nodeNeighbours = array_2d_inspect_value(g->airArray, i, j);
        dlist_insert(dlistNeigbours, nodeNeighbours, position);
        position = dlist_next(dlistNeigbours, position);
        j++;
    }
    return dlistNeigbours;
}

/**
 * graph_kill() - Destroy a given graph.
 * @g: Graph to destroy.
 *
 * Return all dynamic memory used by the graph.
 *
 * Returns: Nothing.
 */
void graph_kill(graph *g){
    node *nodeKill;
    
    for(int i = 0; i < array_2d_high(g->airArray, 1); i++){
        if(array_2d_has_value(g->airArray, i, 0)){
            nodeKill = array_2d_inspect_value(g->airArray, i, 0);
            if(nodeKill != NULL){
                if(nodeKill->nodeName != NULL){
                    free(nodeKill->nodeName);
                }
                free(nodeKill);
            }
        }
    }
    array_2d_kill(g->airArray);
    free(g);
}


/**
 * graph_print() - Iterate over the graph elements and print their values.
 * @g: Graph to inspect.
 *
 * Iterates over the graph and prints its contents.
 *
 * Returns: Nothing.
 */
//void graph_print(const graph *g);

