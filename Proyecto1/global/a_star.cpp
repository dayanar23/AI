/*
This program computes the distance to goal (i.e. the cost of the least-cost path to the goal)
of every state from which the goal can be reached.
It does this by executing Dijkstra's algorithm backwards from the goal.
It prints on stdout each state and its distance (distance first, then the state) and, if a filename is
provided as a command line argument, it prints the state_map it builds to that file.

Copyright (C) 2013 by the PSVN Research Group, University of Alberta
*/
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <csignal>
#include "heuristics.h"
#include <stdio.h>
#include <vector>
#include "priority_queue.hpp"


using namespace std;

long long states; // number of states

int main(int argc, char **argv) {
    state_t state, child;   // NOTE: "child" will be a predecessor of state, not a successor
    int d, ruleid;
    ruleid_iterator_t iter;
    string line;
    PriorityQueue<state_t> open; // used for the states we have generated but not yet expanded (the OPEN list)
    state_map_t *map = new_state_map(); // contains the cost-to-goal for all states that have been generated

    // VARIABLES FOR TIME COUNT
    clock_t begin, end;

    line = argv[1];
    int bound = gaps(&state);

    first_goal_state(&state,&d);
    
    read_state(line.c_str(),&state);

    state_map_add(map, &state, 0);
    open.Add(0,0,state);
    
    d = 0;
    states = 0;
    //printf("Prueba\n");
    begin = clock();

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
            states++;
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
    
    end = clock();

    double secs = double(end - begin)/CLOCKS_PER_SEC;
    double gen = double(states)/secs;

    cout << "X, IDA*, gap, " << fileName << ", \"";
    cout << line << "\", " << d << ", " << bound << ", ";
    cout  << states << ", " << secs << ", " << gen << endl;
    
    return 0;
}

