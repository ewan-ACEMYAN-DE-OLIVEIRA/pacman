#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <ctime>
#include <termios.h>
#include <unistd.h>

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

typedef vector<char> CVLine;
typedef vector<CVLine> CMatrix;
typedef pair<unsigned, unsigned> CPosition;

const char kTokenPlayer1 = 'X';
const char kTokenPlayer2 = 'O';
const char kEmpty = '+';
const char kWall = '#';
const char kBonus = '*';

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
    // Ajout des directions pour les déplacements diagonaux
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for (const auto &dir : directions) {
        int nx = pos.first + dir.first;
        int ny = pos.second + dir.second;
        if (nx >= 0 && nx < Mat.size() && ny >= 0 && ny < Mat[0].size() && Mat[nx][ny] != kWall) {
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

    Mat[x][y] = kEmpty;

    switch (tolower(move)) {
    case 'z': // Haut
        if (x > 0 && Mat[x - 1][y] != kWall) --x;
        break;
    case 's': // Bas
        if (x < Mat.size() - 1 && Mat[x + 1][y] != kWall) ++x;
        break;
    case 'q': // Gauche
        if (y > 0 && Mat[x][y - 1] != kWall) --y;
        break;
    case 'd': // Droite
        if (y < Mat[0].size() - 1 && Mat[x][y + 1] != kWall) ++y;
        break;
    case 'a': // Diagonale haut-gauche
        if (x > 0 && y > 0 && Mat[x - 1][y - 1] != kWall) { --x; --y; }
        break;
    case 'e': // Diagonale haut-droite
        if (x > 0 && y < Mat[0].size() - 1 && Mat[x - 1][y + 1] != kWall) { --x; ++y; }
        break;
    case 'w': // Diagonale bas-gauche
        if (x < Mat.size() - 1 && y > 0 && Mat[x + 1][y - 1] != kWall) { ++x; --y; }
        break;
    case 'c': // Diagonale bas-droite
        if (x < Mat.size() - 1 && y < Mat[0].size() - 1 && Mat[x + 1][y + 1] != kWall) { ++x; ++y; }
        break;
    default:
        cout << "Mouvement invalide." << endl;
        break;
    }

    pos = {x, y};
    Mat[x][y] = kTokenPlayer1;
}

void initMat(CMatrix &Mat, unsigned nbLine, unsigned nbColumn, CPosition &posPlayer1, CPosition &posPlayer2, vector<CPosition> &walls, vector<CPosition> &bonus) {
    Mat.resize(nbLine, CVLine(nbColumn, kEmpty));
    Mat[posPlayer1.first][posPlayer1.second] = kTokenPlayer1;
    Mat[posPlayer2.first][posPlayer2.second] = kTokenPlayer2;
    for (const auto &wall : walls) {
        Mat[wall.first][wall.second] = kWall;
    }
    for (const auto &b : bonus) {
        Mat[b.first][b.second] = kBonus;
    }
}

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
            } else if (Mat[i][j] == kBonus) {
                couleur(KMagenta);
                cout << kBonus;
                couleur(KReset);
            } else {
                cout << kEmpty;
            }
        }
        cout << endl;
    }
}

void displayRules() {
    cout << "Bienvenue dans le jeu ! Voici les règles :\n";
    cout << "- Le joueur 1 est humain, le joueur 2 est l'IA.\n";
    cout << "- Vous pouvez vous déplacer avec les touches Z, S, Q, D (haut, bas, gauche, droite), ou les touches pour les déplacements diagonaux : A, E, W, C.\n";
    cout << "- Des bonus apparaissent aléatoirement sur la grille. Vous pouvez déplacer deux fois si vous touchez un bonus.\n";
    cout << "- Des murs sont présents pour bloquer les déplacements.\n";
    cout << "- L'IA tente de vous capturer en vous 'mangeant' (en atteignant votre position).\n";
    cout << "- Le jeu se termine quand un joueur atteint l'autre joueur.\n\n";
}

void disableEnter() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void restoreEnter() {
    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    oldt.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

char getKey() {
    char buf = 0;
    disableEnter();
    if (read(STDIN_FILENO, &buf, 1) < 0) {
        cout << "Erreur de lecture.\n";
    }
    restoreEnter();
    return buf;
}

int main() {
    unsigned nbLine, nbColumn;
    displayRules();
    cout << "Entrez le nombre de lignes de la grille : ";
    cin >> nbLine;
    cout << "Entrez le nombre de colonnes de la grille : ";
    cin >> nbColumn;

    srand(time(0));

    CMatrix Mat;
    CPosition posPlayer1(0, 0), posPlayer2(nbLine - 1, nbColumn - 1);
    vector<CPosition> walls = {{3, 3}, {4, 4}, {5, 5}, {6, 2}};
    vector<CPosition> bonus = {{2, 2}, {7, 5}};
    const unsigned nbMax = nbLine * nbColumn; // L'aire de la matrice détermine le nombre de coups maximum
    unsigned nbCoup = 1;
    bool victoire = false;
    int joueur = 1;

    initMat(Mat, nbLine, nbColumn, posPlayer1, posPlayer2, walls, bonus);
    showMatrix(Mat);
    string move;

    while (nbCoup < nbMax && !victoire) {
        cout << "Coup numero " << nbCoup << endl;
        if (joueur == 1) {
            cout << "Coup du joueur 1 : ";
            char moveChar = getKey();
            moveToken(Mat, moveChar, posPlayer1);
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
        cout << "Victoire Royale\n";
    } else {
        cout << "Match nul!\n";
    }

    return 0;
}
