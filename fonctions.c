#include <stdio.h>
#include <stdlib.h>
#include <graph.h>
#include <time.h>
#include "fonctions.h"



#define LARGEUR 62
#define HAUTEUR 42

/*La fonction ecranPrincipal créé la fenêtre du jeu*/
void ecranPrincipal(char* c, char* s) {

	int i,j;

	ChoisirCouleurDessin(0x000000);
	RemplirRectangle(0, 0, 1260, 950);
	ChoisirCouleurDessin(0xffffff);

	EcrireTexte(10,900,c,2);
	EcrireTexte(1100,900,s,2);
	
}


/*Cette fonction créé la grille de jeu où se déplace le serpent par-dessus l'écran principal.  0: terrain, 1: pomme, 2: snake */
void initGrille(grille* g) {

	int i, j, k, direction,l,m,ok;
	unsigned long suivant_grille;
	g->collision = 0;
	g->nb_pommes = 0;
	g->score = 0;
	g->cycle=100000L;
	sprintf(g->scoreTxt, "%07d", g->score);
	/*On remplit toute la grille avec des zero*/
	for(i=0; i<LARGEUR; i++){
		for(j=0; j<HAUTEUR; j++){
				g->gr[i][j] = 0;
			}
		}

	/*On place la tete du snake*/
	g->gr[30][20] = 2;
	g->tete_snake[0] = 30;
	g->tete_snake[1] = 20;
	
	/*On choisit la direction de départ du snake aleatoirement*/
	direction = rand()%4;

	/*On la rentre dans la structure*/
	g->direction = direction;

	if (g->direction == 0) {		/*si c'est egal à 0 le snake monte, et on dessine 9cases en bas*/
		for(j=21;j<30;j++) {
			g->gr[30][j] = 2;
		}
		g->queue_snake[0]=30;
		g->queue_snake[1]=j;
	}
	else if (g->direction == 1) {		/* il descend, 9cases en haut*/
		for(j=19;j>9;j--) {
			g->gr[30][j] = 2;
		}
		g->queue_snake[0]=30;
		g->queue_snake[1]=j;
	}
	else if (g->direction == 3) {		/* il va à droite, 9cases à gauche*/
		for(j=29;j>20;j--) {
			g->gr[j][20] = 2;
		}
		g->queue_snake[0]=j;
		g->queue_snake[1]=20;
	}
	else if (g->direction == 2) {		/* il va à gauche, 9cases à droite*/
		for(j=31;j<40;j++) {
			g->gr[j][20] = 2;
		}
		g->queue_snake[0]=j;
		g->queue_snake[1]=20;
	}


	/* on place 5 pommes aleatoirement */
	srand(time(NULL));
	for(i=0; i<5; i++){
		ok=1;
		k = (rand()%59)+1; /* largeur entre 0 et 60*/
		l = (rand()%39)+1; /* hauteur entre 0 et 40*/
		while(ok) {

			if(g->gr[k][l]==2) {
				/*Si les pommes aparaissent sur le serpent, on recalcule leurs coordonées*/
				k = (rand()%59)+1; /* largeur entre 0 et 60*/
				l = (rand()%39)+1; /* hauteur entre 0 et 40*/
			}
			else {
				g->gr[k][l] = 1;
				ok = 0;
			}
		}
	}

	/*creation des bordures, valeur=3*/
	for(i=0;i<62;i++) {
		l=0;
		m=i;
		g->gr[m][l]=3;
	}
	for(i=0;i<42;i++) {
		for(j=0;j<62;j++) {
			if(j==0 || j==61) {
				g->gr[j][i]=3;
			}
		}
	}
	for(i=0;i<62;i++) {
		l=41;
		m=i;
		g->gr[m][l]=3;
	}												
	
}


