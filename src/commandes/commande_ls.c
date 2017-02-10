//
// Created by eliottvct on 10/02/17.
//

#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {

    char* folder=".";
    DIR *d;
    struct dirent *dir;
    struct stat st;
    char buf[512];

    d = opendir(folder);
    if (!d)
        return 0;
    else {
        /*printf( "%d arguments\n", argc );
        for( int i = 0; i < argc; ++i ) {
            printf( "argv[%d] = %s\n", i, argv[i]);
        }*/
        //TODO : chemin absolu en arg
        //TODO : rajouter tri alphabetique




        /*if ((*argv[1] = (char) "-la")) {
            while ((dir = readdir(d)) != NULL) {
                printf("%s\n", dir->d_name);
            }
        }
        else {
            while ((dir = readdir(d)) != NULL) {
                printf("%s", dir->d_name);
            }

        }*/
        while ((dir = readdir(d)) != NULL) {

            if ((*argv[1] = (char) "-la")) {
                printf("%s\n", dir->d_name);
            }
            else {
                printf("%s", dir->d_name);
            }
        }
        closedir(d);
        return 0;
    }
}