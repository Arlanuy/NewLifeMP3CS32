#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define FALSE 0
#define TRUE 1

//global variables for DFS
int time;
char** color = NULL;
int travel_stats_iter;

//structs


typedef struct Node Node;

struct Node
{
    int VRTX;
    char TYPE;
    Node *NEXT;
};

typedef struct {
    int start;
    int finish;
    int TRAVEL_COST;
}Edge;

typedef struct {
    int* pred;
    int* d;
    int* f;
    Edge** edge_list;
    int edge_trav_iter;
    int TOTAL_TRAVEL_COST;
}Travel_Stats;


typedef struct{
    int num_of_vertex;
    Node** node_list;
}GRAPH;

//input helper

char* printMenuWithDataIter(char* menu_choice, int menuchoice_size) {
    printf("How would you like your input to be further processed?\n");
    printf("Enter 0 to process the next dataset\n");
    printf("Enter 1 to process another vertex pair\n");
    printf("Enter 2 to stop processing input on the file chosen\n");
    printf("Enter a number from the menu: ");
    fgets(menu_choice, menuchoice_size, stdin);
    printf("i love you\n");
    menu_choice[strlen(menu_choice) - 1] = '\0';
    printf("menu choice is %s\tc\n", menu_choice);
    return menu_choice;
}

//cost matrix helper

int getNumOfVertex(char* cost_row) {
    int i, vertex_num = 1;
    for (i = 0; cost_row[i] != '\n'; i++) {
        if (cost_row[i] == ' ') {
            vertex_num++;
        }
    }
    //printf("vertex num is %d\n", vertex_num);
    return vertex_num;
}

void freeStrCostAdjMat(char** str_cost_adj_mat, int num_of_vertex) {
    int i;
    for (i = 0; i <  num_of_vertex; i++) {
        free(str_cost_adj_mat[i]);
    }
}

void freeCostAdjMat(int** cost_adj_mat, int num_of_vertex) {
    int i;
    for (i = 0; i <  num_of_vertex; i++) {
        free(cost_adj_mat[i]);
    }

}

printArrContent(int* arr, int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%d\t", arr[i]);
    }
    printf("\n");
}

printColorContent(int size){
    int i;
    for (i = 0; i < size; i++)
    printf("i:%d have color[i]: %s\n", i, color[i]);
}

Travel_Stats* copyOfTravelStats(Travel_Stats* travel_stats, int num_of_vertex) {
    Travel_Stats* new_travel_stats = malloc(sizeof(Travel_Stats));
    new_travel_stats->edge_trav_iter = travel_stats->edge_trav_iter;
    new_travel_stats->edge_list = malloc((num_of_vertex * num_of_vertex) * sizeof(Edge*));
    new_travel_stats->pred = malloc(num_of_vertex * sizeof(int));
    new_travel_stats->d = malloc(num_of_vertex * sizeof(int));
    new_travel_stats->f = malloc(num_of_vertex * sizeof(int));
    int i;
    for(i = 0; i < travel_stats->edge_trav_iter ; i++) {
        new_travel_stats->edge_list[i] = travel_stats->edge_list[i];
    }

    for (i = 0; i < num_of_vertex; i++) {
        new_travel_stats->pred[i] = travel_stats->pred[i];
        new_travel_stats->d[i] = travel_stats->d[i];
        new_travel_stats->f[i] = travel_stats->f[i];
    }
    new_travel_stats->edge_trav_iter = travel_stats->edge_trav_iter;
    new_travel_stats->TOTAL_TRAVEL_COST = travel_stats->TOTAL_TRAVEL_COST;
    return new_travel_stats;
}

void printTravelStatsContent(Travel_Stats* travel_stats, int num_of_vertex) {
    printf("A path consists of edges\n");
    int i;
    for (i = 0; i < travel_stats->edge_trav_iter; i++) {
        printf("%d-%d with cost: %d\n", travel_stats->edge_list[i]->start, travel_stats->edge_list[i]->finish, travel_stats->edge_list[i]->TRAVEL_COST);
    }
    printf("with total cost %d\n", travel_stats->TOTAL_TRAVEL_COST);
    printf("pred content\n");
    printArrContent(travel_stats->pred, num_of_vertex);
    printf("d content\n");
    printArrContent(travel_stats->d, num_of_vertex);
    printf("f content\n");
    printArrContent(travel_stats->f, num_of_vertex);
}

