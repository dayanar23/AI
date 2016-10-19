#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

// FORWARD DECLARATION FOR DFS FUNCTION
int dfs(state_t, int,int,int);

// GLOBAL VARIABLE FOR NUMBER OF STATES 
int states;

int main(int argc, char **argv ) {

    // VARIABLES FOR INPUT
    ifstream in_file;
    string line;
    ssize_t nchars; 
    state_t state; // state_t is defined by the PSVN API. It is the type used for individual states.

    // VARIABLES FOR OUTPUT
    ofstream out_file;
    
   // VARIABLES FOR TIME COUNT
    time_t begin, end;

    in_file.open(argv[1]);
    out_file.open(argv[2]);

    // Header of the summary table
    out_file << " algorithm, domain, instance, cost,";
    out_file << " generated, time, states for sec ";
    out_file << endl;

    // RUN THE ALGORITHM FOR EACH LINE IN INSTANCE'S FILE
    while(getline(in_file,line)){

        // READ A LINE AND BUILD STATE
        read_state(line.c_str(),&state);

         // VARIABLES FOR THE IDDFS
        int cost = 0;
        int max_cost = 0;
        int path_cost;
        int hist = init_history;

        states = 0;

        // BEGIN THE CLOCK
        time(&begin);

        // ITERATIVE DEEPENING LOOP
        while (1){
            path_cost = dfs(state, cost, max_cost,hist);
            if (path_cost != -1){
                break;
            }
            max_cost++;
        }
        
        time(&end);

        out_file << " dfid , X , \"" << line << "\", ";
        out_file << path_cost << ", " << states << ", ";
        out_file << begin-end << ", " << states/(begin-end);
        out_file << endl;
    }

    in_file.close();
    out_file.close();
    return 0;
    
}

int dfs(state_t state, int cost, int max_cost, int hist){

    if (cost + 1 >= max_cost){ 
        return -1;
    }

    // VARIABLES FOR ITERATING THROUGH state's SUCCESSORS
    state_t child;
    // ruleid_terator_t is the type defined by the PSVN API successor/predecessor iterators.
    ruleid_iterator_t iter; 
    int rule; // an iterator returns a number identifying a rule
    int new_hist;

    // AXILIARS VARIABLES FOR THE RECURSION
    int next_cost;
    
    // initialize the child iterator 
    init_fwd_iter(&iter,&state);

    // LOOP THOUGH THE CHILDREN ONE BY ONE
    while((rule = next_ruleid(&iter)) >= 0) {

        if (fwd_rule_valid_for_history(hist,rule)) { 

            new_hist = next_fwd_history(hist,rule);
            apply_fwd_rule(rule, &state, &child);
            states++;

            // SHOW THE STATE AND VALUES
            print_state(stdout, &child);
            printf(" %s (cost %d), goal=%d\n", get_fwd_rule_label(rule), get_fwd_rule_cost(rule), is_goal(&child));

            //CHECK IF THE STATE IS THE GOAL
            if (is_goal(&child)) { 
                return cost++;
            }

            // RECURSION OF DFS
            next_cost = dfs(child,cost+1,max_cost,new_hist);

            if (next_cost != -1) { 
                return next_cost;
            }
        }             
    }

    return -1; 
}
