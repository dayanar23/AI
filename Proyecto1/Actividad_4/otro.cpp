#include <iostream>
#include <ctime>
#include <cstdio>
#include <queue>
#include <map>
#include <climits>
#include "gaph.h"

using namespace std;

long long generated_states;


// Estructutura que se almacena en la cola de prioridades
struct node{
	state_t estado;
	unsigned int costo;
	unsigned int heuristica;
	unsigned int history;
};

struct par{
	node *nodo;
	unsigned int estimado;
};

struct node ida_star(state_t *);

struct par bounded_a(node, unsigned int,state_map_t *);


int main(){
	
	state_t start; 
	string state_string; // Almacena el estado dado por el usuario
	int result; // Valor retornado por la función
	int goalID;
	
	//map <state_t,int> m; //mapea un estado con su prioridad
	
	cout << "Introduzca el estado del problema: " << endl;
	
	getline(cin,state_string);
	if (read_state(state_string.c_str(),&start) == -1){

		cout << "No pudimos transformar tu input a un estado" << endl;
		cout << "Puede que este en formato incorrecto." << endl;

		int goalID;
		first_goal_state(&start,&goalID);

		cout << "La representacion del estado goal del problema es la siguiente" << endl;
		print_state(stdout,&start);
		cout << endl;
		return 0;
	}
	
	generated_states = 0;
	
	clock_t begin = clock();
	
	struct node nodo_result = ida_star(&start);
	result = nodo_result.costo;

	clock_t end = clock();

	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	double gen_per_sec = double(generated_states)/elapsed_secs;

	cout << "algorithm, heuristic, domain, instance, cost, h0, generated, time, gen_per_sec " << endl;

	cout << " IDA*, gap, pancake28,\"" << state_string << "\", " << result << ", " << heuristic(&start);
	cout << ", " << generated_states << ", "  << elapsed_secs << ", ";
	cout << gen_per_sec << endl;
	
	return 0;
}


struct node ida_star(state_t *start){
    unsigned int cota = heuristic(start);
    struct node nodo;
    nodo.estado = *start;
    nodo.costo=0;
    nodo.heuristica=cota;
    nodo.history = init_history;
    struct par p;
    state_map_t *stateMap = new_state_map();
    while(true){
        p = bounded_a(nodo, cota, stateMap);
        if (p.nodo) {return *(p.nodo);}
        cota = p.estimado;
    }
}

struct par bounded_a(node nodo, unsigned int cota, state_map_t *stateMap){
    unsigned int estimado = nodo.costo + nodo.heuristica;
    if (estimado > cota) {par p; p.nodo = NULL; p.estimado=estimado;return p;}
    if (is_goal(&(nodo.estado))) {par p; p.nodo=&nodo;p.estimado=nodo.costo;return p;}
    
    unsigned int min_estimado = UINT_MAX;
    int ruleID, new_hist;
    ruleid_iterator_t iter;
    state_map_add(stateMap,&(nodo.estado),nodo.costo);
    state_t hijo_estado;
    state_t current_state = nodo.estado;
    
    init_fwd_iter(&iter,&current_state);
    struct node hijo;
    struct par p;
    while((ruleID = next_ruleid( &iter )) >= 0) {
        
        if (!fwd_rule_valid_for_history(nodo.history,ruleID)) {continue;}
        
        new_hist = next_fwd_history(nodo.history,ruleID);;
        
        apply_fwd_rule(ruleID, &current_state, &hijo_estado);
        generated_states++;
        hijo.history = new_hist;
        hijo.estado = hijo_estado;
        hijo.costo = nodo.costo + 1;
        hijo.heuristica = heuristic(&hijo_estado);
        state_map_add(stateMap,&hijo_estado,hijo.costo);
        p = bounded_a(hijo,cota, stateMap);
        if (p.nodo) {return p;}
        min_estimado = (min_estimado < p.estimado) ? min_estimado : p.estimado;
    }
    p.nodo = NULL;
    p.estimado = min_estimado;
    return p;
}

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