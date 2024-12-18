#define FPS_LIMIT 60

#include <iostream>
#include <thread>

#include "minGL2/include/mingl/mingl.h"
#include "minGL2/include/mingl/gui/text.h"
#include "minGL2/include/mingl/shape/line.h"
#include "minGL2/include/mingl/shape/rectangle.h"


using namespace std;

nsGraphics::Vec2D rectPos;

const unsigned Kvitesse = 2;

const unsigned int Ktaille = 640;

const nsGraphics::Vec2D obstacleTopLeft(300, 300);
const nsGraphics::Vec2D obstacleBottomRight(350, 350);

void clavier(MinGL &window)
{

    // On vérifie si ZQSD est pressé, et met a jour la position
    int posFuture;

    if (window.isPressed({'z', false})) { // Haut
        posFuture = rectPos.getY() - Kvitesse;
        if (posFuture >= 0)
            rectPos.setY(posFuture);
    }
    if (window.isPressed({'s', false})) { // Bas
        int posFuture = rectPos.getY() + Kvitesse;
        if (posFuture + 20 <= 640) // 20 : hauteur du rectangle, 640 : taille de l'écran
            rectPos.setY(posFuture);
    }
    if (window.isPressed({'q', false})) { // Gauche
        int posFuture = rectPos.getX() - Kvitesse;
        if (posFuture >= 0)
            rectPos.setX(posFuture);
    }
    if (window.isPressed({'d', false})) { // Droite
        int posFuture = rectPos.getX() + Kvitesse;
        if (posFuture + 20 <= 640) // 20 : largeur du rectangle, 640 : taille de l'écran
            rectPos.setX(posFuture);
    }
}

void dessiner(MinGL &window)
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

    // On dessine le rectangle
    window << nsShape::Rectangle(rectPos, rectPos + nsGraphics::Vec2D(20, 20), nsGraphics::KCyan);

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

void detectCollisionWithObstacle(MinGL &window)
{
    string collision;

    // Définir les coins du rectangle mobile
    nsGraphics::Vec2D rect1BottomRight = rectPos + nsGraphics::Vec2D(20, 20);

    // Vérifier si les rectangles se chevauchent
    if (areRectanglesColliding(rectPos, rect1BottomRight, obstacleTopLeft, obstacleBottomRight))
    {
        collision = "Collision avec l'obstacle !";
    }
    else
    {
        collision = "Pas de collision avec l'obstacle !";
    }

    // Afficher le texte à l'écran
    window << nsGui::Text(nsGraphics::Vec2D(20, 20), collision, nsGraphics::KWhite);
}

void actionFrame(MinGL &window)
{
    clavier(window);
    detectCollisionWithObstacle(window);
    dessiner(window);
}

int main()
{
    // Initialise le système
    MinGL window("test MinGL", nsGraphics::Vec2D(Ktaille, Ktaille), nsGraphics::Vec2D(128, 128), nsGraphics::KBlack); //Nom fenetre, Dimension fenetre, Emplacement de la fenetre au lancement
    window.initGlut();
    window.initGraphic();

    // Variable qui tient le temps de frame
    chrono::microseconds frameTime = chrono::microseconds::zero();

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
