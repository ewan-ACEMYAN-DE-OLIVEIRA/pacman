#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>

using namespace std;

void clearScreen() {
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

void couleur(const unsigned &coul) {
    cout << "\033[" << coul << "m";
}

typedef vector<char> CVLine; // un type représentant une ligne de la grille
typedef vector<CVLine> CMatrix; // un type représentant la grille
typedef pair<unsigned, unsigned> CPosition; // un type représentant une coordonnée dans la grille

void configureTerminal(bool enable) {
    struct termios touche;
    tcgetattr(STDIN_FILENO, &touche);
    if(enable) {
        touche.c_lflag |= ICANON;
        touche.c_lflag |= ECHO;
    } else {
        touche.c_lflag &= ~ICANON;
        touche.c_lflag &= ~ECHO;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &touche);
}

int entree(char c) {
    read(STDIN_FILENO, &c, 1); // lire caractère
    return c;
}

const char kTokenPlayer1 = 'X';
const char kTokenPlayer2 = 'O';
const char kEmpty = '+';

void showMatrix(const CMatrix &Mat) {
    clearScreen();
    for (auto i = 0; i < Mat.size(); ++i) {
        for (auto j = 0; j < Mat[i].size(); ++j) {
            if (Mat[i][j] == kTokenPlayer1) {
                couleur(KRouge);
                cout << kTokenPlayer1;
                couleur(KReset);
            }
            else if (Mat[i][j] == kTokenPlayer2) {
                couleur(KBleu);
                cout << kTokenPlayer2;
                couleur(KReset);
            }
            else {
                cout << kEmpty;
            }
        }
        cout << endl;
    }
}

void initMat(CMatrix &Mat, unsigned nbLine, unsigned nbColumn, CPosition &posPlayer1, CPosition &posPlayer2) {
    Mat.resize(nbLine, CVLine(nbColumn, kEmpty));
    Mat[posPlayer1.first][posPlayer1.second] = kTokenPlayer1;
    Mat[posPlayer2.first][posPlayer2.second] = kTokenPlayer2;
}

void moveToken(CMatrix &Mat, char move, CPosition &pos) {
    char element = Mat[pos.first][pos.second];
    Mat[pos.first][pos.second] = kEmpty;
    switch (tolower(move)) {
    case 'a':
        if (!(pos.first < 1 || pos.second < 1)) {
            pos.first -= 1;
            pos.second -= 1;
        }
        break;
    case 'z':
        if (pos.first < 1) {
            pos.first = Mat.size() - 1;
        }
        else {
            pos.first -= 1;
        }
        break;
    case 'e':
        if (!(pos.first < 1 || pos.second > Mat[0].size() - 2)) {
            pos.first -= 1;
            pos.second += 1;
        }
        break;
    case 'q':
        if (pos.second < 1) {
            pos.second = Mat[0].size() - 1;
        }
        else {
            pos.second -= 1;
        }
        break;
    case 'd':
        if (pos.second > Mat[0].size() - 2) {
            pos.second = 0;
        }
        else {
            pos.second += 1;
        }
        break;
    case 'w':
        if (!(pos.first > Mat.size() - 2 || pos.second < 1)) {
            pos.first += 1;
            pos.second -= 1;
        }
        break;
    case 'x':
        if (pos.first > Mat.size() - 2) {
            pos.first = 0;
        }
        else {
            pos.first += 1;
        }
        break;
    case 'c':
        if (!(pos.first > Mat.size() - 2 || pos.second > Mat[0].size() - 2)) {
            pos.first += 1;
            pos.second += 1;
        }
        break;
    }
    Mat[pos.first][pos.second] = element;
}

void showRules() {
    cout << "Règles supplémentaires :\n";
    cout << "Lore :\n"
         << "Nous sommes en 2137. Le monde a irrémédiablement changé avec l'avancée rapide de la technologie et l'urbanisation incessante. Les mégapoles s'étendent jusqu'au ciel, citadelles de verre et d'acier. La société est contrôlée par un réseau de drones, de caméras et d'IA de surveillance qui rend presque impossible toute tentative d'évasion de la part d'un criminel.\n"
         << "À la suite de l'événement désastreux connu sous le nom de Grand Effondrement en 2098, les gouvernements nationaux se sont rassemblés sous la bannière de l'Union Terrienne Unifiée. Il s'agit d'une unité politique mondiale régissant toutes les mégapoles et dotée de forces de sécurité omniprésentes, la Division de la sécurité et de l'ordre public (Security and Public Order Division, SPOD).\n"
         << "Les agents de la SPOD forment un cadre professionnel composé des meilleurs agents, formés aux innovations les plus récentes en matière de combat et de surveillance. Leur mission consiste essentiellement à maintenir l'ordre dans les mégapoles et à traquer les criminels qui osent s'opposer à l'autorité de l'UTU, l'Union Terrestre Unifiée.\n"
         << "Mais dans ce monde hyperconnecté et surveillé, il existe une faction de renégats qui s'élèvent contre l'oppression : les Cyber-Outlaws. Il s'agit de bandits spécialisés dans la cybernétique, passés maîtres dans l'art de la furtivité et de l'infiltration. Opérant à l'ombre des gratte-ciel, ils utilisent des implants et des gadgets futuristes pour échapper aux griffes de la SPOD, tout cela pour la chute de l'UTU et le rétablissement de la liberté de l'humanité.\n"
         << "Les Cyber-Outlaws viennent de réussir un hold-up audacieux en volant des données critiques sur une conspiration au sein de l'UEU. Ces informations révèlent des plans clandestins visant à étrangler davantage les citoyens par des mesures répressives. Le SPOD, dirigé par l'infatigable inspecteur Voss, est à leurs trousses, déterminé à récupérer les informations volées et à mettre fin à leurs activités subversives.\n"
         << "Dans « Cyber-Escape : Futuristic Fugitives », les joueurs doivent choisir leur camp, soit celui d'un agent du SPOD, soit celui d'un membre des Cyber-Outlaws.\n"
         << "Les agents de la SPOD doivent utiliser leurs compétences de détection avancées et leur puissance de feu pour traquer et appréhender les hors-la-loi. Les cyber-Outlaws doivent faire preuve de furtivité, de piratage et d'ingéniosité pour échapper au SPOD, mener à bien des missions secrètes et dévoiler les sombres secrets de l'UEU.\n"
         << "Chaque mouvement doit être planifié stratégiquement pour contourner les murs (représentant des obstacles ou des zones surveillées), et les téléportations (déplacements rapides entre les différents niveaux de la ville) doivent être utilisées judicieusement pour éviter les impasses et les pièges tendus par l'ennemi.\n";
}

int ppal() {
    CMatrix Mat;
    unsigned nbLine;
    unsigned nbColumn;
    cout << "Rentrez la largeur de la carte (en ligne) : ";
    cin >> nbLine;
    cout << "Rentrez la hauteur de la carte (en colonne) : ";
    cin >> nbColumn;
    configureTerminal(false); // Désactiver le mode canonique
    CPosition posPlayer1;
    CPosition posPlayer2;
    posPlayer1 = CPosition(0, 0);
    posPlayer2 = CPosition(nbLine - 1, nbColumn - 1);
    const unsigned nbMax = 20;
    unsigned nbCoup = 1;
    bool victoire = false;
    string joueur = "bleu";
    initMat(Mat, nbLine, nbColumn, posPlayer1, posPlayer2);
    showMatrix(Mat);
    char move;
    cout << "Appuyez sur 'm' pour commencer le jeu ou 'n' pour lire les règles.\n";

    while (true) {
        move = entree(move);

        if (move == 'm') {
            break; // Commencer le jeu
        }
        if (move == 'n') {
            showRules();
        }

    }
    while (nbCoup < nbMax && !victoire) {
        move = entree(move);
        if (move == 27 || move == 8 || move == 127 || move == 13 || move == 10) { // le code ASCII des touches pour quitter
            cout << "\nQuitter...\n";
            break;
        }
        cout << endl;
        if (nbCoup % 2 == 1) {
            moveToken(Mat, move, posPlayer1);
            joueur = "rouge";
        }
        else {
            moveToken(Mat, move, posPlayer2);
            joueur = " bleu";
        }
        showMatrix(Mat);
        if (nbCoup % 2 == 1) {
            couleur(KBleu);
            cout << "Coup numero " << nbCoup << endl;
            couleur(KReset);
        }
        else {
            couleur(KRouge);
            cout << "Coup numero " << nbCoup << endl;
            couleur(KReset);
        }
        cout << "Coup du joueur " << joueur << " : ";
        if (posPlayer1 == posPlayer2)
            victoire = true;
        ++nbCoup;
    }

    if (victoire) {
        cout << "Victoire Royale" << endl;
    }
    else {
        cout << "Match nul!" << endl;
    }

    return 0;
}

int main() {
    ppal();
    configureTerminal(true);    // Restaurer le terminal
    return 0;
}