/* fonction pour gerer la tete du snake 
 G est un pointeur sur la grille
 i,j les coordonnees
 direction : la direction de la tete du snake  , 0=haut, 1=bas, 2=gauche, 3=droite
*/
void deplaceTete(grille* g, int i, int j, int direction, int nb_case) {

	g->gr[i][j] = 2;
	g->tete_snake[0] = i;
	g->tete_snake[1] = j;
	g->direction = direction;

	/* nb_case = 2 donc on a mange une pomme. on alonge le snake de 2 cases supplementaires */
	if(nb_case == 2 && direction == 0) {
		g->gr[i][j-1] = 2;
		/* on test que l'on ne va pas toucher le bord*/
		if(g->gr[i][j-2] == 3) {
			g->collision = 1;
		}
		else {
			g->gr[i][j-2] = 2;
			g->tete_snake[1] = j-2;
		}
	}
	else if(nb_case == 2 && direction == 1) {
		g->gr[i][j+1] = 2;

		if(g->gr[i][j+2] == 3) {
			g->collision = 1;
		}
		else {
			g->gr[i][j+2] = 2;
			g->tete_snake[1] = j+2;
		}
	}
	else if(nb_case == 2 && direction == 2) {
		g->gr[i-1][j] = 2;

		if(g->gr[i-2][j] == 3) {
			g->collision = 1;
		}
		else {
			g->gr[i-2][j] = 2;
			g->tete_snake[0] = i-2;
		}

	}
	else if(nb_case == 2 && direction == 3) {
		g->gr[i+1][j] = 2;

		if(g->gr[i+2][j] == 3) {
			g->collision = 1;
		}
		else {
			g->gr[i+2][j] = 2;
			g->tete_snake[0] = i+2;
		}
	}	

}

/* fonction pour mettre à jour la structure avec le score */
void majScore(grille* g) {
	int k,l,ok;
	k = (rand()%59)+1; /* largeur entre 0 et 60*/
	l = (rand()%39)+1; /* hauteur entre 0 et 40*/
	g->nb_pommes ++;
	g->score = 5*g->nb_pommes;
	sprintf(g->scoreTxt, "%07d", g->score);
	
	/* on ajoute une nouvelle pomme */
	/* on test que l'on apparait pas sur le snake */
	ok=1;
	while(ok) {
		if(g->gr[k][l]==2) {
			srand(time(NULL));
			k = (rand()%59)+1; /* largeur entre 0 et 60*/
			l = (rand()%39)+1; /* hauteur entre 0 et 40*/
		}
		else {
			g->gr[k][l] = 1;
			ok = 0;
		}
	}

	
		/*Toutes les trois pommes on augmente la vitesse du snake en diminuant le cycle*/
	
	if(g->nb_pommes%3 == 0 && g->cycle >= 35000) {
		printf("accélération\n");
		g->cycle=g->cycle-10000;
	}
}

/* 0: vert , 1: pomme, 2 : snake ,  direction du snake 0=haut, 1=bas, 2=gauche, 3=droite */
void majTete(grille* g, int touche) {

	int i, j, k, l, nb_case;
	k = (rand()%59)+1; /* largeur entre 0 et 60*/
	l = (rand()%39)+1; /* hauteur entre 0 et 40*/
	i = g->tete_snake[0];	
	j = g->tete_snake[1];

	nb_case = 1;

	/* on detecte la tête du serpent */
	if ( touche == XK_Up && g->direction != 0 && g->direction != 1) {		/* si touche = haut et se dirige pas vers haut ou bas*/
		if (g->gr[i][j-1] == 3 || g->gr[i][j-1] == 2) {		/*On vérifie que la prochaine case n'est pas le bord ou le serpent*/
				g->collision = 1;
		}
		else {
			if (g->gr[i][j-1] == 1) {
				nb_case = 2;
				majScore(g);
			}
			deplaceTete(g, i, j-1, 0, nb_case);
		}
	}
	else if ( touche == XK_Down && g->direction != 0 && g->direction != 1) {		/* si touche = bas et se dirige pas vers haut ou bas*/
		if (g->gr[i][j+1] == 3 || g->gr[i][j+1] == 2) {
				g->collision = 1;
		}
		else {
			if (g->gr[i][j+1] == 1) {
				majScore(g);
				nb_case = 2;
			}
			deplaceTete(g, i, j+1, 1, nb_case);
		}
		
	}
	else if ( touche == XK_Left && g->direction != 2 && g->direction != 3) {		/* si touche = gauche et se dirige pas vers gauche ou droite*/
		if (g->gr[i-1][j] == 3 || g->gr[i-1][j] == 2) {
				g->collision = 1;
		}
		else {
			if (g->gr[i-1][j] == 1) {
				nb_case = 2;
				majScore(g);
			}
			deplaceTete(g, i-1, j, 2, nb_case);
		}

	}
	else if ( touche == XK_Right && g->direction != 2 && g->direction != 3) {		/* si touche = droite et se dirige pas vers gauche ou droite*/
		if (g->gr[i+1][j] == 3 || g->gr[i+1][j] == 2) {
			g->collision = 1;
		}
		else {
			if (g->gr[i+1][j] == 1) {
				nb_case = 2;
				majScore(g);
			}
			deplaceTete(g, i+1, j, 3, nb_case);
		}
		
	}
	else { /* touche sans effet ou pas de touche appuyée */
		if(g->direction == 0) { /*il monte donc la case en haut prend la valeur du snake*/

			if (g->gr[i][j-1] == 3 || g->gr[i][j-1] == 2) {
				g->collision = 1;
			}
			else {
				if (g->gr[i][j-1] == 1) {
					nb_case = 2;
					majScore(g);
				}
				deplaceTete(g, i, j-1, g->direction, nb_case);
			}
		
		}
		else if(g->direction == 1 ) {	/* il descend, case du bas prend valeur du snake*/
			if (g->gr[i][j+1] == 3 || g->gr[i][j+1] == 2) {
				g->collision = 1;
			}
			else {
				if (g->gr[i][j+1] == 1) {
					nb_case = 2;
					majScore(g);
				}
				deplaceTete(g, i, j+1, g->direction, nb_case);
			}
		}
		else if(g->direction == 2) {	/* va à gauche, case à gauche prend valeur du snake*/
		if (g->gr[i-1][j] == 3 || g->gr[i-1][j] == 2) {
				g->collision = 1;
			} 
			else {
				if (g->gr[i-1][j] == 1) {
					nb_case = 2;
					majScore(g);
				}
				deplaceTete(g, i-1, j, g->direction, nb_case);
			}
		}
		else if(g->direction == 3) {	/* va à droite, case à droite prend valeur du snake*/
			if (g->gr[i+1][j] == 3 || g->gr[i+1][j] == 2) {
				g->collision = 1;
			}
			else {
				if (g->gr[i+1][j] == 1) {
					nb_case = 2;
					majScore(g);
				}
				deplaceTete(g, i+1, j, g->direction, nb_case);
			}
		} 
	}	
}


