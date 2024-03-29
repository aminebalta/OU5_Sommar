//
//  is_connected.c
//  OU5_SommarLocal
//
//  Created by Amine Balta and Matilda Nilsson on 2019-06-09.
//  Version 1.0.
//  Copyright © 2019 Amine Balta and Matilda Nilsson. All rights reserved.
//
//  This is an implementation for an assigment in the course Data structure and algorithms
//  and is based on the datatype files from Cambro. It includes the main function which runs the
//  programme, reads file, creates graph, handles input from user and also includes the
//  breadth first search for finding connected nodes in graph.
//
//  Based on version 'datastructures-v1.0.8.1'.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include <stdbool.h>
#include "graph.h"
#include <ctype.h>
#include <errno.h>

bool find_path(graph *g,node *src,node *dest);
int first_character(const char *c);
int last_character(const char *c);
bool blank_line(const char *c);
bool comment_line(char *c);

/*
 * main() - Runs the program, reads in file, creates a graph, read, handle input from user and
 * free and alloc memory..
 */
int main(int argc, char *argv[]){
    
    char file_content[1000];
    int nr_of_edges = 0;
    
    /*Read file given as an argument*/
    char *file_n;
    if (argc > 1){
        file_n = argv[1];
    }
    /*Exit: file not found.*/
    else{
        fprintf(stderr, "No file input\n");
        exit(EXIT_FAILURE);
    }
    
    FILE *file = fopen(file_n, "r");
    
    /*Exit if file is not opended*/
    if (file == NULL){
        fprintf(stderr, "ERROR: %s could not be opend\n", file_n);
        exit(EXIT_FAILURE);
    }
    /*Reads the numbers of edges*/
    while(fgets(file_content, sizeof(file_content), file) != NULL){
        
        if(blank_line(file_content) || comment_line(file_content)){
            continue;
        }
        else{
            sscanf(file_content, "%d", &nr_of_edges);
            break;
        }
    }
    
    /*If the number of edges are zero, exit*/
    if(nr_of_edges == 0){
        fprintf(stderr, "ERROR: Error reading the numbers of edges\n");
        exit(EXIT_FAILURE);
    }
    
    /*New empty graph*/
    graph *node_graph = graph_empty(nr_of_edges*2);
    
    /*Exit if new graph is not empty*/
    if(!graph_is_empty(node_graph)){
        fprintf(stderr, "ERROR: New graph was not empty\n");
        exit(EXIT_FAILURE);
    }
    
    /*Runs when the file still has content*/
    while(fgets(file_content, sizeof(file_content), file)){
        
        if((blank_line(file_content) || comment_line(file_content))){
            continue;
        }
        /*Alloc memory for nodes*/
        else{
            char *node1 = malloc(sizeof(char)*41);
            char *node2 = malloc(sizeof(char)*41);
            int airport1 = 0;
            int airport2 = 0;
            
            sscanf(file_content, "%s %s", node1, node2);
            
            if(blank_line(node1) || blank_line(node2)){
                fprintf(stderr, "ERROR: Two node named was not found\n");
                exit(EXIT_FAILURE);
            }
            
            /*Insert unique nodes in the the graph*/
            if(!graph_find_node(node_graph, node1)){
                node_graph = graph_insert_node(node_graph, node1);
                airport1 = 1;
            }
            if(!graph_find_node(node_graph, node2)){
                node_graph = graph_insert_node(node_graph, node2);
                airport2 = 1;
            }
            
            node_graph = graph_insert_edge(node_graph, graph_find_node(node_graph, node1), graph_find_node(node_graph, node2));
            
            /*Free memory that is not used*/
            if(airport1 == 0){
                free(node1);
            }
            if(airport2 == 0){
                free(node2);
            }
        }
    }
    
    /*Close file when the it is read*/
    fclose(file);
    
    char user_input_node1[41];
    char user_input_node2[41];
    /*Buffer*/
    char input_line[256];
    
    printf("Enter origin and destination (quit to exit): ");
    
    /*Continue until quit*/
    while(fgets(input_line, sizeof input_line, stdin) != NULL){
    
        /*Read user input*/
        
        bool destination_found = false;
        
        /*Use buffer so there are two imputs, if not exit loop and exit program*/
        if (sscanf(input_line,"%40s %40s", user_input_node1, user_input_node2)!= 2){
            if (strcmp(user_input_node1, "quit") == 0) {
                break;
            }
            printf("You need to enter two airports, try again.\n\n");
        
        }
        /*If node is not found, program contunes and new input i requested.*/
        else if(!graph_find_node(node_graph, user_input_node1) ||
           !graph_find_node(node_graph, user_input_node2)){
            printf("Airport did not exist in map, try again.\n\n");
            user_input_node1[0] = '\0';
            user_input_node2[0] = '\0';
        }
        /*If input is the same, program continues and new input is requested*/
        else if(graph_find_node(node_graph, user_input_node1) ==
           graph_find_node(node_graph, user_input_node2)){
            printf("There is a path from %s to %s.\n\n", user_input_node1, user_input_node2);
            user_input_node1[0] = '\0';
            user_input_node2[0] = '\0';
        }
        else if(!strcmp(user_input_node1, user_input_node2)){
            printf("There is a path from %s to %s.\n\n", user_input_node1, user_input_node2);
        }
        else{
            destination_found = find_path(node_graph, graph_find_node(node_graph, user_input_node1), graph_find_node(node_graph, user_input_node2));
            
            if(destination_found == 1){
                printf("There is a path from %s to %s.\n\n", user_input_node1, user_input_node2);
            }
            
            else if (destination_found == 0){
                printf("There is no path from %s to %s.\n\n", user_input_node1, user_input_node2);
            }
        }
        printf("Enter origin and destination (quit to exit): ");
    }
    printf("Normal exit.\n");
    graph_kill(node_graph);
    return 0;
}

