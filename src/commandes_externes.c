#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <errno.h>
#include "divers.h"
#include "commandes_externes.h"
#include "entities.c"
#include "parse.h"

t_bool ActionEXEC(parse_info *info, int debut, int nbArg) {
    pid_t pid_fils = -1;

    if(info->modificateur[2] != TUBE) {
        pid_fils = fork();
        if (pid_fils == -1) {
            printf("Une erreur a eu lieu : %s\n", strerror(errno));
            return faux;
        }
        else if (pid_fils == 0) //Fils
            execute(info, nbArg, debut);
        else {
            int status;
            if ((info->modificateur[debut + 1] != ARRIERE_PLAN)) {
                waitpid(pid_fils,&status,0);
                if (WIFEXITED(status)) {
                    printf("child %d exited with = %d\n", pid_fils, WEXITSTATUS(status));
                    if (WEXITSTATUS(status) == 0)   //program succeeded
                        return vrai;
                    else    //program failed but exited normally
                        return faux;
                    //exit(WEXITSTATUS(status));
                }
                if(WIFSIGNALED(status)) {
                    printf("Le processus %d est mort: signal %d\n", pid_fils, WTERMSIG(status));
                    return faux;
                }
                else//program exited abnormally
                    return faux;
            }
        }
    }
    else
        execute(info, nbArg, debut);
    return vrai;    //TODO : à modifier
}


void execute(parse_info * info, int nbArg, int debut) {

    char *args[ARG_MAX];
    char *cmd = info->ligne_cmd[debut];

    if (!EST_EGAL(info->sortie, "")) {  //redirection demandée
        int sortie = open(info->sortie, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        if (sortie != 0) {   //si le fichier n'est pas accessible en écriture
            dup2(sortie, 1);
            close(sortie);
        }
    }

    if (!EST_EGAL(info->entree, "")) {  //redirection demandée
        int entree = open(info->entree, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        if (entree != 0) {   //si le fichier n'est pas accessible en écriture
            dup2(entree, 0);
            close(entree);
        }
    }

    for (int j = 0; j < nbArg; j++)
        args[j] = info->ligne_cmd[debut + j];
    args[nbArg] = (char *) 0;

    if (EST_EGAL(cmd, "ls")) {
        execv("ls", args);
        exit(errno);
    }
    else {
        execvp(cmd, args);
        exit(errno);
    }
}
