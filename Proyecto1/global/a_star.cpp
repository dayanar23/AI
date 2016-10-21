/*
This program computes the distance to goal (i.e. the cost of the least-cost path to the goal)
of every state from which the goal can be reached.
It does this by executing Dijkstra's algorithm backwards from the goal.
It prints on stdout each state and its distance (distance first, then the state) and, if a filename is
provided as a command line argument, it prints the state_map it builds to that file.

Copyright (C) 2013 by the PSVN Research Group, University of Alberta
*/

#include "heuristics.h"
#include <stdio.h>
#include <vector>
#include "priority_queue.hpp"

int main(int argc, char **argv) {
    state_t state, child;   // NOTE: "child" will be a predecessor of state, not a successor
    int d, ruleid;
    ruleid_iterator_t iter;

    PriorityQueue<state_t> open; // used for the states we have generated but not yet expanded (the OPEN list)
    state_map_t *map = new_state_map(); // contains the cost-to-goal for all states that have been generated
    FILE *file; // the final state_map is written to this file if it is provided (command line argument)

    first_goal_state(&state,&d);
    
    char* string = "27 26 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25";

    read_state(string,&state);

    state_map_add(map, &state, 0);
    open.Add(0,0,state);
    
    d = 0;
    //printf("Prueba\n");

    while( !open.Empty() ) {
        // get current distance from goal; since operator costs are
        // non-negative this distance is monotonically increasing
        d = open.CurrentPriority();

        // remove top state from priority state
        state = open.Top();
        open.Pop();
        
        // check if we already expanded this state.
        // (entries on the open list are not deleted if a cheaper path to a state is found)
        const int *best_dist = state_map_get(map, &state);
        assert(best_dist != NULL);
        if( *best_dist < d ) continue;
        
        // print the distance then the state
        printf("\n%d  ",d);
        print_state(stdout,&state);
        printf(" \n");

        // look at all predecessors of the state
        init_fwd_iter(&iter, &state);
        while( (ruleid = next_ruleid(&iter) ) >= 0 ) {
            if (is_goal(&state)) {
                printf("Distancia = %d\n", d);
                return true;
            }

            apply_fwd_rule(ruleid, &state, &child);
            const int child_d = d + get_fwd_rule_cost(ruleid);

            const int heur = gaps(&state);

            // check if either this child has not been seen yet or if
            // there is a new cheaper way to get to this child.
            const int *old_child_d = state_map_get(map, &child);
            if( (old_child_d == NULL) || (*old_child_d > child_d + heur) ) {
                // add to open with the new distance
                state_map_add(map, &child, child_d);
                open.Add(child_d, child_d, child);
            }
        }
    }
    
    // write the state map to a file
    if( argc >= 2 ) {
        file = fopen(argv[1], "w");
        if( file == NULL ) {
            fprintf(stderr, "could not open %s for writing\n", argv[1]);
            exit(-1);
        }
        write_state_map(file, map);
        fclose(file);
    }
    
    return 0;
}

