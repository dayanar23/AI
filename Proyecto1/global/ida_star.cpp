#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <csignal>
#include <climits>
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

// STRUCT FOR NODE'S VALUES
struct node{
	state_t state;
	unsigned int cost;
	unsigned int h;
	unsigned int hist;
};

// STRUCT FOR PAIRS NODE-ESTIMATED COST
struct pair_n{
	node *n;
	unsigned int e_cost;
};

// FORWARD DECLARATIONS
struct node ida_star(state_t, unsigned int);
struct pair_n f_bounded_dfs_visit(node, unsigned int,state_map_t*);


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
	states = 0;
	struct node goal_n; 
	unsigned int bound = gaps(&state);

    // BEGIN THE CLOCK
    begin = clock();

    goal_n = ida_star(state, bound);
    cost = goal_n.cost;
    // END THE CLOCK        
    end = clock();


	double secs = double(end - begin)/CLOCKS_PER_SEC;
	double gen = double(states)/secs;

	out_file << "X, IDA*, gap, " << fileName << ", \"";
    out_file << line << "\", " << cost << ", " << bound << ", ";
    out_file  << states << ", " << secs << ", " << gen << endl;

    exit(0);
}


struct node ida_star(state_t state, unsigned int bound){

	// VARIABLES FOR IDA* SEARCH
	struct node node;
	struct pair_n pair;
	state_map_t *state_m= new_state_map();

	// INIT VARIABLES

    node.state = state;
    node.cost = 0;
    node.h = bound;
    node.hist = init_history;

    // IDA* LOOP
    while(true){

        pair = f_bounded_dfs_visit(node, bound, state_m);

        if (pair.n != NULL) {
        	return *(pair.n);
        }

        bound = pair.e_cost;

    }
}

struct pair_n f_bounded_dfs_visit(node n, unsigned int bound, state_map_t *state_m){
    
    // BASE CASES
    unsigned int h = n.cost + n.h;
    if (h > bound) {
    	pair_n p; 
    	p.n = NULL; 
    	p.e_cost = h;
    	return p;
    }

    // CHECK IF THE STATE IS THE GOAL
    if (is_goal(&n.state)) 
    	{
    		pair_n p; 
    		p.n = &n;
    		p.e_cost = n.cost;
    		return p;
    	}  

    // VARIABLES FOR ITERATING THROUGH state's SUCCESSORS
    // ruleid_terator_t is the type defined by the PSVN API successor/predecessor iterators.
    ruleid_iterator_t iter; 
    int rule; // an iterator returns a number identifying a rule
    int hist;
    state_t s_child;
    state_t state = n.state;
    unsigned int min_e_cost;

    state_map_add(state_m,&n.state,n.cost);
    init_fwd_iter(&iter,&state);

    struct node n_child;
    struct pair_n p;

    min_e_cost = UINT_MAX;

    // LOOP THOUGH THE CHILDREN ONE BY ONE
    while((rule = next_ruleid(&iter)) >= 0) {
        
        if (fwd_rule_valid_for_history(n.hist,rule)) {

	        hist = next_fwd_history(n.hist,rule);

	        // GENERATE CHILD STATE
	        apply_fwd_rule(rule, &state, &s_child);

	        // INIT CHILD STATE
	        n_child.hist = hist;
	        n_child.state = s_child;
	        n_child.cost = n.cost++;
	        n_child.h = gaps(&s_child);

	        // ADD STATE TO THE MAP
	        state_map_add(state_m,&s_child,n_child.cost);

	        // RECURSION OF THE FUNCTION
	        p = f_bounded_dfs_visit(n_child, bound, state_m);

	        states++;

	        if (p.n != NULL) {
	        	return p;
	        }

	        // CALCULATE NEW ESTIMATED COST
	        min_e_cost = min(min_e_cost, p.e_cost);

	        //min_estimado = (min_estimado < p.estimado) ? min_estimado : p.estimado;
    	}
    }

    // SET THE VARIABLES OF PAIR
    p.n = NULL;
    p.e_cost = min_e_cost;

    return p;
}