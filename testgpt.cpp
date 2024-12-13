#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <termios.h>
#include <unistd.h>
#include <algorithm>
#include <random>

using namespace std;

// Constants
const char kTokenPlayer1 = 'X';
const char kTokenPlayer2 = 'O';
const char kEmpty = '+';
const char kWall = '#';
const char kBonus = '*';
const unsigned kTurnSwitch = 10; // Number of turns before roles switch

typedef vector<char> CVLine; // Line of the grid
typedef vector<CVLine> CMatrix; // Grid
typedef pair<unsigned, unsigned> CPosition; // Position on the grid

// Function prototypes
void displayRules();
void initMatrix(CMatrix &matrix, unsigned rows, unsigned cols, CPosition &player1, CPosition &player2, vector<CPosition> &walls, vector<CPosition> &bonuses);
void showMatrix(const CMatrix &matrix);
void moveToken(CMatrix &matrix, char move, CPosition &pos, char token);
char getKey();
bool isValidMove(const CMatrix &matrix, int x, int y);
void placeRandomElements(CMatrix &matrix, vector<CPosition> &elements, char elementType, unsigned count);
void aiMove(CMatrix &matrix, CPosition &aiPos, const CPosition &targetPos);
bool checkGameOver(const CPosition &player1, const CPosition &player2);

// Termios setup
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
        cout << "Error reading input." << endl;
    }
    restoreEnter();
    return buf;
}

// Display rules
void displayRules() {
    cout << "Bienvenue dans le jeu du Chat et de la Souris ! Voici les règles :\n";
    cout << "- Le joueur 1 est représenté par 'X', le joueur 2 par 'O'.\n";
    cout << "- Les rôles alternent tous les " << kTurnSwitch << " tours.\n";
    cout << "- Déplacez-vous avec Z (haut), S (bas), Q (gauche), D (droite).\n";
    cout << "- Les bonus ('*') permettent de se déplacer deux fois en un tour.\n";
    cout << "- Les murs ('#') bloquent les mouvements.\n";
    cout << "- Le jeu se termine quand un joueur atteint l'autre.\n\n";
}

// Initialize the matrix
void initMatrix(CMatrix &matrix, unsigned rows, unsigned cols, CPosition &player1, CPosition &player2, vector<CPosition> &walls, vector<CPosition> &bonuses) {
    matrix.assign(rows, CVLine(cols, kEmpty));

    // Place players
    matrix[player1.first][player1.second] = kTokenPlayer1;
    matrix[player2.first][player2.second] = kTokenPlayer2;

    // Place walls
    for (const auto &wall : walls) {
        matrix[wall.first][wall.second] = kWall;
    }

    // Place bonuses
    for (const auto &bonus : bonuses) {
        matrix[bonus.first][bonus.second] = kBonus;
    }
}

// Display the matrix
void showMatrix(const CMatrix &matrix) {
    system("clear");
    for (const auto &row : matrix) {
        for (const auto &cell : row) {
            switch (cell) {
                case kTokenPlayer1: cout << "\033[1;31m" << cell << " \033[0m"; break; // Red for Player 1
                case kTokenPlayer2: cout << "\033[1;34m" << cell << " \033[0m"; break; // Blue for Player 2
                case kWall: cout << "\033[1;37m" << cell << " \033[0m"; break; // White for Walls
                case kBonus: cout << "\033[1;33m" << cell << " \033[0m"; break; // Yellow for Bonuses
                default: cout << cell << ' '; break;
            }
        }
        cout << endl;
    }
}

// Validate movement
bool isValidMove(const CMatrix &matrix, int x, int y) {
    return x >= 0 && x < matrix.size() && y >= 0 && y < matrix[0].size() && matrix[x][y] != kWall;
}

