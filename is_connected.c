//
//  is_connected.c
//  OU5_SommarLocal
//
//  Created by Amine Balta on 2019-06-09.
//  Copyright © 2019 Amine Balta. All rights reserved.
//

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
 * main() - Runs the program
 */
int main(int argc, char *argv[]){
    
    char file_content[1000];
    int nr_of_edges = 0;
    
    //Read file given as an argument
    //char *file_n = argv[argc-1];
    
    char *file_n;
    if (argc > 1){
        //file_n=argv[argc-1];
        file_n=argv[1];
    }
    
    //TA BORT!!!!!!!! hej hej hej
    //If no argument is added, use airmap1.map as default
    /*
    else{
        file_n = "airmap1.map";
        printf("> ./isConnected %s \n", file_n);
    }*/
    
    FILE *file = fopen(file_n, "r");
    
    if (file == NULL){
        fprintf(stderr, "ERROR: %s could not be opend\n", file_n);
        exit(EXIT_FAILURE);
    }
    
    while(fgets(file_content, sizeof(file_content), file) != NULL){
        
        if(blank_line(file_content) || comment_line(file_content)){
            continue;
        }
        else{
            sscanf(file_content, "%d", &nr_of_edges);
            break;
        }
    }
    
    if(nr_of_edges == 0){
        fprintf(stderr, "ERROR: Error reading the numbers of edges\n");
        exit(EXIT_FAILURE);
    }
    
    graph *node_graph = graph_empty(nr_of_edges*2);
    
    if(!graph_is_empty(node_graph)){
        fprintf(stderr, "ERROR: New graph was not empty\n");
        exit(EXIT_FAILURE);
    }
    
    while(fgets(file_content, sizeof(file_content), file)){
        
        if((blank_line(file_content) || comment_line(file_content))){
            continue;
        }
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
            
            //Insert unique nodes in the the graph
            if(!graph_find_node(node_graph, node1)){
                node_graph = graph_insert_node(node_graph, node1);
                airport1 = 1;
            }
            if(!graph_find_node(node_graph, node2)){
                node_graph = graph_insert_node(node_graph, node2);
                airport2 = 1;
            }
            
            node_graph = graph_insert_edge(node_graph, graph_find_node(node_graph, node1), graph_find_node(node_graph, node2));
            
            //Free memory that is not used
            if(airport1 == 0){
                free(node1);
            }
            if(airport2 == 0){
                free(node2);
            }
        }
    }
    //Close file when the it is read
    fclose(file);
    
    char user_input_node1[41];
    char user_input_node2[41];
    
    //Read user input
    printf("Enter origin and destination (quit to exit): ");
    scanf("%40s", user_input_node1);
    
    //Continue unntil quit
    while(strcmp(user_input_node1, "quit")){
        scanf("%40s", user_input_node2);
        bool destination_found = false;
        
        if(!graph_find_node(node_graph, user_input_node1) ||
           !graph_find_node(node_graph, user_input_node2)){
            printf("Airport did not exist in map, try again.\n\n");
            user_input_node1[0] = '\0';
            user_input_node2[0] = '\0';
        }
        else if(graph_find_node(node_graph, user_input_node1) ==
           graph_find_node(node_graph, user_input_node2)){
            printf("Airports is equal, try again.\n\n");
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
        scanf("%40s", user_input_node1);
    }
    printf("Normal exit.\n");
    graph_kill(node_graph);
    return 0;
}

/*
 * find_path() -
 *
 * @g:
 * @src:
 * @dest:
 */
bool find_path(graph *g,node *src,node *dest){
    
    node *n;
    dlist *list_of_neighbour = NULL;
    dlist_pos pos_list;
    bool destination_found = false;
    
    //New empty queue that store nodes to be traversed
    queue *traverse_queue = queue_empty(NULL);
    //Set traversed queue to seen and add to queue
    g = graph_node_set_seen(g, src, true);
    queue_enqueue(traverse_queue, src);
    
    //While there are still unchecked neigbours or if destination is found
    while(!queue_is_empty(traverse_queue) && !destination_found){
        
        //First element in queue
        n = queue_front(traverse_queue);
        queue_dequeue(traverse_queue);
        
        list_of_neighbour = graph_neighbours(g, n);
        pos_list = dlist_first(list_of_neighbour);
        
        while(!dlist_is_end(list_of_neighbour, pos_list)){
            
            if(nodes_are_equal(dest, dlist_inspect(list_of_neighbour, pos_list))){
                destination_found = true;
            }
            
            else if(!graph_node_is_seen(g, dlist_inspect(list_of_neighbour, pos_list))){
                graph_node_set_seen(g, dlist_inspect(list_of_neighbour, pos_list), true);
            }
            pos_list = dlist_next(list_of_neighbour, pos_list);
        }
        dlist_kill(list_of_neighbour);
    }
    
    g = graph_reset_seen(g);
    
    //Remove node from queue after destination has been found
    while (!queue_is_empty(traverse_queue)) {
        queue_dequeue(traverse_queue);
    }
    queue_kill(traverse_queue);
    
    return destination_found;
}


/*
 * first_character() -
 *
 * @c:
 */
int first_character(const char *c){
    //First char
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
 * last_character() -
 *
 * @c:
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
 * blank_line() -
 *
 * @c:
 */
bool blank_line(const char *c){
    //Line is blank if it only has white-space
    return first_character(c) < 0;
}

/*
 * comment_line() -
 *
 * @c:
 */
bool comment_line(char *c){
    int i = first_character(c);
    return (i >= 0 && c[i] == '#');
}