/*
 * find_path() - Use the input souce to check if there is a connection between the airports
 *
 * @g: Graph
 * @src: Input node
 * @dest: Input destination node
 */
bool find_path(graph *g,node *src,node *dest){
    
    node *n;
    dlist *list_of_neighbour = NULL;
    dlist_pos pos_list;
    bool destination_found = false;
    
    /*New empty queue that store nodes to be traversed*/
    queue *traverse_queue = queue_empty(NULL);
    /*Set traversed queue to seen and add to queue*/
    g = graph_node_set_seen(g, src, true);
    traverse_queue = queue_enqueue(traverse_queue, src);
    
    /*While there are still unchecked neigbours or if destination is found*/
    while(!queue_is_empty(traverse_queue) && !destination_found){
        
        /*First element in queue*/
        n = queue_front(traverse_queue);
        traverse_queue = queue_dequeue(traverse_queue);
        
        list_of_neighbour = graph_neighbours(g, n);
        pos_list = dlist_first(list_of_neighbour);
        
        while(!dlist_is_end(list_of_neighbour, pos_list)){
            node *neighbour = dlist_inspect(list_of_neighbour, pos_list);
            if(nodes_are_equal(dest, neighbour)){
                destination_found = true;
            }
            
            else if(!graph_node_is_seen(g, neighbour)){
                traverse_queue = queue_enqueue(traverse_queue, neighbour);
                graph_node_set_seen(g, neighbour, true);
            }
            pos_list = dlist_next(list_of_neighbour, pos_list);
        }
        dlist_kill(list_of_neighbour);
    }
    
    g = graph_reset_seen(g);
    
    /*Remove node from queue after destination has been found*/
    while (!queue_is_empty(traverse_queue)) {
        queue_dequeue(traverse_queue);
    }
    queue_kill(traverse_queue);
    
    return destination_found;
}


/*
 * first_character() - The first non-whitespace character is returned or -1 if
 * only whitespace is found.
 *
 * @c: char
 */
int first_character(const char *c){
    /*First char*/
    int i= 0;
    while (c[i] && isspace(c[i])) {
        i++;
    }
    if(c[i]){
        return i; //Position of non-white-space character
    }
    else{
        return -1; //Fail
    }
}

/*
 * last_character() - The last non-whitespace character is returned or -1 if
 * only whitespace is found.
 *
 * @c: char
 */
int last_character(const char *c){
    //Last char
    int i = strlen(c) - 1;
    
    while (i >= 0 && isspace(c[i])) {
        i--;
    }
    if(i >= 0){
        return i; //Position of non-white-space character
    }
    else{
        return -1; //Fail
    }
}

/*
 * blank_line() - If only whilespace is found, true is returned.
 *
 * @c: char
 */
bool blank_line(const char *c){
    //Line is blank if it only has white-space
    return first_character(c) < 0;
}

/*
 * comment_line() - If a comment is found, true is returned, i.e. fist character is '#'.
 *
 * @c: char
 */
bool comment_line(char *c){
    int i = first_character(c);
    return (i >= 0 && c[i] == '#');
}
