#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/time.h>

#define  MAX_LINE_LENGTH 999 

// FORWARD DECLARATION FOR DFS FUNCTION
int dfs(state_t, int,int,int);

// GLOBAL VARIABLE FOR THE NUMBER OF STATES
int states = 0;

int main(int argc, char **argv ) {

    // VARIABLES FOR INPUT
    char str[MAX_LINE_LENGTH + 1];
    ssize_t nchars; 
    state_t state; // state_t is defined by the PSVN API. It is the type used for individual states.

    // READ A LINE OF INPUT FROM stdin
    printf("Please enter a state followed by ENTER: ");
    if( fgets(str, sizeof str, stdin) == NULL ) {
        printf("Error: empty input line.\n");
        return 0; 
    }

    // CONVERT THE STRING TO A STATE
    nchars = read_state(str, &state);
    if( nchars <= 0 ) {
        printf("Error: invalid state entered.\n");
        return 0; 
    }

    printf("The state you entered is: ");
    print_state(stdout, &state);
    printf("\n");

    // CHECKS IF THE FIRST STATE IS THE GOAL
    if (is_goal(&state)) return 0;

    // VARIABLES FOR THE IDDFS
    int cost = 0;
    int max_cost = 0;
    int path_cost;
    int hist = init_history;

    // ITERATIVE DEEPENING LOOP
    while (1){
        path_cost = dfs(state, cost, max_cost,hist);
        if (path_cost != -1){
            return path_cost;
        }
        max_cost++;
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
            printf("  %s (cost %d), goal=%d\n", get_fwd_rule_label(rule), get_fwd_rule_cost(rule), is_goal(&child));

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