// Move a token
void moveToken(CMatrix &matrix, char move, CPosition &pos, char token) {
    int x = pos.first, y = pos.second;
    matrix[x][y] = kEmpty; // Clear current position

    switch (tolower(move)) {
        case 'z': --x; break; // Up
        case 's': ++x; break; // Down
        case 'q': --y; break; // Left
        case 'd': ++y; break; // Right
        default: cout << "Déplacement invalide !\n"; break;
    }

    if (isValidMove(matrix, x, y)) {
        pos = {x, y};
        matrix[x][y] = token;
    } else {
        matrix[pos.first][pos.second] = token; // Revert if invalid
        cout << "Déplacement bloqué !\n";
    }
}

// Place random elements (walls or bonuses)
void placeRandomElements(CMatrix &matrix, vector<CPosition> &elements, char elementType, unsigned count) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> disX(0, matrix.size() - 1);
    uniform_int_distribution<> disY(0, matrix[0].size() - 1);

    for (unsigned i = 0; i < count; ++i) {
        CPosition element;
        do {
            element = {disX(gen), disY(gen)};
        } while (matrix[element.first][element.second] != kEmpty);

        matrix[element.first][element.second] = elementType;
        elements.push_back(element);
    }
}

// AI movement logic
void aiMove(CMatrix &matrix, CPosition &aiPos, const CPosition &targetPos) {
    int dx = targetPos.first - aiPos.first;
    int dy = targetPos.second - aiPos.second;

    int moveX = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
    int moveY = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;

    CPosition nextPos = {aiPos.first + moveX, aiPos.second + moveY};

    if (isValidMove(matrix, nextPos.first, nextPos.second)) {
        matrix[aiPos.first][aiPos.second] = kEmpty;
        aiPos = nextPos;
        matrix[aiPos.first][aiPos.second] = kTokenPlayer2;
    }
}

// Check game over
bool checkGameOver(const CPosition &player1, const CPosition &player2) {
    return player1 == player2;
}

// Main game loop
int main() {
    displayRules();

    unsigned rows, cols;
    cout << "Entrez le nombre de lignes : ";
    cin >> rows;
    cout << "Entrez le nombre de colonnes : ";
    cin >> cols;

    if (rows < 5 || cols < 5) {
        cout << "Les dimensions de la grille doivent être d'au moins 5x5." << endl;
        return 1;
    }

    CMatrix matrix;
    CPosition player1 = {0, 0}, player2 = {rows - 1, cols - 1};
    vector<CPosition> walls, bonuses;

    unsigned wallCount = (rows * cols) / 10; // Adjust wall count based on grid size
    unsigned bonusCount = (rows * cols) / 20; // Adjust bonus count based on grid size

    placeRandomElements(matrix, walls, kWall, wallCount);
    placeRandomElements(matrix, bonuses, kBonus, bonusCount);

    initMatrix(matrix, rows, cols, player1, player2, walls, bonuses);

    unsigned turnCount = 0;
    bool isPlayer1Hunter = true;

    while (true) {
        showMatrix(matrix);

        if (checkGameOver(player1, player2)) {
            cout << (isPlayer1Hunter ? "Joueur 1" : "Joueur 2") << " a attrapé l'autre ! Jeu terminé.\n";
            break;
        }

        cout << (isPlayer1Hunter ? "Joueur 1 (chasseur)" : "Joueur 2 (chasseur)") << ", c'est votre tour !\n";
        if (isPlayer1Hunter) {
            cout << "Déplacez-vous avec Z (haut), S (bas), Q (gauche), D (droite) : ";
            char move = getKey();
            moveToken(matrix, move, player1, kTokenPlayer1);
        } else {
            cout << "Le joueur 2 joue automatiquement...\n";
            aiMove(matrix, player2, player1);
        }

        ++turnCount;
        if (turnCount % kTurnSwitch == 0) {
            isPlayer1Hunter = !isPlayer1Hunter;
            cout << "Changement de rôle : ";
            cout << (isPlayer1Hunter ? "Joueur 1 devient le chasseur.\n" : "Joueur 2 devient le chasseur.\n");
        }
    }

    cout << "Merci d'avoir joué ! À bientôt !\n";
    return 0;
}
