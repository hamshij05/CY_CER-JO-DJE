//Mode Consultation
//Inclusion des bibliothèques
#include "headerfiles.h"

    // Définition de la taile maximale de chaine de caractères pour les noms des athlètes//
    #define MAXCHAR 25
    #define MAXATH 300
    #define MAX_PARTICIPANTS_RELAIS 4 
    //Nombre maximal de participants au relais//
    typedef struct {
      int heure;
      int minutes;
      double secondes;
    } Temps;

    // Définition de l'énumeration pour représenter les différents épreuves//
    // Définition de l'énumeration pour représenter les différents épreuves//
    typedef enum {
      _100m,
      _400m,
      _5000m,
      _marathon,
      _relais_4x400m,
    } Epreuve;

    typedef enum {
        MASCULIN,
        FEMININ
    } Sexe;

    typedef struct {
      char nom[MAXCHAR];
      char prenom[MAXCHAR];
      Sexe sexe;
      char epreuve[MAXCHAR];
      //char sexe; //'M' pour masculin, 'F' pour feminin//
      char moyenne;
      Temps *temps;
      FILE *perf; // Pointeur vers un tableau dynamique pour stocker les temps de chaque épreuve//

    } Athlete;

    // Structure pour représenter un participant au relais//
    typedef struct {
      char nom[MAXCHAR];
      char prenom[MAXCHAR];
      int num_equipe;
      char equipe[MAXCHAR*8];
      int position; // position dans le relais (1ère, 2ème, 3ème ou 4ème)//
    }ParticipantRelais; 

    // Structure pour représenter un entraînement//
    typedef struct {
      char date[10]; // format de date : "JJ/MM/AAAA"
      char type_epreuve[MAXCHAR];
      Temps *temps;
      ParticipantRelais participants[MAX_PARTICIPANTS_RELAIS];
    } Entrainement;

    //int premier_entree(int sexe){
     // int nb_athletes=0;
      //while(nb_athletes==0){
        //printf("\nCombien y'a t-il d'athlètes pour le moment dans l'équipe de France?");
        //scanf("%d", &nb_athletes);
        //if(nb_athletes<0||nb_athletes>300){
           //nb_athletes=0;
           //printf("Réessayez le nombre d'Athlètes doit être compris entre 0 et 300 au total.\n"); 
        //}  
      //}
      //return nb_athletes; 
    //}  
    Athlete * crea_premier_equipe(){ 
      Athlete *equipe=malloc(MAXATH*(sizeof(Athlete)));
      if (equipe == NULL) {
          perror("\nErreur d'allocation de mémoire");
          exit(EXIT_FAILURE);
      }
     return equipe;
    }  

    Athlete* ajout_equipe(Athlete *equipe, int nb_athletes, int sexe) {
        FILE *fichier = fopen("athletes.txt", "a");
        char k;
        if (fichier == NULL) {
            printf("Erreur lors de l'ouverture du fichier athletes.txt\n");
            exit(1);
        }

        int relais_count = 0;
        if (sexe == 1) {
            sexe = MASCULIN;
        } else if (sexe == 2) {
            sexe = FEMININ;
        }

        for (int i = 0; i < nb_athletes; i++) {
            printf("\nEntrez le nom de l'athlète %d:", i + 1);
            scanf("%s", equipe[i].nom);
            printf("\nSon prénom:");
            scanf("%s", equipe[i].prenom);
            equipe[i].sexe = sexe;
            k = '0';
            while (k == '0') {
                printf("Pour choisir son épreuve, tapez 1 pour 100m, 2 pour 400m, 3 pour 5000m, 4 pour marathon et 5 pour relais 4x400m: ");
                scanf("%s", &k);
                switch (k) {
                    case '1': strcpy(equipe[i].epreuve, "100m");
                        break;
                    case '2': strcpy(equipe[i].epreuve, "400m");
                        break;
                    case '3': strcpy(equipe[i].epreuve, "5000m");
                        break;
                    case '4': strcpy(equipe[i].epreuve, "marathon");
                        break;
                    case '5': strcpy(equipe[i].epreuve, "relais_4x400m");
                        relais_count++;
                        break;
                    default:
                        k = '0';
                        printf("Erreur de saisie\n");
                        break;
                }
            }

        // Créer un fichier pour chaque athlète
        char nom_fichier[MAXCHAR * 2]; // Taille suffisante pour "nom_prenom.txt"
        snprintf(nom_fichier, sizeof(nom_fichier), "%s_%s.txt", equipe[i].nom, equipe[i].prenom);

        equipe[i].perf = fopen(nom_fichier, "w");
        if (equipe[i].perf == NULL) {
            perror("\nErreur lors de l'ouverture du fichier");
        }    
        else {
                printf("Fichier %s créé avec succès\n", nom_fichier);
                if (fclose(equipe[i].perf) != 0) {
                    perror("Erreur lors de la fermeture du fichier");
                } else {
                    equipe[i].perf = NULL;
                }
            }

            // Enregistrer les détails de l'athlète dans le fichier principal
            fprintf(fichier, "%s %s, %s %s\n", equipe[i].nom, equipe[i].prenom, equipe[i].epreuve, nom_fichier);
            printf("Athlète ajouté avec succès.\n");
        }

        fclose(fichier); // Fermer le fichier principal après avoir ajouté tous les athlètes

        if (relais_count != 0 && relais_count < 4) {
            printf("Erreur: Le nombre d'athlètes participant au relais 4x400m n'est pas suffisant pour composer une équipe.\n");
            // On avertit l'utilisateur qu'il n'y a pas assez d'athlètes pour composer une équipe
        }

        return equipe + nb_athletes - 1; // Renvoie l'adresse du dernier athlète ajouté
    }

    int est_date_valide(char *date) {
      int jour, mois, annee;
      if (sscanf(date, "%2d/%2d/%4d", &jour, &mois, &annee) != 3) {
          return 0;
      }
      if (annee < 1900 || annee > 2024 || mois < 1 || mois > 12 || jour < 1 || jour > 31) {
          return 0;
      }
      // Vérification des jours en fonction du mois (généré par Chat GPT)
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

    int est_heure_valide(int heure, int minutes, double secondes) {
      if (heure < 0 || heure > 5) {
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

      // Fonction pour enregistrer un entraînement de relais
      void enregistrer_entrainement_relais(int num_equipe) {
            int verri1=0,verri2=0, verri3=0;  
            char *nom_fichier = malloc(MAX_PARTICIPANTS_RELAIS * (MAXCHAR *sizeof(char)));
            char * nom=malloc(MAX_PARTICIPANTS_RELAIS*(MAXCHAR*sizeof(char)));
            char * prenom=malloc(MAX_PARTICIPANTS_RELAIS*(MAXCHAR*sizeof(char)));  
            Entrainement* entrainement=malloc(sizeof(Entrainement)); 
            entrainement->temps = malloc(sizeof(Temps));
            FILE *fichier1 = fopen("Equipes_relais.txt", "a+"); // ouvrir en mode ajout et lecture
            if (fichier1 == NULL) {
                printf("Erreur lors de l'ouverture du fichier athletes.txt\n");
                exit(1);
            }
            printf("Commençons par entrer les informations de cet entrainement.\n");

            while(verri1==0){  
             printf("Date (format JJ/MM/AAAA) : \n");
             scanf("%s", entrainement->date);
             verri1=est_date_valide(entrainement->date);
             if(verri1==0){
               printf("Erreur de saisie de la date ou dépassement d'intervalle .\n");
             } 
            }
            // Vérifier si un entraînement existe déjà pour cette date
            char ligne[250];
            int date_existe = 0;
            while (fgets(ligne, sizeof(ligne), fichier1)) {
                if (strstr(ligne, entrainement->date)) {
                    date_existe = 1;
                    break;
                }
            }

           if (date_existe) {
                printf("Un entraînement a déjà été enregistré pour cette date. Veuillez choisir une date différente.\n");
                fclose(fichier1);
                free(entrainement->temps);
                free(entrainement);
                return;
            }

            printf("C'est bon pour la date!\n");
            printf("Saisissez le temps effectué lors de cet entrainement:");
            while(verri2==0){  
             printf("Heure : ");
             scanf("%d", &entrainement->temps->heure);
             printf("Minutes : ");
             scanf("%d", &entrainement->temps->minutes);
             printf("Secondes : ");
             scanf("%lf", &entrainement->temps->secondes);
             verri2=est_heure_valide(entrainement->temps->heure, entrainement->temps->minutes, entrainement->temps->secondes);
               if(verri2==0){
                 printf("Erreur de saisie du temps ou depassement d'intervalle.\n");
               }    
            }
              printf("C'est bon pour le temps également!\n");
              for(int i=0;i<MAX_PARTICIPANTS_RELAIS;i++){
               while(verri3==0){
                char *nom_fichier = malloc(MAXCHAR*10*sizeof(char));
                printf("Qui était en position %d est entrainé?\n Nom:",i+1);
                scanf("%s",&nom[i]);
                printf("Prenom:");
                scanf("%s",&prenom[i]);
                snprintf(nom_fichier, sizeof(nom_fichier), "%s_%s.txt", &nom[i], &prenom[i]);

                // Vérifiez si le fichier existe en mode lecture
                FILE *fichier = fopen(nom_fichier, "r");
                if (fichier == NULL) {
                   printf("Le fichier %s n'existe pas, réessayez!\nRéecrivez le nom et le prénom de la manière exacte où vous les aviez écris pour la première fois.\n", nom_fichier);
                   free(nom_fichier);
                   verri3=0;
                } 
                else {
                  fclose(fichier);
                  // Ouvrez le fichier en mode ajout
                  fichier = fopen(nom_fichier, "a");
                   if (fichier == NULL) {
                      printf("Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
                      exit(1);
                   }
                   fprintf(fichier, "%s:%dh%dmin%2f,%s--equipe%d, position%d \n", entrainement->date, entrainement->temps->heure, entrainement->temps->minutes, entrainement->temps->secondes, entrainement->type_epreuve,num_equipe,i+1);
                   fclose(fichier);
                   verri3++;
                   if (i == 0) {
                      fprintf(fichier1, "Equipe%d: %s %s,", num_equipe, &nom[i], &prenom[i]);
                      printf("Athlète ajouté avec succès à l'équipe.\n");
                    }  
                    else if (i == 3) {
                      fprintf(fichier1, "%s %s-->%s\n", &nom[i], &prenom[i], entrainement->date);
                      printf("Athlète ajouté avec succès à l'équipe.\n");
                    }
                    else {
                      fprintf(fichier1, "%s %s,", &nom[i], &prenom[i]);
                      printf("Athlète ajouté avec succès à l'équipe.\n");
                    }
                    free(nom_fichier);
                  }  
                }

               } 
              fclose(fichier1); // Fermer le fichier Equipe_relais
              free(fichier1);
              printf("C'est bon, la séance d'entrainement a été enregistrée avec succès!\n");   
              free(entrainement->temps);
              free(entrainement);
      }


    // Fonction pour enregistrer les performances dans le fichier de l'athlete//
    void enregistrer_entrainement_fichier(int nb_equ_relais){
    int verification1=0,verification2=0, verification3=0;  
    char nom_fichier[MAXCHAR*2], nom[MAXCHAR], prenom[MAXCHAR];  
    Entrainement* entrainement=malloc(sizeof(Entrainement)); 
    entrainement->temps = malloc(sizeof(Temps));
    printf("Commençons par entrer les informations de cet entrainement.\n");
    char k='0';
    while (k == '0') {
       printf("Pour choisir l'épreuve, tapez 1 pour 100m, 2 pour 400m, 3 pour 5000m, 4 pour marathon et 5 pour relais 4x400m: ");
       scanf("%s", &k);
       switch(k){
           case '1': strcpy(entrainement->type_epreuve, "100m"); break;
           case '2': strcpy(entrainement->type_epreuve, "400m"); break;
           case '3': strcpy(entrainement->type_epreuve, "5000m"); break;
           case '4': strcpy(entrainement->type_epreuve, "marathon"); break;
           case '5': strcpy(entrainement->type_epreuve, "relais_4x400m"); 
               printf("Ce sport se pratique par équipe de 4:\n");
               enregistrer_entrainement_relais(nb_equ_relais);
               return;
           default: k = 0; printf("Erreur de saisie\n"); break;
       } 
    }  
    while(verification1==0){  
        printf("Date (format JJ/MM/AAAA) : \n");
        scanf("%s", entrainement->date);
        verification1=est_date_valide(entrainement->date);
        if(verification1==0){
            printf("Erreur de saisie de la date ou dépassement d'intervalle .\n");
        } 
    }
    printf("C'est bon pour la date!\n");
    printf("Saisissez le temps effectué lors de cet entrainement:");
    while(verification2==0){  
     printf("Heure : ");
     scanf("%d", &entrainement->temps->heure);
     printf("Minutes : ");
     if(scanf("%d", &entrainement->temps->minutes) != 1){
         printf("Erreur de saisie des minutes.\n");
     }
     printf("Secondes : ");
     if(scanf("%lf", &entrainement->temps->secondes) != 1){
         printf("Erreur lors de la saisie des secondes.\n");
     }
     verification2=est_heure_valide(entrainement->temps->heure, entrainement->temps->minutes, entrainement->temps->secondes);
       if(verification2==0){
         printf("Erreur de saisie du temps ou depassement d'intervalle.\n");
       }    
    }


      printf("C'est bon pour le temps également!\n");
      while(verification3==0){
          printf("Qui s'est entrainé?\n Nom : ");
           if(scanf("%s",nom) != 1){
               printf("Error lors de la saisie du nom.\n");
           }
           printf("Prenom:");
           if(scanf("%s", prenom) != 1){
               printf("Erreur lors de la saisie du prénom.\n");
           }
           snprintf(nom_fichier, sizeof(nom_fichier), "%s_%s.txt", nom, prenom);

          // Vérifiez si le fichier existe en mode lecture
       FILE *fichier = fopen(nom_fichier, "r");
       if (fichier == NULL) {
          printf("Le fichier %s n'existe pas, réessayez!\nRéecrivez le nom et le prénom de la manière exacte où vous les aviez écris pour la première fois.\n", nom_fichier);
         verification3=0;
       } 
       else {
          fclose(fichier);
          // Ouvrez le fichier en mode ajout
          fichier = fopen(nom_fichier, "a");
          if (fichier == NULL) {
              printf("Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
              exit(1);
          }
          fprintf(fichier, "%s:%dh%dmin%2f,%s \n", entrainement->date, entrainement->temps->heure, entrainement->temps->minutes, entrainement->temps->secondes, entrainement->type_epreuve);
          fclose(fichier);
          verification3++;
       }

      }  
     printf("C'est bon, la séance d'entrainement a été enregistrée avec succès!\n");   
     free(entrainement->temps);
     free(entrainement);
    }

    int premier_accueil(){
      int sexe=0;
      printf("Bienvenue dans votre programme de gestion des entrainements!\n");
        while(sexe==0){
         printf("Quel est le sexe de cette section d'athlète?: 1 pour Masculin et 2 pour Féminin\n");
      scanf("%d",&sexe);
          if (sexe==1||sexe==2){
            break;
          }
          else{
            sexe=0;
            printf("Erreur, veuillez reesayer!");
          }
        }   
      printf("\nCommençons par recensser les premiers athlètes.\n !!! Vous pourrez, par la suite, faire des modifications et des ajouts, il vous suffira de revenir en mode création!!! ");
      return sexe;    
    }

    void supprimer_athlete() {
        char nom[MAXCHAR], prenom[MAXCHAR];
        printf("Quel athlète souhaitez-vous supprimer?\nNom:");
        scanf("%s", nom);
        printf("\nPrénom:");
        scanf("%s", prenom);

        char ligne[MAXCHAR * 5]; // Variable pour lire chaque ligne du fichier
        FILE *fichier = fopen("athletes.txt", "r"); // Ouvre le fichier des athlètes en mode lecture
        FILE *temp = fopen("temp.txt", "w"); // Ouvre un fichier temporaire en mode écriture
        FILE *fichier_Equipe = fopen("Equipes_relais.txt", "r"); // Ouvre le fichier des équipes de relais en mode lecture
        FILE *temp_Relais = fopen("temp_relais.txt", "w"); // Ouvre un fichier temporaire en mode écriture pour les équipes de relais

        // Vérifie si les fichiers ont été ouverts correctement
        if (fichier == NULL) {
            printf("Erreur lors de l'ouverture du fichier athletes.txt\n");
            exit(1);
        }
        if (temp == NULL) {
            printf("Erreur lors de la création du fichier temporaire\n");
            fclose(fichier);
            exit(1);
        }
        if (fichier_Equipe == NULL) {
            printf("Erreur lors de l'ouverture du fichier Equipes_relais.txt\n");
            fclose(fichier);
            fclose(temp);
            exit(1);
        }
        if (temp_Relais == NULL) {
            printf("Erreur lors de la création du fichier temporaire des équipes de relais\n");
            fclose(fichier);
            fclose(temp);
            fclose(fichier_Equipe);
            exit(1);
        }

        int athlete_trouve = 0; // Indicateur pour savoir si l'athlète a été trouvé
        int equipe_trouve = 0; // Indicateur pour savoir si l'équipe a été trouvée
        int num_equipe_supprimee = -1; // Numéro de l'équipe à supprimer
        ParticipantRelais equipiers[MAX_PARTICIPANTS_RELAIS]; // Stockage des coéquipiers pour suppression de l'entraînement

        // Lit chaque ligne du fichier des athlètes
        while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
            Athlete athlete; // Crée une instance d'Athlete pour stocker les données lues
            sscanf(ligne, "%s %s", athlete.nom, athlete.prenom); // Extrait le nom et le prénom de la ligne lue

            // Si le nom et le prénom correspondent à ceux de l'athlète à supprimer
            if (strcmp(athlete.nom, nom) == 0 && strcmp(athlete.prenom, prenom) == 0) {
                athlete_trouve = 1; // Marque l'athlète comme trouvé
                continue; // Passe à la ligne suivante (ne copie pas cette ligne dans le fichier temporaire)
            }
            fputs(ligne, temp); // Copie la ligne actuelle dans le fichier temporaire
        }

        // Lit chaque ligne du fichier des équipes de relais
        while (fgets(ligne, sizeof(ligne), fichier_Equipe) != NULL) {
            sscanf(ligne, "Equipe%d: %s %s, %s %s, %s %s, %s %s",
                   &equipiers[0].num_equipe, equipiers[0].nom, equipiers[0].prenom,
                   equipiers[1].nom, equipiers[1].prenom,
                   equipiers[2].nom, equipiers[2].prenom,
                   equipiers[3].nom, equipiers[3].prenom); // Extrait les informations de l'équipe de relais

            // Vérifie si l'athlète à supprimer fait partie de l'équipe de relais
            for (int i = 0; i < MAX_PARTICIPANTS_RELAIS; i++) {
                if (strcmp(equipiers[i].nom, nom) == 0 && strcmp(equipiers[i].prenom, prenom) == 0) {
                    equipe_trouve = 1; // Marque l'équipe comme trouvée
                    num_equipe_supprimee = equipiers[0].num_equipe; // Sauvegarde le numéro de l'équipe
                    break; // Sort de la boucle dès que l'athlète est trouvé dans l'équipe
                }
            }

            // Si l'équipe n'est pas trouvée, copie la ligne actuelle dans le fichier temporaire
            if (!equipe_trouve) {
                fputs(ligne, temp_Relais);
            }
        }

        // Ferme les fichiers ouverts
        fclose(fichier);
        fclose(temp);
        fclose(fichier_Equipe);
        fclose(temp_Relais);

        // Si l'athlète a été trouvé
        if (athlete_trouve == 1) {
            remove("athletes.txt"); // Supprime le fichier original
            rename("temp.txt", "athletes.txt"); // Renomme le fichier temporaire pour remplacer l'original
            printf("L'athlète %s %s a été supprimé.\n", nom, prenom);

            // Construction du nom de fichier personnel de l'athlète
            char fichier_personnel[MAXCHAR * 2];
            snprintf(fichier_personnel, sizeof(fichier_personnel), "%s_%s.txt", nom, prenom);

            // Suppression du fichier personnel de l'athlète
            if (remove(fichier_personnel) == 0) {
                printf("Le fichier personnel %s a aussi été supprimé.\n", fichier_personnel);
            } else {
                printf("Erreur lors de la suppression du fichier personnel %s.\n", fichier_personnel);
            }
        } else {
            remove("temp.txt"); // Supprime le fichier temporaire (puisqu'il est inutile)
            printf("L'athlète %s %s n'a pas été trouvé.\n", nom, prenom);
        }

        // Si l'équipe de relais a été trouvée
        if (equipe_trouve == 1) {
            remove("Equipes_relais.txt"); // Supprime le fichier original
            rename("temp_relais.txt", "Equipes_relais.txt"); // Renomme le fichier temporaire pour remplacer l'original
            printf("L'équipe de relais incluant l'athlète %s %s a été supprimée.\n", nom, prenom);

            // Suppression des informations d'entraînement en équipe des fichiers personnels des coéquipiers
            for (int i = 0; i < MAX_PARTICIPANTS_RELAIS; i++) {
                if (strcmp(equipiers[i].nom, nom) != 0 || strcmp(equipiers[i].prenom, prenom) != 0) {
                    // Construction du nom de fichier personnel du coéquipier
                    char fichier_coequipier[MAXCHAR * 2];
                    snprintf(fichier_coequipier, sizeof(fichier_coequipier), "%s_%s.txt", equipiers[i].nom, equipiers[i].prenom);

                    // Ouverture du fichier personnel du coéquipier
                    FILE *fichier_coequipier_ptr = fopen(fichier_coequipier, "r");
                    if (fichier_coequipier_ptr == NULL) {
                        printf("Erreur lors de l'ouverture du fichier %s\n", fichier_coequipier);
                        continue;
                    }

                    // Création d'un fichier temporaire pour le coéquipier
                    char fichier_temp_coequipier[MAXCHAR * 2];
                    snprintf(fichier_temp_coequipier, sizeof(fichier_temp_coequipier), "temp_%s_%s.txt", equipiers[i].nom, equipiers[i].prenom);
                    FILE *temp_coequipier_ptr = fopen(fichier_temp_coequipier, "w");
                    if (temp_coequipier_ptr == NULL) {
                        printf("Erreur lors de la création du fichier temporaire %s\n", fichier_temp_coequipier);
                        fclose(fichier_coequipier_ptr);
                        continue;
                    }

                    // Copie des lignes du fichier personnel du coéquipier vers le fichier temporaire, en excluant les lignes correspondant à l'entraînement de l'équipe supprimée
                    while (fgets(ligne, sizeof(ligne), fichier_coequipier_ptr) != NULL) {
                        if (strstr(ligne, equipiers[i].nom) == NULL || strstr(ligne, equipiers[i].prenom) == NULL) {
                            fputs(ligne, temp_coequipier_ptr);
                        }
                    }

                    // Fermeture des fichiers personnels du coéquipier et temporaires
                    fclose(fichier_coequipier_ptr);
                    fclose(temp_coequipier_ptr);

                    // Remplacement du fichier personnel du coéquipier par le fichier temporaire
                    remove(fichier_coequipier);
                    rename(fichier_temp_coequipier, fichier_coequipier);
                }
            }
        } else {
            remove("temp_relais.txt"); // Supprime le fichier temporaire (puisqu'il est inutile)
            printf("Aucune équipe de relais incluant l'athlète %s %s n'a été trouvée.\n", nom, prenom);
        }
    }

Athlete *recuperer_equipe() {
 FILE *fichier = fopen("adresse_equipe.txt", "r");
 if (fichier == NULL) {
    printf("Erreur lors de l'ouverture du fichier adresse_equipe.txt\n");
    exit(1);
 }

void *adresse_equipe = NULL;
fscanf(fichier, "%p", &adresse_equipe);

fclose(fichier);
return (Athlete *)adresse_equipe;
}

void afficher_epreuves_en_date(char *date) {
        if (!est_date_valide(date)) {
        printf("La date n'est pas valide, réessayez.\n");
        return;
        }

        FILE *fichier_relais = fopen("Equipes_relais.txt", "r");
        if (fichier_relais == NULL) {
        printf("Le fichier Equipes_relais.txt n'existe pas, cela signifie donc qu'il n'y a pas d'équipe de relais pour le moment.\n");
        return;
        }

        FILE *fichier_athletes = fopen("athletes.txt", "r");
        if (fichier_athletes == NULL) {
        printf("Le fichier athletes.txt n'existe pas, cela signifie donc qu'il n'y a pas d'athlètes pour le moment.\n");
        fclose(fichier_relais);
        return;
        }

        char ligne[200];
        int epreuves_trouvees = 0;

        printf("Les épreuves se déroulant le %s sont : \n", date);

        // Vérification des épreuves du fichier des épreuves de relais
        while (fgets(ligne, sizeof(ligne), fichier_relais)) {
        if (strncmp(ligne, date, strlen(date)) == 0) {
          printf("%s", ligne);
          epreuves_trouvees = 1;
        }
        }

        // Vérification des fichiers personnels des athlètes
        while (fgets(ligne, sizeof(ligne), fichier_athletes)) {
        Athlete *athlete = malloc(sizeof(Athlete));
        if (athlete == NULL) {
          printf("Erreur d'allocation mémoire.\n");
          break;
        }

        char nom_fichier[MAXCHAR*2];
        if (sscanf(ligne, "%s %s", athlete->nom, athlete->prenom) == 2) {
          snprintf(nom_fichier, sizeof(nom_fichier), "%s_%s.txt", athlete->nom, athlete->prenom);
          FILE *fichier_athlete = fopen(nom_fichier, "r");
          if (fichier_athlete == NULL) {
              printf("Erreur lors de l'ouverture du fichier %s.\n", nom_fichier);
              free(athlete);
              continue;
          }

          while (fgets(ligne, sizeof(ligne), fichier_athlete)) {
              if (strncmp(ligne, date, strlen(date)) == 0) {
                  printf("%s", ligne);
                  epreuves_trouvees = 1;
              }
          }

          fclose(fichier_athlete);
        } else {
          printf("Erreur lors de la lecture du nom et du prénom.\n");
          free(athlete);
        }
        }

        if (!epreuves_trouvees) {
        printf("Aucune épreuve trouvée pour la date %s.\n", date);
        }

        fclose(fichier_relais);
        fclose(fichier_athletes);
}

void consult_epreuve(Athlete* equipe, int nb_athletes) {
          char choix;
          int k = 0;
          char ligne[200];

         printf("Quel historique d'entrainement souhaitez-vous consulter?\n");
            printf("1--100m\n2--400m\n3--5000m\n4--Marathon\n5--Relais 4x400m\n");
            scanf(" %c", &choix);  

            while (k == 0) {
                switch (choix) {
                    case '1':
                        k++;
                        for (int i = 0; i < nb_athletes; i++) {
                            if (strcmp(equipe[i].epreuve, "100m") == 0) {
                                char nom_fichier[MAXCHAR * 2];
                                snprintf(nom_fichier, sizeof(nom_fichier), "%s_%s.txt", equipe[i].nom, equipe[i].prenom);
                                FILE *fichier = fopen(nom_fichier, "r");
                                if (fichier == NULL) {
                                    printf("Erreur lors de l'ouverture du fichier %s.\n", nom_fichier);
                                    continue;
                                }

                                while (fgets(ligne, sizeof(ligne), fichier)) {
                                    printf("%s", ligne);
                                }

                                fclose(fichier);
                            }
                        }
                        break;

                    case '2':
                        k++;
                        for (int i = 0; i < nb_athletes; i++) {
                            if (strcmp(&equipe[i].epreuve, "400m") == 0) {
                                char nom_fichier[MAXCHAR * 2];
                                snprintf(nom_fichier, sizeof(nom_fichier), "%s_%s.txt", equipe[i].nom, equipe[i].prenom);
                                FILE *fichier = fopen(nom_fichier, "r");
                                if (fichier == NULL) {
                                    printf("Erreur lors de l'ouverture du fichier %s.\n", nom_fichier);
                                    continue;
                                }

                                while (fgets(ligne, sizeof(ligne), fichier)) {
                                    printf("%s", ligne);
                                }

                                fclose(fichier);
                            }
                        }
                        break;

                    case '3':
                        k++;
                        for (int i = 0; i < nb_athletes; i++) {
                            if (strcmp(&equipe[i].epreuve, "5000m") == 0) {
                                char nom_fichier[MAXCHAR * 2];
                                snprintf(nom_fichier, sizeof(nom_fichier), "%s_%s.txt", equipe[i].nom, equipe[i].prenom);
                                FILE *fichier = fopen(nom_fichier, "r");
                                if (fichier == NULL) {
                                    printf("Erreur lors de l'ouverture du fichier %s.\n", nom_fichier);
                                    continue;
                                }

                                while (fgets(ligne, sizeof(ligne), fichier)) {
                                    printf("%s", ligne);
                                }

                                fclose(fichier);
                            }
                        }
                        break;

                    case '4':
                        k++;
                        for (int i = 0; i < nb_athletes; i++) {
                            if (strcmp(equipe[i].epreuve, "Marathon") == 0) {
                                char nom_fichier[MAXCHAR * 2];
                                snprintf(nom_fichier, sizeof(nom_fichier), "%s_%s.txt", equipe[i].nom, equipe[i].prenom);
                                FILE *fichier = fopen(nom_fichier, "r");
                                if (fichier == NULL) {
                                    printf("Erreur lors de l'ouverture du fichier %s.\n", nom_fichier);
                                    continue;
                                }

                                while (fgets(ligne, sizeof(ligne), fichier)) {
                                    printf("%s", ligne);
                                }

                                fclose(fichier);
                            }
                        }
                        break;

                  case '5':
                        k++;
                        for (int i = 0; i < nb_athletes; i++) {
                            if (strcmp(equipe[i].epreuve, "Relais 4x400m") == 0) {
                                char nom_fichier[MAXCHAR * 2];
                                snprintf(nom_fichier, sizeof(nom_fichier), "%s_%s.txt", equipe[i].nom, equipe[i].prenom);
                                FILE *fichier = fopen(nom_fichier, "r");
                                if (fichier == NULL) {
                                    printf("Erreur lors de l'ouverture du fichier %s.\n", nom_fichier);
                                    continue;
                                }

                                while (fgets(ligne, sizeof(ligne), fichier)) {
                                    printf("%s", ligne);
                                }

                                fclose(fichier);
                            }
                        }
                        break;

                  default:
                     printf("Choix invalide. Veuillez réessayer.\n");
                     scanf(" %c", &choix);
                     break;
               }
            }
}

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
        void consulter_statistiques(Athlete *athlete, Epreuve epreuve) {
          Athlete *newathlete = malloc(sizeof(Athlete));
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
                printf("L'athlète %s %s a obtenu un meilleur temps de %lf pour l'épreuve %d\n", athlete->nom, athlete->prenom, meilleurTemps);
                printf("L'athlète %s %s a obtenu un pire temps de %lf pour l'épreuve %d\n", athlete->nom, athlete->prenom, pireTemps);
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
      Athlete* newathlete=malloc(sizeof(Athlete));
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
               printf("La moyenne de temps de l'athlète %s %s est de %lf pour l'épreuve %s\n", athlete->nom, athlete->prenom, moyenne, athlete->epreuve);
               fclose(fichier);
         } 
      }
    }
// Fonction comparant les performances des athlete pour les classer dans un ordre décroissant//
int comparer_performances(const void *a, const void *b, Athlete *athlete){
  Athlete *newathlete = malloc(sizeof(Athlete));
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
          Athlete *newathlete = malloc(sizeof(Athlete));
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

void consulter_temps (Athlete *equipe){
    Athlete* athlete=malloc(sizeof(Athlete));
    char nom_fichier[MAXCHAR*2];
    printf("Quel est l'athlète dont vous souhaitez consulter les temps?\nNom:");
    scanf("%s",athlete->nom);
    printf("\nPrénom:");
    scanf("%s",athlete->prenom);
    for(int i=0;i<MAXATH;i++){
     if (strcmp(equipe[i].nom, athlete->nom) == 0 && strcmp(equipe[i].prenom, athlete->prenom) == 0) {
                // L'athlète est trouvé, ouvrir le fichier de performances
                snprintf(nom_fichier, sizeof(nom_fichier), "%s_%s.txt", athlete->nom, athlete->prenom);

                FILE *fichier = fopen(nom_fichier, "r");
                if (fichier == NULL) {
                    printf("Impossible d'ouvrir le fichier des performances pour cet athlète. Il n'existe pas\n");
                    return;
                }

                // Lire et afficher les temps
                printf("Les temps de %s %s pour %s sont :\n", athlete->prenom, athlete->nom, equipe[i].epreuve);
                char ligne[100]; // Définir une chaîne de caractères pour stocker une ligne
    // Lire et afficher chaque ligne du fichier jusqu'à la fin du fichier (EOF)
                while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
                    printf("%s", ligne); // Afficher la ligne lue
                  }

    fclose(fichier); // Fermer le fichier après lecture
}

        // Si l'athlète n'est pas trouvé dans l'équipe
        printf("L'athlète %s %s n'a pas été trouvé dans l'équipe.\n", athlete->nom, athlete->prenom);
    }
}
    
           int main() {
            char choix;
            int sexe;
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
            int nb_athletes = 0;
            int nb_equ_relais = 0;
            Athlete *equipe = NULL;

            // Création initiale de l'équipe avec ajout automatique d'athlètes
            sexe = premier_accueil();
            equipe = crea_premier_equipe();
            nb_athletes = 5;  // Vous pouvez changer ce nombre selon vos besoins
            while (1) {
                printf("\n--- Menu ---\n");
                printf("1. Ajouter des athlètes à l'équipe\n");
                printf("2. Enregistrer un entraînement\n");
                printf("3. Supprimer un athlète\n");
                printf("4. Quitter et aller dans le mode consultation\n");
                printf("Votre choix: ");
                scanf(" %c", &choix);

                switch (choix) {
                    case '1':
                        printf("Combien d'athlètes souhaitez-vous ajouter? ");
                        int nb_nouveaux_athletes;
                        scanf("%d", &nb_nouveaux_athletes);
                        if (nb_athletes + nb_nouveaux_athletes <= MAXATH) {
                            equipe = ajout_equipe(equipe, nb_nouveaux_athletes, sexe);
                            nb_athletes += nb_nouveaux_athletes;
                        } else {
                            printf("Le nombre total d'athlètes dépasse la limite.\n");
                        }
                        break;

                    case '2':
                        if (equipe == NULL) {
                            printf("Veuillez d'abord créer une équipe.\n");
                        } else {
                            enregistrer_entrainement_fichier(nb_equ_relais);
                        }
                        break;

                    case '3':
                        supprimer_athlete();
                        break;

                    case '4': {
                        // Mode consultation
                        //Athlete* athlete= recuperer_equipe();
                        while (1) {
                            printf("\n--- Mode Consultation ---\n");
                            printf("1. Consulter un temps\n");
                            printf("2. Afficher les épreuves en date\n");
                            printf("3. Consulter l'historique d'entraînement d'une épreuve\n");
                            printf("4. Consulter les statistiques de performances de chaque athlète\n");
                            printf("5. Afficher un résumé des performances d'un athlete\n");
                            printf("6. Calculer la moyenne\n");
                            
                            printf("7. Selectionner les 3 meilleurs athlètes pour une discipline\n");
                            printf("8. Afficher la différence de temps d'un athlete pour une meme épreuve entre deux dates\n");
                            printf("9. Calculer l'équation de la droite de régression linéaire\n");
                            printf("Q Quitter le mode consultation et retourner au menu principal\n");
                            printf("Votre choix: ");

                            char choix_consultation;
                            scanf("%s", &choix_consultation);

                            switch (choix_consultation) {
                                case '1':
                                    consulter_temps(equipe);
                                    break;
                                case '2': {
                                    char date[MAXCHAR];
                                    printf("Entrez la date (format JJ/MM/AAAA): ");
                                    scanf("%s", date);
                                    afficher_epreuves_en_date(date);
                                    break;
                                }

                                case '3':
                                    consult_epreuve(equipe, nb_athletes);
                                    break;
                                
                                case '4':
                                    consulter_statistiques(equipe, entrainement.type_epreuve);
                                    break;
                                case '5':
                                    resume_performances(equipe, entrainement.type_epreuve);
                                    break;
                                case '6':
                                    calculer_moyenne(equipe, &entrainement);
                                    break;
                                case '7':
                                    selectionner_meilleurs_athletes(equipe, &entrainement, epreuve);
                                    break;
                                case '8':
                                    calculer_difference_temps(equipe, nb_athletes, entrainement.type_epreuve, t, temps, 5);
                                    break;
                                case '9':
                                    calculer_droite_regression(n,&a,&b,data);
                                    break;
                                case 'Q':
                                    // Quitter le mode consultation et retourner au menu principal
                                    break;
                                default:
                                    printf("Choix invalide.\n");
                                    break;
                            }
                        }
                        break;
                    }

                    default:
                        printf("Choix invalide, veuillez réessayer.\n");
                        break;
                }
            }

            return 0;
        }




