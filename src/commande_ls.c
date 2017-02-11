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


#define COMMENCE_PAR(a, b)    (strncmp((a), (b), strlen((b)))==0)
#define EST_EGAL(a, b)        (strlen((a))==strlen((b)) && COMMENCE_PAR((a),(b)))


#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

int main(int argc, char *argv[]) {

    DIR *d;
    struct dirent *dir;
    struct stat stats;
    struct dirent *array_of_dir[512];
    struct stat array_of_stat[512];

    bool L_OPTION = false;
    bool A_OPTION = false;

    char* folder=".";

    if ((d = opendir(folder)) == NULL) {
        perror ("Failed to open directory");
        return 1;
    }

    else {
        for( int i = 0; i < argc; ++i ) {
            //printf( "argv[%d] = %s\n", i, argv[i]);
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

        //TODO : chemin absolu en arg
        //TODO : rajouter tri alphabetique

        //int i = 0;
        while ((dir = readdir(d)) != NULL) {


            if (stat(dir->d_name, &stats) == 0) {  //modifier le dir name pour chemin custom ? http://stackoverflow.com/questions/28441756/list-directory-and-display-details-of-each-file-owner-octal-permissions-and-fi
                /* nom */
                char *name[512];
                if (S_ISDIR(stats.st_mode))
                    strcpy(name, BLU);
                else if (access(dir->d_name, F_OK|X_OK) == 0)
                    strcpy(name, GRN);
                else
                    strcpy(name, RESET);
                strcat(name, dir->d_name);
                strcat(name, RESET);

                if (L_OPTION) {
                    if (A_OPTION || !(COMMENCE_PAR(dir->d_name, "."))) {
                        /* date de modification */
                        time_t t = stats.st_mtime;
                        struct tm tm = *localtime(&t);
                        char time[32];
                        strftime(time, sizeof time, "%d/%m/%Y %H:%M", &tm);

                        /* propriétaire */
                        struct passwd *pInfo = getpwuid(stats.st_uid);

                        /* groupe */


                        /* permissions */
                        char *permissions[32];
                        strcpy((char *) permissions, (char *) (S_ISDIR(stats.st_mode)) ? "d" : "-"); //condition ternaire
                        strcat((char *) permissions, (char *) (stats.st_mode & S_IRUSR) ? "r" : "-");
                        strcat((char *) permissions, (char *) (stats.st_mode & S_IWUSR) ? "w" : "-");
                        strcat((char *) permissions, (char *) (stats.st_mode & S_IXUSR) ? "x" : "-");
                        strcat((char *) permissions, (char *) (stats.st_mode & S_IRGRP) ? "r" : "-");
                        strcat((char *) permissions, (char *) (stats.st_mode & S_IWGRP) ? "w" : "-");
                        strcat((char *) permissions, (char *) (stats.st_mode & S_IXGRP) ? "x" : "-");
                        strcat((char *) permissions, (char *) (stats.st_mode & S_IROTH) ? "r" : "-");
                        strcat((char *) permissions, (char *) (stats.st_mode & S_IWOTH) ? "w" : "-");
                        strcat((char *) permissions, (char *) (stats.st_mode & S_IXOTH) ? "x " : "- ");

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
        return 0;
    }
}