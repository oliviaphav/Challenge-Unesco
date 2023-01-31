#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "site.h"
#include "liste.h"

listeSites * insertion(listeSites * L, Site S) //Insertion d'éléments dans une liste
{
  listeSites * courant=L; //Parcourir la liste L
  listeSites * nouveau; //Nouveau maillon
  nouveau = (listeSites *)malloc(sizeof(listeSites)); //Allocation du nouveau maillon
  nouveau -> suivant=NULL; //Prochain maillon nul
  nouveau -> S = S;

  if(L!=NULL) //Si la liste n'est pas nulle
  {
    while(courant -> suivant != NULL) //Tant que la liste n'est pas entierement parcouru
    {
      courant = courant -> suivant; //Passage au maillon suivant
    }
    courant -> suivant = nouveau; //Insertion à la fin du maillon
  }

  else //Si la liste est nulle
    L=nouveau; //Le nouveau maillon est le premier de la liste chainée

  return L ;
}

void affichageListe(listeSites * L) //Affichage d'une liste
{
  listeSites * courant = L; //Parcourir la liste L
  int i=0; //Nombre d'itération
  while (courant!=NULL) //Tant que la liste n'est pas entierement parcouru
  {
    printf("%d) ",i); //Affichage du numéro du site dans la liste exemple 1),2)
    affichageSite(courant->S); //Affichage du site avec fonction dans le fichier site.c site.h
    printf("\n");
    courant = courant -> suivant ;//Passage au site suivant
    i++;  //Itération
  }
}

void supprimeDebut(listeSites ** L) //Supprime le premier maillon
{
  listeSites * temp;
  if ((*L)!=NULL)
  {
    temp=(*L)->suivant ;
    free(*L); //Desallocation
    *L=temp;
  }
}

void supprimeListe(listeSites** L) //Supprime l'ensemble de la liste
{ while ((*L)!=NULL) //Tant que la liste n'est pas vide
  {
    supprimeDebut(L); //Supprime le premier maillon
  }
}


int presence(int valeur,int tab[]) //Teste la présence du site dans un tableau
{
  int a=0;//Variable d'itération
  for(int i=0;i<200;i++) //Le tableau contient les sites dans un itinéraire, le nombre de sites est inférieur à 200
  {
    if(valeur==tab[i]) //Si le numero du site est dans le tableau
    a=a+1; //Itération de a
  }
  return a; //Retour de a, a=0 si le numero du site n'est pas dans le tableau
}


