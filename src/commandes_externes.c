#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "divers.h"
#include "commandes_externes.h"
#include "entities.c"
#include "parse.h"

t_bool ActionEXEC(parse_info *info, int debut, int nbArg) {
    char arguments[ARG_MAX][CHAINE_MAX];
    char ligne[CHAINE_MAX];
    char path[CHAINE_MAX];
    int status;
    t_bool premierPlan;
    int i;


    strcpy(ligne, "");
    strcpy(path, "");
    t_bool premierPlan;
    int i;

    for (i = 0; i < nbArg; i++) {
        if (strlen(ligne) != 0) {
            strcat(ligne, " ");
        }
        strcat(ligne, info->ligne_cmd[debut + i]);
        if (i!=0) {
            strcpy(arguments[i-1], info->ligne_cmd[i]);
        }
    }

    strcat(path, "bin/");
    strcat(path, info->ligne_cmd[debut]);
    fork_execute(path, info, nbArg, debut);

    premierPlan = (info->modificateur[debut+1] != ARRIERE_PLAN);
    if(premierPlan) {
        wait(NULL);
    }

    return vrai;
}


t_bool fork_execute(char * p, parse_info * info, int nbArg, int debut) {
    pid_t pid_fils = -1;
    char * args[CHAINE_MAX];
    char *cmd = info->ligne_cmd[debut];
    pid_fils = -1;


    for (int j = 0; j<nbArg; j++)
        args[j] = info->ligne_cmd[j];
    args[nbArg] = (char *)0;

    pid_fils = fork();
    switch(pid_fils) {
        case -1:
            perror("Echec du fork\n");
            exit(EXIT_FAILURE);
            break;

        case 0:
            if (!EST_EGAL(info->sortie, "")) {  //redirection demandée
                int sortie = open(info->sortie, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
                if (!(sortie == NULL)) {   //si le fichier n'est pas accessible en écriture
                    dup2(sortie,1);
                    close(sortie);
                }
            }

            if (!EST_EGAL(info->entree, "")) {  //redirection demandée
                int entree = open(info->entree, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
                if (!(entree == NULL)) {   //si le fichier n'est pas accessible en écriture
                    dup2(entree,0);
                    close(entree);
                }
            }

            DEBUG(printf("Path : %s \n", p));
            execvp(cmd, args);
            exit(255);
            break;

        default: {
            int status;
            if( waitpid(pid_fils, &status, 0) < 0 ) {
                perror("wait");
                //exit(254);
                return faux;
            }
            if(WIFEXITED(status)) {
                //printf("Le processus %d a retourné un status %d\n", pid_fils, WEXITSTATUS(status));
                if (WEXITSTATUS(status) == 255)
                    return faux;
                else
                    return vrai;
                //exit(WEXITSTATUS(status));
            }
            if(WIFSIGNALED(status)) {
                printf("Le processus %d est mort: signal %d%s\n", pid_fils, WTERMSIG(status), WCOREDUMP(status) ? " - core dumped" : "");
                //exit(1);
                return faux;
            }
            break;
        }
    }





/*
    // Sortie du programme en cas d'échec d'execution du fork
    if ((pid_fils = fork()) < 0){
        perror("Echec du fork\n");
        return faux;
    }

    // Code exécuté lorsque l'exec de fork n'est pas un échec
    if (pid_fils == 0){
        if (!EST_EGAL(info->sortie, "")) {  //redirection demandée
            int sortie = open(info->sortie,
                        O_CREAT | O_RDWR | O_APPEND,
            int sortie = open(info->sortie, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
            if (!(sortie == NULL)) {   //si le fichier n'est pas accessible en écriture
                dup2(sortie,1);
                close(sortie);
            }
        }

        if (!EST_EGAL(info->entree, "")) {  //redirection demandée
            int entree = open(info->entree,
                              O_CREAT | O_RDWR | O_APPEND,
            int entree = open(info->entree, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
            if (!(entree == NULL)) {   //si le fichier n'est pas accessible en écriture
                dup2(entree,0);
                close(entree);
            }
        }

        DEBUG(printf("Path : %s \n", p));
        execvp(cmd, args);
        exit(status);
    }
    }*/
    //return vrai;
}
