
#include <stdlib.h>
#include <stdio.h>
#include <graph.h>
#include "fonctions.h"



#define LARGEUR 60
#define HAUTEUR 40

#define CYCLE 1000000L

int main(void) {

	int pomme, touche, secondes, minutes, temps, pause, tete, terrain;
	int go_on = 1;
	unsigned long suivant, suivant_grille;
	char tempsEcoule[100];
	char score[10];
	
	grille grd;
	
	/* initialisation de la fenetre principale*/
	InitialiserGraphique();
	CreerFenetre(50, 100, 1260, 950);
	ChoisirTitreFenetre("SNAKE");
	
	/* on marque le debut du cycle */
	suivant = Microsecondes()+CYCLE;
	suivant_grille = Microsecondes()+grd.cycle;
	
	
	/* initialise la grille de jeu */
	initGrille(&grd);

	secondes=0;
	minutes=0;
	temps=1;

	/* Fenetre de depart */
	pomme = ChargerSprite("./img_pomme.png");
	tete = ChargerSprite("./img_snake.png");
	terrain = ChargerSprite("./img_herbe.png");
	ecranPrincipal("Appuyez sur espace pour commencer",grd.scoreTxt,"\0");
	afficheGrille(&grd, pomme, tete, terrain);

	pause = 1; /* 0 pas en pause , 1 en pause */
	while(go_on)
	{
		/* touche tapée par le joueur*/
		if(ToucheEnAttente()) {
		
			touche = Touche();
			
			/* on sort si le joueur appuie ESC */
			if(touche == XK_Escape) 
			{
				go_on = 0;
			}
			else if(touche == XK_space && pause == 0) { /* mise du jeu en pause */
				pause = 1;
			}
			else if(touche == XK_space && pause == 1) { /* arret de la pause */
				pause = 0;
			}
			else {
				/* on bouge le snake :-) */
				/* pas de mise a jour pendant la pause */		
				if(pause == 0) {
						majGrille(&grd, touche); /* touche = une fleche directionnelle, snake change de direction , grille mise a jour*/
				}
			}
		}
		else 
		{
			/* on verifie que nous ne sommes pas en pause*/
			if(pause == 0) {

				/* on reste dans la boucle sinon */
				/* rafraichissement de l'ecran principal toutes les secondes (pour le decompte du temps : tous les CYCLE)*/
				if (Microsecondes()>suivant)
				{
					secondes++;
					if(secondes > 59) {
						minutes = (secondes*temps)/(60);
						secondes=0;
						temps++;
					}
					/* une periode s'est écoulé */
					sprintf(tempsEcoule,"%02d:%02d",minutes,secondes);
					ecranPrincipal(tempsEcoule,grd.scoreTxt,"Espace=Pause  ESC=quitter");
					afficheGrille(&grd, pomme, tete, terrain);
					/*touche = 0, le serpent avance dans la même direction, grille mise a jour*/
					/* prochaine date */
					suivant = Microsecondes()+CYCLE;
				}

				/* on rafraichit la grille de jeu plus rapidement tous les grd.cycle */
				if (Microsecondes()>suivant_grille)
				{									
					afficheGrille(&grd, pomme, tete, terrain);
					majGrille(&grd, 0);  /*touche = 0, le serpent avance dans la même direction, grille mise a jour */
					/* prochain cycle */
					suivant_grille = Microsecondes()+grd.cycle;
					if(grd.collision == 1) {
						go_on = 0;
					}
				}
			}
		}

	}  
    
	if(grd.collision == 1) {	/*Si collision, fin du jeu*/
		ecranPrincipal("Perdu ! Taper ESC pour quitter.",grd.scoreTxt,tempsEcoule);
		afficheGrille(&grd, pomme, tete, terrain);
	}
	else {	/*Si le joeur appuie sur échap, fin du jeu*/
		ecranPrincipal("Fin de la partie ! Taper ESC pour quitter.",grd.scoreTxt,tempsEcoule);
		afficheGrille(&grd, pomme, tete, terrain);
	}

	/* boucle de sortie pour quitter seulement quand le joueur tape ESC*/
	go_on=1;
	while(go_on) {
		if(ToucheEnAttente()) {
			touche = Touche();
			/* on sort si ESC */
			if(touche == XK_Escape) 
			{
				go_on = 0;
			}		
		}
	}
	LibererSprite(pomme);
	LibererSprite(tete);
	LibererSprite(terrain);
	FermerGraphique();
	return EXIT_SUCCESS;

}
