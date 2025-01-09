#define FPS_LIMIT 60

#include <iostream>
#include <thread>

#include "minGL2/include/mingl/mingl.h"
#include "minGL2/include/mingl/gui/text.h"
#include "minGL2/include/mingl/shape/line.h"
#include "minGL2/include/mingl/shape/rectangle.h"


using namespace std;

nsGraphics::Vec2D rectPosJ1;
nsGraphics::Vec2D rectPosJ2;

bool collisionJoueur = false;

const unsigned Kvitesse = 2;

const int Ktaille = 640;

const nsGraphics::Vec2D obstacleTopLeft(300, 300);
const nsGraphics::Vec2D obstacleBottomRight(350, 350);

chrono::time_point<chrono::steady_clock> startTime;

void clavierJ1(MinGL &window)
{
    // On vérifie si ZQSD est pressé, et met a jour la position
    int posFuture;

    if (window.isPressed({'z', false})) { // Haut
        posFuture = rectPosJ1.getY() - Kvitesse;
        if (posFuture >= 0)
            rectPosJ1.setY(posFuture);
    }
    if (window.isPressed({'s', false})) { // Bas
        int posFuture = rectPosJ1.getY() + Kvitesse;
        if (posFuture + 20 <= Ktaille) // 20 : hauteur du rectangle, 640 : taille de l'écran
            rectPosJ1.setY(posFuture);
    }
    if (window.isPressed({'q', false})) { // Gauche
        int posFuture = rectPosJ1.getX() - Kvitesse;
        if (posFuture >= 0)
            rectPosJ1.setX(posFuture);
    }
    if (window.isPressed({'d', false})) { // Droite
        int posFuture = rectPosJ1.getX() + Kvitesse;
        if (posFuture + 20 <= Ktaille) // 20 : largeur du rectangle, 640 : taille de l'écran
            rectPosJ1.setX(posFuture);
    }
}

void clavierJ2(MinGL &window)
{

    // On vérifie si ZQSD est pressé, et met a jour la position
    int posFuture;

    if (window.isPressed({'o', false})) { // Haut
        posFuture = rectPosJ2.getY() - Kvitesse;
        if (posFuture >= 0)
            rectPosJ2.setY(posFuture);
    }
    if (window.isPressed({'l', false})) { // Bas
        int posFuture = rectPosJ2.getY() + Kvitesse;
        if (posFuture + 20 <= Ktaille) // 20 : hauteur du rectangle, 640 : taille de l'écran
            rectPosJ2.setY(posFuture);
    }
    if (window.isPressed({'k', false})) { // Gauche
        int posFuture = rectPosJ2.getX() - Kvitesse;
        if (posFuture >= 0)
            rectPosJ2.setX(posFuture);
    }
    if (window.isPressed({'m', false})) { // Droite
        int posFuture = rectPosJ2.getX() + Kvitesse;
        if (posFuture + 20 <= Ktaille) // 20 : largeur du rectangle, 640 : taille de l'écran
            rectPosJ2.setX(posFuture);
    }
}

void dessiner(MinGL &window)
{

    // On dessine le rectangle J1
    window << nsShape::Rectangle(rectPosJ1, rectPosJ1 + nsGraphics::Vec2D(20, 20), nsGraphics::KCyan);

    // On dessine le rectangle J2
    window << nsShape::Rectangle(rectPosJ2, rectPosJ2 + nsGraphics::Vec2D(20, 20), nsGraphics::KRed);

    // Dessiner l'obstacle
    window << nsShape::Rectangle(obstacleTopLeft, obstacleBottomRight, nsGraphics::KGreen);
}

bool areRectanglesColliding(const nsGraphics::Vec2D &rect1TopLeft, const nsGraphics::Vec2D &rect1BottomRight,
                            const nsGraphics::Vec2D &rect2TopLeft, const nsGraphics::Vec2D &rect2BottomRight)
{
    return !(rect1BottomRight.getX() <= rect2TopLeft.getX() ||  // Rectangle 1 est à gauche de Rectangle 2
             rect1TopLeft.getX() >= rect2BottomRight.getX() ||  // Rectangle 1 est à droite de Rectangle 2
             rect1BottomRight.getY() <= rect2TopLeft.getY() ||  // Rectangle 1 est au-dessus de Rectangle 2
             rect1TopLeft.getY() >= rect2BottomRight.getY());   // Rectangle 1 est en dessous de Rectangle 2
}

