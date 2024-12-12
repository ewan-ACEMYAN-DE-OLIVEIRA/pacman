#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm> // Pour std::reverse
#include <map> // Pour std::map

using namespace std;

void clearScreen() {
    cout << "\033[H\033[2J";
}

const unsigned KReset(0);
const unsigned KNoir(30);
const unsigned KRouge(31);
const unsigned KVert(32);
const unsigned KJaune(33);
const unsigned KBleu(34);
const unsigned KMagenta(35);
const unsigned KCyan(36);

void couleur(const unsigned &coul) {
    cout << "\033[" << coul << "m";
}

typedef vector<char> CVLine; // Ligne de la grille
typedef vector<CVLine> CMatrix; // Grille
typedef pair<unsigned, unsigned> CPosition; // Coordonnée dans la grille

const char kTokenPlayer1 = 'X';
const char kTokenPlayer2 = 'O';
const char kEmpty = '+';
const char kWall = '#';

void showMatrix(const CMatrix &Mat) {
    clearScreen();
    for (size_t i = 0; i < Mat.size(); ++i) {
        for (size_t j = 0; j < Mat[i].size(); ++j) {
            if (Mat[i][j] == kTokenPlayer1) {
                couleur(KRouge);
                cout << kTokenPlayer1;
                couleur(KReset);
            } else if (Mat[i][j] == kTokenPlayer2) {
                couleur(KBleu);
                cout << kTokenPlayer2;
                couleur(KReset);
            } else if (Mat[i][j] == kWall) {
                couleur(KJaune);
                cout << kWall;
                couleur(KReset);
            } else {
                cout << kEmpty;
            }
        }
        cout << endl;
    }
}

void initMat(CMatrix &Mat, unsigned nbLine, unsigned nbColumn, CPosition &posPlayer1, CPosition &posPlayer2, vector<CPosition> &walls) {
    Mat.resize(nbLine, CVLine(nbColumn, kEmpty));
    Mat[posPlayer1.first][posPlayer1.second] = kTokenPlayer1;
    Mat[posPlayer2.first][posPlayer2.second] = kTokenPlayer2;
    for (const auto &wall : walls) {
        Mat[wall.first][wall.second] = kWall;
    }
}

bool isValidMove(const CMatrix &Mat, int x, int y) {
    return x >= 0 && x < Mat.size() && y >= 0 && y < Mat[0].size() && Mat[x][y] != kWall;
}

struct Node {
    CPosition pos;
    int g, h;
    CPosition parent;
    bool operator>(const Node &other) const {
        return (g + h) > (other.g + other.h);
    }
};

int heuristic(const CPosition &a, const CPosition &b) {
    return abs((int)a.first - (int)b.first) + abs((int)a.second - (int)b.second);
}

vector<CPosition> getNeighbors(const CPosition &pos, const CMatrix &Mat) {
    vector<CPosition> neighbors;
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (const auto &dir : directions) {
        int nx = pos.first + dir.first;
        int ny = pos.second + dir.second;
        if (isValidMove(Mat, nx, ny)) {
            neighbors.emplace_back(nx, ny);
        }
    }
    return neighbors;
}

vector<CPosition> aStar(const CMatrix &Mat, const CPosition &start, const CPosition &goal) {
    priority_queue<Node, vector<Node>, greater<Node>> openSet;
    map<CPosition, Node> allNodes;

    Node startNode{start, 0, heuristic(start, goal), {}};
    openSet.push(startNode);
    allNodes[start] = startNode;

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (current.pos == goal) {
            vector<CPosition> path;
            CPosition step = goal;
            while (step != start) {
                path.push_back(step);
                step = allNodes[step].parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (const auto &neighbor : getNeighbors(current.pos, Mat)) {
            int tentativeG = current.g + 1;
            if (!allNodes.count(neighbor) || tentativeG < allNodes[neighbor].g) {
                Node neighborNode{neighbor, tentativeG, heuristic(neighbor, goal), current.pos};
                openSet.push(neighborNode);
                allNodes[neighbor] = neighborNode;
            }
        }
    }
    return {};
}

void moveAI(CMatrix &Mat, CPosition &posAI, const CPosition &posPlayer1) {
    vector<CPosition> path = aStar(Mat, posAI, posPlayer1);
    if (!path.empty()) {
        Mat[posAI.first][posAI.second] = kEmpty;
        posAI = path.front();
        Mat[posAI.first][posAI.second] = kTokenPlayer2;
    }
}

void moveToken(CMatrix &Mat, char move, CPosition &pos) {
    int x = pos.first;
    int y = pos.second;

    Mat[x][y] = kEmpty; // Enlève l'ancien positionnement du joueur

    switch (tolower(move)) {
    case 'z': // Haut
        if (isValidMove(Mat, x - 1, y)) --x;
        break;
    case 's': // Bas
        if (isValidMove(Mat, x + 1, y)) ++x;
        break;
    case 'q': // Gauche
        if (isValidMove(Mat, x, y - 1)) --y;
        break;
    case 'd': // Droite
        if (isValidMove(Mat, x, y + 1)) ++y;
        break;
    default:
        cout << "Mouvement invalide." << endl;
        break;
    }

    pos = {x, y}; // Met à jour la position
    Mat[x][y] = kTokenPlayer1; // Marque la nouvelle position du joueur
}


int ppal() {
    CMatrix Mat;
    unsigned nbLine = 10;
    unsigned nbColumn = 8;
    CPosition posPlayer1(0, 0);
    CPosition posPlayer2(9, 7);
    vector<CPosition> walls = {{3, 3}, {4, 4}, {5, 5}, {6, 2}};
    const unsigned nbMax = 20;
    unsigned nbCoup = 1;
    bool victoire = false;
    int joueur = 1;

    initMat(Mat, nbLine, nbColumn, posPlayer1, posPlayer2, walls);
    showMatrix(Mat);
    string move;

    while (nbCoup < nbMax && !victoire) {
        cout << "Coup numero " << nbCoup << endl;
        if (joueur == 1) {
            cout << "Coup du joueur 1 : ";
            getline(cin, move);
            moveToken(Mat, move[0], posPlayer1);
            joueur = 2;
        } else {
            moveAI(Mat, posPlayer2, posPlayer1);
            joueur = 1;
        }
        showMatrix(Mat);
        if (posPlayer1 == posPlayer2) {
            victoire = true;
        }
        ++nbCoup;
    }

    if (victoire) {
        cout << "Victoire Royale" << endl;
    } else {
        cout << "Match nul!" << endl;
    }
    return 0;
}

int main() {
    ppal();
    return 0;
}
