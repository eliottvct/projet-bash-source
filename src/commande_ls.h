//
// Created by eliottvct on 12/02/17.
//

#ifndef PROJET_BASH_SOURCE_COMMANDE_LS_H
#define PROJET_BASH_SOURCE_COMMANDE_LS_H

#define COMMENCE_PAR(a, b)    (strncmp((a), (b), strlen((b)))==0)
#define EST_EGAL(a, b)        (strlen((a))==strlen((b)) && COMMENCE_PAR((a),(b)))

/* Définition des balises de couleur */
#define GRN   "\x1B[32m"
#define BLU   "\x1B[34m"
#define RESET "\x1B[0m"

#define CHAINE_MAX 512

typedef enum {
    faux = 0, vrai
} t_bool;

t_bool lister_contenu(char *chemin, bool L_OPTION, bool A_OPTION);

#endif //PROJET_BASH_SOURCE_COMMANDE_LS_H
