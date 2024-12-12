# pacman
<h1>Objectifs de la SAE</h1>

<h2>Votre tâche est de compléter le code fourni lors des TP en suivant ces étapes :</h2>

    Blinder le code fourni lors des TPs (prog 10, prog 13, prog 14) :
        Assurez-vous qu'il n'y ait pas de fautes :
            Par exemple, il doit être impossible de jouer à la place du deuxième joueur.
            Évitez les débordements d’indices, sauf cas particuliers mentionnés dans les règles.

    Générer la documentation du projet avec Doxygen.

    Fournir des améliorations :
        Utilisation de MinGL
        Utilisation de la structure termios (pour ceux qui veulent conserver l’interface terminal)
        Ajout de fonctionnalités (liste non exhaustive) :
            Lecture de fichiers de configuration (terminer les TP 13 & 14)
            Niveaux de jeu
            Points particuliers (téléportation, murs, etc.)
            High score

<h2>Organisation et Livrables</h2>
Groupes

    Vous devez former des groupes de 4 ou 5 étudiants.

Date de rendu

    Le rendu est à effectuer avant le vendredi 10 janvier à 12h30 sur la plateforme Ametice.

Contenu du rendu

    Une archive au format “zip” ne contenant aucun binaire/exécutable (.o).
    Cette archive doit contenir :
        Un PDF expliquant ce que vous avez fait.
        Un répertoire avec la documentation générée.
            N'oubliez pas de générer les graphes des fonctions appelées et appelantes.
        Un fichier texte expliquant comment jouer à votre jeu et les règles.
            Conseil : Vous pouvez lire ce fichier et l’afficher dans votre programme. Faites attention au chemin/emplacement de ce fichier.

<h2>Nom de l'archive</h2>

    La nomenclature de l'archive doit être GX_nom1_nom2_nom3.zip :
        X sera remplacé par votre numéro de groupe.
        nom1, nom2, etc., seront remplacés par les noms des membres du groupe, classés par ordre alphabétique.

<h2>Présentation</h2>

    Préparez une présentation d'environ 10 minutes lors de la semaine du 13 janvier.
    
<h2>Amelioration TP10</h2>

rajoutez "X" si vous avez effectué une amélioration

- Transformer le joueur 2 en IA.
- Si le joueur 2 est une IA, rajouter un highScore pour le joueur qui a survecu le plus longtemps.
- Proposer de choisir entre 1v1 et Vs IA
- Rajouter des bonus qui apparaissent aléatoirement sur certaines cases (exemple de bonus, un joueur peut se deplacer deux fois).
- Rajouter des murs.
- "Termios" de Maxime <=> pas besoin d'appuyer sur *Entrée* à chaque coup.
- [X] Avant de lancer la partie, demander à l'utilisateur la taille de la carte (=Matrice)
- Egalement au lancement du programme, on peut afficher un message qui explique les règles (bonus, miroir, mur, deplacement interdit...)
- Passer à une vraie interface graphique avec MinGL

    

