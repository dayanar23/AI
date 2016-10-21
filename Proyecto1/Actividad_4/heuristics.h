#include <stdio.h>
#include <stdlib.h>

int manhattan_distance(int *estado_actual, int N){
	int h = 0;
	int size = N * N;

	int pos_actual[2];
	int pos_deseada[2];

	for (int i = 0; i < size; ++i)
	{
		if (estado_actual[i] != 0) {

			pos_actual[0] = i / N;
			pos_actual[1] = i % N;

			/*
				La posicion de una pieza en el estado goal es i + 1
			*/
			pos_deseada[0] = estado_actual[i] / N;
			pos_deseada[1] = estado_actual[i] % N;

			h += abs(pos_deseada[0] - pos_actual[0]);
			h += abs(pos_deseada[1] - pos_actual[1]);
		}
	}

	return h;
}

unsigned int gaps(int *estado_actual){
	unsigned int h = 0;
	unsigned int N = sizeof(estado_actual->vars)/sizeof(estado_actual->vars[0]);
	unsigned int diferencia;

	for (int i = 1; i < N; ++i)
	{
		diferencia = abs(estado_actual->vars[i] - estado_actual->vars[i + 1]);
		
		if (diferencia > 1 || diferencia < -1) {
			h += 1;
		}
	}

	return h;
}