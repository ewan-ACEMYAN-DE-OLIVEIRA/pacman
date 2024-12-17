#include <iostream>
#include <vector>

using namespace std;

void clearScreen () {
    cout << "\033[H\033[2J";
}

const unsigned KReset   (0);
const unsigned KNoir    (30);
const unsigned KRouge   (31);
const unsigned KVert    (32);
const unsigned KJaune   (33);
const unsigned KBleu    (34);
const unsigned KMAgenta (35);
const unsigned KCyan    (36);

void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}

typedef vector <char> CVLine; // un type représentant une ligne de la grille
typedef vector <CVLine> CMatrix; // un type représentant la grille
typedef pair   <unsigned, unsigned> CPosition; // un type représentant une coordonnée dans la grille

const char kTokenPlayer1 = 'X';
const char kTokenPlayer2 = 'O';
const char kEmpty        = '+';
const char kWall         = '/';
void  showMatrix (const CMatrix & Mat){
    clearScreen();
    for(auto i=0; i<Mat.size(); ++i){
        for(auto j=0; j<Mat[i].size(); ++j){
            if (Mat[i][j]== kTokenPlayer1){
                couleur(KRouge);
                cout << kTokenPlayer1;
                couleur (KReset);
            }
            else if (Mat[i][j]== kTokenPlayer2){
                couleur(KBleu);
                cout << kTokenPlayer2;
                couleur (KReset);
            }
            else if (Mat[i][j]==kWall){
                couleur(KJaune);
                cout << kWall;
                couleur(KReset);
            }
            else{
                cout << kEmpty;
            }
        }
        cout << endl;
    }
}

void initMat(CMatrix &Mat, unsigned nbLine, unsigned nbColumn, CPosition &posPlayer1, CPosition &posPlayer2) {
    Mat.resize(nbLine, CVLine(nbColumn, kEmpty));

    // Remplir une ligne sur deux avec des kWall
    for (unsigned i = 0; i < nbLine; ++i) {
        if (i % 2 != 0) {  // Si la ligne est impaire
            for (unsigned j = 0; j < nbColumn; ++j) {
                if (j % 2 != 0) {
                    Mat[i][j] = kWall;
                }
            }
        }
    }
    // Placer les joueurs
    Mat[posPlayer1.first][posPlayer1.second] = kTokenPlayer1;
    Mat[posPlayer2.first][posPlayer2.second] = kTokenPlayer2;
}

void moveToken(CMatrix &Mat, char move, CPosition &pos) {
    char element = Mat[pos.first][pos.second];
    Mat[pos.first][pos.second] = kEmpty;
    switch(tolower(move)) {
    case 'a': // Diagonal haut-gauche
        if (pos.first > 0 && pos.second > 0 && Mat[pos.first - 1][pos.second - 1] != kWall) {
            pos.first -= 1;
            pos.second -= 1;
        }
        break;
    case 'z': // Haut
        if (pos.first == 0) {
            if (Mat[Mat.size()-1][pos.second]!=kWall){
                pos.first = Mat.size() - 1;
            }
        }
        else if (Mat[pos.first - 1][pos.second] != kWall) {
            pos.first -= 1;
        }
        break;
    case 'e': // Diagonal haut-droite
        if (pos.first > 0 && pos.second < Mat[0].size() - 1 && Mat[pos.first - 1][pos.second + 1] != kWall) {
            pos.first -= 1;
            pos.second += 1;
        }
        break;
    case 'q': // Gauche
        if (pos.second == 0) {
            if(Mat[pos.first][Mat[0].size()-1]!=kWall){
                pos.second = Mat[0].size() - 1;
            }
        }
        else if (Mat[pos.first][pos.second - 1] != kWall) {
            pos.second -= 1;
        }
        break;
    case 'd': // Droite
        if (pos.second == Mat[0].size() - 1) {
            if (Mat[pos.first][0]!=kWall){
                pos.second = 0;
            }
        }
        else if (Mat[pos.first][pos.second + 1] != kWall) {
            pos.second += 1;
        }
        break;
    case 'w': // Diagonal bas-gauche
        if ((pos.first < Mat.size() - 1 && pos.second > 0) && Mat[pos.first + 1][pos.second - 1] != kWall) {
            pos.first += 1;
            pos.second -= 1;
        }
        break;
    case 'x': // Bas
        if (pos.first == Mat.size() - 1) {
            if (Mat[0][pos.second]!=kWall){
                pos.first = 0;
            }
        }
        else if (Mat[pos.first + 1][pos.second] != kWall) {
            pos.first += 1;
        }
        break;
    case 'c': // Diagonal bas-droite
        if (pos.first < Mat.size() - 1 && pos.second < Mat[0].size() - 1 && Mat[pos.first + 1][pos.second + 1] != kWall) {
            pos.first += 1;
            pos.second += 1;
        }
        break;
    }
    Mat[pos.first][pos.second] = element;
}

int ppal(){
    CMatrix Mat;
    unsigned nbLine = 10;
    unsigned nbColumn = 8;
    CPosition posPlayer1;
    CPosition posPlayer2;
    posPlayer1 = CPosition(0, 0);
    posPlayer2 = CPosition(9, 7);
    const unsigned nbMax = 20;
    unsigned nbCoup = 1;
    bool victoire = false;
    int joueur = 1;
    initMat(Mat, nbLine, nbColumn, posPlayer1, posPlayer2);
    showMatrix(Mat);
    string move;
    while(nbCoup<nbMax && !victoire){
        cout << "Coup numero " << nbCoup << endl;
        cout << "Coup du joueur " << joueur << " : ";
        getline(cin, move);
        cout << endl;
        if(nbCoup%2 == 1){
            moveToken(Mat, move[0], posPlayer1);
            joueur = 2;
        }
        else{
            moveToken(Mat, move[0], posPlayer2);
            joueur = 1;
        }
        showMatrix(Mat);
        if(posPlayer1 == posPlayer2)
            victoire = true;
        ++nbCoup;
    }
    if (victoire){
        cout<<"Victoire Royale"<<endl;
    }
    else{
        cout<<"match nul!"<<endl;
    }
    return 0;
}

int main() {
    ppal();
    return 0;
}
