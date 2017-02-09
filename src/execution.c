#include "execution.h"
#include "commandes_internes.h"
#include "commandes_externes.h"
#include "entities.c"

void execution_ligne_cmd(parse_info *info) {

    int i, j, n, nb_arg;
    int resultat;

    i = 0;
    while (i < info->nb_arg) {

        strcpy(info->entree, "");
        strcpy(info->sortie, "");

        /* n aura la valeur de la fin de ligne de commande, ou du debut
         * de la prochaine commande a executer
         */
        n = detection_fin_commande(info, i);

        j = i;
        nb_arg = 0;
        while (j < n) {
            /* Attention cas non detecte arguments apres une redirection :
             * commande arg1 < redirection arg2
             * il s'agit a priori d'une erreur de syntaxe du shell
             */

            switch (info->modificateur[j]) {
                case REDIRECTION_ENTREE:
                    strcpy(info->entree, info->ligne_cmd[j]);
                    break;
                case REDIRECTION_SORTIE:
                    strcpy(info->sortie, info->ligne_cmd[j]);
                    break;
                case ARRIERE_PLAN:
                    info->modificateur[i] = ARRIERE_PLAN;
                    break;
                default:
                    nb_arg++;
            }
            j++;
        }

        affiche_commande(info, i);
        DEBUG(printf("\n"));

        if (info->modificateur[j] == TUBE) {
            /* il faut traiter (par simplification uniquement pour deux commandes)
             * le cas de la communication via un tube
             */
        } else {
            resultat = execution_cmd(info, i, nb_arg);
        }

        if (info->modificateur[j] == COMMENTAIRE) {
            /* il suffit de sortir de la fonction, on est arrive a un commentaire
             * le reste de la ligne de commande n'est pas a traiter
             */
            while (j < info->nb_arg) {  //on "décale" le j jusqu'à la fin
                j++;
            }
        }

        /* a ce stade, on a le resultat de la commande, et on sait s'il faut traiter la commande
         * suivante en sequence ou avec une condition
         */

        /* traitement post-execution de la partie concernant inf->modificateur[j] != RIEN */
        switch (info->modificateur[j]) {
            case EXECUTION:
                break;

            case EXECUTION_SI:
                if (!resultat) {
                    while (j < info->nb_arg && (info->modificateur[j] != EXECUTION && info->modificateur[j] != EXECUTION_SINON)) {
                        j++;
                    }
                }
                break;
            case EXECUTION_SINON:
                if (resultat) {
                    while (j < info->nb_arg && (info->modificateur[j] != EXECUTION && info->modificateur[j] != EXECUTION_SI)) {
                        j++;
                    }
                }
                break;
            default :
                /* cas impossible a priori */
                break;
        }

        info->modificateur[j] = AUTRE; /* il suffit d'executer la commande suivante en sequence */
        i = j;
    }
}

t_bool execution_cmd(parse_info *info, int debut, int nb_arg) {
    if (EST_EGAL(info->ligne_cmd[debut], "echo")) {
        return ActionECHO(info, debut, nb_arg);
    }
    else if (EST_EGAL (info->ligne_cmd[debut], "cd")) {
        return ActionCD(info, debut, nb_arg);
    }
    /*else if (EST_EGAL (info->ligne_cmd[debut], "ls")) {
        return ActionLS(info, debut, nb_arg);
    }*/
    else if (EST_EGAL (info->ligne_cmd[debut], "set")) {
        return ActionSET(info, debut, nb_arg);
    }
    /*else if (EST_EGAL (info->ligne_cmd[debut], "pwd")) {
        return ActionPWD(info, debut, nb_arg);
    }*/
    else {
        return ActionEXEC(info, debut, nb_arg);
    }
}
