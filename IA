#include "source/AStar.hpp"

CPosition ia(CMatrix & Mat, CPosition & posPlayer2, CPosition & posPlayer1){
    AStar::Generator generator;
    generator.setWorldSize({Mat.size(), Mat[0].size()});
    generator.setDiagonalMovement(false);
    auto coup = generator.findPath({posPlayer1.first, posPlayer1.second}, {posPlayer2.first, posPlayer2.second});
    return {coup[1].x, coup[1].y};
}

void iaMove(CMatrix & Mat, CPosition & posPlayer2, CPosition & posPlayer1){
    char element = Mat[posPlayer2.first][posPlayer2.second];
    Mat[posPlayer2.first][posPlayer2.second] = kEmpty;
    posPlayer2=ia(Mat, posPlayer2, posPlayer1);
    Mat[posPlayer2.first][posPlayer2.second] = element;
}

# pour faire bouger l'ia il vaut appeler iaMove     posPlayer 2 représente l'ia
