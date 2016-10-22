#include <stdio.h>
#include <stdlib.h>

int manhattan(state_t *state){
    int h= 0;
    int N = 4;
    int size = N * N;
    string c_position; 

    for (int i = 0; i < size; ++i)
    {   
        c_position = state.vars[i];
        if ( c_position != "B"){
            
            int position = state.vars[i];
            int current_p0 = i / N;
            int current_p1 = i % N;
            /*
                La posicion de una pieza en el estado goal es i + 1
            */
            int goal_p0 = position / N;
            int goal_p1 = position % N;
            h += abs(goal_p0 - current_p0);
            h += abs(goal_p1 - current_p1);

        }
        i++;

    }

    return h;
}

int gaps(state_t *estado_actual){
	int h = 0;
	int N = sizeof(estado_actual->vars)/sizeof(estado_actual->vars[0]);
	int diferencia;

	for (int i = 1; i < N; ++i)
	{
		diferencia = abs(estado_actual->vars[i] - estado_actual->vars[i+1]);
		
		if (diferencia > 1) {
			h += 1;
		}
	}

	return h;
}