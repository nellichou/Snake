/* types.h 
contient les definitions des types utilisés*/

typedef struct {

    int gr[62][42];
    int tete_snake[2]; /* coordonnes tete snake 1er: X , 2eme Y*/
    int queue_snake[2]; /* coordonnes queue du snake*/
    int direction; /* direction du snake 0=haut, 1=bas, 2=gauche, 3=droite*/
    int collision;
    int score;
    char scoreTxt[10];
    int nb_pommes;
    unsigned long cycle; /*Le cycle est la vitesse du snake. Plus il est bas, plus il va vite*/

} grille;