listeSites * itineraire(listeSites * L, Site *tab,double **M,int nbSites, Site dep,double *temps)
/* Itinéraire méthode du plus proche voisin, liste contenant l'itinéraire, tableau avec tous les sites, matrices des distances,
le nombre de sites en tout, site dep correspondant au site de depart, pointeur sur variable temps défini dans le main*/
{
  int tabSites[200]={0};//Tableau contenant les numéros des sites qui vont être parcouru
  int site1=0;int site2=0;
  int depart=0; //double temps=0.0;
  double distance=0.0;
  int p=1; //Nombre d'itération pour le while
  L=insertion(L,dep);//Insertion du site correspondant au point de départ dans la liste

  for(int j=1;j<nbSites;j++) //Cherche la distance la plus petite avec le site de depart dans la matrice des distances
  {
    if(M[depart][j]!=0 && M[depart][j] < M[depart][site1]) //La distance doit être non nulle
    {
      site1=j;
    }
  }

  tabSites[0]=site1; //Le numero du premier site est enregistré dans le tableau de sites parcouru
  distance=M[depart][site1]; //Distance parcouru
  *temps=*temps + (M[depart][site1]/85)+ 7; //Temps mis pour la distance entre les 2 endroits et 7h pour la visite du site
  L=insertion(L,tab[site1]);//Insertion du premier site
  int a=site1; //Variable pour garder le site pour le temps de retour

  while(*temps<(504-7-(M[0][a]/85)))
  //Tant que le temps est inferieur à 3semaines moins 7h pour la visite du dernier site moins le temps du retour
  {
    for(int j=1;j<nbSites;j++) //Dans l'ensemble du tableau des sites
    {
      if(M[site1+1][site2]==0)//Si le premier site du tableau
      {
        site2=site2+2; //Comparer avec le prochain site
      }

      if(M[site1+1][j]!=0 && M[site1+1][j] < M[site1+1][site2] && presence(j,tabSites)==0 )
      //Cherche la distance la plus petite avec le site précedent dans la matrice des distances et teste la presence dans le tableau des sites deja parcouru
      {
        if (strcmp(tab[site1].categorie,tab[835].categorie)==0) //Si la catégorie du site précédent est mixte
        {
          if(strcmp(tab[j].categorie,tab[860].categorie)==0 || strcmp(tab[site1].categorie,tab[10].categorie)==0)
          //Le site en question doit être culturelle ou naturelle
            site2=j;
        }

        if (strcmp(tab[site1].categorie,tab[10].categorie)==0 ) //Si la catégorie du site précédent est culturelle
        {
          if (strcmp(tab[j].categorie,tab[835].categorie)==0 || strcmp(tab[j].categorie,tab[860].categorie)==0)
          //Le site en question doit être mixte ou naturelle
            site2=j;
        }

        if (strcmp(tab[site1].categorie,tab[860].categorie)==0 ) //Si la catégorie du site précédent est naturelle
        {
          if (strcmp(tab[j].categorie,tab[835].categorie)==0 || strcmp(tab[j].categorie,tab[10].categorie)==0)
          //Le site en question doit être culturelle ou mixte
            site2=j;
        }
      }
    }


    *temps=*temps + (M[site1+1][site2]/85) + 7; //Ajout au temps precedent 7h pour la visite et le temps du trajet entre les 2 points
    distance=distance+M[site1+1][site2]; //Ajout de la distance entre les 2 points
    if(*temps<(504-(M[0][site2]/85.0))) //Si le temps est inferieur a 3 semaines moins le temps du retour
      L=insertion(L,tab[site2]); //Insertion du site
    else
      *temps=*temps-(M[site1+1][site2]/85) - 7;//Sinon le site ne va pas être visité, on retire le temps du trajet et 7h pour la visite
    tabSites[p]=site2; //Ajout du numero du site dans le tableau des sites parcourus
    site1=site2; //L'arrivée du trajet precedent devient le depart du prochain
    site2=0;
    a=tabSites[p];//Garde le site calculer le temps de retour

    p++; //Itération
  }


  L=insertion(L,dep); //Insertion du retour
  *temps=*temps+(M[0][a]/85.0); //Temps total avec le trajet de retour
  distance=distance+M[0][site2]; //Distance totale avec le retour

  return L;
}

void evaluation(listeSites * L)
//Comptage de points d'un itinéraire
{
  listeSites * courant = L; //Parcourir la liste L1
  int nb_sites_visites=-2; int nb_pays=-1; int sites_danger=0; int i=0; int a=0;//Variables
  char **pays; //Tableau de chaines de caractères contenant les pays parcourus

  pays=(char **)malloc(60*sizeof(char *)); //Allocation du tableau
  for (int i=0;i<60;i++)
  {
    pays[i]= (char *)malloc(200*sizeof(char ));
  }

  while(courant!=NULL)//Tant que la fin de la liste n'est pas atteinte
  {
    nb_sites_visites++; //Comptage du nombre de sites dans la liste
    for(int j=0;j<40;j++) //Teste la presence du pays dans le tableau de pays
    {
      if(strcmp(courant -> S.pays,pays[j])==0)
        a=1;
    }
    pays[i]=strdup(courant -> S.pays); //Copie du pays dans le tableau de pays
    if(a==0) //Si le pays n'est pas dans le tableau
      nb_pays++; //Le nombre de pays visité augmente
    if(courant -> S.enDanger ==1) //Comptage du nombre de sites en danger
      sites_danger++;
    courant = courant -> suivant;//Prochain site
    i++;//Itération
    a=0;//Remise a zero de a
  }

  for (int i=0;i<60;i++)
  {
    free(pays[i]);
  }
  free(pays);

  printf("Evaluation :\n");
  printf("\t%d sites visités * 10 points = %d points\n",nb_sites_visites, nb_sites_visites*10);
  printf("\t%d pays visités * 20 points = %d points\n",nb_pays,nb_pays*20);
  printf("\t%d sites en danger visités * 30 points = %d points\n",sites_danger,sites_danger*30);
  printf("\tScore final : %d\n",nb_sites_visites*10+nb_pays*20+sites_danger*30);
}


