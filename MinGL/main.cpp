#define FPS_LIMIT 60

#include <iostream>
#include <thread>
#include <vector>
#include "mingl/include/mingl/mingl.h"
#include "mingl/include/mingl/gui/text.h"
#include "mingl/include/mingl/shape/line.h"
#include "mingl/include/mingl/shape/rectangle.h"

using namespace std;
vector <size_t> posWall;

nsGraphics::Vec2D rectPosJ1;
nsGraphics::Vec2D rectPosJ2;

bool collisionJoueur = false;

const unsigned Kvitesse = 2;

bool isGameOver = false;

const int Ktaille = 640; //taille jeu

//Position mur
const nsGraphics::Vec2D PosWallY(100, 100);
const nsGraphics::Vec2D PosWallX(110, 140);
const nsGraphics::Vec2D PosWallY2(330, 120);
const nsGraphics::Vec2D PosWallX2(340, 180);
const nsGraphics::Vec2D PosWallY3(470, 350);
const nsGraphics::Vec2D PosWallX3(475, 500);
const nsGraphics::Vec2D PosWallY4(200, 400);
const nsGraphics::Vec2D PosWallX4(260, 410);
const nsGraphics::Vec2D PosWallY5(80, 300);
const nsGraphics::Vec2D PosWallX5(150, 310);
const nsGraphics::Vec2D PosWallY6(500, 100);
const nsGraphics::Vec2D PosWallX6(550, 115);
const nsGraphics::Vec2D PosWallY7(230, 540);
const nsGraphics::Vec2D PosWallX7(240, 600);

//Position buisson
const nsGraphics::Vec2D buisson1X(300, 300);
const nsGraphics::Vec2D buisson1Y(350, 350);
const nsGraphics::Vec2D buisson2X(400, 200);
const nsGraphics::Vec2D buisson2Y(450, 250);
const nsGraphics::Vec2D buisson3X(150, 150);
const nsGraphics::Vec2D buisson3Y(200, 200);
const nsGraphics::Vec2D buisson4X(400, 520);
const nsGraphics::Vec2D buisson4Y(480, 600);

chrono::time_point<chrono::steady_clock> startTime;

