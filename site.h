#ifndef SITE_H
#define SITE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct site //Definition de la structure d'un site
{
  char * nom;
  float LAT;
  float LONG;
  char * categorie; //cultural,natural,mixed
  char * pays;
  char * continent;
  int enDanger; //0,1
} Site;

//Signature des fonctions

Site construireSite(char * nom,float LAT,float LONG,char * categorie,char * pays,char * continent,int enDanger);
void affichageSite(Site S);
double **matrices(Site*tab,double lat_depart,double long_depart);

#endif
