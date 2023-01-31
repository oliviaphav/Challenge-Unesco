#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "site.h"
#include "haversine.h"
#include "lectureFichiers.h"
#include "liste.h"

int main(int argc, char* argv[])
{
  double p; //variable sur le temps
  clock_t start, end ;
  start=clock(); //Calcul du temps d'execution
  FILE *p_fichier ; //pointeur sur fichier

  p_fichier = fopen("sites.csv", "r") ; //Lecture du fichier site.csv
  if(p_fichier == NULL)//Si erreur dans la creation du fichier, sortir
    {
      printf("Erreur de création du fichier \n") ;
      exit(-1) ;//Abandonner le programme
    }

    int nbSites=-1; //Variable sur le nombre de sites
    while(!feof(p_fichier)) //Comptage du nombre de lignes du fichier
    {
      SkipLine(p_fichier);
      nbSites++; //Comptage du nombre de sites
    }
  fclose(p_fichier) ;//Fermeture du fichier

  p_fichier = fopen("sites.csv", "r") ;//Réouverture du fichier
  if(p_fichier == NULL)//Si erreur dans la creation du fichier, sortir
    { printf("Erreur de création du fichier \n") ;
      exit(-1) ;//Abandonner le programme
    }

  Site *tab=NULL;//Creation d'un tableau de sites
  tab=(Site *)malloc(nbSites*sizeof(Site ));//allocation du tableau

  char *nom=NULL;
  nom=(char *)malloc(250*sizeof(char )); //Allocation du tableau de chaine de caractères pour le nom
  float LA; //Variable pour la latitude
  float LO; //Variable pour la longitude
  char *cat=NULL;
  cat=(char *)malloc(250*sizeof(char ));//Allocation du tableau de chaine de caractères pour la catégorie
  char *pays=NULL;
  pays=(char *)malloc(250*sizeof(char ));//Allocation du tableau de chaine de caractères pour le pays
  char *cont=NULL;
  cont=(char *)malloc(250*sizeof(char ));//Allocation du tableau de chaine de caractères pour le continent
  int danger=0; //Variable pour le danger, 0 si non en danger, 1 si en danger

  int nb=0;//Variable qui parcourt le tableau
  SkipLine(p_fichier); //Saut de la premiere ligne correspondant à l'entête

while(nb!=nbSites-1)//Tant que l'ensemble des sites n'ont pas été parcouru
  {
      GetChaine(p_fichier,250,nom); //Copier la chaine de caractères du nom dans le tableau nom
      Skip(p_fichier); //Prochaine chaine de caractère apres la virgule
      fscanf(p_fichier,"%f",&LA); //Copier la latitude dans la variable LA
      Skip(p_fichier);
      fscanf(p_fichier,"%f",&LO);//Copier la longitude dans la variable LO
      Skip(p_fichier);
      GetChaine(p_fichier,250,cat);//Copier la chaine de caractères du catégorie dans le tableau catégorie
      Skip(p_fichier);
      GetChaine(p_fichier,250,pays);//Copier la chaine de caractères du pays dans le tableau pays
      Skip(p_fichier);
      GetChaine(p_fichier,250,cont);//Copier la chaine de caractères du continent dans le tableau continent
      Skip(p_fichier);
      fscanf(p_fichier,"%d",&danger);
      tab[nb]=construireSite(nom,LA,LO,cat,pays,cont,danger); //Creation du site avec les arguments lus dans le fichier
      nb++;//Itération
  }

  free(nom); //Desallocation
  free(cat);
  free(pays);
  free(cont);
  fclose(p_fichier); //Fermeture du fichier

  //double lat_depart = 41.4, long_depart = 124.4;
  //double lat_depart = 48.8464111, long_depart = 2.3548468; //Coordonnées de Jussieu
  double lat_depart, long_depart; //Variable de la latitude et longitude de depart
  if(argc==3) //Si le nombre d'arguments est correct donc 3
  {
    sscanf(argv[1],"%lf",&lat_depart); //Lire la latitude dans les arguments du main
    sscanf(argv[2],"%lf",&long_depart);//Lire la longitude dans les arguments du main
  }
  else
  {
    exit(-1);//Abandonner le programme
  }

  double **M=NULL; //Création d'une matrice de distance
  M=matrices(tab,lat_depart,long_depart); double temps1=0.0; double temps2=0.0;

  listeSites * L1=NULL; //Création de la liste contenant l'itinéraire1
  listeSites * L2=NULL; //Création de la liste contenant l'itinéraire2
  listeSites * L3=NULL; //Création de la liste contenant l'itinéraire final
  Site depart; //Création du point de depart
  depart=construireSite("Depart",lat_depart,long_depart,"Unknown"," "," ",0);
  L1=itineraire(L1,tab,M,nbSites,depart,&temps1); //Creation de l'itinéraire1
  L2=itineraire2(L2,tab,M,nbSites,depart,&temps2); //Creation de l'itinéraire2
  printf("\nChallenge Unesco\n");
  L3=evaluation2(L1,L2,tab,temps1,temps2); //Comparer les 2 itinéraires et afficher celui a rapporte le plus de points
  end=clock(); //Fin du temps
  p=(double) (end - start) / CLOCKS_PER_SEC ;

  printf("Le temps d’execution est : %lf secondes\n",p); //Affichage du temps d'execution


  //Application Java
  FILE *tour_fichier ; //pointeur sur fichier
  tour_fichier = fopen("Tour.txt", "w") ; // Écriture dans le fichier Tour.txt
  if(tour_fichier == NULL) //Si erreur dans la création du fichier, sortir
    { printf("Erreur de création du fichier \n") ;
      exit(-1) ; //Abandonner le programme
    }

  listeSites * courant=L3; //Parcourt la liste contenant l'itinéraire
  while(courant!=NULL) //Tant que la fin de la liste n'est pas atteinte
  {
    fprintf(tour_fichier,"%lf,",courant->S.LAT); //Ecrire la latitude du site dans le fichier
    fprintf(tour_fichier,"%lf,",courant->S.LONG); //Ecrire la longitude du site dans le fichier
    fprintf(tour_fichier,"%s\n",courant->S.categorie); //Ecrire la categorie du site dans le fichier
    SkipLine(tour_fichier); //Passage a la ligne
    courant = courant -> suivant; //Prochain site
  }

  fclose(tour_fichier); //Fermeture du fichier

  const char * commandLine="java -jar UnescoMap.jar"; //Lancement de l'application JAVA
  system(commandLine);


  free(tab);
  for (int i=0;i<1100;i++)
  {
    free(M[i]);
  }
  free(M);
  supprimeListe(&L1); //Libération de memoire
  supprimeListe(&L2);

  return 0;
}