/*  direction du snake 0=haut, 1=bas, 2=gauche, 3=droite*/
void deplaceQueue(grille* g) {

	int i, j;

	/*gestion de la queue du snake*/
	i = g->queue_snake[0]; /* X */
	j = g->queue_snake[1]; /* Y */
	g->gr[i][j] = 0;

	if(g->gr[i][j-1] == 2) {
		g->queue_snake[0] = i;
		g->queue_snake[1] = j-1;
	}
	else if(g->gr[i][j+1] == 2) {
		g->queue_snake[0] = i;
		g->queue_snake[1] = j+1;
	}
	else if(g->gr[i-1][j] == 2) {
		g->queue_snake[0] = i-1;
		g->queue_snake[1] = j;
	}
	else if(g->gr[i+1][j] == 2) {
		g->queue_snake[0] = i+1;
		g->queue_snake[1] = j;
	}

}



/* 0: vert , 1: pomme, 2 : snake ; 3 = bord;
 
 touche = 0 , pas d'action du joueur
 touche = 65362 => haut
 touche = 65364 => bas
 touche = 65361 => gauche
 touche = 64363 => droite

 direction du snake 0=haut, 1=bas, 2=gauche, 3=droite

*/
void majGrille(grille* g, int touche) {

	/* mise a jour de la tete*/

	/*gestion de la queue du snake*/
	deplaceQueue(g);
	majTete(g, touche);


}


/* 0: vert , 1: pomme, 2 : snake
 Sert à afficher la grille dans la console pour debug*/
void afficheGrilleTxt(grille* g) {

	int i, j;
	/* on affiche le snake et les pommes */
	for(i=0; i<LARGEUR; i++){
		printf("| ");
		for(j=0; j<HAUTEUR; j++){
			printf("%d |",g->gr[i][j]);
		}
		printf("\n");
	}

}


/* 0: vert , 1: pomme, 2 : snake */
void afficheGrille(grille* g, int pomme, int tete, int terrain) {

	int i, j;
	/*ChoisirCouleurDessin(0x4eb164);
	RemplirRectangle(40, 40, 1200, 800);*/
	AfficherSprite(terrain,40,40);

	/* on affiche le snake et les pommes */
	for(i=0; i<LARGEUR; i++){
		for(j=0; j<HAUTEUR; j++){

			if( g->gr[i][j] == 1 ) {
				/*ChoisirCouleurDessin(0xee111c);
				RemplirRectangle(i*20+20, j*20+20, 20, 20);*/
				AfficherSprite(pomme,i*20+20, j*20+20);
			}
			else if( g->gr[i][j] == 2 ) {
				/*ChoisirCouleurDessin(0xffd700);
				RemplirRectangle(i*20+20, j*20+20, 20, 20);*/
				AfficherSprite(tete, i*20+20, j*20+20);
			}
			/*else if( g->gr[i][j] == 0 ) {
				ChoisirCouleurDessin(0x4eb164);
				RemplirRectangle(i*20+20, j*20+20, 20, 20);
			}*/
		}
	}

}
