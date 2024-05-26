//Mode Consultation
//Inclusion des bibliothèques
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Définition de la taile maximale de chaîne de caractères pour les noms des athlètes et le maximum des athlètes//
#define MAXCHAR 100
#define MAXATH 300
// Nombre maximal de participants au relais//
#define MAX_PARTICIPANTS_RELAIS 4 

// Définition de l'énumeration pour représenter les différents épreuves//
typedef enum {
  _100m,
  _400m,
  _5000m,
  _marathon,
  _relais_4x400m,
} Epreuve;

// Défintion de la structure temps//
typedef struct {
  int heure;
  int minutes;
  double secondes;
} Temps;

//Définition de la strcuture date//
typedef struct{
   int jour;
   int mois;
   int annee;
}Date;

typedef struct {
  char nom[MAXCHAR];
  char prenom[MAXCHAR];
  char sexe; 
  char epreuve;
  char moyenne;
  //char sexe; //'M' pour masculin, 'F' pour féminin//
  FILE *perf; // Pointeur vers un tableau dynamique pour stocker les temps de chaque épreuve//
  Temps *temps;
} Athlete;    

// Structure pour représenter un participant au relais//
typedef struct {
  char nom[MAXCHAR];
  char prenom[MAXCHAR];
  //char sexe;
  int position; // position dans le relais (1ère, 2ème, 3ème ou 4ème)//
} ParticipantRelais;

// Structure pour représenter un entraînement//
typedef struct {
  char date[15]; // format de date : "JJ/MM/AAAA"
  Epreuve type_epreuve;
  Temps *temps;
  ParticipantRelais participants[MAX_PARTICIPANTS_RELAIS];
} Entrainement;

//Structure pour les données de performance//
typedef struct {
 double x; //temps (en secondes)//
  double y; //performances des athlètes//
}Data;

// Fonction pour créer le fichier de l'athlète//
//void creer_fichier_athlete(char nomAthlete[]) {
  //FILE *fichier = fopen("nomAthlete", "w"); // ouvrir le fichier en mode écriture//
 //Vérification si le fichier a bien été ouvert//
  //if (fichier == NULL) {
   // printf("Ouverture du fichier impossible %s\n", nomAthlete);
   // exit(1);
  //}
  //fclose(fichier);
//}

// Fonction convertissant le temps en secondes//
double temps_en_secondes(Temps *temps){
int heure = temps->heure*3600;
int minutes = temps->minutes*60;
double secondes = temps->secondes;
return heure+minutes+secondes;
}

// Fonction pour afficher le temps sous le format HH:MM:SS//
Temps * temps_en_heure_minutes_secondes(double temps_en_seconde) {
Temps *temps = malloc(sizeof(Temps));
temps->heure = (int)temps_en_seconde/3600;
temps->minutes = (temps->heure)%3600/60;
temps->secondes = (temps->minutes)%60;
return temps;
}
//Fonction est heure valide pour sauvegarder des détails//
int est_heure_valide(int heure, int minutes, double secondes) {
    if (heure < 0 || heure > 3) {
        return 0;
    }
    if (minutes < 0 || minutes > 59) {
        return 0;
    }
    if (secondes < 0.0 || secondes >= 60.0) {
        return 0;
    }
    return 1;
}