listeSites * itineraire2(listeSites * L, Site *tab,double **M,int nbSites, Site dep,double *temps)
/* Itinéraire2 méthode du plus proche voisin + alternance des pays entre les sites, liste contenant l'itinéraire,
 tableau avec tous les sites, matrices des distances, le nombre de sites en tout, site dep correspondant au site de depart,
 pointeur sur variable temps défini dans le main */
{
  int tabSites2[200]={0};//Tableau contenant les numéros des sites qui vont être parcouru
  int site1=0;int site2=0;
  int depart=0; double distance=0.0;
  int p=1; //Nombre d'itération pour le while
  L=insertion(L,dep);//Insertion du site correspondant au point de départ dans la liste

  for(int j=1;j<nbSites;j++) //Cherche la distance la plus petite avec le site de depart dans la matrice des distances
  {
    if(M[depart][j]!=0 && M[depart][j] < M[depart][site1]) //La distance doit être non nulle
    {
      site1=j;
    }
  }

  tabSites2[0]=site1; //Le numero du premier site est enregistré dans le tableau de sites parcouru
  distance=M[depart][site1]; //Distance parcouru
  *temps=*temps + (M[depart][site1]/85)+ 7; //Temps mis pour la distance entre les 2 endroits et 7h pour la visite du site
  L=insertion(L,tab[site1]);//Insertion du premier site
  int a=site1; //Variable pour garder le site pour le temps de retour

  while(*temps<(504-7-(M[0][a]/85)))
  //Tant que le temps est inferieur à 3semaines moins 7h pour la visite du dernier site moins le temps du retour
  {
    for(int j=1;j<nbSites;j++) //Dans l'ensemble du tableau des sites
    {
      if(M[site1+1][site2]==0)//Si le premier site du tableau
      {
        site2=site2+2; //Comparer avec le prochain site
      }

      if(M[site1+1][j]!=0 && M[site1+1][j] < M[site1+1][site2] && presence(j,tabSites2)==0 && strcmp(tab[site1].pays,tab[j].pays)!=0)
      //Cherche la distance la plus petite avec le site précedent dans la matrice des distances et teste la presence dans le tableau des sites deja parcouru
      {
        if (strcmp(tab[site1].categorie,tab[835].categorie)==0) //Si la catégorie du site précédent est mixte
        {
          if(strcmp(tab[j].categorie,tab[860].categorie)==0 || strcmp(tab[site1].categorie,tab[10].categorie)==0)
          //Le site en question doit être culturelle ou naturelle
            site2=j;
        }

        if (strcmp(tab[site1].categorie,tab[10].categorie)==0 ) //Si la catégorie du site précédent est culturelle
        {
          if (strcmp(tab[j].categorie,tab[835].categorie)==0 || strcmp(tab[j].categorie,tab[860].categorie)==0)
          //Le site en question doit être mixte ou naturelle
            site2=j;
        }

        if (strcmp(tab[site1].categorie,tab[860].categorie)==0 ) //Si la catégorie du site précédent est naturelle
        {
          if (strcmp(tab[j].categorie,tab[835].categorie)==0 || strcmp(tab[j].categorie,tab[10].categorie)==0)
          //Le site en question doit être culturelle ou mixte
            site2=j;
        }
      }
    }


    *temps=*temps + (M[site1+1][site2]/85) + 7; //Ajout au temps precedent 7h pour la visite et le temps du trajet entre les 2 points
    distance=distance+M[site1+1][site2]; //Ajout de la distance entre les 2 points
    if(*temps<(504-(M[0][site2]/85.0))) //Si le temps est inferieur a 3 semaines moins le temps du retour
      {
        L=insertion(L,tab[site2]);//Insertion du site
      }
    else
      {
        *temps=*temps-(M[site1+1][site2]/85) - 7;
        distance=distance-M[site1+1][site2];
      }

    tabSites2[p]=site2; //Ajout du numero du site dans le tableau des sites parcourus
    site1=site2; //L'arrivée du trajet precedent devient le depart du prochain
    site2=0; //Ajout du numero du site dans le tableau des sites parcourus
    a=tabSites2[p];//Garde le site pour calculer le temps de retour

    p++;
  }

  L=insertion(L,dep); //Insertion du retour
  *temps=*temps+(M[0][a]/85.0); //Temps totale avec le trajet de retour
  distance=distance+M[0][site2]; //Distance totale avec le retour
  return L;
}

