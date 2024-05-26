//Mode Création 
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <time.h>

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
                char *nom_fichier = malloc(MAXCHAR*2*sizeof(char));
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

    int main() {
        char choix;
        int sexe;
        int nb_athletes = 0;
        int nb_equ_relais = 0;
        Athlete *equipe = NULL;

        // Création initiale de l'équipe
        sexe = premier_accueil();
        equipe = crea_premier_equipe();
        printf("Combien y a-t-il d'athlètes à ajouter (max %d)? ", MAXATH);
        scanf("%d", &nb_athletes);
        if (nb_athletes > 0 && nb_athletes <= MAXATH) {
            equipe = ajout_equipe(equipe, nb_athletes, sexe);
        } else {
            printf("Nombre d'athlètes invalide.\n");
        }

        while (1) {
            printf("\n--- Menu ---\n");
            printf("1. Ajouter des athlètes à l'équipe\n");
            printf("2. Enregistrer un entraînement\n");
            printf("3. Supprimer un athlète\n");
            printf("4. Quitter\n");
            printf("Votre choix: ");
            scanf("%s", &choix);

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

                case '4':
                    printf("Merci d'avoir utilisé le programme de gestion des entraînements. Au revoir!\n");
                    return 0;

                default:
                    printf("Choix invalide, veuillez réessayer.\n");
                    break;
            }
        }

        return 0;
    }
