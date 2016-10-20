#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <csignal>

using namespace std;

string line;
ofstream out_file;

void signalHandler(int signum)
{
    out_file << "X, dfid , 11puzzle , \"" << line << "\", ";
    out_file << "na, na, na, na" << endl;
    out_file.close();

   exit(signum);  
}

// FORWARD DECLARATION FOR DFS FUNCTION
int dfs(state_t, int,int,int);

// GLOBAL VARIABLE FOR NUMBER OF STATES 
long long states;

int main(int argc, char **argv ) {
    signal(SIGINT, signalHandler); 
    // VARIABLES FOR INPUT
    ssize_t nchars; 
    state_t state; // state_t is defined by the PSVN API. It is the type used for individual states.

    // VARIABLES FOR OUTPUT 
    out_file.open(argv[2]);
    
   // VARIABLES FOR TIME COUNT
    clock_t begin, end;

    line = argv[1];
    //getline(cin,line);

    // Header of the summary table
    out_file << "group, algorithm, domain, instance, cost,";
    out_file << " generated, time, states for sec ";
    out_file << endl;

    // READ A LINE AND BUILD STATE
    read_state(line.c_str(),&state);

    // VARIABLES FOR THE IDDFS
    int cost = 0;
    int max_cost = 0;
    int path_cost;
    int hist = init_history;

    states = 0;

    try {
        // BEGIN THE CLOCK
        begin = clock();

        
        // ITERATIVE DEEPENING LOOP
        while (1){
            path_cost = dfs(state, cost, max_cost,hist);
            if (path_cost != -1){
                break;
            }
            max_cost++;
        }

        // END THE CLOCK        
        end = clock();

        long double secs = double(end - begin)/ CLOCKS_PER_SEC;
        double gen = double(states)/secs;

        out_file << "X, dfid, 11-puzzle, \"" << line << "\", ";
        out_file << path_cost << ", " << states << ", ";
        out_file << secs << ", " << gen << endl;
        out_file.close();
    }
    catch(int e){
        out_file << " dfid , 11puzzle , \"" << line << "\", ";
        out_file << "na, na, na, na" << endl;
        out_file.close();
        exit(0);
    }

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