// Fonction pour vérifier si la date est correcte//
int est_date_valide(const char *date) {
  int jour, mois, annee;
  if (sscanf(date, "%d/%d/%4d", &jour, &mois, &annee) != 3) {
      return 0;
  }
  if (annee < 1900 || annee > 2024 || mois < 1 || mois > 12 || jour < 1 || jour > 31) {
      return 0;
  }
  // Vérifier les jours en fonction du mois
  if ((mois == 4 || mois == 6 || mois == 9 || mois == 11) && jour > 30) {
      return 0;
  }
  if (mois == 2) {
      int bissextile = (annee % 4 == 0 && (annee % 100 != 0 || annee % 400 == 0));
      if (jour > 28 + bissextile) {
          return 0;
      }
  }
  return 1;
}
// Fonction affichant les épreuves se déroulant en entrant une date//
void afficher_epreuves_en_date(const char *date) {
if (!est_date_valide(date)) {
printf("La date n'est pas valide, réessayez.\n");
} else {
FILE *fichier = fopen("Equipes_relais.txt", "r"); // ouvrir en mode lecture
if (fichier == NULL) {
printf("Le fichier Equipes_relais.txt n'existe pas, cela signifie donc qu'il n'y a pas d'équipe de relais pour le moment.\n");
} else {
char ligne[200];
int epreuves_trouvees = 0;
printf("Les épreuves se déroulant le %s sont : \n", date);
while (fgets(ligne, sizeof(ligne), fichier)) {
    // Vérifiez si la ligne contient la date au début
    if (strncmp(ligne, date, strlen(date)) == 0) {
        printf("%s", ligne);
        epreuves_trouvees = 1;
    }
}

if (!epreuves_trouvees) {
    printf("Aucun entrainement de relais trouvée pour la date %s.\n", date);
}

fclose(fichier);
}
}
}

