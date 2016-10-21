#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <csignal>
#include "heuristics.h"

using namespace std;

long long states; // number of states
string line,fileName;
ofstream out_file; 

// HANDLER FOR TIMEOUT
void signalHandler(int signum)
{
    out_file << "X, IDA*, gap" << fileName << ", \"";
    out_file << line << "\", " << "na, na, na, na, na"; 
    out_file << endl;

   exit(signum);  
}

// FORWARD DECLARATIONS
int ida_star(state_t);
int f_bounded_dfs_visit(state_t, int, int, int);


int main(int argc, char **argv){

	signal(SIGINT, signalHandler);

	// VARIABLES FOR INPUT
    state_t state; // state_t is defined by the PSVN API. It is the type used for individual states.
    fileName = argv[3]; 
    // VARIABLES FOR OUTPUT 
    out_file.open(argv[2], std::fstream::out | std::fstream::app);

	// VARIABLES FOR TIME COUNT
    clock_t begin, end;

    line = argv[1];

    // READ A LINE AND BUILD STATE
    read_state(line.c_str(),&state);

	int cost;
    int bound = gaps(&state);

    // BEGIN THE CLOCK
    begin = clock();

    cost = ida_star(state);
    // END THE CLOCK        
    end = clock();


	double secs = double(end - begin)/CLOCKS_PER_SEC;
	double gen = double(states)/secs;

	out_file << "X, IDA*, gap, " << fileName << ", \"";
    out_file << line << "\", " << cost << ", " << bound << ", ";
    out_file  << states << ", " << secs << ", " << gen << endl;
    
    
    return 0;
}


int ida_star(state_t state){

	// VARIABLES FOR IDA* SEARCH
    states = 0;
    int bound = 0;
    int cost;
    int hist = init_history;

    // IDA* LOOP
    while(true){

        cost = f_bounded_dfs_visit(state, hist, 0, bound);

        if (cost!= -1) {
        	return cost;
        }

        bound++;
    }
}

int f_bounded_dfs_visit(state_t state, int hist, int cost, int bound){
    
    // BASE CASES
    int h = gaps(&state);
    int f = cost + h;

    if (f > bound) {
    	return -f;
    }

    // CHECK IF THE STATE IS THE GOAL
    if (is_goal(&state)) 
    	{
    		return cost;
    	}  

    // VARIABLES FOR ITERATING THROUGH state's SUCCESSORS
    // ruleid_terator_t is the type defined by the PSVN API successor/predecessor iterators.
    ruleid_iterator_t iter; 
    int rule; // an iterator returns a number identifying a rule
    int new_hist;
    state_t child;
    int new_cost;

    init_fwd_iter(&iter,&state);

    // LOOP THOUGH THE CHILDREN ONE BY ONE
    while((rule = next_ruleid(&iter)) >= 0) {
        
        if (fwd_rule_valid_for_history(hist,rule)) {

	        new_hist = next_fwd_history(hist,rule);

	        // GENERATE CHILD STATE
	        apply_fwd_rule(rule, &state, &child);

            states++;
            // SHOW THE STATE AND VALUES
            print_state(stdout, &child);
            printf(" %s (cost %d), goal=%d\n", get_fwd_rule_label(rule), get_fwd_rule_cost(rule), is_goal(&child));

	        // RECURSION OF THE FUNCTION
	        new_cost = f_bounded_dfs_visit(child, new_hist, cost++, bound);	        

	        if (new_cost != -1) {
	        	return new_cost;
	        }
    	}
    }

    return -1;
}