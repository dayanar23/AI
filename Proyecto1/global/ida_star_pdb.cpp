#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <csignal>
#include <climits>

using namespace std;

abstraction_t *a1, *a2, *a3;
state_map_t *m1, *m2, *m3;

int heuristic(state_t *state){

    int h = 0;
    int *h_aux;
    state_t new_state;


    abstract_state(a1, state, &new_state);
    h_aux = state_map_get(m1, &new_state);
    h = h + *h_aux;

    assert(*h_aux != NULL);
    abstract_state(a2, state, &new_state);
    h_aux = state_map_get(m2, &new_state);

    h = h + *h_aux;

    abstract_state(a3, state, &new_state);
    h_aux = state_map_get(m3, &new_state);

    h = h + *h_aux;

    return h;

}

long long int states;

// FORWARD DECLARATION
pair<int,bool> f_bounded_dfs_visit(state_t, int, int, int);

void signalHandler(int signum)
{
    cout << "X, IDA*, PDB, 15-puzzle" << ", \"";
    cout << "\", " << "na, na, na, na, na"; 
    cout << endl;

   exit(signum);  
}


int main(int argc, char **argv){
    
    signal(SIGINT, signalHandler);
    // VARIABLES FOR INPUT
    state_t state; // state_t is defined by the PSVN API. It is the type used for individual states.
    string line;
    FILE* file;

    // VARIABLES FOR TIME COUNT
    clock_t begin, end;

    line = argv[1];

    // READ A LINE AND BUILD STATE
    read_state(line.c_str(),&state);

    pair<int,bool> cost;
    int bound = heuristic(&state);
    int h = bound;

    states = 0;

    a1 = read_abstraction_from_file("PDB/abs1.abst");
    a2 = read_abstraction_from_file("PDB/abs2.abst");
    a3 = read_abstraction_from_file("PDB/abs3.abst");
    file = fopen("PDB/m1.pdb", "r");
    m1 = read_state_map(file);
    file = fopen("PDB/m2.pdb","r");
    m2 = read_state_map(file);
    file = fopen("PDB/m3.pdb","r");
    m3 = read_state_map(file);

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

    cout << "X, IDA*, PDB, 15-puzzle" << ", \"";
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

    int f = h + heuristic(&state);
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