// Fonction pour consulter le temps d'un athlète pour une épreuve//
void consulter_temps (Athlete *athlete, Epreuve epreuve){
    printf("Le temps de l'athlete %s %s pour l'épreuve %c est de %d heures, %d minutes, %lf secondes\n", athlete->nom, athlete->prenom, athlete->epreuve,athlete->temps->heure, athlete->temps->minutes, athlete->temps->secondes);

}

  // Fonction pour entrer les détails d'un nouvel entraînement//
  void entrer_sauvegarde_details(Athlete *athlete, Entrainement *entrainement) {
    int verification1=0, verification2=0, i=0;
    char nom_fichier[MAXCHAR *2];

    while(verification1 == 0){
      printf("Entrez la date de l'entraînement (format JJ/MM/AAAA) : \n");
      scanf("%s", entrainement->date);
      verification1 = est_date_valide(entrainement->date);
      if (verification1 == 0){
        printf("La date entree n'est pas valide. Veuillez réessayer.\n");
      }
    }
    printf("Date enregistrée avec succès.\n");

    while(verification2 == 0){
      printf("Heure : ");
      scanf("%d", &entrainement->temps->heure);
      printf("Minutes : ");
      scanf("%d", &entrainement->temps->minutes);
      printf("Secondes : ");
      scanf("%lf", &entrainement->temps->secondes);
      //Utilisation de la fonction est_heure_valide//
      verification2 = est_heure_valide(entrainement->temps->heure, entrainement->temps->minutes, entrainement->temps->secondes);
      if (verification2 == 0){
        printf("Erreur de saisie du temps. Veuillez réessayer.\n");
      }
    }
    printf("Temps enregistré avec succès\n");

    printf("Enregistrement des participants à l'entraînement :\n");
    for (i = 0; i < MAX_PARTICIPANTS_RELAIS; i++) {
      char nom[MAXCHAR], prenom[MAXCHAR];
      printf("Participant %d:\nNom: ", i + 1);
      scanf("%s", nom);
      printf("Prenom: ");
      scanf("%s", prenom);
      snprintf(nom_fichier, sizeof(nom_fichier), "%s_%s.txt", nom, prenom);
      FILE *fichier = fopen(nom_fichier, "r");
      if (fichier == NULL) {
        printf("Le fichier %s n'existe pas, veuillez réessayer.\n", nom_fichier);
        i--; // Décrémenter pour rester au même indice de participants
      } else {
        fclose(fichier);
      }
    }
  }

  // Fontion pour permettre à l'entraîneur de consulter les statistiques de performances de chaque athlète - les meilleurs, pire temps et la moyenne pour chaque épreuve//
        void consulter_statistiques(Athlete *athlete, Entrainement *entrainement, Epreuve epreuve) {
          Athlete *nouvelathlete = malloc(sizeof(Athlete));
          char nom_fichier[MAXCHAR * 2];
          char ligne[MAXCHAR * 5]; // Variable pour lire chaque ligne du fichier
          int verification = 0;
          int compt = 0;
          Temps *temps = malloc(2 * sizeof(Temps));
          while (verification == 0) {
            printf("\nSaisir le nom de l'athlete recherché :");
            scanf("%s", athlete->nom);
            printf("\nSaisir le prénom de l'athlete recherché :");
            scanf("%s", athlete->prenom);
            snprintf(nom_fichier, sizeof(nom_fichier), "%s_%s.txt", athlete->nom, athlete->prenom);
            // Vérifier si le fichier exsiste en mode lecture//
            FILE *fichier = fopen(nom_fichier, "r");
            if (fichier == NULL) {
              printf("Le fichier %s n'existe pas, veuillez le renseigner à nouveau d'une manière correcte\n", nom_fichier);
              verification = 0;
            } 
            else {
              Entrainement *entrainement = malloc(40 * sizeof(Entrainement));
              while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
                sscanf(ligne, "%s : %dh%dmin%lf", entrainement[compt].date, &entrainement[compt].temps->heure, &entrainement[compt].temps->minutes, &entrainement[compt].temps->secondes);
                compt++;
              }
              verification++;
              // Ouvrir le fichier de l'athlète en mode lecture//
              double *performances = malloc((compt + 2) * sizeof(double));
              double meilleurTemps;
              double pireTemps;
              for (int i = 0; i < compt; i++) {
                performances[i] = temps_en_secondes(entrainement[i].temps);
              }
              meilleurTemps = performances[0];
              pireTemps = performances[0];
              for (int i = 1; i < compt; i++) {
                if (performances[i] < meilleurTemps) {
                  meilleurTemps = performances[i];
                }
                if (performances[i] > pireTemps) {
                  pireTemps = performances[i];
                }
                temps[0] = *temps_en_heure_minutes_secondes(meilleurTemps);
                temps[1] = *temps_en_heure_minutes_secondes(pireTemps);
                printf("L'athlète %s %s a obtenu un meilleur temps de %lf pour l'épreuve %d:\n", athlete->nom, athlete->prenom, meilleurTemps, epreuve);
                printf("L'athlète %s %s a obtenu un pire temps de %lf pour l'épreuve %d \n", athlete->nom, athlete->prenom, pireTemps,epreuve);
                free(performances);
                //free(meilleurTemps);
                //free(pireTemps);
                fclose(fichier);
              }
            }
          }
        }

       // Calcul du temps moyen pour chaque athlète//
    void calculer_moyenne(Athlete *athlete, Entrainement *entrainement) {
      Athlete* nouvelathlete=malloc(sizeof(Athlete));
      char nom_fichier[MAXCHAR*2];
      char ligne[MAXCHAR * 5]; // Variable pour lire chaque ligne du fichier
      int verification = 0;
      int compt=0;
      Temps *temps=malloc(sizeof(Temps));
      while(verification == 0){
        printf("\nSaisir le nom de l'athlète recherché : ");
        scanf("%s", athlete->nom);
        printf("\nSaisir le prénom de l'athlète recherché : ");
        scanf("%s", athlete->prenom);
        char nom_fichier[MAXCHAR*2]; //Declarer le variable nom_fichier//
        snprintf(nom_fichier, sizeof(nom_fichier),"%s_%s.txt", athlete->nom, athlete->prenom);

           // Vérifier si le fichier existe en mode lecture//
            FILE *fichier=fopen(nom_fichier, "r");
            Entrainement *entrainement=malloc(40*sizeof(Entrainement));
            if(fichier==NULL) {
              printf("Le fichier %s n'existe pas, veuillez le renseigner à nouveau d'une manière correcte\n" , nom_fichier);
              verification = 0; 
            }
            else{
              while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
                sscanf(ligne, "%s:%dh%dmin%lf", entrainement[compt].date, &entrainement[compt].temps->heure, &entrainement[compt].temps->minutes, &entrainement[compt].temps->secondes);
                compt++; 
              }
              verification++;

              // Ouvrir le fichier de l'athlète en mode lecture//
             //Temps* temps= malloc(sizeof(Temps));
            double *performances=malloc((compt+2)*sizeof(double));
            double somme=0; 
            for(int i=0;i<compt;i++){ 
              performances[i]=temps_en_secondes(entrainement[i].temps);
              somme+=performances[i];
            }
               double moyenne=somme/compt;
               temps = temps_en_heure_minutes_secondes(moyenne);
               printf("La moyenne de temps de l'athlète %s %s est de %lf pour l'épreuve %d\n", athlete->nom, athlete->prenom, moyenne, athlete->epreuve);
               fclose(fichier);
         } 
      }
    }
