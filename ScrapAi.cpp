// MyOthelloAI.cpp

#include <ics46/factory/DynamicFactory.hpp>
#include "ScrapAi.hpp"
#include <tuple>
#include <functional>
// #include <iostream>
// #include <ctime>


ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, alanc15::Scrappy, "Scrappy (Required)");

namespace{
    //optimal seemed to be depth of 4, anything higher im getting > 3 seconds;
    int DEPTH_FOR_SEARCH = 4;

    void modifyVal(std::vector<int>& vals,
    std::unique_ptr<OthelloGameState>& state, int depth, bool isBlack,
    std::function<int(std::unique_ptr<OthelloGameState>&, int, bool)> search){
        for(int i = 0; i < state->board().height(); i++){
            for(int j = 0; j < state->board().width(); j++){
                if(state->isValidMove(i,j)){
                    auto stateClone = state->clone();
                    stateClone->makeMove(i,j);
                    int score = search(stateClone, depth-1, isBlack);
                    vals.push_back(score);
                }
            }
        }
    }

    int eval(std::unique_ptr<OthelloGameState>& state, bool isBlack){
        if(isBlack){
            return state->blackScore() - state->whiteScore();
        }
        else{
            return state->whiteScore() - state->blackScore();
        }
    }



    bool checkBlack(const OthelloGameState& state){
        return state.isBlackTurn();
    }

    int maxVal(const std::vector<int>& vals){
        int max = -999;
        for(int i = 0; i< vals.size(); i++){
            if (max < vals[i]){
                max = vals[i];
            }
        }
        return max;
    }

    int minVal(const std::vector<int>& vals){
        int min = 999;
        for(int i = 0; i< vals.size(); i++){
            if (min > vals[i]){
                min = vals[i];
            }
        }
        return min;
    }

    int search(std::unique_ptr<OthelloGameState>& state, int depth, bool isBlack){
        std::vector<int> vals;
        if(depth == 0){
            return eval(state, isBlack);
        } else {
            if((isBlack && state -> isBlackTurn()) || (!isBlack && state -> isWhiteTurn()) ){
                modifyVal(vals, state,depth,isBlack, search);
                return maxVal(vals);
            }else{
                modifyVal(vals, state, depth, isBlack, search);
                return minVal(vals);
            } 
        }
    }
}

std::pair<int, int> alanc15::Scrappy::chooseMove(const OthelloGameState& state)
{


    if(!state.isGameOver()){
        // std::clock_t c_start = std::clock();

        bool isBlack = checkBlack(state);

        //get a vector containing the possible moves in this state with ranking
        std::vector<std::tuple<int,int, int>> moveVector;
        std::vector<int> scoreValues;
        for(int i = 0; i < state.board().height(); i++){
            for(int j = 0; j < state.board().width(); j++){
                if(state.isValidMove(i,j)){
                    //clone to make move so that the current state is not effected
                    auto stateClone = state.clone();
                    stateClone -> makeMove(i,j);
                    int score = search(stateClone, DEPTH_FOR_SEARCH, isBlack);
                    scoreValues.push_back(score);

                    moveVector.push_back(std::make_tuple(i, j, score));
                }
            }
        }
        //return the best possible move from search
        std::pair<int,int> move;
        int max = maxVal(scoreValues);
        for(int i =0; i < moveVector.size(); i++){
            if(std::get<2>(moveVector[i]) == max){
                move = std::make_pair(std::get<0>(moveVector[i]),std::get<1>(moveVector[i]));
                break;
            }
        }
        //debugging
        // std::clock_t c_end = std::clock();

        // long time_elapsed_ms = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;
        // std::cout << "Move took: " << time_elapsed_ms/(double)1000 << " s\n";
        return move;

    } else {
        //return bogus move if gamestate is gameover;
        return std::make_pair(-1,-1);
    }
}


