#include <cstdio>
#include <iostream>

unsigned int heuristic(state_t *state){
    
    unsigned int number_not_adjacent = 0;
    int gap;
    unsigned int tam = sizeof(state->vars)/sizeof(state->vars[0]);
    for(unsigned int i=0; i<tam -1; i++){
        
        gap = state->vars[i] - state->vars[i+1]; 
        if ((gap < -1) || (gap > 1)){
            number_not_adjacent++;
        }
        
    }

    if (state->vars[tam-1] != 27){
        number_not_adjacent++;
    }
    
    return number_not_adjacent;
}