// Fonction comparant les performances des athlete pour les classer dans un ordre décroissant//
int comparer_performances(const void *a, const void *b, Athlete *athlete){
  Athlete *nouvelathlete= malloc(sizeof(Athlete));
  char nom_fichier[MAXCHAR*2];
  char ligne[MAXCHAR *5]; // Variable pour lire chaque ligne du fichier
  int verification=0;
  int compt=0;
  Temps *temps=malloc(sizeof(Temps));
  while(verification == 0) {
      printf("\nSaisir le nom de l'athlète recherché : ");
      scanf("%s", athlete->nom);
      printf("\nSaisir le prénom de l'athlète recherché : ");
      scanf("%s", athlete->prenom);
      snprintf(nom_fichier, sizeof(nom_fichier),"%s_%s.txt" , athlete->nom, athlete->prenom);

     // Vérifier si le fichier existe en mode lecture//
      FILE *fichier=fopen(nom_fichier, "r");
      Entrainement *entrainement=malloc(40*sizeof(Entrainement));
      if(fichier==NULL) {
      printf("Le fichier %s n'existe pas, veuillez le renseigner à nouveau d'une manière correcte\n" , nom_fichier);
      verification = 0; 
      }
      else{
        while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
          sscanf(ligne, "%s:%dh%dmin%lf", entrainement[compt].date, &entrainement[compt].temps->heure, &entrainement[compt].temps->minutes, &entrainement[compt].temps->secondes);
          compt++; 
        }
        verification++;
 // generer par ChatGPT//
  Athlete *athleteA = (Athlete *)a;
  Athlete *athleteB = (Athlete *)b;
  if (athleteA->moyenne < athleteB->moyenne) return -1;
  if (athleteA->moyenne > athleteB->moyenne) return 1;
  return 0;
}
  }
}


  void selectionner_meilleurs_athletes(Athlete *athlete, Entrainement* entrainement, Epreuve epreuve) {
          Athlete *nouvelathlete = malloc(sizeof(Athlete));
          char nom_fichier[MAXCHAR*2];
          char ligne[MAXCHAR *5]; // Variable pour lire chaque ligne du fichier
          int verification=0;
          int compt=0;
          Temps *temps=malloc(sizeof(Temps));
          while(verification == 0) {
              printf("\nSaisir le nom de l'athlète recherché : ");
              scanf("%s", athlete->nom);
              printf("\nSaisir le prénom de l'athlète recherché : ");
              scanf("%s", athlete->prenom);
              snprintf(nom_fichier, sizeof(nom_fichier),"%s_%s.txt" , athlete->nom, athlete->prenom);

             // Vérifier si le fichier existe en mode lecture//
              FILE *fichier=fopen(nom_fichier, "r");
              Entrainement *entrainement=malloc(40*sizeof(Entrainement));
              if(fichier==NULL) {
              printf("Le fichier %s n'existe pas, veuillez le renseigner à nouveau d'une manière correcte\n" , nom_fichier);
              verification = 0; 
              }
              else{
                while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
                  sscanf(ligne, "%s:%dh%dmin%lf", entrainement[compt].date, &entrainement[compt].temps->heure, &entrainement[compt].temps->minutes, &entrainement[compt].temps->secondes);
                  compt++; 
                }
                verification++;

                // Ouvrir le fichier de l'athlète en mode lecture//
                double *performances=malloc((compt+2)*sizeof(double));

                   
        printf(" Les trois meilleurs athlètes %s %s, %s %s et %s %s ont obtenu les meilleurs temps moyens pour cette épreuve %d:\n", athlete[0].nom, athlete[0].prenom, athlete[1].nom, athlete[1].prenom, athlete[2].nom, athlete[2].prenom, epreuve);
            
        }
        }
          }

  
  void resume_performances(Athlete *athlete, Epreuve epreuve) {

    int compt = 0;
      Temps *performances_temps = malloc(MAXATH * sizeof(Temps));
      double *performances_secondes = malloc(MAXATH * sizeof(double));

      // Lire les performances de l'athlète depuis le fichier
      char nom_fichier[MAXCHAR * 2];
      snprintf(nom_fichier, sizeof(nom_fichier), "%s_%s.txt", athlete->nom, athlete->prenom);
      FILE *fichier = fopen(nom_fichier, "r");
      if (fichier == NULL) {
        printf("Le fichier %s n'existe pas.\n", nom_fichier);
        return;
      }

      char ligne[MAXCHAR * 5];
      while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        sscanf(ligne, "%*s : %dh%dmin%lf", &performances_temps[compt].heure, &performances_temps[compt].minutes, &performances_temps[compt].secondes);
        performances_secondes[compt] = temps_en_secondes(&performances_temps[compt]);
        compt++;
      }
      fclose(fichier);

      // Calculer la moyenne, le meilleur et le pire temps
      double somme = 0;
      double meilleurTemps = performances_secondes[0];
      double pireTemps = performances_secondes[0];
      for (int i = 0; i < compt; i++) {
        if (performances_secondes[i] < meilleurTemps) {
          meilleurTemps = performances_secondes[i];
        }
        if (performances_secondes[i] > pireTemps) {
          pireTemps = performances_secondes[i];
        }
        somme += performances_secondes[i];
      }
      double moyenne = somme / compt;

      // Afficher le résumé des performances
      printf("Résumé des performances pour l'athlète %s %s durant l'épreuve %d :\n", athlete->nom, athlete->prenom, epreuve);
      printf("  Meilleur temps : %lf secondes\n", meilleurTemps);
      printf("  Pire temps : %lf secondes\n", pireTemps);
      printf("  Moyenne : %lf secondes\n", moyenne);

      free(performances_temps);
      free(performances_secondes);
    }


  // Fonction pour calculer les différences et comparer//
        //void 
       // Ajouter une fonction pour calculer la différence de temps : paramètres : le nom de l'athlète, l'épreuve, la date du premier entraînement et la date du deuxieme entrainement

        // Fonction pour calculer la différence de temps entre les deux entraînements (entre 2 dates)//
        void calculer_difference_temps(Athlete *athlete, int nb_athletes, Epreuve epreuve, Temps date1, Temps date2, int N) {
          Temps temps = {0,0,0.5};
          // Initialise la structure Temps avec les valeurs 0 pour 'heure' et 'minutes', et 5.0 pour 'secondes'//


  //Calculer la différence de temps entre les deux entraînements//
        double temps_difference = (date2.heure - date1.heure) * 3600 + (date2.minutes - date1.minutes) * 60 + (date2.secondes - date1.secondes);
          if(temps_difference < 0){
            temps_difference = -temps_difference; //Convertir negative différence à positive//
          }
          printf("La différence de temps entre les deux entraînements est de %lf secondes\n", temps_difference);


//Fonction pour calculer l'équation de la droite d'évolution des performances d'un athlète//
Data data[N]; //déclaration et initilsiation tableau des données//
          for (int i = 0; i < N; i++) {
              data[i].x = i; //valeur pour x
              data[i].y = i * 2; 
          }
        //Σ(x_i - x_moy)² : somme des carrés des écarts à la moyenne des valeurs x
       // Σ(y_i - y_moy) : somme des écarts à la moyenne des valeurs y
       // Σ(x_i - x_moy)(y_i - y_moy) : somme des produits des écarts à la moyenne des valeurs x et y
        // y = ax+b
        //a=Σ(x_i - x_moy)² / Σ(y_i - y_moy)
        }
          //fonction pour calculer la moyenne//
      double moyenne(const double* valeurs, int n){
          double somme = 0;
            for(int i=0; i<n; i++){
              somme =+- valeurs[i];
          }
          return somme/n;
      }
        //fonction pour calculer la somme des carrées des écarts à la moyenne//
    double somme_carres_ecarts(double* valeurs, double moyenne, int n){
      double somme = 0;
        for (int i=0; i<n; i++){
           somme=+-pow(valeurs[i] - moyenne, 2);
        }
      return somme;
    }


      //fonction ppour calculer la somme des produits des écarts à la moyenne//
    double somme_produits_ecarts(const double* valeurs_x, const double* valeurs_y, double x_moyenne, double y_moyenne, int n){
      double somme = 0;
        for (int i=0; i<n; i++){
            somme =+- (valeurs_x[i] - x_moyenne) * (valeurs_y[i] - y_moyenne);
      }
      return somme;
    }
    //Fonction pour calculer l'equation de la droite de régression linéaire//
