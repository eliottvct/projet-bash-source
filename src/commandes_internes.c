#include <dirent.h>
#include <sys/stat.h>

#include "divers.h"
#include "commandes_internes.h"
#include "entities.c"


t_bool ActionECHO(parse_info *info, int debut, int nbArg) {

    int i;
    FILE *sortie;

    if (!EST_EGAL(info->sortie, "")) {  //si une redirection est demandée
        sortie = fopen(info->sortie, "w");  //on ouvre le fichier en écriture
        if (sortie == NULL) //si le fichier n'est pas accessible en écriture
            return faux;
    }
    else    //aucune redirection demandée
        sortie = stdout;    //la sortie est la sortie standard stdout

    i = 1;
    while (i < nbArg) { //tant qu'il y a des arguments
        fprintf(sortie, "%s ", info->ligne_cmd[debut + i]); //on "écrit" chaque argument dans la sortie
        i++;
    }
    printf("\n");

    if (!EST_EGAL(info->sortie, "")) {      //si aucune redirection n'est demandée
        fclose(sortie); //fermeture de la sortie (uniquement pour le fichier)
    }
    return vrai;
}


t_bool ActionSET(parse_info *info, int debut, int nbArg) {
    affiche_parse_info(info);

    if (!EST_EGAL(info->ligne_cmd[debut], "set") && !EST_EGAL(info->ligne_cmd[debut + 2], "=")) {
        DEBUG(printf("Impossible de mettre a jour la variable, appel incorrect\n"));
        return faux;
    }

    if (nbArg == 4) {   //si l'utilisateur a saisi 4 arguments
        return ecrire_variable(info->ligne_cmd[debut + 1], info->ligne_cmd[debut + 3]);
    }
    else if (nbArg == 3) {  //si l'utilisateur a saisi 3 arguments
        return ecrire_variable(info->ligne_cmd[debut + 1], NULL);   //alors la variable n'aura pas de valeur (d'où le NULL)
    }
    else {
        return faux;
    }
}

t_bool ActionCD(parse_info *info, int debut, int nbArg) {
    char def[MAX_PATH];
    char dirName[MAX_PATH];
    int i;

    if (nbArg == 1) {   //si l'utilisateur n'a pas rajouté d'argument
        lire_variable("HOME", def); //on récupère le répertoire "HOME" de l'utilisateur
        if (chdir(def) == -1) {
            DEBUG(printf("Impossible de changer vers le repertoire '%s' \n", def));
            return faux;
        }
    } else {
        strcpy(dirName, "");
        for (i = 1; i < nbArg; i++) {   //aggrégation de tout les arguments
            if (strlen(dirName) != 0) strcat(dirName, " ");
            strcat(dirName, info->ligne_cmd[debut + i]);
        }
        if (chdir(dirName) == -1) {
            DEBUG(printf("Impossible de changer vers le repertoire '%s'\n", dirName));
            return faux;
        }
    }
    return vrai;

}

t_bool ActionPWD() {
    char wd[MAX];
    wd[MAX-1] = '\0';

    if (getcwd(wd, MAX-1) == NULL) {
        printf("Impossible de récupérer le chemin courant.\n");
        return faux;
    }
    else {
        printf("%s\n", wd);
        return vrai;
    }
}