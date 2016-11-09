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

int negamax(state_t state, int depth, int color, bool use_tt){

    int alpha; // int for the result
    state_t child_t; // next state
    bool turn = color == 1; // player black (true) or white (false)
    bool moves = false; // has possible moves

    // check if the state is a terminal node
    if(state.terminal()){
        return color * state.value();
    }

    // alpha starts with the min possible value
    alpha = std::numeric_limits<int>::min();

    // negamax loop 
    for( int pos = 0; pos < DIM; ++pos ) {
        if (state.outflank(turn, pos)) {
            child_t = state.move(turn, pos);
            generated++;
            moves = true;
            alpha = std::max(alpha, -negamax(child_t, depth-1, -color, use_tt));
            expanded++;
        }
    }

    // if not possible moves, play the same player
    if(!moves){
        alpha = std::max(alpha, -negamax(state, depth-1, -color, use_tt));
    }

    return alpha;
}


int negamax(state_t state, int depth, int alpha, int beta, int color, bool use_tt){

    int val, score; // aux variables for calculate result
    state_t child_t; // next state
    bool turn = color == 1; // player black (true) or white (false)
    bool moves = false; // has possible moves

    // check if the state is a terminal node
    if(state.terminal()){
        return color * state.value();
    }

    // alpha starts with the min possible value
    score = std::numeric_limits<int>::min();

    // negamax loop
    for( int pos = 0; pos < DIM; ++pos ) {
        if (state.outflank(turn, pos)) {
            child_t = state.move(turn, pos);
            generated++;
            moves = true;
            val = - negamax(child_t, depth-1, -beta, -alpha, -color, use_tt);
            expanded++;
            score = std::max(score, val);
            alpha = std::max(alpha, val);

            if (alpha >= beta){
                break;
            }
        }
    }

    // if not possible moves, play the same player
    if (!moves) {
        val = -negamax(state, depth + 1, -beta, -alpha, -color, use_tt);
        score = std::max(score, val);
        alpha = std::max(alpha, val);
    }

    return alpha;
}




#endif