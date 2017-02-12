//
// Created by eliottvct on 10/02/17.
//

#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pwd.h>
#include <unistd.h>

#include "commande_ls.h"

int main(int argc, char *argv[]) {

    bool L_OPTION = false;
    bool A_OPTION = false;

    char folder[CHAINE_MAX];
    strcpy(folder, ".");

    for( int i = 0; i < argc; ++i ) {
        //printf( "argv[%d] = %s\n", i, argv[i]);
        if (COMMENCE_PAR(argv[i], "-")) {
            if (EST_EGAL(argv[i], "-l")) {
                L_OPTION = true;
            }
            if (EST_EGAL(argv[i], "-a")) {
                A_OPTION = true;
            }
            if (EST_EGAL(argv[i], "-la")) {
                A_OPTION = true;
                L_OPTION = true;
            }
        }
        else if(i != 0) {
            printf("other : %s\n", argv[i]);
            strcpy(folder, argv[i]);
        }
    }

    list_content(folder, L_OPTION, A_OPTION);
    return 0;
}

void list_content(char *folder, bool L_OPTION, bool A_OPTION) {
    DIR *d;
    struct dirent *dir;
    struct stat stats;

    if ((d = opendir(folder)) == NULL) {
        printf("Erreur");
    }
    while ((dir = readdir(d)) != NULL) {
        char *dwRet;
        char var[CHAINE_MAX];
        dwRet = getcwd(var, sizeof(var));
        printf("PATH : %s\n", var);

        if (stat(dir->d_name, &stats) == 0) {  //modifier le dir name pour chemin custom ? http://stackoverflow.com/questions/28441756/list-directory-and-display-details-of-each-file-owner-octal-permissions-and-fi
            /* nom */
            char *name[512];
            if (S_ISDIR(stats.st_mode)) //si c'est un dossier
                strcpy((char *) name, BLU);
            else if (access(dir->d_name, F_OK|X_OK) == 0)   //si c'est un executable (condition else car les dossiers sont aussi des executables)
                strcpy((char *) name, GRN);
            else
                strcpy((char *) name, RESET);
            strcat((char *) name, dir->d_name);
            strcat((char *) name, RESET);

            if (L_OPTION) {
                if (A_OPTION || !(COMMENCE_PAR(dir->d_name, "."))) {
                    /* date de modification */
                    time_t t = stats.st_mtime;
                    struct tm tm = *localtime(&t);
                    char time[32];
                    strftime(time, sizeof time, "%d/%m/%Y %H:%M", &tm);

                    /* propriétaire */
                    struct passwd *pInfo = getpwuid(stats.st_uid);

                    /* permissions */
                    char *permissions[32];
                    strcpy((char *) permissions, (char *) (intptr_t) (S_ISDIR(stats.st_mode)) ? "d" : "-"); //condition ternaire
                    strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IRUSR) ? "r" : "-");
                    strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IWUSR) ? "w" : "-");
                    strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IXUSR) ? "x" : "-");
                    strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IRGRP) ? "r" : "-");
                    strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IWGRP) ? "w" : "-");
                    strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IXGRP) ? "x" : "-");
                    strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IROTH) ? "r" : "-");
                    strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IWOTH) ? "w" : "-");
                    strcat((char *) permissions, (char *) (intptr_t) (stats.st_mode & S_IXOTH) ? "x " : "- ");

                    printf("%s %d %s %-8d %-10s %s\n", (char *) permissions, (int) stats.st_nlink, pInfo->pw_name, (int) stats.st_size, time, (char *) name);
                }
            }
            else {
                if (A_OPTION || !(COMMENCE_PAR(dir->d_name, "."))) {
                    printf("%-10s ", (char *) name);
                }
            }
        }
    }

    closedir(d);
}