listeSites * evaluation2(listeSites * L1,listeSites * L2,Site * tab,double temps1, double temps2)
//Comptage de points et comparaison entre deux itinéraires, afficher celui qui a le plus de points
{
  listeSites * courant1 = L1; //Parcourir la liste L1
  int nb_sites_visites=-2; int nb_pays=-1; int sites_danger=0; int i=0; int a=0;//Variables
  char **pays; //Tableau de chaines de caractères contenant les pays parcourus

  pays=(char **)malloc(60*sizeof(char *)); //Allocation du tableau
  for (int i=0;i<60;i++)
  {
    pays[i]= (char *)malloc(200*sizeof(char ));
  }

  int natural1=0,cultural1=0,mixed1=0;
  while(courant1!=NULL)//Tant que la fin de la liste n'est pas atteinte
  {
    nb_sites_visites++; //Comptage du nombre de sites dans la liste
      if (strcmp(courant1 -> S.categorie,tab[860].categorie)==0)
      /* Si la catégorie est naturelle. Comparaison avec la catégorie d'un site
      dans le tableau avec l'ensemble des sites, site 860 est un site naturel */
        natural1++;
      if (strcmp(courant1 -> S.categorie,tab[10].categorie)==0)
      /* Si la catégorie est culturelle. Comparaison avec la catégorie d'un site
      dans le tableau avec l'ensemble des sites, site 10 est un site culturel */
        cultural1++;
      if (strcmp(courant1 -> S.categorie,tab[835].categorie)==0)
      /* Si la catégorie est mixte. Comparaison avec la catégorie d'un site
      dans le tableau avec l'ensemble des sites, site 10 est un site mixte */
        mixed1++;
    for(int j=0;j<40;j++) //Teste la presence du pays dans le tableau de pays
    {
      if(strcmp(courant1 -> S.pays,pays[j])==0)
        a=1;
    }
    pays[i]=strdup(courant1 -> S.pays); //Copie du pays dans le tableau de pays
    if(a==0) //Si le pays n'est pas dans le tableau
      nb_pays++; //Le nombre de pays visité augmente
    if(courant1 -> S.enDanger ==1) //Comptage du nombre de sites en danger
      sites_danger++;
    courant1 = courant1 -> suivant;//Prochain site
    i++;//Itération
    a=0;//Remise a zero de a
  }

  listeSites * courant2 = L2; //Parcourir la liste L1
  int nb_sites_visites2=-2; int nb_pays2=-1; int sites_danger2=0; i=0; int b=0;//Variables
  char **pays2; //Tableau de chaines de caractères contenant les pays parcourus

  pays2=(char **)malloc(60*sizeof(char *)); //Allocation du tableau
  for (int i=0;i<60;i++)
  {
    pays2[i]= (char *)malloc(200*sizeof(char ));
  }

  int natural2=0,cultural2=0,mixed2=0;
  while(courant2!=NULL)//Tant que la fin de la liste n'est pas atteinte
  {
    nb_sites_visites2++; //Comptage du nombre de sites dans la liste

      if (strcmp(courant2 -> S.categorie,tab[860].categorie)==0)
      /* Si la catégorie est naturelle. Comparaison avec la catégorie d'un site
      dans le tableau avec l'ensemble des sites, site 860 est un site naturel */
        natural2++;
      if (strcmp(courant2 -> S.categorie,tab[10].categorie)==0)
      /* Si la catégorie est culturelle. Comparaison avec la catégorie d'un site
      dans le tableau avec l'ensemble des sites, site 10 est un site culturel */
        cultural2++;
      if (strcmp(courant2 -> S.categorie,tab[835].categorie)==0)
      /* Si la catégorie est mixte. Comparaison avec la catégorie d'un site
      dans le tableau avec l'ensemble des sites, site 10 est un site mixte */
        mixed2++;
    for(int j=0;j<40;j++) //Teste la presence du pays dans le tableau de pays
    {
      if(strcmp(courant2 -> S.pays,pays2[j])==0)
        b=1;
    }
    pays2[i]=strdup(courant2 -> S.pays); //Copie du pays dans le tableau de pays
    if(b==0) //Si le pays n'est pas dans le tableau
      nb_pays2++; //Le nombre de pays visité augmente
    if(courant2 -> S.enDanger ==1) //Comptage du nombre de sites en danger
      sites_danger2++;
    courant2 = courant2 -> suivant;//Prochain site
    i++;//Itération
    b=0;//Remise a zero de a
  }

  for (int i=0;i<60;i++)
  {
    free(pays[i]);
  }
  free(pays);

  for (int i=0;i<60;i++)
  {
    free(pays2[i]);
  }
  free(pays2);

  //Si l'itinéraire 1 génère le plus de points alors afficher l'itinéraire 1
  if(nb_sites_visites*10+nb_pays*20+sites_danger*30 > nb_sites_visites2*10+nb_pays2*20+sites_danger2*30)
  {
    printf("Durée totale du trajet : %3.2lfh\n",temps1);
    printf("Le nombre de sites naturels : %d\n",natural1);
    printf("Le nombre de sites culturels : %d\n",cultural1);
    printf("Le nombre de sites mixtes : %d\n",mixed1);
    printf("\n");
    printf("Itinéraire de voyage :\n");
    affichageListe(L1);
    printf("\n");
    printf("Evaluation :\n");
    printf("\t%d sites visités * 10 points = %d points\n",nb_sites_visites, nb_sites_visites*10);
    printf("\t%d pays visités * 20 points = %d points\n",nb_pays,nb_pays*20);
    printf("\t%d sites en danger visités * 30 points = %d points\n",sites_danger,sites_danger*30);
    printf("\tScore final : %d\n",nb_sites_visites*10+nb_pays*20+sites_danger*30);
    return L1;
  }
  else //Sinon afficher l'itinéraire 2
  {
    printf("Durée totale du trajet : %3.2lfh\n",temps2);
    printf("Le nombre de sites naturels : %d\n",natural2);
    printf("Le nombre de sites culturels : %d\n",cultural2);
    printf("Le nombre de sites mixtes : %d\n",mixed2);
    printf("\n");
    printf("Itinéraire de voyage :\n");
    affichageListe(L2);
    printf("\n");
    printf("Evaluation :\n");
    printf("\t%d sites visités * 10 points = %d points\n",nb_sites_visites2, nb_sites_visites2*10);
    printf("\t%d pays visités * 20 points = %d points\n",nb_pays2,nb_pays2*20);
    printf("\t%d sites en danger visités * 30 points = %d points\n",sites_danger2,sites_danger2*30);
    printf("\tScore final : %d\n",nb_sites_visites2*10+nb_pays2*20+sites_danger2*30);
    return L2;
  }
}
