#include <stdio.h>
#include <stdlib.h>
#include "divers.h"
#include "parse.h"
#include "entities.c"

#define SEPARATEUR " \n"

parse_info *parse(char *ligne_cmd) {
    DEBUG(printf("------------ Début de parse_info()\n"));
    parse_info *info;
    char *tok;
    int i;

    /* L'appel calloc initialise � z�ro la structure de donn�es,
     * ainsi on est certain d'avoir des 'AUTRE' dans tout le tableau
     * modificateur.
     * La m�me remarque est valable pour les chemin entree et sortie.
     */
    info = calloc(1, sizeof(parse_info));

    tok = strtok(ligne_cmd, SEPARATEUR);
    i = 0;
    while (tok) {
        if (EST_EGAL(tok, "<")) {
            info->modificateur[i] = REDIRECTION_ENTREE;
        } else if (EST_EGAL(tok, ">>")) {
            info->modificateur[i] = REDIRECTION_SORTIE_AJOUT;
        } else if (EST_EGAL(tok, ">")) {
            info->modificateur[i] = REDIRECTION_SORTIE;
        } else if (EST_EGAL(tok, ";")) {
            info->modificateur[i] = EXECUTION;
        } else if (EST_EGAL(tok, "&&")) {
            info->modificateur[i] = EXECUTION_SI;
        } else if (EST_EGAL(tok, "||")) {
            info->modificateur[i] = EXECUTION_SINON;
        } else if (EST_EGAL(tok, "|")) {
            info->modificateur[i] = TUBE;
        } else if (EST_EGAL(tok, "&")) {
            info->modificateur[i] = ARRIERE_PLAN;
        } else if (EST_EGAL(tok, "#")) {
            /* Attention pour la prise en compte du commentaire :
             * ... # commande en arriere plan
             * le commentaire doit �tre attach� � la suite
             */
            i++;
            info->modificateur[i] = COMMENTAIRE;
            strcpy(info->ligne_cmd[i], ""); //on copie par d�faut une chaine vide qui sera remplac�e dans les it�rations suivantes
        } else if (COMMENCE_PAR(tok, "#")) {
            /* Attention pour la prise en compte du commentaire :
             * ... #commande en arriere plan
             * le commentaire doit �tre attach� au token courant
             */
            i++;
            info->modificateur[i] = COMMENTAIRE;
            strcpy(info->ligne_cmd[i], &(tok[1]));
        } else if (COMMENCE_PAR(tok, "%")) {
            /* Comme la substitution est faite directement, il est en r�alit� inutile
             * de marquer la cha�ne comme une variable.
             * Ce ne serait pas le cas si on choisissait de faire la substitution de
             * la variable par sa valeur lors de l'interpr�tation de la commande
             */

            //info->modificateur[i] = VARIABLE;
            lire_variable(&(tok[1]), info->ligne_cmd[i]);
            i++;
        } else {
            strcpy(info->ligne_cmd[i], tok);
            if (info->modificateur[i] != COMMENTAIRE) /* pour le commentaire l'incr�ment de i est d�j� r�alis� */
                i++;
        }

        tok = strtok(NULL, SEPARATEUR);
    }

    info->nb_arg = i;

    affiche_parse_info(info);
    DEBUG(printf("------------ Fin de parse_info()\n"));

    return info;
}

int detection_fin_commande(parse_info *info, int debut) {
    int i;

    i = debut + 1;
    while (i < info->nb_arg && \
            (info->modificateur[i] != EXECUTION && \
                    info->modificateur[i] != EXECUTION_SI && \
                    info->modificateur[i] != EXECUTION_SINON && \
                    info->modificateur[i] != TUBE && \
                    info->modificateur[i] != COMMENTAIRE)) {
        /* Les seuls �l�ments pouvant appara�tre entre deux commandes
         * sont ceux d'ex�cution conditionnelle, de tube ou de
         * commentaire (ce qui vient apr�s n'�tant plus une commande)
         */
        i++;
    }
    return i;
}

void affiche_parse_info(parse_info *info) {
    int i;
    i = 0;

    while (i < info->nb_arg) {
        affiche_commande(info, i);
        i = detection_fin_commande(info, i);

        /* Les seuls �l�ments pouvant appara�tre entre deux commandes
         * sont ceux d'ex�cution conditionnelle, de tube ou de
         * commentaire (ce qui vient apr�s n'�tant plus une commande)
         */
        if (i < info->nb_arg && info->modificateur[i] == EXECUTION) {
            DEBUG(printf(";"));
        } else if (i < info->nb_arg && info->modificateur[i] == EXECUTION_SI) {
            DEBUG(printf("&&"));
        } else if (i < info->nb_arg && info->modificateur[i] == EXECUTION_SINON) {
            DEBUG(printf("||"));
        } else if (i < info->nb_arg && info->modificateur[i] == TUBE) {
            DEBUG(printf("|"));
        } else if (i < info->nb_arg && info->modificateur[i] == COMMENTAIRE) {
            DEBUG(printf("#"));
        }
    }
    DEBUG(printf("\n"));
}

void affiche_commande(parse_info *info, int debut) {
    DEBUG(printf("------------ Début de affiche_commande()\n"));

    int i, n;

    n = detection_fin_commande(info, debut);

    i = debut;
    while (i < n) {
        /* Les seuls �l�ments pouvant appara�tre dans une commande
         * sont ceux de redirection ou d'ex�cution en arri�re-plan
         */
        if (info->modificateur[i] == REDIRECTION_ENTREE) {
            DEBUG(printf("<"));
        } else if (info->modificateur[i] == REDIRECTION_SORTIE) {
            DEBUG(printf(">"));
        } else if (info->modificateur[i] == REDIRECTION_SORTIE_AJOUT) {
            DEBUG(printf(">>"));
        } else if (info->modificateur[i] == ARRIERE_PLAN) {
            DEBUG(printf("&"));
        }
        DEBUG(printf(" '%s' ", info->ligne_cmd[i]));

        i++;
    }
    DEBUG(printf("\n------------ Fin de affiche_commande()\n"));
}
