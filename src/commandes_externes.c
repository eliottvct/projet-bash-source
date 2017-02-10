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
    int status;
    pid_t pid_fils = -1;
    t_bool result = faux;

    if(info->modificateur[2] != TUBE) {
        pid_fils = fork();
        if (pid_fils == -1){
            //TODO : Ajouter printf error
            return faux;
        }else if (pid_fils == 0){ //Fils
            execute(info, nbArg, debut);
        }else{
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
                    printf("Le processus %d est mort: signal %d%s\n", pid_fils, WTERMSIG(status), WCOREDUMP(status) ? " - core dumped" : "");
                    return faux;
                }
                else {  //program exited abnormally
                    return faux;
                }
            }
        }
    }else{
        execute(info, nbArg, debut);
    }
}


void execute(parse_info * info, int nbArg, int debut) {
    // On prépare la commande
    char * args[ARG_MAX];

    //TODO : if ligne_cmd debut != ls ou echo
    char *cmd = info->ligne_cmd[debut];
    if (!EST_EGAL(info->sortie, "")) {  //redirection demandée
        int sortie = open(info->sortie, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
        if (sortie != NULL) {   //si le fichier n'est pas accessible en écriture
            dup2(sortie,1);
            close(sortie);
        }
    }

    if (!EST_EGAL(info->entree, "")) {  //redirection demandée
        int entree = open(info->entree, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
        if (entree != NULL) {   //si le fichier n'est pas accessible en écriture
            dup2(entree,0);
            close(entree);
        }
    }

    for (int j = 0; j<nbArg; j++)
        args[j] = info->ligne_cmd[debut+j];
    args[nbArg] = (char *)0;



    DEBUG(printf("Path : %s \n", p));
    // TODO if ligne_cmd debut est ls on fait un execlp avec notre path
    execvp(cmd, args);
    exit(errno);
}
