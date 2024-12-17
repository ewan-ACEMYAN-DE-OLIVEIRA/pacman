//Dernières modifications :
//Murs et bonus opérationnels
//La couleur redevient blanche dans le terminal quoi qu'il arrive
//On peut jouer tant qu'on a pas rentré une touche valide
//L'onglet règle quand on appuie sur 'n' est réparé
//Changement de showMatrix() : le booléen "rejouer" est ajouté dans le corps et un if(rejouer){ a été ajouté
//Choix du mode de jeu (beta)
//Les touches Maj fonctionnent aussi

//Problèmes :
//les cout "rejouer" ne s'affichent pas a cause de clearscreen() de showMatrix(Mat) : il faut régler ça

#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <csignal>

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
const char kEmpty        = '+';
const char kWall         = '/';
const char kBonus        = '*';

void  showMatrix (const CMatrix & Mat,bool & rejouer){
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
            else if (Mat[i][j] == kBonus) {
                couleur(KMAgenta);
                cout << kBonus;
                couleur(KReset);
            }
            else{
                cout << kEmpty;
            }
        }
        cout << endl;
    }
    if(rejouer){
        cout <<"BONUS : Veuillez rejouer !"<<endl ;
    }
}
unsigned randomRange (const unsigned& min, const unsigned& max) {
    return min + rand() % (max-min);
}

void initMat (CMatrix & Mat, unsigned nbLine, unsigned nbColumn, CPosition & posPlayer1, CPosition & posPlayer2){
    Mat.resize(nbLine, CVLine(nbColumn, kEmpty));

    unsigned borneMinRandom = 1;
    unsigned borneMaxRandom = 6;

    // Remplir aléatoirement avec des kWall
    for (unsigned i = 0; i < nbLine; ++i) {
        for (unsigned j = 0; j < nbColumn; ++j) {
            if (randomRange(borneMinRandom, borneMaxRandom) == randomRange(borneMinRandom, borneMaxRandom)) {
                Mat[i][j] = kWall;
            }
        }
    }
    borneMinRandom = 1;
    borneMaxRandom = 10;
    for (unsigned i = 0; i < nbLine; ++i) {
        for (unsigned j = 0; j < nbColumn; ++j) {
            if (randomRange(borneMinRandom, borneMaxRandom) == randomRange(borneMinRandom, borneMaxRandom)) {
                Mat[i][j] = kBonus;
            }
        }
    }
    // Placer les joueurs
    Mat[posPlayer1.first][posPlayer1.second] = kTokenPlayer1;
    Mat[posPlayer2.first][posPlayer2.second] = kTokenPlayer2;
}


void moveToken(CMatrix &Mat, char move, CPosition &pos, bool &rejouer) {
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
    if (Mat[pos.first][pos.second] == kBonus) {
        rejouer = true;
        Mat[pos.first][pos.second] = kEmpty;
    } else {
        rejouer = false;
    }
    Mat[pos.first][pos.second] = element;
}

