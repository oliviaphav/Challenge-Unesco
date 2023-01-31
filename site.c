#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "site.h"
#include "haversine.h"

Site construireSite(char * nom,float LAT,float LONG,char * categorie,char * pays,char * continent,int enDanger)
//Contruction d'un site avec l'ensemble des champs
{
  Site S;
  S.nom=strdup(nom);
  S.LAT=LAT;
  S.LONG=LONG;
  S.categorie =strdup(categorie);
  S.pays =strdup(pays) ;
  S.continent=strdup(continent);
  S.enDanger=enDanger;
  return S;
}

void desalloue(Site * S) //Désallocation d'un site
{
  free(S->nom);
  free(S->categorie);
  free(S->pays);
  free(S->continent);
}

void affichageSite(Site S) //Affichage d'un site
{
  printf("%s, %f, %f, %s, %s, %s, %d",S.nom,S.LAT,S.LONG,S.categorie,S.pays,S.continent,S.enDanger);
}


double **matrices(Site*tab,double lat_depart,double long_depart) //Contruction de la matrice des distances
//Fonction permettant de créer une matrice dynamique et saisir des valeurs
{
  double **mat; //Matrice qui va contenir les distances

  mat=(double **)malloc(1100*sizeof(double *)); //allocation de la matrice
  for (int i=0;i<1100;i++)
  {
    mat[i]= (double *)malloc(1100*sizeof(double ));
  }

  for (int j=0;j<1053;j++)//Distance du point de depart et les differents sites
  {
    mat[0][j]=calculDistance(lat_depart,long_depart,tab[j].LAT,tab[j].LONG);
  }

  //La matrice contient une ligne de plus que le tableau de l'ensemble des sites car elle contient le point de depart

  for (int i=1;i<1054;i++)
  {
    for (int j=0;j<1053;j++) //Distances entre les differents sites
    {
      mat[i][j]=calculDistance(tab[i-1].LAT,tab[i-1].LONG,tab[j].LAT,tab[j].LONG);
    }
  }
  return mat; //retourner le tableau final
}