// Generer avec ChatGPT;
     void calculer_droite_regression(int n, double* a, double* b, Data data){
            double x[n];
            double y[n];

       
            //calcul des moyennes//
            double x_moyenne = moyenne(x,n);
            double y_moyenne = moyenne(y,n);

            //calcul des sommes des carrées des écarts à la moyenne
            double x_somme_carres = somme_carres_ecarts(x,x_moyenne, n);
            double y_somme_carres = somme_carres_ecarts(y,y_moyenne, n);
            double xy_somme_produits= somme_produits_ecarts(x,y, x_moyenne, y_moyenne, n);

            // calcul de la pente (a) et de l'ordonnée à l'origine (b)
            *a = xy_somme_produits / x_somme_carres;
            *b = y_moyenne - (*a) * x_moyenne;
          }


//Programme principal//
 int main(){
      Athlete athlete;
      Entrainement entrainement;
      Temps t;
      Temps temps;
      Epreuve epreuve;
      double MeilleurTemps;
      double PireTemps;
      Data data;
     double a;
     double b;
    int n;

      int nb_athletes = 0; //Initialisation de nb_athletes à 0//
      int max_athletes = MAXATH; //Initialisation de max_athletes à MAXATH//
      Athlete *athletes = malloc(max_athletes * sizeof(Athlete)); //Allocation dynamique pour le tableau d'athlètes//
      if (athletes == NULL){
        printf("Erreur de l'allocation mémoire\n");
        exit(1);
        }

    // code pour initialiser les structures
  while (1) {
        printf("Menu :\n");
        printf("1. Consulter un temps\n");
        printf("2. Entrez et sauvegardez les informations d'un nouvel entraînement\n");
        printf("3. Consulter les statistiques de performances de chaque athlète\n");
        printf("4. Afficher un résumé des performances d'un athlete\n");
        printf("5. Calculer la moyenne\n");
        printf("6. Selectionner les 3 meilleurs athlètes pour une discipline\n");
        printf("7. Afficher la différence de temps d'un athlete pour une meme épreuve entre deux dates\n");
        printf("8. Calculer l'équation de la droite de régression linéaire\n");
        printf("9. Quitter\n");

        int choix;
        scanf("%d", &choix);
        switch (choix) {
          case 1:
            consulter_temps(&athlete, epreuve);
            break;
          case 2:
            entrer_sauvegarde_details(&athlete, &entrainement);
            break;
          case 3:
            consulter_statistiques(&athlete, &entrainement, epreuve);
            break;
          case 4:
            resume_performances(&athlete, entrainement.type_epreuve);
            break;
          case 5:
            calculer_moyenne(&athlete, &entrainement);
            break;
          case 6:
            selectionner_meilleurs_athletes(&athlete, &entrainement, epreuve);
            break;
          case 7:
            calculer_difference_temps( &athlete, nb_athletes, entrainement.type_epreuve,t, temps, 5);
            break;
          case 8:
             calculer_droite_regression(n, &a, &b, data);
            break;
          case 9:
            printf("Quitter le programme\n");
            return 0;
          default:
            printf("Choix invalide\n");
        }
        return 0;
        }

   
        //Demander le nombre d'athlètes a ajouter//
        printf("Combien d'athletes voulez-vous d'ajouter?");
        scanf("%d", &nb_athletes);

  char nomAthlete[]="athletes.txt";
  FILE *fichier = fopen(nomAthlete, "a");
  printf("Fichier de l'athlète cree avec succès\n");

  //le nombre maximal d'athletes//  int max_athletes=300;
 athletes = malloc(max_athletes* sizeof(Athlete)); //allocation de la mémoire pour athletes
  //int nb_athletes = 0;



  // Liberez la memoire allouee dynamiquement//
  free(athletes);

  // consultation des temps pour differents épreuves//
    printf("Temps pour le 100m est: %2f", athlete.temps->secondes);
    printf("Temps pour le 400m est: %2f", athlete.temps->secondes);
    printf("Temps pour le 500m est: %2f", athlete.temps->secondes);
    printf("Temps pour le marathon est %d: %d: %lf", athlete.temps->heure, athlete.temps->minutes, athlete.temps->secondes);
    printf("Temps pour le relais est %2f", athlete.temps->secondes);

   
//  Athlete athlete;
  printf("Entrez le nom de l'athlète : ");
  scanf("%s", athlete.nom);
  printf("Entrez le prénom de l'athlète : ");
  scanf("%s", athlete.prenom);
  printf("Entrez le sexe de l'athlète (M ou F) : ");
  scanf("%c", &athlete.sexe);

  // Demander à l'utilisateur de saisir le type d'épreuve


  // Demander à l'utilisateur de saisir le detail des athletes
  printf("Entrez la date de l'entraînement (format JJ/MM/AAAA) : ");
  scanf("%s", entrainement.date);
  int type_epreuve_input;
  printf("Entrez le type d'épreuve (1: 100m, 2: 400m, 3: 5000m, 4: marathon, 5: relais 4x100m) : ");
  scanf("%d", &type_epreuve_input);
  entrainement.type_epreuve = (Epreuve)type_epreuve_input;
  printf("Entrez le temps de l'épreuve (format HH:MM:SS) : ");
  scanf("%lf", &entrainement.temps->secondes);


  //Enregistrer les perfomances dans le fichier
//  enregistrer_entrainement_fichier(nomAthlete,&entrainement);

        // Afficher le resume des performances
   int verification = 0; //déclaration de la variable verification//
   double somme = 0; //déclartion de la somme//
   int compt = 0; //déclaration de compt//

   if (verification != 0) {
     double *performances = malloc((compt + 2) * sizeof(double));
     double meilleurTemps = performances[0]; //déclaration meilleurTemps
     double pireTemps = performances[0]; //déclaration pireTemps

     for (int i = 1; i < compt; i++) {
         if (performances[i] < meilleurTemps) {
             meilleurTemps = performances[i];
         }
         if (performances[i] > pireTemps) {
             pireTemps = performances[i];
         }
     }


     // Calcul de somme et de compt//
     for (int i = 0; i < compt; i++) {
       //performances[i] = temps_en_secondes(entrainement[i].temps);
       somme += performances[i]; // la somme
     }
     double moyenne = somme / compt;
     for (int i = 1; i < compt; i++) {
       if (performances[i] < meilleurTemps) {
           meilleurTemps = performances[i];
       }
       if (performances[i] > pireTemps) {
           pireTemps = performances[i];
       }
     }

     printf("Résumé des performances pour l'athlète %s %s durant l'épreuve %d : Meilleur temps : %lf, Pire temps : %lf, Moyenne : %lf\n", athlete.nom, athlete.prenom, athlete.epreuve, meilleurTemps, pireTemps, moyenne);
   }
   else {
     printf("Aucune donnée de performance n'a été trouvée pour l'athlète %s %s\n", athlete.nom, athlete.prenom);
   }
  // Afficher les trois meilleurs athletes selectione pour aller aux J-O//
printf("Les trois meilleurs athlètes pour les J-O sont : %s %s, %s %s et %s %s\n", athletes[0].nom, athletes[0].prenom, athletes[1].nom, athletes[1].prenom, athletes[2].nom, athletes[2].prenom);
//Fonction pour calculer l'équation de la droite d'évolution des performances d'un athlète//
       
       calculer_droite_regression(n, &a, &b, data);
       printf("Equation de la droite d'évolution : y = %.lf*x + %lf\n", a, b);

  return(0);

}