void showRules() {
    cout << endl << "------------------------" << endl << "Règles supplémentaires :\n" << "------------------------" << endl
         << "z : se déplacer vers le haut" << endl
         << "q : se déplacer vers la gauche"<< endl
         << "x : se déplacer vers le bas" << endl
         << "d : se déplacer vers la droite" << endl
         << "a : se déplacer en haut à gauche" << endl
         << "w : se déplacer en bas à gauche"<< endl
         << "c : se  déplacer en bas à droite"<< endl
         << "e : se déplacer en haut à droite" <<endl << "------" << endl;
    cout << "Lore :\n" << "------" << endl
         << "Nous sommes en 2137. Le monde a irrémédiablement changé avec l'avancée rapide de la technologie et l'urbanisation incessante. Les mégapoles s'étendent jusqu'au ciel, citadelles de verre et d'acier. La société est contrôlée par un réseau de drones, de caméras et d'IA de surveillance qui rend presque impossible toute tentative d'évasion de la part d'un criminel.\n"
         << "À la suite de l'événement désastreux connu sous le nom de Grand Effondrement en 2098, les gouvernements nationaux se sont rassemblés sous la bannière de l'Union Terrienne Unifiée. Il s'agit d'une unité politique mondiale régissant toutes les mégapoles et dotée de forces de sécurité omniprésentes, la Division de la sécurité et de l'ordre public (Security and Public Order Division, SPOD).\n"
         << "Les agents de la SPOD forment un cadre professionnel composé des meilleurs agents, formés aux innovations les plus récentes en matière de combat et de surveillance. Leur mission consiste essentiellement à maintenir l'ordre dans les mégapoles et à traquer les criminels qui osent s'opposer à l'autorité de l'UTU, l'Union Terrestre Unifiée.\n"
         << "Mais dans ce monde hyperconnecté et surveillé, il existe une faction de renégats qui s'élèvent contre l'oppression : les Cyber-Outlaws. Il s'agit de bandits spécialisés dans la cybernétique, passés maîtres dans l'art de la furtivité et de l'infiltration. Opérant à l'ombre des gratte-ciel, ils utilisent des implants et des gadgets futuristes pour échapper aux griffes de la SPOD, tout cela pour la chute de l'UTU et le rétablissement de la liberté de l'humanité.\n"
         << "Les Cyber-Outlaws viennent de réussir un hold-up audacieux en volant des données critiques sur une conspiration au sein de l'UEU. Ces informations révèlent des plans clandestins visant à étrangler davantage les citoyens par des mesures répressives. Le SPOD, dirigé par l'infatigable inspecteur Voss, est à leurs trousses, déterminé à récupérer les informations volées et à mettre fin à leurs activités subversives.\n"
         << "Dans « Cyber-Escape : Futuristic Fugitives », les joueurs doivent choisir leur camp, soit celui d'un agent du SPOD, soit celui d'un membre des Cyber-Outlaws.\n"
         << "Les agents de la SPOD doivent utiliser leurs compétences de détection avancées et leur puissance de feu pour traquer et appréhender les hors-la-loi. Les cyber-Outlaws doivent faire preuve de furtivité, de piratage et d'ingéniosité pour échapper au SPOD, mener à bien des missions secrètes et dévoiler les sombres secrets de l'UEU.\n"
         << "Chaque mouvement doit être planifié stratégiquement pour contourner les murs (représentant des obstacles ou des zones surveillées), et les téléportations (déplacements rapides entre les différents niveaux de la ville) doivent être utilisées judicieusement pour éviter les impasses et les pièges tendus par l'ennemi.\n";
}

void resetTerminal(int sig) {
    // Cette fonction permet de réinitialiser l'état du terminal lorsqu'on appuie sur ctrl+c
    // Elle permet surtout d'éviter de conserver la couleur bleu ou rouge lorsqu'on interrompt la partie
    cout << "\033[0m" << endl ;  // Réinitialisation de toutes les propriétés (y compris couleur)
    configureTerminal(true);
    exit(0);  // Quitter proprement le programme
}



