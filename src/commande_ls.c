//
// Created by eliottvct on 10/02/17.
//

#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pwd.h>
#include <unistd.h>
#include <errno.h>

#include "commande_ls.h"

int main(int argc, char *argv[]) {

    t_bool L_OPTION = faux;
    t_bool A_OPTION = faux;

    char chemin[CHAINE_MAX];
    strcpy(chemin, ".");    //par défaut, on met le chemin du chemin courant dans la variable chemin

    for( int i = 0; i < argc; ++i ) {   //on parcourt tout les arguments saisis par l'utilisateur
        if (COMMENCE_PAR(argv[i], "-")) {   //on vérifie d'abord ceux commençant par un "-"
            if (EST_EGAL(argv[i], "-l")) {
                L_OPTION = vrai;
            }
            if (EST_EGAL(argv[i], "-a")) {
                A_OPTION = vrai;
            }
            if (EST_EGAL(argv[i], "-la")) {
                A_OPTION = vrai;
                L_OPTION = vrai;
            }
        }
        else if(i != 0) //si l'argument ne commence pas par un "-", et si ce n'est pas le premier argument, on suppose que l'utilisateur a précisé un chemin
            strcpy(chemin, argv[i]);
    }

    return lister_contenu(chemin, L_OPTION, A_OPTION);
}

t_bool lister_contenu(char *chemin, bool L_OPTION, bool A_OPTION) {
    DIR *d;
    struct dirent *dir;
    char path[CHAINE_MAX];
    char fpath[CHAINE_MAX];
    t_bool resultat = vrai;

    if ((d = opendir(chemin)) == NULL) {    //si l'ouverture du chemin a échoué
        printf("Une erreur a eu lieu : %s\n", strerror(errno));
        resultat = faux;
    }
    else {
        strcpy(path, chemin);
        strcat(path, "/");

        while ((dir = readdir(d)) != NULL) {    //tant que readdir() renvoie des résultats
            struct stat stats;  //création d'une statistique
            strcpy(fpath, path);
            strcat(fpath, dir->d_name);

            if (stat(fpath, &stats) == 0) {

                /* nom */
                char *name[512];
                if (S_ISDIR(stats.st_mode)) //si c'est un dossier
                    strcpy((char *) name, BLU); //alors la ligne commencera par une balise activant la coloration bleue
                else if (access(fpath, F_OK|X_OK) == 0)   //si c'est un executable (condition else car les dossiers sont aussi des executables)
                    strcpy((char *) name, GRN);
                else
                    strcpy((char *) name, RESET);
                strcat((char *) name, dir->d_name); //on rajoute le nom du résultat
                strcat((char *) name, RESET);   //puis on rajoute la balise RESET pour remettre la coloration à zéro

                if (L_OPTION) { //si l'option "-l" est activée
                    if (A_OPTION || !(COMMENCE_PAR(dir->d_name, "."))) {    //si l'option "-a" est activée, ou alors si le résultat ne commence pas par un "."

                        /* date de modification */
                        time_t t = stats.st_mtime;
                        struct tm tm = *localtime(&t);
                        char time[32];
                        strftime(time, sizeof time, "%d/%m/%Y %H:%M", &tm);

                        /* propriétaire */
                        struct passwd *pInfo = getpwuid(stats.st_uid);

                        /* permissions */
                        char *permissions[32];
                        strcpy((char *) permissions, (char *) (intptr_t) (S_ISDIR(stats.st_mode)) ? "d" : "-"); //condition ternaire pour choisir entre "d" et "-"
                        strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IRUSR) ? "r" : "-");
                        strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IWUSR) ? "w" : "-");
                        strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IXUSR) ? "x" : "-");
                        strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IRGRP) ? "r" : "-");
                        strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IWGRP) ? "w" : "-");
                        strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IXGRP) ? "x" : "-");
                        strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IROTH) ? "r" : "-");
                        strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IWOTH) ? "w" : "-");
                        strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IXOTH) ? "x " : "- ");

                        printf("%s %-2d %s %-10d %-10s %s\n", (char *) permissions, (int) stats.st_nlink, pInfo->pw_name, (int) stats.st_size, time, (char *) name);
                    }
                }
                else {  //on rentre dans cette partie seulement si l'option "-l" n'est pas activée
                    if (A_OPTION || !(COMMENCE_PAR(dir->d_name, "."))) {    //si l'option "-a" est activée, ou alors si le résultat ne commence pas par un "."
                        printf("%-10s ", (char *) name);
                    }
                }
            }
        }
    }
    closedir(d);    //on ferme le dossier

    return resultat;
}