int DFS(GRAPH* graph, Edge** edge_list, int i, int* travelled_destination, Travel_Stats* travel_stats, Travel_Stats** all_travel_stats, int start_vertex, int end_vertex) {
    Node* alpha;
    int color_white_final_vertex = TRUE, traverse_list = TRUE;
    if ((start_vertex != end_vertex) && (i == end_vertex)) {
        all_travel_stats[travel_stats_iter] = travel_stats;
        //printTravelStatsContent(travel_stats, graph->num_of_vertex);
        travel_stats_iter++;
        travel_stats->d[i - 1] = time;
        time++;
        traverse_list = FALSE;
        *travelled_destination = TRUE;
        color_white_final_vertex = FALSE;
    }

    else {
        color[i - 1] = "gray";
        travel_stats->d[i - 1] = time;
        time++;
    }

    alpha = graph->node_list[i];


    int j = 0;
    Travel_Stats* current_travel_stats;
    while (alpha != NULL) {
        alpha = alpha->NEXT;

        if (alpha != NULL) {
            if (traverse_list == TRUE) {
                printf("at i is %d with color %s\n", i, color[i - 1]);
                if (j != 0) {
                    printf("at j is %d with color %s\n", j, color[j - 1]);
                }
                current_travel_stats = copyOfTravelStats(travel_stats, graph->num_of_vertex);
            }

            //printTravelStatsContent(travel_stats, graph->num_of_vertex);
            j = alpha->VRTX;
        }

        else {
            break;
        }
        /**
        printf("WOW color at j:%d and i:%d", j, i);
        printf(" is %s and %s respectively\n", color[j - 1], color[i - 1]); */

        if ((traverse_list == TRUE) && (strcmp(color[j - 1], "white") == 0)) {
            alpha->TYPE = 'T';
            travel_stats->pred[j - 1] = i;
            Edge* edge = malloc(sizeof(edge));
            edge->start = i;
            edge->finish = j;
            edge->TRAVEL_COST = edge_list[(i - 1) * (graph->num_of_vertex) + (j - 1)]->TRAVEL_COST;
            //printf("an edge of start: %d and finish: %d is created with cost %d\n", edge->start, edge->finish, edge->TRAVEL_COST);
            current_travel_stats->TOTAL_TRAVEL_COST += edge->TRAVEL_COST;
            current_travel_stats->edge_list[current_travel_stats->edge_trav_iter] = edge;
            (current_travel_stats->edge_trav_iter)++;
            DFS(graph, edge_list, j, travelled_destination, current_travel_stats, all_travel_stats, start_vertex, end_vertex);

        }

        else if (strcmp(color[j - 1], "gray") == 0) {
            if (travel_stats->pred[i - 1] != j) {
                alpha->TYPE = 'B';
            }
            if ((start_vertex == end_vertex) && (end_vertex == j)) {
                travel_stats->pred[j - 1] = i;
                Edge* edge = malloc(sizeof(edge));
                edge->start = i;
                edge->finish = j;
                edge->TRAVEL_COST = edge_list[(i - 1) * (graph->num_of_vertex) + (j - 1)]->TRAVEL_COST;
                //printf("an edge of start: %d and finish: %d is created with cost %d\n", edge->start, edge->finish, edge->TRAVEL_COST);
                current_travel_stats->TOTAL_TRAVEL_COST += edge->TRAVEL_COST;
                current_travel_stats->edge_list[current_travel_stats->edge_trav_iter] = edge;
                (current_travel_stats->edge_trav_iter)++;
                all_travel_stats[travel_stats_iter] = current_travel_stats;
                //printf("vertex %d went to gray\n", j);
                //printTravelStatsContent(current_travel_stats, graph->num_of_vertex);
                travel_stats_iter++;
                *travelled_destination = TRUE;
            }
        }

        else if (strcmp(color[j - 1], "black") == 0) {
            if (travel_stats->d[i - 1] < travel_stats->d[j - 1]) {
                alpha->TYPE = 'F';
            }
            else {
                alpha->TYPE = 'X';
            }
            color[j - 1] = "white";

        }

    }
    if (color_white_final_vertex == TRUE) {
        color[i - 1] = "white";
        travel_stats->f[i - 1] = time;
        time++;
    }

    return 0;
}