bool areRectanglesColliding(const nsGraphics::Vec2D &rect1TopLeft, const nsGraphics::Vec2D &rect1BottomRight,
                            const nsGraphics::Vec2D &rect2TopLeft, const nsGraphics::Vec2D &rect2BottomRight)
{
    return !(rect1BottomRight.getX() <= rect2TopLeft.getX() ||  // J1 est à gauche de J2
             rect1TopLeft.getX() >= rect2BottomRight.getX() ||  // J1 est à droite de J2
             rect1BottomRight.getY() <= rect2TopLeft.getY() ||  // J1 est au-dessus de J2
             rect1TopLeft.getY() >= rect2BottomRight.getY());   // J1 est en dessous de J2
}
bool detectCollisionWithWalls(const nsGraphics::Vec2D &playerTopLeft, const nsGraphics::Vec2D &playerBottomRight)
{
    // Liste mur
    vector<pair<nsGraphics::Vec2D, nsGraphics::Vec2D>> walls = {
        {PosWallY, PosWallX},
        {PosWallY2, PosWallX2},
        {PosWallY3, PosWallX3},
        {PosWallY4, PosWallX4},
        {PosWallY5, PosWallX5},
        {PosWallY6, PosWallX6},
        {PosWallY7, PosWallX7}
    };
    // Vérifie collisions murs
    for (size_t i = 0; i < walls.size(); ++i) {
        if (areRectanglesColliding(playerTopLeft, playerBottomRight, walls[i].first, walls[i].second)){
            return true;
        }
    }
    return false; // Pas de collision
}
void clavierJ1(MinGL &window){
   //Deplacement J1
    int posFuture;

    if (window.isPressed({'z', false})) { // Haut
        posFuture = rectPosJ1.getY() - Kvitesse;
        if (posFuture >= 0) {
            nsGraphics::Vec2D futurePos(rectPosJ1.getX(), posFuture);
            nsGraphics::Vec2D futureBottomRight = futurePos + nsGraphics::Vec2D(20, 20);
            if (!detectCollisionWithWalls(futurePos, futureBottomRight))
                rectPosJ1.setY(posFuture);
        }
    }
    if (window.isPressed({'s', false})) { // Bas
        posFuture = rectPosJ1.getY() + Kvitesse;
        if (posFuture + 20 <= Ktaille) {
            nsGraphics::Vec2D futurePos(rectPosJ1.getX(), posFuture);
            nsGraphics::Vec2D futureBottomRight = futurePos + nsGraphics::Vec2D(20, 20);
            if (!detectCollisionWithWalls(futurePos, futureBottomRight))
                rectPosJ1.setY(posFuture);
        }
    }

    if (window.isPressed({'q', false})) { // Gauche
        posFuture = rectPosJ1.getX() - Kvitesse;
        if (posFuture >= 0) {
            nsGraphics::Vec2D futurePos(posFuture, rectPosJ1.getY());
            nsGraphics::Vec2D futureBottomRight = futurePos + nsGraphics::Vec2D(20, 20);
            if (!detectCollisionWithWalls(futurePos, futureBottomRight))
                rectPosJ1.setX(posFuture);
        }
    }

    if (window.isPressed({'d', false})) { // Droite
        posFuture = rectPosJ1.getX() + Kvitesse;
        if (posFuture + 20 <= Ktaille) {
            nsGraphics::Vec2D futurePos(posFuture, rectPosJ1.getY());
            nsGraphics::Vec2D futureBottomRight = futurePos + nsGraphics::Vec2D(20, 20);
            if (!detectCollisionWithWalls(futurePos, futureBottomRight))
                rectPosJ1.setX(posFuture);
        }
    }
}
void clavierJ2(MinGL &window){
    //Deplacement J2
    int posFuture;

    if (window.isPressed({'o', false})) { // Haut
        posFuture = rectPosJ2.getY() - Kvitesse;
        if (posFuture >= 0) {
            nsGraphics::Vec2D futurePos(rectPosJ2.getX(), posFuture);
            nsGraphics::Vec2D futureBottomRight = futurePos + nsGraphics::Vec2D(20, 20);
            if (!detectCollisionWithWalls(futurePos, futureBottomRight))
                rectPosJ2.setY(posFuture);
        }
    }

    if (window.isPressed({'l', false})) { // Bas
        posFuture = rectPosJ2.getY() + Kvitesse;
        if (posFuture + 20 <= Ktaille) {
            nsGraphics::Vec2D futurePos(rectPosJ2.getX(), posFuture);
            nsGraphics::Vec2D futureBottomRight = futurePos + nsGraphics::Vec2D(20, 20);
            if (!detectCollisionWithWalls(futurePos, futureBottomRight))
                rectPosJ2.setY(posFuture);
        }
    }

    if (window.isPressed({'k', false})) { // Gauche
        posFuture = rectPosJ2.getX() - Kvitesse;
        if (posFuture >= 0) {
            nsGraphics::Vec2D futurePos(posFuture, rectPosJ2.getY());
            nsGraphics::Vec2D futureBottomRight = futurePos + nsGraphics::Vec2D(20, 20);
            if (!detectCollisionWithWalls(futurePos, futureBottomRight))
                rectPosJ2.setX(posFuture);
        }
    }

    if (window.isPressed({'m', false})) { // Droite
        posFuture = rectPosJ2.getX() + Kvitesse;
        if (posFuture + 20 <= Ktaille) {
            nsGraphics::Vec2D futurePos(posFuture, rectPosJ2.getY());
             nsGraphics::Vec2D futureBottomRight = futurePos + nsGraphics::Vec2D(20, 20);
            if (!detectCollisionWithWalls(futurePos, futureBottomRight))
                rectPosJ2.setX(posFuture);
        }
    }
}
// Aléatoire des murs et des buissons SOON !!!!!
// unsigned randomRange (const unsigned& min, const unsigned& max) {
//     return min + rand() % (max-min);
// }
// void Pos(){
//     for (int i = 0; i < 20; ++i) {
//         posWall.push_back(randomRange(10, Ktaille-40));
//     }
// }
void dessiner(MinGL &window)
{
    // DessinJ1
    window << nsShape::Rectangle(rectPosJ1, rectPosJ1 + nsGraphics::Vec2D(20, 20), nsGraphics::KCyan);

    // Dessin J2
    window << nsShape::Rectangle(rectPosJ2, rectPosJ2 + nsGraphics::Vec2D(20, 20), nsGraphics::KRed);

    // Dessiner les obstacles et buissons

    window << nsShape::Rectangle(buisson1X, buisson1Y, nsGraphics::KGreen);
    window << nsShape::Rectangle(buisson2X, buisson2Y, nsGraphics::KGreen);
    window << nsShape::Rectangle(buisson3X, buisson3Y, nsGraphics::KGreen);
    window << nsShape::Rectangle(buisson4X, buisson4Y, nsGraphics::KGreen);

    window << nsShape::Rectangle(PosWallX, PosWallY, nsGraphics::KBlack);
    window << nsShape::Rectangle(PosWallX2, PosWallY2, nsGraphics::KBlack);
    window << nsShape::Rectangle(PosWallX3, PosWallY3, nsGraphics::KBlack);
    window << nsShape::Rectangle(PosWallX4, PosWallY4, nsGraphics::KBlack);
    window << nsShape::Rectangle(PosWallX5, PosWallY5, nsGraphics::KBlack);
    window << nsShape::Rectangle(PosWallX6, PosWallY6, nsGraphics::KBlack);
    window << nsShape::Rectangle(PosWallX7, PosWallY7, nsGraphics::KBlack);
}
bool detectCollisionEntreJoueur(){
    string collision;
    bool collisionJoueur = false;

    nsGraphics::Vec2D rect1BottomRight = rectPosJ1 + nsGraphics::Vec2D(20, 20);
    nsGraphics::Vec2D rect2BottomRight = rectPosJ2 + nsGraphics::Vec2D(20, 20);

    // Vérifie si J1 sur J2
    if (areRectanglesColliding(rectPosJ1, rect1BottomRight, rectPosJ2, rect2BottomRight)){
        collision = "Collision entre les joueurs !";
        collisionJoueur = true;
    }
    else{
        collision = "Pas de collision entre les joueurs !";
        collisionJoueur = false;
    }
    return collisionJoueur;
}
void affichageFin(MinGL &window, string &timerText){
    int textWidth = 220;
    int textHeight = 50;
    int offsetX = 220;
    int offsetY = 150;
    int padding = 10;

    // Fond txt
    window << nsShape::Rectangle(
        nsGraphics::Vec2D(offsetX - padding, offsetY - padding),
        nsGraphics::Vec2D(offsetX + textWidth + padding, offsetY + textHeight + padding),
        nsGraphics::KBlack // Couleur du fond
        );

    // Affiche le temps
    window << nsGui::Text(nsGraphics::Vec2D(230, 200), timerText, nsGraphics::KWhite, nsGui::GlutFont::BITMAP_HELVETICA_12);
    window << nsGui::Text(nsGraphics::Vec2D(250, 160), "Victoire du joueur 1", nsGraphics::KWhite, nsGui::GlutFont::BITMAP_HELVETICA_18);
}
void affichageRejouer(MinGL &window){
    // Dimensions txt
    int textWidth = 280;
    int textHeight = 50;
    int offsetX = 200;
    int offsetY = 420;
    int padding = 10;

    // Dessiner un rectangle de fond derrière le texte
    window << nsShape::Rectangle(
        nsGraphics::Vec2D(offsetX - padding, offsetY - padding),
        nsGraphics::Vec2D(offsetX + textWidth + padding, offsetY + textHeight + padding),
        nsGraphics::KBlue
        );

    window << nsGui::Text(nsGraphics::Vec2D(200, 450), "Appuyez sur Espace pour rejouer", nsGraphics::KWhite, nsGui::GlutFont::BITMAP_HELVETICA_18);
}
void resetGame() {
    rectPosJ1.setX(0);
    rectPosJ1.setY(0);

    rectPosJ2.setX(Ktaille - 20);
    rectPosJ2.setY(Ktaille - 20);

    startTime = chrono::steady_clock::now(); // Réinitialisation du timer
    isGameOver = false; // Le jeu redémarre
}
void actionFrame(MinGL &window){
    bool collision = detectCollisionEntreJoueur();

    static long lastElapsedTime = 0; //Variable chrono
    string timerText = "Le joueur 2 a survecu pendant : " + to_string(lastElapsedTime)  + "s !";

    if (!collision) {
        // MAJ position joueur
        clavierJ1(window);
        clavierJ2(window);

        // Calcule le temps écoulé
        auto currentTime = chrono::steady_clock::now();
        lastElapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();

        // Affiche le temps
        timerText = "Temps: " + to_string(lastElapsedTime) + "s";
        window << nsGui::Text(nsGraphics::Vec2D(20, 50), timerText, nsGraphics::KWhite, nsGui::GlutFont::BITMAP_9_BY_15);
    }

    // Affichage du jeu
    dessiner(window);
    if (collision){
        isGameOver = true;
        affichageFin(window, timerText);
        affichageRejouer(window);

        if (window.isPressed({' ', false})) {
            resetGame();
        }
    }
}

int main(){
    MinGL window("MinGL", nsGraphics::Vec2D(Ktaille, Ktaille), nsGraphics::Vec2D(128, 128), nsGraphics::KGray); //Nom fenetre, Dimension fenetre, Emplacement de la fenetre au lancement
    window.initGlut();
    window.initGraphic();

    startTime = chrono::steady_clock::now(); // Initialisation du timer

    // Variable qui tient le temps de frame
    chrono::microseconds frameTime = chrono::microseconds::zero();

    rectPosJ1.setX(0);
    rectPosJ1.setY(0);

    rectPosJ2.setX(Ktaille - 20);
    rectPosJ2.setY(Ktaille - 20);

    // Tant que la fenetre est ouverte:
    while (window.isOpen()){
        // Récupèration chrono
        chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();

        window.clearScreen();

        ////////////////////////////////////////////////////////////////////////
        actionFrame(window);
        ////////////////////////////////////////////////////////////////////////

        window.finishFrame();

        window.getEventManager().clearEvents();

        // Si la valeur est baissée le jeu va plus vite, attention des fois ça lag quand on met trop petit
        this_thread::sleep_for(chrono::milliseconds(900 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start));

        frameTime = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start);
    }
    return 0;
}
