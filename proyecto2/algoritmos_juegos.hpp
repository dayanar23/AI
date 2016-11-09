#ifndef ALGORITMOS_JUEGOS_H
#define ALGORITMOS_JUEGOS_H

#include <limits>
#include <algorithm>

//int maxmin(state_t state, int depth, bool use_tt);
int minmax(state_t state, int depth, bool use_tt);


int maxmin(state_t state, int depth, bool use_tt) {
    //std::cout << "Mi valor maxmin" << state.value();
    if (state.terminal()) return state.value();

    bool jugado = false;
    int score = std::numeric_limits<int>::min();
    for (int i = 4; i < 36 ; i += 1)
    {
        // El jugador max son las fichas negras
        if (state.outflank(true,i)) {
            jugado = true;
            generated += 1;
            score = std::max(score,minmax(state.move(true,i),0,use_tt));
            //std::cout << "maxmin score: " << score << std::endl;
        }
    }

    expanded +=1;
    if (!jugado)
        score = std::max(score,minmax(state,0,use_tt));

    return score;
}

int minmax(state_t state, int depth, bool use_tt) {
    //std::cout << "Mi valor minmax: " << state.value() << std::endl;
    if (state.terminal()) return state.value();

    //bool no_pass = false;
    int score = std::numeric_limits<int>::max();
    for (int i = 4; i < 36 ; i += 1)
    {
        // El jugador min son las fichas blancas
        if (state.outflank(false,i)) {
            generated += 1;
            score = std::min(score,maxmin(state.move(false,i),0,use_tt));
            //std::cout << "minmax score: " << score << std::endl;
        }
    }
    expanded +=1;
    /*if (!no_pass)
    {
        score = std::min(score,maxmin(state,0,use_tt));
    }*/

    return score;
}

#endif