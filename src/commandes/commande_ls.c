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
        printf( "%d\n arguments", argc );
        for( int i = 0; i < argc; ++i ) {
            printf( "argv[%d] = %s\n", i, argv[i] );
        }
        while ((dir = readdir(d)) != NULL) {
            switch((*argv)[1]) {
                //
                default:
                    printf("%s\n", dir->d_name);
                    break;
            }
        }
        closedir(d);
        return 0;
    }
}