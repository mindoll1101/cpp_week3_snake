#include "console.h"
#include <string>
#include <cstdlib>
#include <ctime>

int randomIdx(int start_num, int end_num){
    srand(time(NULL));
    int pos = rand() % (end_num - start_num) + start_num;

    return pos;
}

void printScore(int score, int size){
    std::string sc = "score: ";
    sc = sc + std::to_string(score);
    console::draw(size/2 - sc.length()/2, size, sc);
}