// void detectCollisionWithObstacle()
// {
//     string collision;

//     // Définir les coins du rectangle mobile
//     nsGraphics::Vec2D rect1BottomRight = rectPosJ1 + nsGraphics::Vec2D(20, 20);

//     // Vérifier si les rectangles se chevauchent
//     if (areRectanglesColliding(rectPosJ1, rect1BottomRight, obstacleTopLeft, obstacleBottomRight))
//     {
//         collision = "Collision avec l'obstacle !";
//     }
//     else
//     {
//         collision = "Pas de collision avec l'obstacle !";
//     }

// }

bool detectCollisionEntreJoueur()
{
    string collision;
    bool collisionJoueur = false;

    // Définir les coins du rectangle mobile
    nsGraphics::Vec2D rect1BottomRight = rectPosJ1 + nsGraphics::Vec2D(20, 20);
    nsGraphics::Vec2D rect2BottomRight = rectPosJ2 + nsGraphics::Vec2D(20, 20);

    // Vérifier si les rectangles se chevauchent
    if (areRectanglesColliding(rectPosJ1, rect1BottomRight, rectPosJ2, rect2BottomRight))
    {
        collision = "Collision entre les joueurs !";
        collisionJoueur = true;
    }
    else
    {
        collision = "Pas de collision entre les joueurs !";
        collisionJoueur = false;
    }

    return collisionJoueur;
}

void actionFrame(MinGL &window)
{
    bool collision = detectCollisionEntreJoueur();

    static long lastElapsedTime = 0; // Variable persistante pour conserver la dernière valeur du chrono
    string timerText = "Le joueur 2 a survecu pendant : " + to_string(lastElapsedTime)  + "s !";

    if (!collision) {
        // Actions pendant que le jeu est en cours
        clavierJ1(window);
        clavierJ2(window);

        // Calcule le temps écoulé
        auto currentTime = chrono::steady_clock::now();
        lastElapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();

        // Affiche le temps
        timerText = "Temps: " + to_string(lastElapsedTime) + "s";
        window << nsGui::Text(nsGraphics::Vec2D(20, 50), timerText, nsGraphics::KWhite, nsGui::GlutFont::BITMAP_9_BY_15);
    }

    // Dessiner le jeu
    dessiner(window);


    if (collision){
        // Dimensions approximatives du texte
        int textWidth = 220;  // Largeur approximative
        int textHeight = 50;  // Hauteur approximative
        int offsetX = 220;    // Position X du rectangle
        int offsetY = 150;    // Position Y du rectangle
        int padding = 10;     // Ajout d'espace autour du texte

        // Dessiner un rectangle de fond derrière le texte
        window << nsShape::Rectangle(
            nsGraphics::Vec2D(offsetX - padding, offsetY - padding), // Coin supérieur gauche
            nsGraphics::Vec2D(offsetX + textWidth + padding, offsetY + textHeight + padding), // Coin inférieur droit
            nsGraphics::KBlack // Couleur du fond (noir ici)
            );

        // Affiche le temps (toujours basé sur la dernière valeur calculée)
        window << nsGui::Text(nsGraphics::Vec2D(230, 200), timerText, nsGraphics::KWhite, nsGui::GlutFont::BITMAP_HELVETICA_12);
        window << nsGui::Text(nsGraphics::Vec2D(250, 160), "Victoire du joueur 1", nsGraphics::KWhite, nsGui::GlutFont::BITMAP_HELVETICA_18);
    }
}



int main()
{
    // Initialise le système
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

    // On fait tourner la boucle tant que la fenêtre est ouverte
    while (window.isOpen())
    {
        // Récupère l'heure actuelle
        chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();

        // On efface la fenêtre
        window.clearScreen();

        ////////////////////////////////////////////////////////////////////////

        actionFrame(window);

        ////////////////////////////////////////////////////////////////////////

        // On finit la frame en cours
        window.finishFrame();

        // On vide la queue d'évènements
        window.getEventManager().clearEvents();

        // On attend un peu pour limiter le framerate et soulager le CPU
        this_thread::sleep_for(chrono::milliseconds(1000 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start));

        // On récupère le temps de frame
        frameTime = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start);
    }

    return 0;
}