void freeMatrixesContent (char** str_cost_adj_mat, int** cost_adj_mat, int num_of_vertex) {
    int i;
    if (str_cost_adj_mat != NULL) {
        for (i = 0; i < num_of_vertex; i++) {
            if (str_cost_adj_mat[i] != NULL) {
                free(str_cost_adj_mat[i]);
            }
        }
    }

    if (cost_adj_mat != NULL) {
        for (i = 0; i < num_of_vertex; i++) {
            if (cost_adj_mat[i] != NULL)
             {
                free(cost_adj_mat[i]);
             }
        }
    }
}

int main(void) {
     int filename_size = 50, line_size = 500, string_size = 70, menuchoice_size = 10;

    //for input
    char* input_name = NULL;
    input_name = malloc(sizeof(char) * filename_size);
    assert(input_name != NULL);
    FILE* input_file, *output_file;
    /**
    printf("Enter the name of the input file: ");
    fgets(input_name, filename_size, stdin);
    input_name[strlen(input_name) - 1] = '\0';
    input_file = fopen(input_name, "r");
    assert(input_file != NULL); */
    input_file = fopen("input.txt", "r");

    //for output
    /**
    char* output_name = malloc(sizeof(char) * filename_size);
    assert(output_name != NULL);
    printf("Enter the name of the output file: ");
    fgets(output_name, filename_size, stdin);
    output_name[strlen(output_name) - 1] = '\0';
    output_file = fopen(output_name, "a");
    assert(output_file != NULL); */

    output_file = fopen("output.txt", "a");

    //for menu choice
    int success_choice = FALSE, num_menu;
    char* line = malloc(sizeof(line_size)), *str_vertex_pair = malloc(sizeof(line_size));
    assert(line != NULL);
    assert(str_vertex_pair != NULL);
    char* menu_choice = malloc(sizeof(char) * menuchoice_size);
    assert(menu_choice != NULL);
    int more_dataset = TRUE, line_iter, start_line_iter, space_iter, row_iter = 0, line_bounds;
    int** cost_adj_mat = NULL;
    char** str_cost_adj_mat = NULL;
    int num_of_vertex;//, str_cost_alloc_iter, cost_alloc_iter;

    //for the graph
    GRAPH* graph = malloc(sizeof(GRAPH));
    int vertex_create_iter, array_filler_iter;
    int start_vertex = 0, end_vertex = 0;
    graph->node_list = NULL;
    Node* node;
    //for str_cost_adj_mat helpers
    int str_max_of_chars_for_num = 5, end_of_line_chars = 4, prev_num_of_vertex, started = FALSE;
    //for finding longest distance path helpers
    int temp_travel_stats_iter, edge_iter;
    Travel_Stats** all_travel_stats  = all_travel_stats = malloc(1000000 * sizeof(Travel_Stats*));
    Edge** max_travelled_edge_list = malloc(num_of_vertex * num_of_vertex * sizeof(Edge*));
    int max_edge_trav_iter;
    Edge** edge_list = NULL;

    //for menu
    menu_choice = printMenuWithDataIter(menu_choice, menuchoice_size);
    while (more_dataset == TRUE) {
        switch(atoi(menu_choice)) {
            case 0:
                row_iter = 0;
                while (fgets(line, line_size, input_file) && strcmp(line, "end\n") != 0) {
                    if (row_iter == 0) {
                        num_of_vertex = getNumOfVertex(line);
                        //printf("num of vertex is %d\n", num_of_vertex);
                        if (started == TRUE) {
                            freeMatrixesContent(str_cost_adj_mat, cost_adj_mat, prev_num_of_vertex);
                            started = FALSE;
                        }

                        str_cost_adj_mat = malloc(num_of_vertex * sizeof(char*));
                        cost_adj_mat = malloc(num_of_vertex * sizeof(int*));
                        prev_num_of_vertex = num_of_vertex;
                    }

                    space_iter = 0;
                    start_line_iter = 0;
                    //printf("string line is %s\n", line);
                    line_bounds = strlen(line);
                    str_cost_adj_mat[row_iter] = malloc((line_bounds + 10) * sizeof(char));//(num_of_vertex * str_max_of_chars_for_num * sizeof(char)) + end_of_line_chars);
                    strncpy(str_cost_adj_mat[row_iter], line, line_bounds);
                    //printf("copy_of_input: %s\n", str_cost_adj_mat[row_iter]);
                    for (line_iter = 0; line_iter < line_bounds; line_iter++) {
                        if (line_iter == 0) {
                            cost_adj_mat[row_iter] = malloc(num_of_vertex * sizeof(int));
                        }
                        if (line[line_iter] == ' ' || line[line_iter] == '\n') {
                            line[line_iter] = '\0';
                            cost_adj_mat[row_iter][space_iter] = atoi(&line[start_line_iter]);
                            //printf("string is %s at i %d and j %d is %d\n", &line[start_line_iter], row_iter, space_iter, cost_adj_mat[row_iter][space_iter]);
                            space_iter++;
                            start_line_iter = line_iter + 1;
                        }
                    }
                    line[line_bounds - 1] = '\0';
                    row_iter++;
                }

                //constructing the graph

                 graph->node_list = malloc(sizeof(Node*) * (num_of_vertex + 1));
                 for (vertex_create_iter = 0; vertex_create_iter <= num_of_vertex; vertex_create_iter++) {
                    node = malloc(sizeof(Node));


                    graph->node_list[vertex_create_iter] = node;
                    graph->node_list[vertex_create_iter]->VRTX = vertex_create_iter;
                    graph->node_list[vertex_create_iter]->NEXT = NULL;

                    //printf("vti: %d\tnode vrtx is %d\n with pointer %p\n", vertex_create_iter, graph->node_list[vertex_create_iter]->VRTX, graph->node_list[vertex_create_iter]);
                 }
                  break;



             case 1:
                printf("Enter a vertex pair (separate it by space): ");
                fgets(str_vertex_pair, string_size, stdin);
                int str_vertex_pair_bounds = strlen(str_vertex_pair);
                //printf("bounds is %d\n", str_vertex_pair_bounds);
                int svp_iter;
                for (svp_iter = 0; svp_iter < str_vertex_pair_bounds; svp_iter++) {
                    if (str_vertex_pair[svp_iter] == ' ') {
                        str_vertex_pair[svp_iter] = '\0';
                        start_vertex = atoi(&str_vertex_pair[0]);
                        break;
                    }
                }
                svp_iter++;
                int new_svp_iter;
                for (new_svp_iter = svp_iter; new_svp_iter < str_vertex_pair_bounds; new_svp_iter++) {
                    if (str_vertex_pair[new_svp_iter] == '\n') {
                        str_vertex_pair[new_svp_iter] = '\0';
                        end_vertex = atoi(&str_vertex_pair[svp_iter]);
                        break;
                    }
                }

                if ((start_vertex != 0 && (start_vertex <= num_of_vertex) && (start_vertex >= 1)) &&
                    (end_vertex != 0 && (end_vertex <= num_of_vertex) && (end_vertex >= 1))) {

                    int pred = 0, vertex_trav_iter, vertex_trav_iter2, color_iter;

                    //using the cost adjacency matrix to build up every element of node_list by appending adjacent vertex

                    if (edge_list  != NULL) {
                        free(edge_list);
                    }
                    edge_list = malloc(num_of_vertex * num_of_vertex * sizeof(Edge*));
                    Node* rover;
                    for (vertex_trav_iter = 1; vertex_trav_iter <= num_of_vertex; vertex_trav_iter++) {
                        rover = graph->node_list[vertex_trav_iter];
                       for (vertex_trav_iter2 = 1; vertex_trav_iter2 <= num_of_vertex; vertex_trav_iter2++) {
                            if (cost_adj_mat[vertex_trav_iter - 1][vertex_trav_iter2 - 1] != 1000 && cost_adj_mat[vertex_trav_iter - 1][vertex_trav_iter2 - 1] != 0) {
                                node = malloc(sizeof(Node));
                                assert(node != NULL);
                                node->VRTX = vertex_trav_iter2;
                                rover->NEXT = node;
                                rover = node;
                                //printf("start vertex: %d current_vertex: %d\n", graph->node_list[vertex_trav_iter]->VRTX, node->VRTX);
                                edge_list[(vertex_trav_iter - 1) * (num_of_vertex) + (vertex_trav_iter2 - 1)] = malloc(sizeof(Edge));
                                edge_list[(vertex_trav_iter - 1) * (num_of_vertex) + (vertex_trav_iter2 - 1)]->start = vertex_trav_iter;
                                edge_list[(vertex_trav_iter - 1) * (num_of_vertex) + (vertex_trav_iter2 - 1)]->finish = vertex_trav_iter2;
                                edge_list[(vertex_trav_iter - 1) * (num_of_vertex) + (vertex_trav_iter2 - 1)]->TRAVEL_COST = cost_adj_mat[vertex_trav_iter - 1][vertex_trav_iter2 - 1];
                                //printf("edge_list at %d-%d has cost %d\n", vertex_trav_iter, vertex_trav_iter2, edge_list[(vertex_trav_iter - 1) * (num_of_vertex) + (vertex_trav_iter2 - 1)]->TRAVEL_COST);
                            }
                            rover->NEXT = NULL;

                        }
                    }

                    //DFS driver
                    if (color != NULL) {
                        free(color);
                    }

                    color = malloc(sizeof(char*) * num_of_vertex);
                    time = 1;

                    for (color_iter = 0; color_iter < num_of_vertex; color_iter++) {
                        color[color_iter] = "white";
                    }

                    int travelled_destination = FALSE, max_travel_cost = 0;

                    travel_stats_iter = 0;
                    Travel_Stats* travel_stats = malloc(sizeof(Travel_Stats));
                    travel_stats->edge_list = malloc(num_of_vertex * num_of_vertex * sizeof(Edge*));
                    travel_stats->edge_trav_iter = 0;
                    travel_stats->TOTAL_TRAVEL_COST = 0;
                    travel_stats->pred = malloc(num_of_vertex * sizeof(int));
                    travel_stats->d = malloc(num_of_vertex * sizeof(int));
                    travel_stats->f = malloc(num_of_vertex * sizeof(int));

                    for(array_filler_iter = 0; array_filler_iter < num_of_vertex; array_filler_iter++) {
                        travel_stats->pred[array_filler_iter] = 0;
                        travel_stats->d[array_filler_iter] = 0;
                        travel_stats->f[array_filler_iter] = 0;
                    }

                    all_travel_stats[travel_stats_iter] = travel_stats;
                    travel_stats_iter++;
                    graph->num_of_vertex = num_of_vertex;
                    DFS(graph, edge_list, start_vertex, &travelled_destination, travel_stats, all_travel_stats, start_vertex, end_vertex);


                    //finding the maximum travel path

                    for (temp_travel_stats_iter = 0; temp_travel_stats_iter < travel_stats_iter; temp_travel_stats_iter++) {
                        if (all_travel_stats[temp_travel_stats_iter] != NULL && all_travel_stats[temp_travel_stats_iter]->TOTAL_TRAVEL_COST > max_travel_cost) {
                            //printf("i is %d\n", temp_travel_stats_iter);
                            if (end_vertex == all_travel_stats[temp_travel_stats_iter]->edge_list[all_travel_stats[temp_travel_stats_iter]->edge_trav_iter - 1]->finish) {
                                max_travel_cost = all_travel_stats[temp_travel_stats_iter]->TOTAL_TRAVEL_COST;
                                max_edge_trav_iter = all_travel_stats[temp_travel_stats_iter]->edge_trav_iter;
                                for (edge_iter = 0; edge_iter < max_edge_trav_iter; edge_iter++) {
                                    max_travelled_edge_list[edge_iter] = all_travel_stats[temp_travel_stats_iter]->edge_list[edge_iter];
                                }

                            }


                        }
                    }
                    if (travelled_destination == TRUE) {
                        printf("With %d edges and total cost %d There is a longest path and it is ", max_edge_trav_iter, max_travel_cost);

                        for (edge_iter = 0; edge_iter < max_edge_trav_iter; edge_iter++) {
                            if (max_travelled_edge_list[edge_iter] != NULL) {
                                printf("%d-%d with cost: %d\n", max_travelled_edge_list[edge_iter]->start, max_travelled_edge_list[edge_iter]->finish, max_travelled_edge_list[edge_iter]->TRAVEL_COST);
                                if (edge_iter == max_edge_trav_iter - 1) {
                                    printf("%d\n", max_travelled_edge_list[edge_iter]->finish);
                                }

                            }


                        }
                    }

                    else {
                        printf("There is no longest path\n");
                    }


                }
                else {
                    printf("Vertex pair invalid input\nPlease follow the format: d d where d is an integer within bounds\n");
                }

                break;
        }

        if (menu_choice[0] - '0' == 0) {
            prev_num_of_vertex = num_of_vertex;
            started = TRUE;
        }

        //another choice of input mode

        success_choice = FALSE;
        while (success_choice == FALSE) {
            menu_choice = printMenuWithDataIter(menu_choice, menuchoice_size);
            //printf("mc %s\n", menu_choice);
            if (strlen(menu_choice) == 1) {
                num_menu = menu_choice[0] - '0';
                if ((num_menu >= 0) && (num_menu <= 2)) {
                    success_choice = TRUE;
                }

                else {
                    printf("Enter a number menu from 0 to 1 only\n");
                }
            }

            else {
                printf("Enter a choice of length 1 only\n");
            }
        }
        if (num_menu == 2) {
            freeMatrixesContent(str_cost_adj_mat, cost_adj_mat, num_of_vertex);
            more_dataset = FALSE;
        }
    }

    //freeing all the max_edge_list pointers
    for (edge_iter = 0; edge_iter < max_edge_trav_iter; edge_iter++) {
        free(max_travelled_edge_list[edge_iter]);
    }
    free(max_travelled_edge_list);



    //freeing all the travel_stats created as well as the pointers in its content
     for (temp_travel_stats_iter = 0; temp_travel_stats_iter < travel_stats_iter; temp_travel_stats_iter++) {
        Travel_Stats* current = all_travel_stats[temp_travel_stats_iter];
        free(current->pred);
        free(current->d);
        free(current->f);
        for (edge_iter = 0; edge_iter < current->edge_trav_iter; edge_iter++) {
            free(current->edge_list[edge_iter]);
        }
        free(current->edge_list);
     }
    //freeing all the pointers used in constructing the graph
     for (vertex_create_iter = 0; vertex_create_iter <= num_of_vertex; vertex_create_iter++) {
        Node* alpha = graph->node_list[vertex_create_iter]->NEXT;
        while (alpha != NULL) {
            Node* beta = alpha->NEXT;
            free(alpha);
            alpha = beta;
        }
        free(graph->node_list[vertex_create_iter]);
     }
     free(graph->node_list);

     //freeing all the other used pointers
     //freeStrCostAdjMat(str_cost_adj_mat, num_of_vertex);
     free(str_cost_adj_mat);
     //freeCostAdjMat(cost_adj_mat, num_of_vertex);
     free(cost_adj_mat);
     //free(input_name);
     free(line);
     free(str_vertex_pair);
     free(menu_choice);
     fclose(input_file);
     fclose(output_file);

}


