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
    char line[BUFSIZ];
    
    //Read file given as an argument
    char *file_n = argv[argc-1];
    FILE *file = fopen(file_n, "r");
    
    if (file == NULL){
        fprintf(stderr, "ERROR: %s could not be opend\n", file_n);
        exit(1);
    }
    
    do{
        sscanf(line, "%d", &nr_of_edges);
    }while(fgets(line, BUFSIZ, file) != NULL && (blank_line(line)
                                                 || comment_line(line)));
    if(nr_of_edges == 0){
        fprintf(stderr, "ERROR: Error reading the numbers of edges");
        exit(1);
    }
    
    graph *node_graph = graph_empty(nr_of_edges*2);
    
    if(!graph_empty(node_graph)){
        fprintf(stderr, "ERROR: New graph was not empty");
        exit(1);
    }
    
    while(fgets(file_content, sizeof(file_content), file)){
        
        if((blank_line(line) || comment_line(line))){
            continue;
        }
        else{
            char *src = malloc(sizeof(char)*41);
            char *destination = malloc(sizeof(char)*41);
            
            
        }
    }
}
