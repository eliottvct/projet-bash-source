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
    t_bool resultat = faux;

    if(info->modificateur[2] == TUBE) { //si la ligne saisie par l'utilisateur contient un tube "|"
        execute(info, nbArg, debut);    //on exécute directment la commande
    }
    else {
        pid_fils = fork();  //on créé un fils grâce à fork()
        if (pid_fils == -1) {   //erreur lors du fork
            printf("Une erreur a eu lieu : %s\n", strerror(errno));
            resultat = faux;
        }
        else if (pid_fils == 0) //on se trouve dans le fils
            execute(info, nbArg, debut);    //on lance l'exécution de la commande
        else {  //on se trouve dans le père
            int status;
            if ((info->modificateur[debut + 1] != ARRIERE_PLAN)) {  //si la commande n'est pas éxécutée en arrière plan
                waitpid(pid_fils, &status, 0);   //alors on attend la fin de son exécution et on récupère son statut de retour
                if (WIFEXITED(status)) {    //
                    DEBUG(printf("Le fils %d s'est terminé avec = %d\n", pid_fils, WEXITSTATUS(status)));
                    if (WEXITSTATUS(status) == 0)   //la commande s'est terminée sans erreur
                        resultat = vrai;
                    else    //la commande a échoué mais s'est terminée normalement
                        resultat = faux;
                }
                if (WIFSIGNALED(status)) {
                    DEBUG(printf("Le processus %d est mort: signal %d\n", pid_fils, WTERMSIG(status)));
                    resultat = faux;
                }
                else    //la commande s'est terminée anormalement
                    resultat = faux;
            }
        }
    }
    return resultat;
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

    for (int j = 0; j < nbArg; j++) //pour chaque argument
        args[j] = info->ligne_cmd[debut + j];   //on l'ajoute au tableau d'arguments
    args[nbArg] = (char *) 0;   //on conclue en fermant le tableau

    if (EST_EGAL(cmd, "ls")) {  //si la commande à exécuter est "ls"
        char chemin_commande_ls[CHAINE_MAX];    //alors on recherche le chemin de cette commande "spéciale", sauvegardé au lancement du shell
        lire_variable("PROJECT_PATH", chemin_commande_ls);
        strcat(chemin_commande_ls, "/ls");
        execv(chemin_commande_ls, args);    //on exécute cette commande avec la primitive execv() en lui passant le chemin et les arguments
        exit(errno);
    }
    else {
        execvp(cmd, args);  //on exécute la commande avec la primitive execvp() en lui passant le nom de la commande et les arguments
        exit(errno);
    }
}
