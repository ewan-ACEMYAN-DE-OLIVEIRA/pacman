#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

typedef vector <char> CVLine; // un type représentant une ligne de la grille
typedef vector <CVLine> CMatrix; // un type représentant la grille
typedef pair   <unsigned, unsigned> CPosition; // un type représentant une coordonnée dans la grille


const unsigned KReset   (0);
const unsigned KNoir    (30);
const unsigned KRouge   (31);
const unsigned KVert    (32);
const unsigned KJaune   (33);
const unsigned KBleu    (34);
const unsigned KMAgenta (35);
const unsigned KCyan    (36);
const char kTokenPlayer1 = 'X';
const char kTokenPlayer2 = 'O';
const char kEmpty        = '_';

void clearScreen () {
    cout << "\033[H\033[2J";
}

void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}
void  showMatrix (const CMatrix & Mat){
    clearScreen();
    couleur (KReset);
    for (size_t i(0);i<Mat.size();++i){
        for(size_t j(0);j<Mat[i].size();++j){
            if (Mat[i][j]==kTokenPlayer1){
                couleur(KBleu+10);
                cout<<kTokenPlayer1;
            }
            else if (Mat[i][j]==kTokenPlayer2){
                couleur(KRouge+10);
                cout<<kTokenPlayer2;
            }
            else{
                couleur(KReset);
                cout<<kEmpty;
            }
        }
        cout<<endl;
        couleur(KReset);
    }
}

void initMat (CMatrix & Mat, unsigned nbLine, unsigned nbColumn, CPosition & posPlayer1, CPosition & posPlayer2)
{
    Mat.resize(nbColumn, CVLine(nbLine, kEmpty));
    Mat[posPlayer1.first][posPlayer1.second]=kTokenPlayer1;
    Mat[posPlayer2.first][posPlayer2.second]=kTokenPlayer2;
}

void moveToken (CMatrix & Mat, char move, CPosition  & pos)
{
    char element = Mat[pos.first][pos.second];
    Mat[pos.first][pos.second]=kEmpty;
    switch(tolower(move))
    {
    case 'z':
        if (pos.first<1){
            pos.first=Mat.size()-1;
        }
        else{
            pos.first -=1;
        }
        break;
    case 'q':
        if (pos.second < 1){
            pos.second=Mat[0].size()-1;
        }
        else{
            pos.second=pos.second-1;
        }
        break;
    case 'd':

        if (pos.second>Mat[0].size()-2){
            pos.second=0;
        }
        else{
        pos.second=pos.second+1;
        }
        break;
    case 'x':
        if(pos.first>Mat.size()-2){
            pos.first=0;
        }
        else{
        pos.first=pos.first+1;
        }
        break;
    case 'a':
        if(!(pos.first<1||pos.second<1)){
            pos.first=pos.first-1;
            pos.second=pos.second-1;
        }
        break;
    case 'e':
        if(!(pos.first<1||pos.second>Mat[0].size()-2)){
            pos.second=pos.second-1;
            pos.second=pos.second+1;
        }
        break;
    case 'w':
        if(!(pos.first>Mat.size()-1||pos.second<1)){
            pos.first=pos.first+1;
            pos.second=pos.second-1;
        }
        break;
    case 'c':
        if(!(pos.first>Mat.size()-1||pos.second>Mat[0].size()-2)){
            pos.first=pos.first+1;
            pos.second=pos.second+1;
        }
        break;

    }
    Mat[pos.first][pos.second]=element;
}

int ppal(){
    CMatrix Mat;
    unsigned nbLine=10;
    unsigned nbColumn=8;
    CPosition posPlayer1;
    CPosition posPlayer2;
    posPlayer1=CPosition(0,0);
    posPlayer2=CPosition(7,9);
    const unsigned nbmax = 20;
    unsigned nbcoup = 1;
    bool victoire = false;
    int joueur = 1;
    initMat(Mat,nbLine,nbColumn,posPlayer1,posPlayer2);
    showMatrix(Mat);
    string move;
    while(nbcoup<nbmax && !victoire){
        cout<<"Coup numero"<<nbcoup<<endl;
        cout<<"Coup du joueur"<<joueur<<" : ";
        getline(cin, move);
        if (nbcoup%2==1){
            moveToken(Mat,move[0],posPlayer1);
            joueur=2;
        }
        else{
            moveToken(Mat,move[0],posPlayer2);
            joueur=1;
        }
        showMatrix(Mat);
        if(posPlayer1==posPlayer2)
            victoire=true;
        ++nbcoup;
        cout<<"Nombre de coups: "<<nbcoup<<endl;
    }
    if (victoire){
        cout<<"Bien joué!";
        return 1;
    }
    else{
        cout<<"match nul!";
        return 0;
    }
}
int main(){
    ppal();
    return 0;
}