int ppal(){
    srand(time(NULL));
    CMatrix Mat;
    unsigned nbLine;
    unsigned nbColumn;
    cout << "Rentrez la largeur de la carte (en ligne) : ";
    cin >> nbLine;
    cout << "Rentrez la hauteur de la carte (en colonne) : ";
    cin >> nbColumn;
    configureTerminal(false); // Désactiver le mode canonique
    signal(SIGINT, resetTerminal);
    CPosition posPlayer1;
    CPosition posPlayer2;
    posPlayer1 = CPosition(0, 0);
    posPlayer2 = CPosition(nbLine-1, nbColumn-1);
    const unsigned nbMax = nbLine * nbColumn;
    unsigned nbCoup= 1;
    bool victoire = false;
    bool rejouer = false;
    int joueur = 1;
    char mode_de_jeu;
    initMat(Mat, nbLine, nbColumn, posPlayer1, posPlayer2);
    showMatrix(Mat,rejouer);
    char move;
    cout << "Appuyez sur 'm' pour commencer le jeu ou 'n' pour lire les règles.\n";
    while (true) {
        move = entree(move);

        if (tolower(move) == 'm') {
            break; // Commencer le jeu
        }
        if (tolower(move) == 'n') {
            showRules();
            cout << endl << "Appuyer sur 'm' pour lancer le jeu" <<endl;
            move=entree(move);
            if (tolower(move) == 'm'){
                showMatrix(Mat,rejouer);
                break ;
            }
            else{
                continue ;
            }
        }

    }

    cout<< endl <<"Choisissez votre mode de jeu:"<<endl;
    cout<<"Appuyez sur 'j' pour commencer le mode 1v1, sur 'k' pour commencer le mode 1vIA ou sur 'l' pour le mode ChasseurVChassé\n";
    while(true){
        move=entree(move);
        if(tolower(move)=='j' || tolower(move)=='k' || tolower(move)=='l'){
            if(tolower(move)=='j'){
                mode_de_jeu='j';
                break;
            }
            if(tolower(move)=='k'){
                mode_de_jeu='k';
                break;
            }
            if(tolower(move)=='l'){
                mode_de_jeu='l';
                break;
            }
        }
        else{
            continue;
        }

    }
    if (mode_de_jeu=='j'){
        showMatrix(Mat,rejouer);
        couleur(KRouge);
        cout << "Coup numero " << nbCoup << endl;
        couleur(KReset);
        ++nbCoup ;


        while (nbCoup < nbMax && !victoire) {

            if (joueur == 1) {
                move = entree(move) ;
                if(tolower(move) == 'a' || tolower(move) == 'z' || tolower(move) == 'e' || tolower(move) == 'd' || tolower(move) == 'c' || tolower(move) == 'x' || tolower(move) == 'w' || tolower(move) == 'q'){
                    moveToken(Mat, move, posPlayer1,rejouer);
                    while (rejouer) {
                        cout << "Rejoue, joueur " << joueur << " : ";
                        showMatrix(Mat,rejouer);
                        move = entree(move) ;
                        cout << endl;
                        moveToken(Mat, move, posPlayer1, rejouer);
                    }

                    joueur = 2;
                }
                else{
                    cout << "Touche invalide : rejouez";
                    continue ;
                }
            }
            else {
                move = entree(move) ;
                if(tolower(move) == 'a' || tolower(move) == 'z' || tolower(move) == 'e' || tolower(move) == 'd' || tolower(move) == 'c' || tolower(move) == 'x' || tolower(move) == 'w' || tolower(move) == 'q'){
                    moveToken(Mat, move, posPlayer2,rejouer);
                    while (rejouer) {
                        cout << "Rejoue, joueur " << joueur << " : ";
                        showMatrix(Mat,rejouer);
                        move = entree(move);
                        cout << endl;
                        moveToken(Mat, move, posPlayer2, rejouer);
                    }
                    joueur = 1;
                }
                else{
                    cout << "Touche invalide : rejouez";
                    continue ;
                }
            }
            showMatrix(Mat,rejouer);
            if (nbCoup % 2 == 0) {
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
            else
                ++nbCoup;
            if(nbCoup%10==0){
                chasseur=(nbCoup/10)%2;
                cout<<"Le chasseur est le joueur: "<<chasseur+1<<endl;
            }
        }
        if (victoire){
            if(nbCoup % 2 == 1 ){
                couleur(KBleu);
            }
            else{
                couleur(KRouge);
            }
            cout<<"Victoire Royale du joueur "<< joueur << " !"<<endl;
            couleur(KReset);
            cout << "Bien joué !" << endl ;
        }
        else{
            cout<<"Match nul!"<<endl;
        }
        return 0;
    }
    else if (mode_de_jeu=='l'){
        showMatrix(Mat,rejouer);
        couleur(KRouge);
        cout << "Coup numero " << nbCoup << endl;
        couleur(KReset);
        ++nbCoup;
        int chasseur = 1;
        while (nbCoup < nbMax && !victoire) {
            if (joueur == 1) {
                move = entree(move) ;
                if(move == 'a' || move == 'z' || move == 'e' || move == 'd' || move == 'c' || move == 'x' || move == 'w' || move == 'q'){
                    moveToken(Mat, move, posPlayer1,rejouer);
                    while (rejouer) {
                        showMatrix(Mat,rejouer);
                        cout << "Rejoue, joueur " << joueur << " : ";
                        move = entree(move) ;
                        cout << endl;
                        moveToken(Mat, move, posPlayer1, rejouer);
                    }
                    joueur = 2;
                }
                else{
                    cout << "Touche invalide : rejouez";
                    continue ;
                }
            }
            else {
                move = entree(move) ;
                if(move == 'a' || move == 'z' || move == 'e' || move == 'd' || move == 'c' || move == 'x' || move == 'w' || move == 'q'){
                    moveToken(Mat, move, posPlayer2,rejouer);
                    while (rejouer) {
                        showMatrix(Mat,rejouer);
                        cout << "Rejoue, joueur " << joueur << " : ";
                        move = entree(move) ;
                        cout << endl;
                        moveToken(Mat, move, posPlayer2, rejouer);
                    }
                    joueur = 1;
                }
                else{
                    cout << "Touche invalide : rejouez";
                    continue ;
                }
            }
            showMatrix(Mat,rejouer);
            if (nbCoup % 2 == 0) {
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
            else
                ++nbCoup;
        }
        if (victoire)
            cout<<"Le chasseur a gagné!"<<endl;
        else
            cout<<"Le chassé a gagné"<<endl;
    }

}

int main() {
    ppal();
    configureTerminal(true);
    return 0;
}
