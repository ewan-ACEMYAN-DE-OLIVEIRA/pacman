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

// Fonction pour lire une seule touche sans avoir besoin d'appuyer sur Entrée
char getKey() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt); // Sauvegarde des paramètres de terminal
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Désactive le mode canonique et l'écho
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Applique les nouveaux paramètres
    ch = getchar(); // Récupère un caractère
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restaure les paramètres de terminal
    return ch; // Retourne le caractère
}

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
const char kBonus = '*';
const char kBonusRed = 'R'; // Bonus de 2 tours
const char kBonusGreen = 'G'; // Bonus de 3 tours
const char kBonusMagenta = 'M'; // Bonus de 1 tour

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
            } else if (Mat[i][j] == kBonusRed) {
                couleur(KRouge);
                cout << kBonusRed;
                couleur(KReset);
            } else if (Mat[i][j] == kBonusGreen) {
                couleur(KVert);
                cout << kBonusGreen;
                couleur(KReset);
            } else if (Mat[i][j] == kBonusMagenta) {
                couleur(KMagenta);
                cout << kBonusMagenta;
                couleur(KReset);
            } else {
                cout << kEmpty;
            }
        }
        cout << endl;
    }
}

void initMat(CMatrix &Mat, unsigned nbLine, unsigned nbColumn, CPosition &posPlayer1, CPosition &posPlayer2, unsigned nbWalls, unsigned nbBonus) {
    Mat.resize(nbLine, CVLine(nbColumn, kEmpty));
    Mat[posPlayer1.first][posPlayer1.second] = kTokenPlayer1;
    Mat[posPlayer2.first][posPlayer2.second] = kTokenPlayer2;

    srand(time(0));

    // Générer des murs
    for (unsigned i = 0; i < nbWalls; ++i) {
        unsigned x = rand() % nbLine;
        unsigned y = rand() % nbColumn;
        if (Mat[x][y] == kEmpty) {
            Mat[x][y] = kWall;
        } else {
            --i; // Si la case est déjà occupée, refaire un essai
        }
    }

    // Générer des bonus
    for (unsigned i = 0; i < nbBonus; ++i) {
        unsigned x = rand() % nbLine;
        unsigned y = rand() % nbColumn;
        if (Mat[x][y] == kEmpty) {
            int bonusType = rand() % 3;
            if (bonusType == 0) Mat[x][y] = kBonusMagenta;  // Bonus de 1 tour
            else if (bonusType == 1) Mat[x][y] = kBonusRed; // Bonus de 2 tours
            else Mat[x][y] = kBonusGreen; // Bonus de 3 tours
        } else {
            --i; // Si la case est déjà occupée, refaire un essai
        }
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
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
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

void moveToken(CMatrix &Mat, char move, CPosition &pos, bool &extraTurns) {
    int x = pos.first;
    int y = pos.second;

    Mat[x][y] = kEmpty; // Enlève l'ancien positionnement du joueur

    switch (tolower(move)) {
    case 'z': // Haut
        if (isValidMove(Mat, x - 1, y)) --x;
        else extraTurns = true; // Effet miroir sur le haut
        break;
    case 's': // Bas
        if (isValidMove(Mat, x + 1, y)) ++x;
        else extraTurns = true; // Effet miroir sur le bas
        break;
    case 'q': // Gauche
        if (isValidMove(Mat, x, y - 1)) --y;
        else extraTurns = true; // Effet miroir sur la gauche
        break;
    case 'd': // Droite
        if (isValidMove(Mat, x, y + 1)) ++y;
        else extraTurns = true; // Effet miroir sur la droite
        break;
    default:
        cout << "Mouvement invalide." << endl;
        break;
    }

    pos = {x, y}; // Met à jour la position
    Mat[x][y] = kTokenPlayer1; // Marque la nouvelle position du joueur
}

void handleBonus(CMatrix &Mat, CPosition &pos, int &extraTurns) {
    if (Mat[pos.first][pos.second] == kBonusMagenta) {
        cout << "Vous avez gagné 1 tour supplémentaire !\n";
        Mat[pos.first][pos.second] = kEmpty; // Retire le bonus de la grille
        extraTurns = 1;
    } else if (Mat[pos.first][pos.second] == kBonusRed) {
        cout << "Vous avez gagné 2 tours supplémentaires !\n";
        Mat[pos.first][pos.second] = kEmpty; // Retire le bonus de la grille
        extraTurns = 2;
    } else if (Mat[pos.first][pos.second] == kBonusGreen) {
        cout << "Vous avez gagné 3 tours supplémentaires !\n";
        Mat[pos.first][pos.second] = kEmpty; // Retire le bonus de la grille
        extraTurns = 3;
    }
}

void switchRoles(CPosition &posPlayer1, CPosition &posPlayer2) {
    swap(posPlayer1, posPlayer2);
}

void displayRules() {
    cout << "Bienvenue dans le jeu ! Voici les règles :\n";
    cout << "- Utilisez les touches Z, S, Q, D pour vous déplacer.\n";
    cout << "- Vous pouvez aussi vous déplacer en diagonale avec A, E, W, C.\n";
    cout << "- Les murs bloquent votre déplacement.\n";
    cout << "- L'objectif est de capturer l'autre joueur.\n";
    cout << "- Des murs et des bonus apparaissent aléatoirement.\n";
    cout << "- L'effet miroir : si vous essayez de vous déplacer hors de la grille, vous êtes "
         << "ramené à l'autre côté.\n";
    cout << "- Les bonus : \n";
    cout << "  * Bonus Magenta : 1 tour supplémentaire\n";
    cout << "  * Bonus Rouge : 2 tours supplémentaires\n";
    cout << "  * Bonus Vert : 3 tours supplémentaires\n";
    cout << "- Le rôle de 'chat' et 'souris' change tous les X tours !\n";
    cout << endl;
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
    unsigned nbWalls = (nbLine * nbColumn) / 10;  // 10% de la carte
    unsigned nbBonus = (nbLine * nbColumn) / 15;  // 6.67% de la carte

    initMat(Mat, nbLine, nbColumn, posPlayer1, posPlayer2, nbWalls, nbBonus);
    showMatrix(Mat);

    string move;
    unsigned nbCoup = 1;
    int joueur = 1;
    bool victoire = false;
    int extraTurns1 = 0, extraTurns2 = 0; // Nombre de tours supplémentaires pour chaque joueur
    int chatTour = 10;  // Nombre de tours avant de changer les rôles de chat et souris

    while (nbCoup < nbLine * nbColumn && !victoire) {
        cout << "Coup numéro " << nbCoup << endl;

        if (extraTurns1 > 0) {
            cout << "Le joueur 1 a " << extraTurns1 << " tour(s) supplémentaire(s).\n";
            --extraTurns1;
            continue;
        }

        if (extraTurns2 > 0) {
            cout << "Le joueur 2 a " << extraTurns2 << " tour(s) supplémentaire(s).\n";
            --extraTurns2;
            continue;
        }

        if (joueur == 1) {
            cout << "Joueur 1, votre tour !\n";
            cout << "Déplacez-vous (Z/S/Q/D) : ";
            move = getKey();
            moveToken(Mat, move[0], posPlayer1, extraTurns1);
            handleBonus(Mat, posPlayer1, extraTurns1);
            showMatrix(Mat);
            joueur = 2;
        } else {
            cout << "Joueur 2, votre tour !\n";
            cout << "Déplacez-vous (Z/S/Q/D) : ";
            move = getKey();
            moveToken(Mat, move[0], posPlayer2, extraTurns2);
            handleBonus(Mat, posPlayer2, extraTurns2);
            showMatrix(Mat);
            joueur = 1;
        }

        // Vérification des rôles de chat et souris tous les 10 tours
        if (nbCoup % chatTour == 0) {
            switchRoles(posPlayer1, posPlayer2);
            cout << "Les rôles de 'chat' et 'souris' ont été échangés !" << endl;
        }

        ++nbCoup;
    }
    return 0;
}
