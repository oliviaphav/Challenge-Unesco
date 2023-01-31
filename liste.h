#ifndef LISTE_H
#define LISTE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct listeSites //Definition de la structure liste chainée des sites
{
  Site S;
  struct listeSites * suivant;
} listeSites;

//Signatures des fonctions

listeSites * insertion(listeSites * L, Site S);
void affichageListe(listeSites * L);
void supprimeDebut(listeSites ** L);
void supprimeListe(listeSites** L);
int presence(int valeur,int tab[]);
listeSites * itineraire(listeSites * L, Site *tab,double **M,int nbSites, Site dep,double *temps);
void evaluation(listeSites * L);
listeSites * itineraire2(listeSites * L, Site *tab,double **M,int nbSites, Site dep,double *temps);
listeSites * evaluation2(listeSites * L1,listeSites * L2,Site * tab,double temps1, double temps2);


#endif
