#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <csignal>

using namespace std;

void signalHandler(int signum)
{
    cout << "X, dfid, 11-puzzle " << ", \"";
    cout << "\", " << "na, na, na, na, na"; 
    cout << endl;

   exit(signum);  
}
// FORWARD DECLARATION FOR FUNCTIONS
int iddfs(state_t);
int dfs(state_t, int,int,int);

// GLOBAL VARIABLE FOR NUMBER OF STATES 
long long states;

int main(int argc, char **argv ) {
    signal(SIGINT, signalHandler);
    // VARIABLES FOR INPUT
    ssize_t nchars; 
    state_t state; // state_t is defined by the PSVN API. It is the type used for individual states.
    string line;
    // VARIABLES FOR OUTPUT 
    
   // VARIABLES FOR TIME COUNT
    clock_t begin, end;

    line = argv[1];
    //getline(cin,line);

    // READ A LINE AND BUILD STATE
    read_state(line.c_str(),&state);
    int cost;

    // BEGIN THE CLOCK
    begin = clock();

    cost = iddfs(state);
        
    // END THE CLOCK        
    end = clock();

    long double secs = double(end - begin)/ CLOCKS_PER_SEC;
    double gen = double(states)/secs;

    cout << "X, dfid, 11-puzzle" << ", \"";
    cout << line << "\", " << cost << ", " << states;
    cout << ", " << secs << ", " << gen << endl;

    exit(0);
}

int iddfs(state_t state){

    // VARIABLES FOR THE IDDFS
    int max_cost = 0;
    int path_cost;
    int hist = init_history;

    // ITERATIVE DEEPENING LOOP
    while (1){
        path_cost = dfs(state, 0, max_cost,hist);
        if (path_cost != -1){
            return path_cost;
        }
        max_cost++;
    }
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
