#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/time.h>

#define  MAX_LINE_LENGTH 999 


int main(int argc, char **argv ) {
    // VARIABLES FOR INPUT
    char str[MAX_LINE_LENGTH + 1];
    ssize_t nchars; 
    state_t state; // state_t is defined by the PSVN API. It is the type used for individual states.

    // VARIABLES FOR ITERATING THROUGH state's SUCCESSORS
    state_t child;
    ruleid_iterator_t iter; // ruleid_terator_t is the type defined by the PSVN API successor/predecessor iterators.
    int rule; // an iterator returns a number identifying a rule

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

    if (is_goal(&state)) return 0;

    int cost = 0;
    int max_cost = 0;
    int hist = init_history;
    int path_cost;
    int states = 0;

    while (1){
        init_fwd_iter(&iter,&state);
        while((rule = next_ruleid(&iter)) >= 0) {
            if (fwd_rule_valid_for_history(hist,rule)) {
                if (cost + 1 >= max_cost){cost = -1; continue;}
                hist = next_fwd_history(hist,rule);
                apply_fwd_rule(rule, &state, &child);
                print_state(stdout, &child);
                printf("  %s (cost %d), goal=%d\n", get_fwd_rule_label(rule), get_fwd_rule_cost(rule), is_goal(&child));
                states++;
                if (is_goal(&child)) {cost++; continue;}
                cost++;
                state = child;
            }
            cost = -1;
            break;
        }
        if (cost != -1) {
            print_state(stdout, &child);
            break;
        }
        max_cost++;
    }

}
