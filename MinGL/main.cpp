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

void clavierJ2(MinGL &window,  bool &collisionJoueur)
{
    if (!collisionJoueur){
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
}

void dessiner(MinGL &window, bool &collisionJoueur)
{
    // // MinGL 2 supporte l'affichage de texte sur la fenêtre assez simplement.
    // window << nsGui::Text(nsGraphics::Vec2D(20, 20), "Hello, World!", nsGraphics::KWhite);

    // // Vous pouvez aussi changer la police (parmi un choix assez limité, malheureusement).
    // window << nsGui::Text(nsGraphics::Vec2D(20, 40), "Hello, World! mais plus grand", nsGraphics::KWhite, nsGui::GlutFont::BITMAP_9_BY_15);
    // window << nsGui::Text(nsGraphics::Vec2D(20, 60), "Hello, World! mais en Helvetica 18", nsGraphics::KWhite, nsGui::GlutFont::BITMAP_HELVETICA_18);

    // // L'alignement vertical et horizontal peut aussi être changé.
    // window << nsShape::Line(nsGraphics::Vec2D(320, 120), nsGraphics::Vec2D(320, 220), nsGraphics::KRed);
    // window << nsShape::Line(nsGraphics::Vec2D(10, 310), nsGraphics::Vec2D(630, 310), nsGraphics::KRed);

    // window << nsGui::Text(nsGraphics::Vec2D(320, 135), "Gauche horizontalement", nsGraphics::KWhite, nsGui::GlutFont::BITMAP_9_BY_15);
    // window << nsGui::Text(nsGraphics::Vec2D(320, 160), "Centre horizontalement", nsGraphics::KWhite, nsGui::GlutFont::BITMAP_9_BY_15,
    //                       nsGui::Text::HorizontalAlignment::ALIGNH_CENTER);
    // window << nsGui::Text(nsGraphics::Vec2D(320, 185), "Droite horizontalement", nsGraphics::KWhite, nsGui::GlutFont::BITMAP_9_BY_15,
    //                       nsGui::Text::HorizontalAlignment::ALIGNH_RIGHT);

    // window << nsGui::Text(nsGraphics::Vec2D(15, 310), "Bas verticalement", nsGraphics::KWhite, nsGui::GlutFont::BITMAP_9_BY_15,
    //                       nsGui::Text::HorizontalAlignment::ALIGNH_LEFT, nsGui::Text::VerticalAlignment::ALIGNV_BOTTOM);
    // window << nsGui::Text(nsGraphics::Vec2D(200, 310), "Centre verticalement", nsGraphics::KWhite, nsGui::GlutFont::BITMAP_9_BY_15,
    //                       nsGui::Text::HorizontalAlignment::ALIGNH_LEFT, nsGui::Text::VerticalAlignment::ALIGNV_CENTER);
    // window << nsGui::Text(nsGraphics::Vec2D(400, 310), "Haut verticalement", nsGraphics::KWhite, nsGui::GlutFont::BITMAP_9_BY_15,
    //                       nsGui::Text::HorizontalAlignment::ALIGNH_LEFT, nsGui::Text::VerticalAlignment::ALIGNV_TOP);

    // On dessine le rectangle J1
    window << nsShape::Rectangle(rectPosJ1, rectPosJ1 + nsGraphics::Vec2D(20, 20), nsGraphics::KCyan);

    if (!collisionJoueur)
        // On dessine le rectangle J2
        window << nsShape::Rectangle(rectPosJ2, rectPosJ2 + nsGraphics::Vec2D(20, 20), nsGraphics::KWhite);

    // Dessiner l'obstacle
    window << nsShape::Rectangle(obstacleTopLeft, obstacleBottomRight, nsGraphics::KRed);
}

bool areRectanglesColliding(const nsGraphics::Vec2D &rect1TopLeft, const nsGraphics::Vec2D &rect1BottomRight,
                            const nsGraphics::Vec2D &rect2TopLeft, const nsGraphics::Vec2D &rect2BottomRight)
{
    return !(rect1BottomRight.getX() <= rect2TopLeft.getX() ||  // Rectangle 1 est à gauche de Rectangle 2
             rect1TopLeft.getX() >= rect2BottomRight.getX() ||  // Rectangle 1 est à droite de Rectangle 2
             rect1BottomRight.getY() <= rect2TopLeft.getY() ||  // Rectangle 1 est au-dessus de Rectangle 2
             rect1TopLeft.getY() >= rect2BottomRight.getY());   // Rectangle 1 est en dessous de Rectangle 2
}

void detectCollisionWithObstacle()
{
    string collision;

    // Définir les coins du rectangle mobile
    nsGraphics::Vec2D rect1BottomRight = rectPosJ1 + nsGraphics::Vec2D(20, 20);

    // Vérifier si les rectangles se chevauchent
    if (areRectanglesColliding(rectPosJ1, rect1BottomRight, obstacleTopLeft, obstacleBottomRight))
    {
        collision = "Collision avec l'obstacle !";
    }
    else
    {
        collision = "Pas de collision avec l'obstacle !";
    }

}

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
    collisionJoueur = detectCollisionEntreJoueur();
    clavierJ1(window);
    clavierJ2(window, collisionJoueur);
    detectCollisionWithObstacle();

    // Calcule le temps écoulé
    auto currentTime = chrono::steady_clock::now();
    auto elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();

    // Affiche le temps écoulé à l'écran
    string timerText = "Temps: " + to_string(elapsedTime) + "s";
    window << nsGui::Text(nsGraphics::Vec2D(20, 50), timerText, nsGraphics::KWhite);

    // Dessiner le reste
    dessiner(window, collisionJoueur);

}

int main()
{
    // Initialise le système
    MinGL window("MinGL", nsGraphics::Vec2D(Ktaille, Ktaille), nsGraphics::Vec2D(128, 128), nsGraphics::KBlack); //Nom fenetre, Dimension fenetre, Emplacement de la fenetre au lancement
    window.initGlut();
    window.initGraphic();

    startTime = chrono::steady_clock::now(); // Initialisation du timer

    // Variable qui tient le temps de frame
    chrono::microseconds frameTime = chrono::microseconds::zero();

    rectPosJ2.setX(0);
    rectPosJ2.setY(0);

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
