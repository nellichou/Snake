/* fonctions.h 
contient les prototypes
*/
#include "types.h"
void initGrille(grille* g); /* affichage grille initiale*/
void afficheGrille(grille* g, int pomme, int tete, int terrain); /* affichage nouvelle grille*/
void afficheGrilleTxt(grille* g); /* affichage grille dans terminal*/
void majGrille(grille* g, int touche); /*mise à jour grille*/
void ecranPrincipal(char* c, char* s);
