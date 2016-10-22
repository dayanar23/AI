#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <csignal>
#include <climits>
//#include "heuristics.h"

using namespace std;

long long int states;

// FORWARD DECLARATION
pair<int,bool> f_bounded_dfs_visit(state_t, int, int, int);

void signalHandler(int signum)
{
    cout << "X, dfid, pancakes28" << ", \"";
    cout << "\", " << "na, na, na, na, na"; 
    cout << endl;

   exit(signum);  
}

int gaps(state_t state){

    int i = 0;
    int h = 0;

    while (i < 27){

        if ( abs(state.vars[i] - state.vars[i+1]) != 1 )
            h++;
        i++;

    }

    return h;
}


int main(int argc, char **argv){
    
    signal(SIGINT, signalHandler);
    // VARIABLES FOR INPUT
    state_t state; // state_t is defined by the PSVN API. It is the type used for individual states.
    string line;

    // VARIABLES FOR TIME COUNT
    clock_t begin, end;

    line = argv[1];

    // READ A LINE AND BUILD STATE
    read_state(line.c_str(),&state);

    pair<int,bool> cost;
    int bound = gaps(state);
    int h = bound;

    states = 0;
    // BEGIN THE CLOCK
    begin = clock();

    while (true) {
        cost = f_bounded_dfs_visit(state, init_history, 0, bound);
        if (cost.second){
            break;
        }
        bound = cost.first;
    }

    // END THE CLOCK        
    end = clock();


    double secs = double(end - begin)/CLOCKS_PER_SEC;
    double gen = double(states)/secs;

    cout << "X, IDA*, gap, pancakes28" << ", \"";
    cout << line << "\", " << cost.first << ", " << bound << ", ";
    cout  << states << ", " << secs << ", " << gen << endl;
    
    
    return 0;
}

std::pair<int,bool> f_bounded_dfs_visit(state_t state, int hist, int h, int bound){

    std::pair<int,bool> max_cost;

    // VARIABLES FOR ITERATING THROUGH state's SUCCESSORS
    state_t child;
    ruleid_iterator_t iter; // ruleid_terator_t is the type defined by the PSVN API successor/predecessor iterators.
    int ruleid, new_hist; 
    int t;

    int f = h + gaps(state);
    if (f > bound){
        max_cost.first = f;
        max_cost.second = false;
        return max_cost;
    }

    if (is_goal(&state)) {
        max_cost.first = h;
        max_cost.second = true;
        return max_cost;
    }

    t = INT_MAX;

    // LOOP THOUGH THE CHILDREN ONE BY ONE
    init_fwd_iter(&iter, &state);  // initialize the child iterator

    while ( (ruleid = next_ruleid(&iter)) >= 0 ){
        if (!fwd_rule_valid_for_history(hist,ruleid)){continue;} 
        new_hist = next_fwd_history(hist,ruleid);
        apply_fwd_rule(ruleid, &state, &child);
        ++states; 
        max_cost = f_bounded_dfs_visit(child, new_hist, h+1, bound);

        if (max_cost.second) {
            return max_cost;
        }

        t = min(t,max_cost.first);

        }

    max_cost.first = t;
    max_cost.second = false;
    return max_cost;
}